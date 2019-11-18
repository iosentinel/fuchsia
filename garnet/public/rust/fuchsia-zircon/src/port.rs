// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//! Type-safe bindings for Zircon port objects.

use std::mem;

use crate::ok;
use crate::{guest, AsHandleRef, GPAddr, Handle, HandleBased, HandleRef, Signals, Status, Time};
use fuchsia_zircon_sys as sys;

/// An object representing a Zircon
/// [port](https://fuchsia.dev/fuchsia-src/concepts/objects/port.md).
///
/// As essentially a subtype of `Handle`, it can be freely interconverted.
#[derive(Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
#[repr(transparent)]
pub struct Port(Handle);
impl_handle_based!(Port);

/// A packet sent through a port. This is a type-safe wrapper for
/// [zx_port_packet_t](https://fuchsia.dev/fuchsia-src/reference/syscalls/port_wait.md).
#[derive(PartialEq, Eq, Debug)]
pub struct Packet(sys::zx_port_packet_t);

/// The contents of a `Packet`.
#[derive(Debug, Copy, Clone)]
pub enum PacketContents {
    /// A user-generated packet.
    User(UserPacket),
    /// A one-shot signal packet generated via `object_wait_async`.
    SignalOne(SignalPacket),
    /// A guest bell packet
    GuestBell(GuestBellPacket),
    /// A guest mem packet
    GuestMem(GuestMemPacket),
    /// A Guest I/O Packet
    GuestIo(GuestIoPacket),

    #[doc(hidden)]
    __Nonexhaustive,
}

/// Contents of a user packet (one sent by `port_queue`). This is a type-safe wrapper for
/// [zx_packet_user_t](https://fuchsia.dev/fuchsia-src/reference/syscalls/port_wait.md).
#[derive(Debug, Copy, Clone)]
pub struct UserPacket(sys::zx_packet_user_t);

/// Contents of a signal packet (one generated by the kernel). This is a type-safe wrapper for
/// [zx_packet_signal_t](https://fuchsia.dev/fuchsia-src/reference/syscalls/port_wait.md).
#[derive(Debug, Copy, Clone)]
pub struct SignalPacket(sys::zx_packet_signal_t);

/// Contents of a guest bell packet generated by the kernel. This is a type-safe wrapper for
/// zx_packet_guest_bell_t
#[derive(Debug, Copy, Clone)]
pub struct GuestBellPacket(sys::zx_packet_guest_bell_t);

/// Contents of a guest memory packet generated by the kernel. This is a type-safe wrapper for
/// zx_packet_guest_memory_t
#[derive(Debug, Copy, Clone)]
pub struct GuestMemPacket(sys::zx_packet_guest_mem_t);

/// Contents of a guest I/O packet generated by the kernel. This is a type-safe wrapper for
/// zx_packet_guest_io_t
#[derive(Debug, Copy, Clone)]
pub struct GuestIoPacket(sys::zx_packet_guest_io_t);

impl Packet {
    /// Creates a new packet with `UserPacket` data.
    pub fn from_user_packet(key: u64, status: i32, user: UserPacket) -> Packet {
        Packet(sys::zx_port_packet_t {
            key: key,
            packet_type: sys::zx_packet_type_t::ZX_PKT_TYPE_USER,
            status: status,
            union: user.0,
        })
    }

    /// The packet's key.
    pub fn key(&self) -> u64 {
        self.0.key
    }

    /// The packet's status.
    // TODO: should this type be wrapped?
    pub fn status(&self) -> i32 {
        self.0.status
    }

