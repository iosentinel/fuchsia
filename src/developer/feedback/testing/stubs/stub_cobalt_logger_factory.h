// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_DEVELOPER_FEEDBACK_TESTING_STUBS_STUB_COBALT_LOGGER_FACTORY_H_
#define SRC_DEVELOPER_FEEDBACK_TESTING_STUBS_STUB_COBALT_LOGGER_FACTORY_H_

#include <fuchsia/cobalt/cpp/fidl.h>
#include <lib/fidl/cpp/binding.h>
#include <lib/fidl/cpp/interface_handle.h>

#include <limits>
#include <memory>

#include "src/developer/feedback/testing/stubs/stub_cobalt_logger.h"
#include "src/developer/feedback/utils/cobalt_event.h"
#include "src/lib/fxl/logging.h"

namespace feedback {

// Defines the interface all stub logger factories must implement and provides common functionality.
class StubCobaltLoggerFactoryBase : public fuchsia::cobalt::LoggerFactory {
 public:
  StubCobaltLoggerFactoryBase(std::unique_ptr<StubCobaltLoggerBase> logger)
      : logger_(std::move(logger)) {}
  virtual ~StubCobaltLoggerFactoryBase() {}

  // Returns a request handler for binding to this stub service.
  fidl::InterfaceRequestHandler<fuchsia::cobalt::LoggerFactory> GetHandler() {
    return [this](fidl::InterfaceRequest<fuchsia::cobalt::LoggerFactory> request) {
      factory_binding_ =
          std::make_unique<fidl::Binding<fuchsia::cobalt::LoggerFactory>>(this, std::move(request));
    };
  }

  const CobaltEvent& LastEvent() const { return logger_->LastEvent(); }
  const std::vector<CobaltEvent>& Events() const { return logger_->Events(); }

  bool WasLogEventCalled() const { return logger_->WasLogEventCalled(); }
  bool WasLogEventCountCalled() const { return logger_->WasLogEventCountCalled(); }
  bool WasLogElapsedTimeCalled() const { return logger_->WasLogElapsedTimeCalled(); }
  bool WasLogFrameRateCalled() const { return logger_->WasLogFrameRateCalled(); }
  bool WasLogMemoryUsageCalled() const { return logger_->WasLogMemoryUsageCalled(); }
  bool WasStartTimerCalled() const { return logger_->WasStartTimerCalled(); }
  bool WasEndTimerCalled() const { return logger_->WasEndTimerCalled(); }
  bool WasLogIntHistogramCalled() const { return logger_->WasLogIntHistogramCalled(); }
  bool WasLogCustomEventCalled() const { return logger_->WasLogCustomEventCalled(); }
  bool WasLogCobaltEventCalled() const { return logger_->WasLogCobaltEventCalled(); }
  bool WasLogCobaltEventsCalled() const { return logger_->WasLogCobaltEventsCalled(); }

  void CloseFactoryConnection();
  void CloseLoggerConnection();
  void CloseAllConnections();

 protected:
  void CreateLoggerFromProjectId(
      uint32_t project_id, ::fidl::InterfaceRequest<fuchsia::cobalt::Logger> logger,
      fuchsia::cobalt::LoggerFactory::CreateLoggerFromProjectIdCallback callback) override {
    FXL_NOTIMPLEMENTED();
  }
  void CreateLoggerSimpleFromProjectId(
      uint32_t project_id, ::fidl::InterfaceRequest<fuchsia::cobalt::LoggerSimple> logger,
      fuchsia::cobalt::LoggerFactory::CreateLoggerSimpleFromProjectIdCallback callback) override {
    FXL_NOTIMPLEMENTED();
  }

  std::unique_ptr<StubCobaltLoggerBase> logger_;
  std::unique_ptr<fidl::Binding<fuchsia::cobalt::Logger>> logger_binding_;
  std::unique_ptr<fidl::Binding<fuchsia::cobalt::LoggerFactory>> factory_binding_;
};

// Always succeed in setting up the logger.
class StubCobaltLoggerFactory : public StubCobaltLoggerFactoryBase {
 public:
  StubCobaltLoggerFactory(
      std::unique_ptr<StubCobaltLoggerBase> logger = std::make_unique<StubCobaltLogger>())
      : StubCobaltLoggerFactoryBase(std::move(logger)) {}

 private:
  void CreateLoggerFromProjectId(
      uint32_t project_id, fidl::InterfaceRequest<fuchsia::cobalt::Logger> logger,
      LoggerFactory::CreateLoggerFromProjectIdCallback callback) override;
};

// Always close the connection before setting up the logger.
class StubCobaltLoggerFactoryClosesConnection : public StubCobaltLoggerFactoryBase {
 public:
  StubCobaltLoggerFactoryClosesConnection()
      : StubCobaltLoggerFactoryBase(std::make_unique<StubCobaltLoggerBase>()) {}

 private:
  void CreateLoggerFromProjectId(
      uint32_t project_id, fidl::InterfaceRequest<fuchsia::cobalt::Logger> logger,
      LoggerFactory::CreateLoggerFromProjectIdCallback callback) override;
};

// Fail to create the logger.
class StubCobaltLoggerFactoryFailsToCreateLogger : public StubCobaltLoggerFactoryBase {
 public:
  StubCobaltLoggerFactoryFailsToCreateLogger()
      : StubCobaltLoggerFactoryBase(std::make_unique<StubCobaltLoggerBase>()) {}

 private:
  void CreateLoggerFromProjectId(
      uint32_t project_id, fidl::InterfaceRequest<fuchsia::cobalt::Logger> logger,
      LoggerFactory::CreateLoggerFromProjectIdCallback callback) override;
};

// Fail to create the logger until |succeed_after_| attempts have been made.
class StubCobaltLoggerFactoryCreatesOnRetry : public StubCobaltLoggerFactoryBase {
 public:
  StubCobaltLoggerFactoryCreatesOnRetry(uint64_t succeed_after)
      : StubCobaltLoggerFactoryBase(std::make_unique<StubCobaltLogger>()),
        succeed_after_(succeed_after) {}

 private:
  void CreateLoggerFromProjectId(
      uint32_t project_id, fidl::InterfaceRequest<fuchsia::cobalt::Logger> logger,
      LoggerFactory::CreateLoggerFromProjectIdCallback callback) override;

  const uint64_t succeed_after_;
  uint64_t num_calls_ = 0;
};

// Delay calling the callee provided callback by the specified delay.
class StubCobaltLoggerFactoryDelaysCallback : public StubCobaltLoggerFactoryBase {
 public:
  StubCobaltLoggerFactoryDelaysCallback(std::unique_ptr<StubCobaltLoggerBase> logger,
                                        async_dispatcher_t* dispatcher, zx::duration delay)
      : StubCobaltLoggerFactoryBase(std::move(logger)), dispatcher_(dispatcher), delay_(delay) {}

 private:
  void CreateLoggerFromProjectId(
      uint32_t project_id, fidl::InterfaceRequest<fuchsia::cobalt::Logger> logger,
      fuchsia::cobalt::LoggerFactory::CreateLoggerFromProjectIdCallback callback) override;

  async_dispatcher_t* dispatcher_;
  zx::duration delay_;
};

}  // namespace feedback

#endif  // SRC_DEVELOPER_FEEDBACK_TESTING_STUBS_STUB_COBALT_LOGGER_FACTORY_H_
