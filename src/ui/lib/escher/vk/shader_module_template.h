// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_UI_LIB_ESCHER_VK_SHADER_MODULE_TEMPLATE_H_
#define SRC_UI_LIB_ESCHER_VK_SHADER_MODULE_TEMPLATE_H_

#include <string>
#include <utility>

#include "src/lib/fxl/memory/weak_ptr.h"
#include "src/ui/lib/escher/forward_declarations.h"
#include "src/ui/lib/escher/fs/hack_filesystem.h"
#include "src/ui/lib/escher/util/hash_map.h"
#include "src/ui/lib/escher/util/hashable.h"
#include "src/ui/lib/escher/vk/shader_module.h"
#include "src/ui/lib/escher/vk/shader_variant_args.h"

namespace escher {

class ShaderModuleTemplate;
using ShaderModuleTemplatePtr = fxl::RefPtr<ShaderModuleTemplate>;

// ShaderModuleTemplate is a caching factory for ShaderModule variants,
// each parameterized by ShaderVariantArgs; see GetShaderModuleVariant().
//
// Currently each variant is generated by compiling GLSL to SPIR-V.  All
// variants are compiled from the same base file, but with different
// preprocessor definitions.  The base file can #include other files, and
// different variants may include different sets of files, depending on whether
// various #include directives are "#ifdeffed out".
class ShaderModuleTemplate
    : public fxl::RefCountedThreadSafe<ShaderModuleTemplate> {
 public:
  ShaderModuleTemplate(vk::Device device, shaderc::Compiler* compiler,
                       ShaderStage shader_stage, HackFilePath path,
                       HackFilesystemPtr filesystem);

  ~ShaderModuleTemplate();

  // Obtain a ShaderModule variant from the template, either a cached one, or a
  // newly-created module if none matches the provided args.  If any of the
  // assets used to generate the variant (e.g. files) change dynamically, then
  // the module is automatically invalidated and updated.  As with any
  // ShaderModule, clients are notified of these changes by registering a
  // ShaderModuleListener; see ShaderModule/ShaderModuleListener for more more
  // details.
  ShaderModulePtr GetShaderModuleVariant(const ShaderVariantArgs& args);

 private:
  // The ShaderModules returned by GetShaderModuleVariant() are actually
  // instances of Variant, which has the following responsibilities:
  // - encapsulate the args that distinguish a variant from others generated by
  //   the template.
  // - use these args to compile GLSL into SPIR-V.
  // - remember and watch the files #included during compliation, triggering
  //   recompilation if any of them change.
  // - notify listeners whenever new SPIR-V is successfully generated.
  // - upon destruction, unregister itself with the template.
  class Variant : public ShaderModule {
   public:
    Variant(ShaderModuleTemplate* tmplate, ShaderVariantArgs args);
    ~Variant();

    const ShaderVariantArgs& args() const { return args_; }

    // Compiles the template's main file, and watches all #included files
    // for changes.
    void Compile();

    // Called when Variant is initially created, and also whenever a change is
    // observed in any of the #included files from the last compilation attempt.
    void ScheduleCompilation();

   private:
    const fxl::RefPtr<ShaderModuleTemplate> template_;
    ShaderVariantArgs args_;

    // TODO(SCN-664): this means that every Variant has a copy of every path
    // that it uses.  This might add up; consider addressing when replacing
    // HackFilesystem.
    std::unique_ptr<HackFilesystemWatcher> filesystem_watcher_;

    fxl::WeakPtrFactory<Variant> weak_factory_;
  };

  void RegisterVariant(Variant* variant);
  void UnregisterVariant(Variant* variant);

  void ScheduleVariantCompilation(fxl::WeakPtr<Variant> variant);

  // TODO(SCN-663): enforce lifecycle constraints, e.g. all instances must be
  // destroyed before the device/compiler (hence before the Escher, assuming
  // that's where they came from).
  vk::Device device_;
  shaderc::Compiler* const compiler_;

  ShaderStage shader_stage_;
  HackFilePath path_;
  HackFilesystemPtr filesystem_;
  HashMap<ShaderVariantArgs, Variant*> variants_;
};

}  // namespace escher

#endif  // SRC_UI_LIB_ESCHER_VK_SHADER_MODULE_TEMPLATE_H_