    /// The contents of the packet.
    pub fn contents(&self) -> PacketContents {
        match self.0.packet_type {
            sys::zx_packet_type_t::ZX_PKT_TYPE_USER => {
                PacketContents::User(UserPacket(self.0.union))
            }

            sys::zx_packet_type_t::ZX_PKT_TYPE_SIGNAL_ONE => {
                PacketContents::SignalOne(SignalPacket(unsafe {
                    mem::transmute_copy(&self.0.union)
                }))
            }

            sys::zx_packet_type_t::ZX_PKT_TYPE_GUEST_BELL => {
                PacketContents::GuestBell(GuestBellPacket(unsafe {
                    mem::transmute_copy(&self.0.union)
                }))
            }

            sys::zx_packet_type_t::ZX_PKT_TYPE_GUEST_MEM => {
                PacketContents::GuestMem(GuestMemPacket(unsafe {
                    mem::transmute_copy(&self.0.union)
                }))
            }

            sys::zx_packet_type_t::ZX_PKT_TYPE_GUEST_IO => {
                PacketContents::GuestIo(GuestIoPacket(unsafe {
                    mem::transmute_copy(&self.0.union)
                }))
            }

            _ => panic!("unexpected packet type"),
        }
    }
}

impl UserPacket {
    pub fn from_u8_array(val: [u8; 32]) -> UserPacket {
        UserPacket(val)
    }

    pub fn as_u8_array(&self) -> &[u8; 32] {
        &self.0
    }

    pub fn as_mut_u8_array(&mut self) -> &mut [u8; 32] {
        &mut self.0
    }
}

impl SignalPacket {
    /// The signals used in the call to `object_wait_async`.
    pub fn trigger(&self) -> Signals {
        Signals::from_bits_truncate(self.0.trigger)
    }

    /// The observed signals.
    pub fn observed(&self) -> Signals {
        Signals::from_bits_truncate(self.0.observed)
    }

    /// A per object count of pending operations.
    pub fn count(&self) -> u64 {
        self.0.count
    }
}

impl GuestBellPacket {
    /// The guest physical address that was accessed that triggered the bell.
    pub fn addr(&self) -> GPAddr {
        GPAddr(self.0.addr)
    }
}

impl GuestMemPacket {
    /// The guest physical address that was accessed that triggered this signal.
    pub fn addr(&self) -> GPAddr {
        GPAddr(self.0.addr)
    }
}

#[cfg(target_arch = "aarch64")]
impl GuestMemPacket {
    /// Size of the access.
    ///
    /// Returns `None` should it find and invalid size in the packet.
    pub fn access_size(&self) -> Option<guest::MemAccessSize> {
        match self.0.access_size {
            1 => Some(guest::MemAccessSize::Bits8),
            2 => Some(guest::MemAccessSize::Bits16),
            4 => Some(guest::MemAccessSize::Bits32),
            8 => Some(guest::MemAccessSize::Bits64),
            _ => None,
        }
    }

    /// Whether or not data should be sign extended.
    pub fn sign_extend(&self) -> bool {
        self.0.sign_extend
    }

    /// Register number of the Rt operand of the faulting instruction.
    pub fn reg(&self) -> u8 {
        self.0.xt
    }

    /// For AccessType::Write this is the data that was being written.
    pub fn data(&self) -> Option<u64> {
        match self.access_type() {
            guest::AccessType::Read => None,
            guest::AccessType::Write => Some(self.0.data),
        }
    }

    /// Type of access (read or write).
    pub fn access_type(&self) -> guest::AccessType {
        match self.0.read {
            true => guest::AccessType::Read,
            false => guest::AccessType::Write,
        }
    }
}

#[cfg(target_arch = "x86_64")]
impl GuestMemPacket {
    /// Specifies the default operand size encoded by the CS descriptor.
    ///
    /// Returns a `None` should it find an invalid size in the packet.
    pub fn default_operand_size(&self) -> Option<guest::CSDefaultOperandSize> {
        // TODO: use try_from when it is stable.
        match self.0.default_operand_size {
            2 => Some(guest::CSDefaultOperandSize::Bits16),
            4 => Some(guest::CSDefaultOperandSize::Bits32),
            _ => None,
        }
    }

