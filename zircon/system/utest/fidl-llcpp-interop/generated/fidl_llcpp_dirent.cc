// WARNING: This file is machine generated by fidlgen.

#include <fidl_llcpp_dirent.h>
#include <memory>

namespace llcpp {

namespace fidl {
namespace test {
namespace llcpp {
namespace dirent {

namespace {

[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_CountNumDirectories_Ordinal = 0x6915d08800000000lu;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_CountNumDirectories_GenOrdinal = 0x331f16100bcf01a1lu;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceCountNumDirectoriesRequestTable;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceCountNumDirectoriesResponseTable;
extern "C" const fidl_type_t v1_fidl_test_llcpp_dirent_DirEntTestInterfaceCountNumDirectoriesResponseTable;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_ReadDir_Ordinal = 0x52407b00000000lu;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_ReadDir_GenOrdinal = 0x1b5e15bc80c7cf42lu;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceReadDirRequestTable;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceReadDirResponseTable;
extern "C" const fidl_type_t v1_fidl_test_llcpp_dirent_DirEntTestInterfaceReadDirResponseTable;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_ConsumeDirectories_Ordinal = 0x19655c0700000000lu;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_ConsumeDirectories_GenOrdinal = 0x5d25ec8ecff39cd9lu;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceConsumeDirectoriesRequestTable;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceConsumeDirectoriesResponseTable;
extern "C" const fidl_type_t v1_fidl_test_llcpp_dirent_DirEntTestInterfaceConsumeDirectoriesResponseTable;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_OnDirents_Ordinal = 0x4c81d6ba00000000lu;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_OnDirents_GenOrdinal = 0x7a75e26a45782c25lu;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceOnDirentsRequestTable;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceOnDirentsEventTable;
extern "C" const fidl_type_t v1_fidl_test_llcpp_dirent_DirEntTestInterfaceOnDirentsEventTable;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_OneWayDirents_Ordinal = 0x6beae37500000000lu;
[[maybe_unused]]
constexpr uint64_t kDirEntTestInterface_OneWayDirents_GenOrdinal = 0x3b04d61f058a9043lu;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceOneWayDirentsRequestTable;
extern "C" const fidl_type_t fidl_test_llcpp_dirent_DirEntTestInterfaceOneWayDirentsResponseTable;
extern "C" const fidl_type_t v1_fidl_test_llcpp_dirent_DirEntTestInterfaceOneWayDirentsResponseTable;

}  // namespace
template <>
DirEntTestInterface::ResultOf::CountNumDirectories_Impl<DirEntTestInterface::CountNumDirectoriesResponse>::CountNumDirectories_Impl(::zx::unowned_channel _client_end, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<CountNumDirectoriesRequest, ::fidl::MessageDirection::kSending>();
  std::unique_ptr _write_bytes_boxed = std::make_unique<::fidl::internal::AlignedBuffer<_kWriteAllocSize>>();
  auto& _write_bytes_array = *_write_bytes_boxed;
  CountNumDirectoriesRequest _request = {};
  _request.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<CountNumDirectoriesRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      DirEntTestInterface::InPlace::CountNumDirectories(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

DirEntTestInterface::ResultOf::CountNumDirectories DirEntTestInterface::SyncClient::CountNumDirectories(::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
    return ResultOf::CountNumDirectories(::zx::unowned_channel(this->channel_), std::move(dirents));
}

DirEntTestInterface::ResultOf::CountNumDirectories DirEntTestInterface::Call::CountNumDirectories(::zx::unowned_channel _client_end, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  return ResultOf::CountNumDirectories(std::move(_client_end), std::move(dirents));
}

template <>
DirEntTestInterface::UnownedResultOf::CountNumDirectories_Impl<DirEntTestInterface::CountNumDirectoriesResponse>::CountNumDirectories_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < CountNumDirectoriesRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<CountNumDirectoriesResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  CountNumDirectoriesRequest _request = {};
  _request.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<CountNumDirectoriesRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      DirEntTestInterface::InPlace::CountNumDirectories(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

DirEntTestInterface::UnownedResultOf::CountNumDirectories DirEntTestInterface::SyncClient::CountNumDirectories(::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::CountNumDirectories(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(dirents), std::move(_response_buffer));
}

DirEntTestInterface::UnownedResultOf::CountNumDirectories DirEntTestInterface::Call::CountNumDirectories(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::CountNumDirectories(std::move(_client_end), std::move(_request_buffer), std::move(dirents), std::move(_response_buffer));
}

::fidl::DecodeResult<DirEntTestInterface::CountNumDirectoriesResponse> DirEntTestInterface::InPlace::CountNumDirectories(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<CountNumDirectoriesRequest> params, ::fidl::BytePart response_buffer) {
  DirEntTestInterface::SetTransactionHeaderFor::CountNumDirectoriesRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<DirEntTestInterface::CountNumDirectoriesResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<CountNumDirectoriesRequest, CountNumDirectoriesResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<DirEntTestInterface::CountNumDirectoriesResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
DirEntTestInterface::ResultOf::ReadDir_Impl<DirEntTestInterface::ReadDirResponse>::ReadDir_Impl(::zx::unowned_channel _client_end) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ReadDirRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, ReadDirRequest::PrimarySize);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(ReadDirRequest));
  ::fidl::DecodedMessage<ReadDirRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      DirEntTestInterface::InPlace::ReadDir(std::move(_client_end), Super::response_buffer()));
}

DirEntTestInterface::ResultOf::ReadDir DirEntTestInterface::SyncClient::ReadDir() {
    return ResultOf::ReadDir(::zx::unowned_channel(this->channel_));
}

DirEntTestInterface::ResultOf::ReadDir DirEntTestInterface::Call::ReadDir(::zx::unowned_channel _client_end) {
  return ResultOf::ReadDir(std::move(_client_end));
}

template <>
DirEntTestInterface::UnownedResultOf::ReadDir_Impl<DirEntTestInterface::ReadDirResponse>::ReadDir_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  FIDL_ALIGNDECL uint8_t _write_bytes[sizeof(ReadDirRequest)] = {};
  ::fidl::BytePart _request_buffer(_write_bytes, sizeof(_write_bytes));
  memset(_request_buffer.data(), 0, ReadDirRequest::PrimarySize);
  _request_buffer.set_actual(sizeof(ReadDirRequest));
  ::fidl::DecodedMessage<ReadDirRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      DirEntTestInterface::InPlace::ReadDir(std::move(_client_end), std::move(_response_buffer)));
}

DirEntTestInterface::UnownedResultOf::ReadDir DirEntTestInterface::SyncClient::ReadDir(::fidl::BytePart _response_buffer) {
  return UnownedResultOf::ReadDir(::zx::unowned_channel(this->channel_), std::move(_response_buffer));
}

DirEntTestInterface::UnownedResultOf::ReadDir DirEntTestInterface::Call::ReadDir(::zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::ReadDir(std::move(_client_end), std::move(_response_buffer));
}

::fidl::DecodeResult<DirEntTestInterface::ReadDirResponse> DirEntTestInterface::InPlace::ReadDir(::zx::unowned_channel _client_end, ::fidl::BytePart response_buffer) {
  constexpr uint32_t _write_num_bytes = sizeof(ReadDirRequest);
  ::fidl::internal::AlignedBuffer<_write_num_bytes> _write_bytes;
  ::fidl::BytePart _request_buffer = _write_bytes.view();
  _request_buffer.set_actual(_write_num_bytes);
  ::fidl::DecodedMessage<ReadDirRequest> params(std::move(_request_buffer));
  DirEntTestInterface::SetTransactionHeaderFor::ReadDirRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<DirEntTestInterface::ReadDirResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<ReadDirRequest, ReadDirResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<DirEntTestInterface::ReadDirResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
DirEntTestInterface::ResultOf::ConsumeDirectories_Impl<DirEntTestInterface::ConsumeDirectoriesResponse>::ConsumeDirectories_Impl(::zx::unowned_channel _client_end, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ConsumeDirectoriesRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  ConsumeDirectoriesRequest _request = {};
  _request.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<ConsumeDirectoriesRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      DirEntTestInterface::InPlace::ConsumeDirectories(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

DirEntTestInterface::ResultOf::ConsumeDirectories DirEntTestInterface::SyncClient::ConsumeDirectories(::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
    return ResultOf::ConsumeDirectories(::zx::unowned_channel(this->channel_), std::move(dirents));
}

DirEntTestInterface::ResultOf::ConsumeDirectories DirEntTestInterface::Call::ConsumeDirectories(::zx::unowned_channel _client_end, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  return ResultOf::ConsumeDirectories(std::move(_client_end), std::move(dirents));
}

template <>
DirEntTestInterface::UnownedResultOf::ConsumeDirectories_Impl<DirEntTestInterface::ConsumeDirectoriesResponse>::ConsumeDirectories_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < ConsumeDirectoriesRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<ConsumeDirectoriesResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  ConsumeDirectoriesRequest _request = {};
  _request.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<ConsumeDirectoriesRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      DirEntTestInterface::InPlace::ConsumeDirectories(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

DirEntTestInterface::UnownedResultOf::ConsumeDirectories DirEntTestInterface::SyncClient::ConsumeDirectories(::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::ConsumeDirectories(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(dirents), std::move(_response_buffer));
}

DirEntTestInterface::UnownedResultOf::ConsumeDirectories DirEntTestInterface::Call::ConsumeDirectories(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::ConsumeDirectories(std::move(_client_end), std::move(_request_buffer), std::move(dirents), std::move(_response_buffer));
}

::fidl::DecodeResult<DirEntTestInterface::ConsumeDirectoriesResponse> DirEntTestInterface::InPlace::ConsumeDirectories(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<ConsumeDirectoriesRequest> params, ::fidl::BytePart response_buffer) {
  DirEntTestInterface::SetTransactionHeaderFor::ConsumeDirectoriesRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<DirEntTestInterface::ConsumeDirectoriesResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<ConsumeDirectoriesRequest, ConsumeDirectoriesResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<DirEntTestInterface::ConsumeDirectoriesResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}


DirEntTestInterface::ResultOf::OneWayDirents_Impl::OneWayDirents_Impl(::zx::unowned_channel _client_end, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::zx::eventpair ep) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<OneWayDirentsRequest, ::fidl::MessageDirection::kSending>();
  std::unique_ptr _write_bytes_boxed = std::make_unique<::fidl::internal::AlignedBuffer<_kWriteAllocSize>>();
  auto& _write_bytes_array = *_write_bytes_boxed;
  OneWayDirentsRequest _request = {};
  _request.dirents = std::move(dirents);
  _request.ep = std::move(ep);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<OneWayDirentsRequest> _decoded_request = std::move(_linearize_result.message);
  Super::operator=(
      DirEntTestInterface::InPlace::OneWayDirents(std::move(_client_end), std::move(_decoded_request)));
}

DirEntTestInterface::ResultOf::OneWayDirents DirEntTestInterface::SyncClient::OneWayDirents(::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::zx::eventpair ep) {
    return ResultOf::OneWayDirents(::zx::unowned_channel(this->channel_), std::move(dirents), std::move(ep));
}

DirEntTestInterface::ResultOf::OneWayDirents DirEntTestInterface::Call::OneWayDirents(::zx::unowned_channel _client_end, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::zx::eventpair ep) {
  return ResultOf::OneWayDirents(std::move(_client_end), std::move(dirents), std::move(ep));
}


DirEntTestInterface::UnownedResultOf::OneWayDirents_Impl::OneWayDirents_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::zx::eventpair ep) {
  if (_request_buffer.capacity() < OneWayDirentsRequest::PrimarySize) {
    Super::status_ = ZX_ERR_BUFFER_TOO_SMALL;
    Super::error_ = ::fidl::internal::kErrorRequestBufferTooSmall;
    return;
  }
  OneWayDirentsRequest _request = {};
  _request.dirents = std::move(dirents);
  _request.ep = std::move(ep);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<OneWayDirentsRequest> _decoded_request = std::move(_linearize_result.message);
  Super::operator=(
      DirEntTestInterface::InPlace::OneWayDirents(std::move(_client_end), std::move(_decoded_request)));
}

DirEntTestInterface::UnownedResultOf::OneWayDirents DirEntTestInterface::SyncClient::OneWayDirents(::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::zx::eventpair ep) {
  return UnownedResultOf::OneWayDirents(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(dirents), std::move(ep));
}

DirEntTestInterface::UnownedResultOf::OneWayDirents DirEntTestInterface::Call::OneWayDirents(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents, ::zx::eventpair ep) {
  return UnownedResultOf::OneWayDirents(std::move(_client_end), std::move(_request_buffer), std::move(dirents), std::move(ep));
}

::fidl::internal::StatusAndError DirEntTestInterface::InPlace::OneWayDirents(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<OneWayDirentsRequest> params) {
  DirEntTestInterface::SetTransactionHeaderFor::OneWayDirentsRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::internal::StatusAndError::FromFailure(
        std::move(_encode_request_result));
  }
  zx_status_t _write_status =
      ::fidl::Write(std::move(_client_end), std::move(_encode_request_result.message));
  if (_write_status != ZX_OK) {
    return ::fidl::internal::StatusAndError(_write_status, ::fidl::internal::kErrorWriteFailed);
  } else {
    return ::fidl::internal::StatusAndError(ZX_OK, nullptr);
  }
}

zx_status_t DirEntTestInterface::SyncClient::HandleEvents(DirEntTestInterface::EventHandlers handlers) {
  return DirEntTestInterface::Call::HandleEvents(::zx::unowned_channel(channel_), std::move(handlers));
}

zx_status_t DirEntTestInterface::Call::HandleEvents(::zx::unowned_channel client_end, DirEntTestInterface::EventHandlers handlers) {
  zx_status_t status = client_end->wait_one(ZX_CHANNEL_READABLE | ZX_CHANNEL_PEER_CLOSED,
                                            ::zx::time::infinite(),
                                            nullptr);
  if (status != ZX_OK) {
    return status;
  }
  constexpr uint32_t kReadAllocSize = ([]() constexpr {
    uint32_t x = 0;
    if (::fidl::internal::ClampedMessageSize<OnDirentsResponse, ::fidl::MessageDirection::kReceiving>() >= x) {
      x = ::fidl::internal::ClampedMessageSize<OnDirentsResponse, ::fidl::MessageDirection::kReceiving>();
    }
    return x;
  })();
  constexpr uint32_t kHandleAllocSize = ([]() constexpr {
    uint32_t x = 0;
    if (OnDirentsResponse::MaxNumHandles >= x) {
      x = OnDirentsResponse::MaxNumHandles;
    }
    if (x > ZX_CHANNEL_MAX_MSG_HANDLES) {
      x = ZX_CHANNEL_MAX_MSG_HANDLES;
    }
    return x;
  })();
  ::fidl::internal::ByteStorage<kReadAllocSize> read_storage;
  uint8_t* read_bytes = read_storage.buffer().data();
  zx_handle_t read_handles[kHandleAllocSize];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  status = client_end->read(ZX_CHANNEL_READ_MAY_DISCARD,
                            read_bytes, read_handles,
                            kReadAllocSize, kHandleAllocSize,
                            &actual_bytes, &actual_handles);
  if (status == ZX_ERR_BUFFER_TOO_SMALL) {
    // Message size is unexpectedly larger than calculated.
    // This can only be due to a newer version of the protocol defining a new event,
    // whose size exceeds the maximum of known events in the current protocol.
    return handlers.unknown();
  }
  if (status != ZX_OK) {
    return status;
  }
  if (actual_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(read_handles, actual_handles);
    return ZX_ERR_INVALID_ARGS;
  }
  auto msg = fidl_msg_t {
      .bytes = read_bytes,
      .handles = read_handles,
      .num_bytes = actual_bytes,
      .num_handles = actual_handles
  };
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg.bytes);
  status = fidl_validate_txn_header(hdr);
  if (status != ZX_OK) {
    return status;
  }
  switch (hdr->ordinal) {
    case kDirEntTestInterface_OnDirents_Ordinal:
    case kDirEntTestInterface_OnDirents_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<OnDirentsResponse>(&msg);
      if (result.status != ZX_OK) {
        return result.status;
      }
      auto message = result.message.message();
      return handlers.on_dirents(std::move(message->dirents));
    }
    default:
      zx_handle_close_many(read_handles, actual_handles);
      return handlers.unknown();
  }
}

bool DirEntTestInterface::TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  if (msg->num_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_INVALID_ARGS);
    return true;
  }
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg->bytes);
  zx_status_t status = fidl_validate_txn_header(hdr);
  if (status != ZX_OK) {
    txn->Close(status);
    return true;
  }
  switch (hdr->ordinal) {
    case kDirEntTestInterface_CountNumDirectories_Ordinal:
    case kDirEntTestInterface_CountNumDirectories_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<CountNumDirectoriesRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->CountNumDirectories(std::move(message->dirents),
          Interface::CountNumDirectoriesCompleter::Sync(txn));
      return true;
    }
    case kDirEntTestInterface_ReadDir_Ordinal:
    case kDirEntTestInterface_ReadDir_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<ReadDirRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      impl->ReadDir(
          Interface::ReadDirCompleter::Sync(txn));
      return true;
    }
    case kDirEntTestInterface_ConsumeDirectories_Ordinal:
    case kDirEntTestInterface_ConsumeDirectories_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<ConsumeDirectoriesRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->ConsumeDirectories(std::move(message->dirents),
          Interface::ConsumeDirectoriesCompleter::Sync(txn));
      return true;
    }
    case kDirEntTestInterface_OneWayDirents_Ordinal:
    case kDirEntTestInterface_OneWayDirents_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<OneWayDirentsRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->OneWayDirents(std::move(message->dirents), std::move(message->ep),
          Interface::OneWayDirentsCompleter::Sync(txn));
      return true;
    }
    default: {
      return false;
    }
  }
}

