// WARNING: This file is machine generated by fidlgen.

#pragma once

#include <lib/fidl/internal.h>
#include <lib/fidl/txn_header.h>
#include <lib/fidl/llcpp/array.h>
#include <lib/fidl/llcpp/coding.h>
#include <lib/fidl/llcpp/connect_service.h>
#include <lib/fidl/llcpp/service_handler_interface.h>
#include <lib/fidl/llcpp/string_view.h>
#include <lib/fidl/llcpp/sync_call.h>
#include <lib/fidl/llcpp/traits.h>
#include <lib/fidl/llcpp/transaction.h>
#include <lib/fidl/llcpp/vector_view.h>
#include <lib/fit/function.h>
#include <lib/zx/channel.h>
#include <zircon/fidl.h>

namespace llcpp {

namespace fidl {
namespace test {
namespace simple {

class Simple;

extern "C" const fidl_type_t fidl_test_simple_SimpleEchoRequestTable;
extern "C" const fidl_type_t v1_fidl_test_simple_SimpleEchoRequestTable;
extern "C" const fidl_type_t fidl_test_simple_SimpleEchoResponseTable;
extern "C" const fidl_type_t v1_fidl_test_simple_SimpleEchoResponseTable;
extern "C" const fidl_type_t fidl_test_simple_SimpleCloseRequestTable;
extern "C" const fidl_type_t v1_fidl_test_simple_SimpleCloseRequestTable;
extern "C" const fidl_type_t fidl_test_simple_SimpleCloseResponseTable;
extern "C" const fidl_type_t v1_fidl_test_simple_SimpleCloseResponseTable;

class Simple final {
  Simple() = delete;
 public:

  struct EchoResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t reply;

    static constexpr const fidl_type_t* Type = &v1_fidl_test_simple_SimpleEchoResponseTable;
    static constexpr const fidl_type_t* AltType = &fidl_test_simple_SimpleEchoResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr uint32_t AltPrimarySize = 24;
    static constexpr uint32_t AltMaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr bool HasPointer = false;
    static constexpr bool ContainsUnion = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  struct EchoRequest final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t request;

    static constexpr const fidl_type_t* Type = &v1_fidl_test_simple_SimpleEchoRequestTable;
    static constexpr const fidl_type_t* AltType = &fidl_test_simple_SimpleEchoRequestTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr uint32_t AltPrimarySize = 24;
    static constexpr uint32_t AltMaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr bool HasPointer = false;
    static constexpr bool ContainsUnion = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kRequest;
    using ResponseType = EchoResponse;
  };

  struct CloseResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t v;

    static constexpr const fidl_type_t* Type = &v1_fidl_test_simple_SimpleCloseResponseTable;
    static constexpr const fidl_type_t* AltType = &fidl_test_simple_SimpleCloseResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr uint32_t AltPrimarySize = 24;
    static constexpr uint32_t AltMaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr bool HasPointer = false;
    static constexpr bool ContainsUnion = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  using CloseRequest = ::fidl::AnyZeroArgMessage;


  // Collection of return types of FIDL calls in this interface.
  class ResultOf final {
    ResultOf() = delete;
   private:
    template <typename ResponseType>
    class Echo_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Echo_Impl(::zx::unowned_channel _client_end, int32_t request);
      ~Echo_Impl() = default;
      Echo_Impl(Echo_Impl&& other) = default;
      Echo_Impl& operator=(Echo_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Close_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Close_Impl(::zx::unowned_channel _client_end);
      ~Close_Impl() = default;
      Close_Impl(Close_Impl&& other) = default;
      Close_Impl& operator=(Close_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };

   public:
    using Echo = Echo_Impl<EchoResponse>;
    using Close = Close_Impl<CloseResponse>;
  };

  // Collection of return types of FIDL calls in this interface,
  // when the caller-allocate flavor or in-place call is used.
  class UnownedResultOf final {
    UnownedResultOf() = delete;
   private:
    template <typename ResponseType>
    class Echo_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Echo_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, int32_t request, ::fidl::BytePart _response_buffer);
      ~Echo_Impl() = default;
      Echo_Impl(Echo_Impl&& other) = default;
      Echo_Impl& operator=(Echo_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Close_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Close_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);
      ~Close_Impl() = default;
      Close_Impl(Close_Impl&& other) = default;
      Close_Impl& operator=(Close_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };

   public:
    using Echo = Echo_Impl<EchoResponse>;
    using Close = Close_Impl<CloseResponse>;
  };

  class SyncClient final {
   public:
    explicit SyncClient(::zx::channel channel) : channel_(std::move(channel)) {}
    ~SyncClient() = default;
    SyncClient(SyncClient&&) = default;
    SyncClient& operator=(SyncClient&&) = default;

    const ::zx::channel& channel() const { return channel_; }

    ::zx::channel* mutable_channel() { return &channel_; }

    // Replies with the value requested.
    // Allocates 48 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Echo Echo(int32_t request);

    // Replies with the value requested.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Echo Echo(::fidl::BytePart _request_buffer, int32_t request, ::fidl::BytePart _response_buffer);

    // Never actually replies.  Just closes instead.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Close Close();