    /// Retrieves the instruction that caused this trap.
    ///
    /// Returns a `None` if the packet is corrupt and  claims to have an instruction that is too
    /// long.
    pub fn instruction(&self) -> Option<&[u8]> {
        self.0.inst_buf.get(0..self.0.inst_len.into())
    }
}

impl GuestIoPacket {
    /// First port number of the attempted access
    pub fn port(&self) -> u16 {
        self.0.port
    }

    /// Size of the access.
    ///
    /// Returns `None` should it find an invalid size in the packet.
    pub fn access_size(&self) -> Option<guest::PortAccessSize> {
        match self.0.access_size {
            1 => Some(guest::PortAccessSize::Bits8),
            2 => Some(guest::PortAccessSize::Bits16),
            4 => Some(guest::PortAccessSize::Bits32),
            _ => None,
        }
    }

    /// Type of access (read or write).
    pub fn access_type(&self) -> guest::AccessType {
        match self.0.input {
            true => guest::AccessType::Read,
            false => guest::AccessType::Write,
        }
    }

    /// For `PortAccessType::Write` this is the data that was being written.
    pub fn data(&self) -> Option<guest::PortData> {
        #[repr(C)]
        union DataUnion {
            bit8: [u8; 4],
            bit16: [u16; 2],
            bit32: [u32; 1],
        }
        if let guest::AccessType::Write = self.access_type() {
            unsafe {
                let data = &*(self.0.data.as_ptr() as *const DataUnion);
                self.access_size().map(|size| match size {
                    guest::PortAccessSize::Bits8 => guest::PortData::Data8(data.bit8[0]),
                    guest::PortAccessSize::Bits16 => guest::PortData::Data16(data.bit16[0]),
                    guest::PortAccessSize::Bits32 => guest::PortData::Data32(data.bit32[0]),
                })
            }
        } else {
            None
        }
    }
}

impl Port {
    /// Create an IO port, allowing IO packets to be read and enqueued.
    ///
    /// Wraps the
    /// [zx_port_create](https://fuchsia.dev/fuchsia-src/reference/syscalls/port_create.md)
    /// syscall.
    pub fn create() -> Result<Port, Status> {
        unsafe {
            let mut handle = 0;
            let opts = 0;
            let status = sys::zx_port_create(opts, &mut handle);
            ok(status)?;
            Ok(Handle::from_raw(handle).into())
        }
    }

    /// Attempt to queue a user packet to the IO port.
    ///
    /// Wraps the
    /// [zx_port_queue](https://fuchsia.dev/fuchsia-src/reference/syscalls/port_queue.md)
    /// syscall.
    pub fn queue(&self, packet: &Packet) -> Result<(), Status> {
        let status = unsafe {
            sys::zx_port_queue(self.raw_handle(), &packet.0 as *const sys::zx_port_packet_t)
        };
        ok(status)
    }

    /// Wait for a packet to arrive on a (V2) port.
    ///
    /// Wraps the
    /// [zx_port_wait](https://fuchsia.dev/fuchsia-src/reference/syscalls/port_wait.md)
    /// syscall.
    pub fn wait(&self, deadline: Time) -> Result<Packet, Status> {
        let mut packet = Default::default();
        let status = unsafe {
            sys::zx_port_wait(
                self.raw_handle(),
                deadline.into_nanos(),
                &mut packet as *mut sys::zx_port_packet_t,
            )
        };
        ok(status)?;
        Ok(Packet(packet))
    }

    /// Cancel pending wait_async calls for an object with the given key.
    ///
    /// Wraps the
    /// [zx_port_cancel](https://fuchsia.dev/fuchsia-src/reference/syscalls/port_cancel.md)
    /// syscall.
    pub fn cancel<H>(&self, source: &H, key: u64) -> Result<(), Status>
    where
        H: HandleBased,
    {
        let status = unsafe { sys::zx_port_cancel(self.raw_handle(), source.raw_handle(), key) };
        ok(status)
    }
}