bool DirEntTestInterface::Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  bool found = TryDispatch(impl, msg, txn);
  if (!found) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_NOT_SUPPORTED);
  }
  return found;
}


void DirEntTestInterface::Interface::CountNumDirectoriesCompleterBase::Reply(int64_t num_dir) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<CountNumDirectoriesResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<CountNumDirectoriesResponse*>(_write_bytes);
  DirEntTestInterface::SetTransactionHeaderFor::CountNumDirectoriesResponse(
      ::fidl::DecodedMessage<CountNumDirectoriesResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              CountNumDirectoriesResponse::PrimarySize,
              CountNumDirectoriesResponse::PrimarySize)));
  _response.num_dir = std::move(num_dir);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(CountNumDirectoriesResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<CountNumDirectoriesResponse>(std::move(_response_bytes)));
}

void DirEntTestInterface::Interface::CountNumDirectoriesCompleterBase::Reply(::fidl::BytePart _buffer, int64_t num_dir) {
  if (_buffer.capacity() < CountNumDirectoriesResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  auto& _response = *reinterpret_cast<CountNumDirectoriesResponse*>(_buffer.data());
  DirEntTestInterface::SetTransactionHeaderFor::CountNumDirectoriesResponse(
      ::fidl::DecodedMessage<CountNumDirectoriesResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              CountNumDirectoriesResponse::PrimarySize,
              CountNumDirectoriesResponse::PrimarySize)));
  _response.num_dir = std::move(num_dir);
  _buffer.set_actual(sizeof(CountNumDirectoriesResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<CountNumDirectoriesResponse>(std::move(_buffer)));
}