    // Never actually replies.  Just closes instead.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Close Close(::fidl::BytePart _response_buffer);

   private:
    ::zx::channel channel_;
  };

  // Methods to make a sync FIDL call directly on an unowned channel, avoiding setting up a client.
  class Call final {
    Call() = delete;
   public:

    // Replies with the value requested.
    // Allocates 48 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Echo Echo(::zx::unowned_channel _client_end, int32_t request);

    // Replies with the value requested.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Echo Echo(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, int32_t request, ::fidl::BytePart _response_buffer);

    // Never actually replies.  Just closes instead.
    // Allocates 40 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Close Close(::zx::unowned_channel _client_end);

    // Never actually replies.  Just closes instead.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Close Close(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);

  };

  // Messages are encoded and decoded in-place when these methods are used.
  // Additionally, requests must be already laid-out according to the FIDL wire-format.
  class InPlace final {
    InPlace() = delete;
   public:

    // Replies with the value requested.
    static ::fidl::DecodeResult<EchoResponse> Echo(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<EchoRequest> params, ::fidl::BytePart response_buffer);

    // Never actually replies.  Just closes instead.
    static ::fidl::DecodeResult<CloseResponse> Close(::zx::unowned_channel _client_end, ::fidl::BytePart response_buffer);

  };

  // Pure-virtual interface to be implemented by a server.
  class Interface {
   public:
    Interface() = default;
    virtual ~Interface() = default;
    using _Outer = Simple;
    using _Base = ::fidl::CompleterBase;

    class EchoCompleterBase : public _Base {
     public:
      void Reply(int32_t reply);
      void Reply(::fidl::BytePart _buffer, int32_t reply);
      void Reply(::fidl::DecodedMessage<EchoResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using EchoCompleter = ::fidl::Completer<EchoCompleterBase>;

    virtual void Echo(int32_t request, EchoCompleter::Sync _completer) = 0;

    class CloseCompleterBase : public _Base {
     public:
      void Reply(int32_t v);
      void Reply(::fidl::BytePart _buffer, int32_t v);
      void Reply(::fidl::DecodedMessage<CloseResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using CloseCompleter = ::fidl::Completer<CloseCompleterBase>;

    virtual void Close(CloseCompleter::Sync _completer) = 0;

  };

  // Attempts to dispatch the incoming message to a handler function in the server implementation.
  // If there is no matching handler, it returns false, leaving the message and transaction intact.
  // In all other cases, it consumes the message and returns true.
  // It is possible to chain multiple TryDispatch functions in this manner.
  static bool TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn);

  // Dispatches the incoming message to one of the handlers functions in the interface.
  // If there is no matching handler, it closes all the handles in |msg| and closes the channel with
  // a |ZX_ERR_NOT_SUPPORTED| epitaph, before returning false. The message should then be discarded.
  static bool Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn);

  // Same as |Dispatch|, but takes a |void*| instead of |Interface*|. Only used with |fidl::Bind|
  // to reduce template expansion.
  // Do not call this method manually. Use |Dispatch| instead.
  static bool TypeErasedDispatch(void* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
    return Dispatch(static_cast<Interface*>(impl), msg, txn);
  }


  // Helper functions to fill in the transaction header in a |DecodedMessage<TransactionalMessage>|.
  class SetTransactionHeaderFor final {
    SetTransactionHeaderFor() = delete;
   public:
    static void EchoRequest(const ::fidl::DecodedMessage<Simple::EchoRequest>& _msg);
    static void EchoResponse(const ::fidl::DecodedMessage<Simple::EchoResponse>& _msg);
    static void CloseRequest(const ::fidl::DecodedMessage<Simple::CloseRequest>& _msg);
    static void CloseResponse(const ::fidl::DecodedMessage<Simple::CloseResponse>& _msg);
  };
};

}  // namespace simple
}  // namespace test
}  // namespace fidl
}  // namespace llcpp

namespace fidl {

template <>
struct IsFidlType<::llcpp::fidl::test::simple::Simple::EchoRequest> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fidl::test::simple::Simple::EchoRequest> : public std::true_type {};
static_assert(sizeof(::llcpp::fidl::test::simple::Simple::EchoRequest)
    == ::llcpp::fidl::test::simple::Simple::EchoRequest::PrimarySize);
static_assert(offsetof(::llcpp::fidl::test::simple::Simple::EchoRequest, request) == 16);

template <>
struct IsFidlType<::llcpp::fidl::test::simple::Simple::EchoResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fidl::test::simple::Simple::EchoResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fidl::test::simple::Simple::EchoResponse)
    == ::llcpp::fidl::test::simple::Simple::EchoResponse::PrimarySize);
static_assert(offsetof(::llcpp::fidl::test::simple::Simple::EchoResponse, reply) == 16);

template <>
struct IsFidlType<::llcpp::fidl::test::simple::Simple::CloseResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fidl::test::simple::Simple::CloseResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fidl::test::simple::Simple::CloseResponse)
    == ::llcpp::fidl::test::simple::Simple::CloseResponse::PrimarySize);
static_assert(offsetof(::llcpp::fidl::test::simple::Simple::CloseResponse, v) == 16);

}  // namespace fidl
