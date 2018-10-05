// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_ESCHER_PAPER_PAPER_LIGHT_H_
#define LIB_ESCHER_PAPER_PAPER_LIGHT_H_

#include "lib/escher/geometry/types.h"

namespace escher {

// Represents the color/intensity of ambient light in a PaperScene.
struct PaperAmbientLight {
  // Each component represents the intensity of one of the RGB wavelengths of
  // the light, ranging from 0 to 1, such that a light with intensity (1,1,1)
  // and a material with color (1,1,1) will result in an on-screen RGB pixel
  // with intensity (255,255,255).
  //
  // TODO(ES-162): define physically-based units of light intensity.
  vec3 color;
};

// Represents the position and color/intensity of a point light in a PaperScene.
//
// NOTE: good enough for now, but at some point we may want additional
// properties such as a primary direction and angular falloff (although in that
// case, maybe this would be renamed to PaperSpotLight?).
//
// TODO(ES-162): define physically-based units of light intensity.
struct PaperPointLight {
  vec3 position;

  // Each component represents the intensity of one of the RGB wavelengths of
  // the light, ranging from 0 to 1.  The pseudo-units used here are slightly
  // different than PaperAmbientLight, such that a Lambertian material with
  // coefficients (1,1,1) lit by a point light at distance 100 will result in
  // an on-screen RGB pixel with intensity (255, 255, 255).
  //
  // TODO(ES-162): define physically-based units of light intensity.
  vec3 color;

  // The intensity of a light upon a surface is attenuated proportional to the
  // squared distance between them.  This parameter provides artistic control
  // over the rate of fall off, where 0 means no distance-based falloff
  // whatsoever, and 1 is the normal physically-based falloff.
  //
  // TODO(ES-163): not currently implemented.
  float falloff = 1.f;
};

}  // namespace escher

#endif  // LIB_ESCHER_PAPER_PAPER_TRANSFORM_LIGHT_H_