void DirEntTestInterface::Interface::CountNumDirectoriesCompleterBase::Reply(::fidl::DecodedMessage<CountNumDirectoriesResponse> params) {
  DirEntTestInterface::SetTransactionHeaderFor::CountNumDirectoriesResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void DirEntTestInterface::Interface::ReadDirCompleterBase::Reply(::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ReadDirResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize];
  ReadDirResponse _response = {};
  DirEntTestInterface::SetTransactionHeaderFor::ReadDirResponse(
      ::fidl::DecodedMessage<ReadDirResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              ReadDirResponse::PrimarySize,
              ReadDirResponse::PrimarySize)));
  _response.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void DirEntTestInterface::Interface::ReadDirCompleterBase::Reply(::fidl::BytePart _buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  if (_buffer.capacity() < ReadDirResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  ReadDirResponse _response = {};
  DirEntTestInterface::SetTransactionHeaderFor::ReadDirResponse(
      ::fidl::DecodedMessage<ReadDirResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              ReadDirResponse::PrimarySize,
              ReadDirResponse::PrimarySize)));
  _response.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}

void DirEntTestInterface::Interface::ReadDirCompleterBase::Reply(::fidl::DecodedMessage<ReadDirResponse> params) {
  DirEntTestInterface::SetTransactionHeaderFor::ReadDirResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void DirEntTestInterface::Interface::ConsumeDirectoriesCompleterBase::Reply() {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<ConsumeDirectoriesResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<ConsumeDirectoriesResponse*>(_write_bytes);
  DirEntTestInterface::SetTransactionHeaderFor::ConsumeDirectoriesResponse(
      ::fidl::DecodedMessage<ConsumeDirectoriesResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              ConsumeDirectoriesResponse::PrimarySize,
              ConsumeDirectoriesResponse::PrimarySize)));
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(ConsumeDirectoriesResponse));
  CompleterBase::SendReply(::fidl::DecodedMessage<ConsumeDirectoriesResponse>(std::move(_response_bytes)));
}


