// WARNING: This file is machine generated by fidlgen.

#pragma once

#include <lib/fidl/internal.h>
#include <lib/fidl/cpp/vector_view.h>
#include <lib/fidl/cpp/string_view.h>
#include <lib/fidl/llcpp/array.h>
#include <lib/fidl/llcpp/coding.h>
#include <lib/fidl/llcpp/traits.h>
#include <lib/fidl/llcpp/transaction.h>
#include <lib/fit/function.h>
#include <lib/zx/channel.h>
#include <lib/zx/eventpair.h>
#include <zircon/fidl.h>

namespace fidl {
namespace test {
namespace llcpp {
namespace basictypes {

struct SimpleUnion;
struct SimpleStruct;
class TestInterface;

extern "C" const fidl_type_t fidl_test_llcpp_basictypes_SimpleUnionTable;

struct SimpleUnion {
  enum class Tag : fidl_union_tag_t {
    kFieldA = 0,
    kFieldB = 1,
    Invalid = ::std::numeric_limits<::fidl_union_tag_t>::max(),
  };

  SimpleUnion();
  ~SimpleUnion();

  SimpleUnion(SimpleUnion&& other) {
    tag_ = Tag::Invalid;
    memset(reinterpret_cast<uint8_t*>(&tag_) + sizeof(tag_),
           0,
           offsetof(SimpleUnion, field_a_) - sizeof(tag_));
    if (this != &other) {
      MoveImpl_(std::move(other));
    }
  }

  SimpleUnion& operator=(SimpleUnion&& other) {
    if (this != &other) {
      MoveImpl_(std::move(other));
    }
    return *this;
  }

  bool has_invalid_tag() const { return tag_ == Tag::Invalid; }

  bool is_field_a() const { return tag_ == Tag::kFieldA; }

  int32_t& mutable_field_a();

  template <typename T>
  std::enable_if_t<std::is_convertible<T, int32_t>::value && std::is_copy_assignable<T>::value>
  set_field_a(const T& v) {
    mutable_field_a() = v;
  }

  template <typename T>
  std::enable_if_t<std::is_convertible<T, int32_t>::value && std::is_move_assignable<T>::value>
  set_field_a(T&& v) {
    mutable_field_a() = std::move(v);
  }

  int32_t const & field_a() const { return field_a_; }

  bool is_field_b() const { return tag_ == Tag::kFieldB; }

  int32_t& mutable_field_b();

  template <typename T>
  std::enable_if_t<std::is_convertible<T, int32_t>::value && std::is_copy_assignable<T>::value>
  set_field_b(const T& v) {
    mutable_field_b() = v;
  }

  template <typename T>
  std::enable_if_t<std::is_convertible<T, int32_t>::value && std::is_move_assignable<T>::value>
  set_field_b(T&& v) {
    mutable_field_b() = std::move(v);
  }

  int32_t const & field_b() const { return field_b_; }

  Tag which() const { return tag_; }

  static constexpr const fidl_type_t* Type = &fidl_test_llcpp_basictypes_SimpleUnionTable;
  static constexpr uint32_t MaxNumHandles = 0;
  static constexpr uint32_t PrimarySize = 8;
  [[maybe_unused]]
  static constexpr uint32_t MaxOutOfLine = 0;

 private:
  void Destroy();
  void MoveImpl_(SimpleUnion&& other);
  static void SizeAndOffsetAssertionHelper();
  Tag tag_;
  union {
    int32_t field_a_;
    int32_t field_b_;
  };
};

extern "C" const fidl_type_t fidl_test_llcpp_basictypes_SimpleStructTable;

struct SimpleStruct {
  static constexpr const fidl_type_t* Type = &fidl_test_llcpp_basictypes_SimpleStructTable;
  static constexpr uint32_t MaxNumHandles = 21;
  static constexpr uint32_t PrimarySize = 88;
  [[maybe_unused]]
  static constexpr uint32_t MaxOutOfLine = 0;

  int32_t field{};

  ::zx::eventpair ep{};

  ::fidl::Array<::fidl::Array<::zx::eventpair, 4>, 5> arr{};
};

extern "C" const fidl_type_t fidl_test_llcpp_basictypes_TestInterfaceConsumeSimpleStructRequestTable;

// Test interface implemented by both C and LLCPP
class TestInterface final {
 public:

  struct ConsumeSimpleStructResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;
    int32_t field;

    static constexpr const fidl_type_t* Type = nullptr;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
  };
  struct ConsumeSimpleStructRequest final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    SimpleStruct arg;