/// Options for wait_async.
#[repr(u32)]
#[derive(Debug, Copy, Clone, Eq, PartialEq)]
pub enum WaitAsyncOpts {
    Once = sys::ZX_WAIT_ASYNC_ONCE,
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{DurationNum, Event};

    #[test]
    fn port_basic() {
        let ten_ms = 10.millis();

        let port = Port::create().unwrap();

        // Waiting now should time out.
        assert_eq!(port.wait(Time::after(ten_ms)), Err(Status::TIMED_OUT));

        // Send a valid packet.
        let packet = Packet::from_user_packet(42, 123, UserPacket::from_u8_array([13; 32]));
        assert!(port.queue(&packet).is_ok());

        // Waiting should succeed this time. We should get back the packet we sent.
        let read_packet = port.wait(Time::after(ten_ms)).unwrap();
        assert_eq!(read_packet, packet);
    }

    #[test]
    fn wait_async_once() {
        let ten_ms = 10.millis();
        let key = 42;

        let port = Port::create().unwrap();
        let event = Event::create().unwrap();

        assert!(event
            .wait_async_handle(&port, key, Signals::USER_0 | Signals::USER_1, WaitAsyncOpts::Once)
            .is_ok());

        // Waiting without setting any signal should time out.
        assert_eq!(port.wait(Time::after(ten_ms)), Err(Status::TIMED_OUT));

        // If we set a signal, we should be able to wait for it.
        assert!(event.signal_handle(Signals::NONE, Signals::USER_0).is_ok());
        let read_packet = port.wait(Time::after(ten_ms)).unwrap();
        assert_eq!(read_packet.key(), key);
        assert_eq!(read_packet.status(), 0);
        match read_packet.contents() {
            PacketContents::SignalOne(sig) => {
                assert_eq!(sig.trigger(), Signals::USER_0 | Signals::USER_1);
                assert_eq!(sig.observed(), Signals::USER_0);
                assert_eq!(sig.count(), 1);
            }
            _ => panic!("wrong packet type"),
        }

        // Shouldn't get any more packets.
        assert_eq!(port.wait(Time::after(ten_ms)), Err(Status::TIMED_OUT));

        // Calling wait_async again should result in another packet.
        assert!(event.wait_async_handle(&port, key, Signals::USER_0, WaitAsyncOpts::Once).is_ok());
        let read_packet = port.wait(Time::after(ten_ms)).unwrap();
        assert_eq!(read_packet.key(), key);
        assert_eq!(read_packet.status(), 0);
        match read_packet.contents() {
            PacketContents::SignalOne(sig) => {
                assert_eq!(sig.trigger(), Signals::USER_0);
                assert_eq!(sig.observed(), Signals::USER_0);
                assert_eq!(sig.count(), 1);
            }
            _ => panic!("wrong packet type"),
        }

        // Calling wait_async_handle then cancel, we should not get a packet as cancel will
        // remove it from  the queue.
        assert!(event.wait_async_handle(&port, key, Signals::USER_0, WaitAsyncOpts::Once).is_ok());
        assert!(port.cancel(&event, key).is_ok());
        assert_eq!(port.wait(Time::after(ten_ms)), Err(Status::TIMED_OUT));

        // If the event is signalled after the cancel, we also shouldn't get a packet.
        assert!(event.signal_handle(Signals::USER_0, Signals::NONE).is_ok()); // clear signal
        assert!(event.wait_async_handle(&port, key, Signals::USER_0, WaitAsyncOpts::Once).is_ok());
        assert!(port.cancel(&event, key).is_ok());
        assert!(event.signal_handle(Signals::NONE, Signals::USER_0).is_ok());
        assert_eq!(port.wait(Time::after(ten_ms)), Err(Status::TIMED_OUT));
    }
}