zx_status_t DirEntTestInterface::SendOnDirentsEvent(::zx::unowned_channel _chan, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<OnDirentsResponse, ::fidl::MessageDirection::kSending>();
  std::unique_ptr<uint8_t[]> _write_bytes_unique_ptr(new uint8_t[_kWriteAllocSize]);
  uint8_t* _write_bytes = _write_bytes_unique_ptr.get();
  OnDirentsResponse _response = {};
  DirEntTestInterface::SetTransactionHeaderFor::OnDirentsResponse(
      ::fidl::DecodedMessage<OnDirentsResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              OnDirentsResponse::PrimarySize,
              OnDirentsResponse::PrimarySize)));
  _response.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    return _linearize_result.status;
  }
  return ::fidl::Write(::zx::unowned_channel(_chan), std::move(_linearize_result.message));
}

zx_status_t DirEntTestInterface::SendOnDirentsEvent(::zx::unowned_channel _chan, ::fidl::BytePart _buffer, ::fidl::VectorView<::llcpp::fidl::test::llcpp::dirent::DirEnt> dirents) {
  if (_buffer.capacity() < OnDirentsResponse::PrimarySize) {
    return ZX_ERR_BUFFER_TOO_SMALL;
  }
  OnDirentsResponse _response = {};
  DirEntTestInterface::SetTransactionHeaderFor::OnDirentsResponse(
      ::fidl::DecodedMessage<OnDirentsResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              OnDirentsResponse::PrimarySize,
              OnDirentsResponse::PrimarySize)));
  _response.dirents = std::move(dirents);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    return _linearize_result.status;
  }
  return ::fidl::Write(::zx::unowned_channel(_chan), std::move(_linearize_result.message));
}