    static constexpr const fidl_type_t* Type = &fidl_test_llcpp_basictypes_TestInterfaceConsumeSimpleStructRequestTable;
    static constexpr uint32_t MaxNumHandles = 21;
    static constexpr uint32_t PrimarySize = 104;
    static constexpr uint32_t MaxOutOfLine = 0;
    using ResponseType = ConsumeSimpleStructResponse;
  };

  struct ConsumeSimpleUnionResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    uint32_t index;
    int32_t field;

    static constexpr const fidl_type_t* Type = nullptr;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
  };
  struct ConsumeSimpleUnionRequest final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    SimpleUnion arg;

    static constexpr const fidl_type_t* Type = nullptr;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    using ResponseType = ConsumeSimpleUnionResponse;
  };


  class SyncClient final {
   public:
    SyncClient(::zx::channel channel) : channel_(std::move(channel)) {}

    ~SyncClient() {}

    // Verifies that all the handles are valid channels, then returns
    // ZX_OK and loops back the field member. Otherwise, returns an error.
    zx_status_t ConsumeSimpleStruct(SimpleStruct arg, int32_t* out_status, int32_t* out_field);

    // Verifies that all the handles are valid channels, then returns
    // ZX_OK and loops back the field member. Otherwise, returns an error.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    // The lifetime of handles in the response, unless moved, is tied to the returned RAII object.
    ::fidl::DecodeResult<ConsumeSimpleStructResponse> ConsumeSimpleStruct(::fidl::BytePart _request_buffer, SimpleStruct arg, ::fidl::BytePart _response_buffer, int32_t* out_status, int32_t* out_field);

    // Verifies that all the handles are valid channels, then returns
    // ZX_OK and loops back the field member. Otherwise, returns an error.
    // Messages are encoded and decoded in-place.
    ::fidl::DecodeResult<ConsumeSimpleStructResponse> ConsumeSimpleStruct(::fidl::DecodedMessage<ConsumeSimpleStructRequest> params, ::fidl::BytePart response_buffer);

    // Loops back the field which is set, along with its index.
    zx_status_t ConsumeSimpleUnion(SimpleUnion arg, uint32_t* out_index, int32_t* out_field);

    // Loops back the field which is set, along with its index.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    // The lifetime of handles in the response, unless moved, is tied to the returned RAII object.
    ::fidl::DecodeResult<ConsumeSimpleUnionResponse> ConsumeSimpleUnion(::fidl::BytePart _request_buffer, SimpleUnion arg, ::fidl::BytePart _response_buffer, uint32_t* out_index, int32_t* out_field);

    // Loops back the field which is set, along with its index.
    // Messages are encoded and decoded in-place.
    ::fidl::DecodeResult<ConsumeSimpleUnionResponse> ConsumeSimpleUnion(::fidl::DecodedMessage<ConsumeSimpleUnionRequest> params, ::fidl::BytePart response_buffer);

   private:
    ::zx::channel channel_;
  };

  // Methods to make a sync FIDL call directly on an unowned channel, avoiding setting up a client.
  class Call final {
   public:

    // Verifies that all the handles are valid channels, then returns
    // ZX_OK and loops back the field member. Otherwise, returns an error.
    static zx_status_t ConsumeSimpleStruct(zx::unowned_channel _client_end, SimpleStruct arg, int32_t* out_status, int32_t* out_field);

    // Verifies that all the handles are valid channels, then returns
    // ZX_OK and loops back the field member. Otherwise, returns an error.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    // The lifetime of handles in the response, unless moved, is tied to the returned RAII object.
    static ::fidl::DecodeResult<ConsumeSimpleStructResponse> ConsumeSimpleStruct(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, SimpleStruct arg, ::fidl::BytePart _response_buffer, int32_t* out_status, int32_t* out_field);

    // Verifies that all the handles are valid channels, then returns
    // ZX_OK and loops back the field member. Otherwise, returns an error.
    // Messages are encoded and decoded in-place.
    static ::fidl::DecodeResult<ConsumeSimpleStructResponse> ConsumeSimpleStruct(zx::unowned_channel _client_end, ::fidl::DecodedMessage<ConsumeSimpleStructRequest> params, ::fidl::BytePart response_buffer);

    // Loops back the field which is set, along with its index.
    static zx_status_t ConsumeSimpleUnion(zx::unowned_channel _client_end, SimpleUnion arg, uint32_t* out_index, int32_t* out_field);

    // Loops back the field which is set, along with its index.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    // The lifetime of handles in the response, unless moved, is tied to the returned RAII object.
    static ::fidl::DecodeResult<ConsumeSimpleUnionResponse> ConsumeSimpleUnion(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, SimpleUnion arg, ::fidl::BytePart _response_buffer, uint32_t* out_index, int32_t* out_field);

    // Loops back the field which is set, along with its index.
    // Messages are encoded and decoded in-place.
    static ::fidl::DecodeResult<ConsumeSimpleUnionResponse> ConsumeSimpleUnion(zx::unowned_channel _client_end, ::fidl::DecodedMessage<ConsumeSimpleUnionRequest> params, ::fidl::BytePart response_buffer);

  };

  // Pure-virtual interface to be implemented by a server.
  class Interface {
   public:
    Interface() = default;
    virtual ~Interface() = default;
    using _Outer = TestInterface;
    using _Base = ::fidl::CompleterBase;

    class ConsumeSimpleStructCompleterBase : public _Base {
     public:
      void Reply(int32_t status, int32_t field);
      void Reply(::fidl::BytePart _buffer, int32_t status, int32_t field);
      void Reply(::fidl::DecodedMessage<ConsumeSimpleStructResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using ConsumeSimpleStructCompleter = ::fidl::Completer<ConsumeSimpleStructCompleterBase>;

    virtual void ConsumeSimpleStruct(SimpleStruct arg, ConsumeSimpleStructCompleter::Sync _completer) = 0;

    class ConsumeSimpleUnionCompleterBase : public _Base {
     public:
      void Reply(uint32_t index, int32_t field);
      void Reply(::fidl::BytePart _buffer, uint32_t index, int32_t field);
      void Reply(::fidl::DecodedMessage<ConsumeSimpleUnionResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using ConsumeSimpleUnionCompleter = ::fidl::Completer<ConsumeSimpleUnionCompleterBase>;

    virtual void ConsumeSimpleUnion(SimpleUnion arg, ConsumeSimpleUnionCompleter::Sync _completer) = 0;

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

};

}  // namespace basictypes
}  // namespace llcpp
}  // namespace test
}  // namespace fidl

namespace fidl {

template <>
struct IsFidlType<::fidl::test::llcpp::basictypes::SimpleUnion> : public std::true_type {};
static_assert(std::is_standard_layout_v<::fidl::test::llcpp::basictypes::SimpleUnion>);

template <>
struct IsFidlType<::fidl::test::llcpp::basictypes::SimpleStruct> : public std::true_type {};
static_assert(std::is_standard_layout_v<::fidl::test::llcpp::basictypes::SimpleStruct>);
static_assert(offsetof(::fidl::test::llcpp::basictypes::SimpleStruct, field) == 0);
static_assert(offsetof(::fidl::test::llcpp::basictypes::SimpleStruct, ep) == 4);
static_assert(offsetof(::fidl::test::llcpp::basictypes::SimpleStruct, arr) == 8);
static_assert(sizeof(::fidl::test::llcpp::basictypes::SimpleStruct) == ::fidl::test::llcpp::basictypes::SimpleStruct::PrimarySize);

template <>
struct IsFidlType<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructRequest> : public std::true_type {};
template <>
struct IsFidlMessage<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructRequest> : public std::true_type {};
static_assert(sizeof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructRequest)
    == ::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructRequest::PrimarySize);
static_assert(offsetof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructRequest, arg) == 16);

template <>
struct IsFidlType<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructResponse> : public std::true_type {};
static_assert(sizeof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructResponse)
    == ::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructResponse::PrimarySize);
static_assert(offsetof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructResponse, status) == 16);
static_assert(offsetof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleStructResponse, field) == 20);

template <>
struct IsFidlType<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionRequest> : public std::true_type {};
template <>
struct IsFidlMessage<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionRequest> : public std::true_type {};
static_assert(sizeof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionRequest)
    == ::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionRequest::PrimarySize);
static_assert(offsetof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionRequest, arg) == 16);

template <>
struct IsFidlType<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionResponse> : public std::true_type {};
static_assert(sizeof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionResponse)
    == ::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionResponse::PrimarySize);
static_assert(offsetof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionResponse, index) == 16);
static_assert(offsetof(::fidl::test::llcpp::basictypes::TestInterface::ConsumeSimpleUnionResponse, field) == 20);

}  // namespace fidl