zx_status_t DirEntTestInterface::SendOnDirentsEvent(::zx::unowned_channel _chan, ::fidl::DecodedMessage<OnDirentsResponse> params) {
  DirEntTestInterface::SetTransactionHeaderFor::OnDirentsResponse(params);
  return ::fidl::Write(::zx::unowned_channel(_chan), std::move(params));
}



void DirEntTestInterface::SetTransactionHeaderFor::CountNumDirectoriesRequest(const ::fidl::DecodedMessage<DirEntTestInterface::CountNumDirectoriesRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_CountNumDirectories_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void DirEntTestInterface::SetTransactionHeaderFor::CountNumDirectoriesResponse(const ::fidl::DecodedMessage<DirEntTestInterface::CountNumDirectoriesResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_CountNumDirectories_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void DirEntTestInterface::SetTransactionHeaderFor::ReadDirRequest(const ::fidl::DecodedMessage<DirEntTestInterface::ReadDirRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_ReadDir_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void DirEntTestInterface::SetTransactionHeaderFor::ReadDirResponse(const ::fidl::DecodedMessage<DirEntTestInterface::ReadDirResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_ReadDir_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void DirEntTestInterface::SetTransactionHeaderFor::ConsumeDirectoriesRequest(const ::fidl::DecodedMessage<DirEntTestInterface::ConsumeDirectoriesRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_ConsumeDirectories_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}
void DirEntTestInterface::SetTransactionHeaderFor::ConsumeDirectoriesResponse(const ::fidl::DecodedMessage<DirEntTestInterface::ConsumeDirectoriesResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_ConsumeDirectories_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void DirEntTestInterface::SetTransactionHeaderFor::OnDirentsResponse(const ::fidl::DecodedMessage<DirEntTestInterface::OnDirentsResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_OnDirents_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

void DirEntTestInterface::SetTransactionHeaderFor::OneWayDirentsRequest(const ::fidl::DecodedMessage<DirEntTestInterface::OneWayDirentsRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kDirEntTestInterface_OneWayDirents_GenOrdinal);
  _msg.message()->_hdr.flags[0] |= FIDL_TXN_HEADER_UNION_FROM_XUNION_FLAG;
}

}  // namespace dirent
}  // namespace llcpp
}  // namespace test
}  // namespace fidl
}  // namespace llcpp
