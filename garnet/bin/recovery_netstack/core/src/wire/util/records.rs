// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//! Utilities for parsing sequential records.
//!
//! This module provides utilities for parsing sequential records. IGMP message
//! parsing is built using these utilities, as is another set of general
//! utilities for IPv4, TCP, and NDP options parsing, provided in the
//! [`options`] submodule.
//!
//! [`options`]: crate::wire::util::records::options

use packet::BufferView;
use std::ops::Deref;
use zerocopy::ByteSlice;

/// A parsed set of arbitrary sequential records.
///
/// `Records` represents a pre-parsed set of records whose structure is enforced
/// by the impl in `O`.
#[derive(Debug)]
pub(crate) struct Records<B, R: RecordsImplLimit> {
    bytes: B,
    limit: R::Limit,
}

/// An iterator over the records contained inside a `Records` instance.
pub(crate) struct RecordsIter<'a, R: RecordsImpl<'a>> {
    bytes: &'a [u8],
    limit: R::Limit,
}

/// Trait that specifies the type of errors to emit
/// when parsing sequential records.
pub(crate) trait RecordsImplErr {
    type Error;
}

/// Trait that provides a way to limit the amount of records read from a buffer.
/// Some protocols will have some sort of header preceding the records that will
/// indicate the number of records to follow (e.g. igmp), while others will have
/// that information inline (e.g. IP options).
///
/// This crate provides `usize` and `()` implementations for `RecordsLimit` that
/// can be provided as the `Limit` type in the `RecordsImpl` trait. The `usize`
/// impl will limit to `n` records read, while `()` will not impose any limit.
pub(crate) trait RecordsLimit {
    /// Decrements the current value held by 1. Implementers should `panic` if
    /// `decrement` is called when `has_more` would've returned `false`.
    fn decrement(&mut self);
    /// Returns whether more records are available. For a simple counter
    /// implementation, this will return if counter is > 0.
    fn has_more(&self) -> bool;
}

impl RecordsLimit for () {
    fn decrement(&mut self) {}

    fn has_more(&self) -> bool {
        true
    }
}

impl RecordsLimit for usize {
    fn decrement(&mut self) {
        *self = self.checked_sub(1).expect("Can't decrement counter below 0");
    }

    fn has_more(&self) -> bool {
        *self > 0
    }
}

/// Trait that provides record limiting to `RecordsImpl` implementers.
pub(crate) trait RecordsImplLimit {
    /// A limit type that can be used to limit record-reading. See the
    /// `RecordsLimit` trait.
    type Limit: Sized + RecordsLimit + Clone;
}

/// An implementation of a records parser.
///
/// `RecordsImpl` provides functions to parse sequential records. It is required
///  in order to construct a `Records` or `RecordsIter`.
pub(crate) trait RecordsImpl<'a>: RecordsImplErr + RecordsImplLimit {
    /// The type of a single record; the output from the `parse` function.
    ///
    /// For long or variable-length data, the user is advised to make `Record` a
    /// reference into the bytes passed to `parse`. This is achievable because
    /// of the lifetime parameter to this trait.
    type Record;

    /// If Some(Self::Error), will emit the provided constant as an error if the
    /// provided buffer is exhausted while `Self::Limit` still reports
    /// `has_more` as `true`.
    const EXACT_LIMIT_ERROR: Option<Self::Error> = None;

    /// Parse a record.
    ///
    /// `parse` takes a kind byte and variable-length data associated and
    /// returns `Ok(Some(o))` if the the record is successfully parsed as `o`,
    /// `Ok(None)` if data is not malformed but the implementer can't extract a
    /// concrete object (e.g. record is an unimplemented enumeration, but we can
    /// still safely "skip" it), and `Err(err)` if the `data` was malformed for
    /// the attempted record parsing.
    ///
    ///  When returning `Ok(None)` it's the implementer's responsibility to
    ///  nonetheless skip the record (which may not be possible for some
    ///  implementations, in which case it should return an `Err`).
    ///
    /// `parse` must be deterministic, or else `Records::parse` cannot guarantee
    /// that future iterations will not produce errors (and panic).
    fn parse<BV: BufferView<&'a [u8]>>(data: &mut BV) -> Result<Option<Self::Record>, Self::Error>;
}

impl<B, R> Records<B, R>
where
    B: ByteSlice,
    R: for<'a> RecordsImpl<'a>,
{
    /// Parse a set of records with an informed limit.
    ///
    /// `parse_with_limit` parses `bytes` as a sequence of records. `limit` is
    /// used to specify how many records are expected to be parsed.
    ///
    /// If `limit` is a `usize`, then `parse_with_limit` will stop after having
    /// found that many records. If `R::EXACT_LIMIT_ERROR` is provided, that
    /// error will be returned in case `bytes` is exhausted and the limit hasn't
    /// been reached OR the limit has been reached and there are leftover bytes.
    ///
    /// If `limit` is a `()`, then it will be ignored, and records will be
    /// parsed until `bytes` is exhausted.
    ///
    /// `parse_with_limit` performs a single pass over all of the records to
    /// verify that they are well-formed. Once `parse_with_limit` returns
    /// successfully, the resulting `Records` can be used to construct
    /// infallible iterators.
    pub(crate) fn parse_with_limit(bytes: B, limit: R::Limit) -> Result<Records<B, R>, R::Error> {
        // First, do a single pass over the bytes to detect any errors up front.
        // Once this is done, since we have a reference to `bytes`, these bytes
        // can't change out from under us, and so we can treat any iterator over
        // these bytes as infallible. This makes a few assumptions, but none of
        // them are that big of a deal. In all cases, breaking these assumptions
        // would just result in a runtime panic.
        // - B could return different bytes each time
        // - R::parse could be non-deterministic
        let mut lim = limit.clone();
        let mut b = LongLivedBuff::new(bytes.deref());
        while next::<_, R>(&mut b, &mut lim)?.is_some() {}
        Ok(Records { bytes, limit })
    }
}

impl<B, R> Records<B, R>
where
    B: ByteSlice,
    R: for<'a> RecordsImpl<'a, Limit = ()>,
{
    /// Parses a set of records.
    ///
    /// Equivalent to calling `parse_with_limit` with `limit = ()`.
    pub(crate) fn parse(bytes: B) -> Result<Records<B, R>, R::Error> {
        Self::parse_with_limit(bytes, ())
    }
}

impl<B: Deref<Target = [u8]>, R> Records<B, R>
where
    R: for<'a> RecordsImpl<'a>,
{
    /// Get the underlying bytes.
    ///
    /// `bytes` returns a reference to the byte slice backing this `Options`.
    pub(crate) fn bytes(&self) -> &[u8] {
        &self.bytes
    }
}

impl<'a, B, R> Records<B, R>
where
    B: 'a + ByteSlice,
    R: RecordsImpl<'a>,
{
    /// Create an iterator over options.
    ///
    /// `iter` constructs an iterator over the records. Since the records were
    /// validated in `parse`, then so long as `from_kind` and `from_data` are
    /// deterministic, the iterator is infallible.
    pub(crate) fn iter(&'a self) -> RecordsIter<'a, R> {
        RecordsIter { bytes: &self.bytes, limit: self.limit.clone() }
    }
}

impl<'a, R> Iterator for RecordsIter<'a, R>
where
    R: RecordsImpl<'a>,
{
    type Item = R::Record;

    fn next(&mut self) -> Option<R::Record> {
        let mut bytes = LongLivedBuff::new(self.bytes);
        // use match rather than expect because expect requires that Err: Debug
        #[allow(clippy::match_wild_err_arm)]
        let result = match next::<_, R>(&mut bytes, &mut self.limit) {
            Ok(o) => o,
            Err(_) => panic!("already-validated options should not fail to parse"),
        };
        self.bytes = bytes.into_rest();
        result
    }
}

/// Gets the next entry for a set of sequential records in `bytes`.
///
/// On return, `bytes` will be pointing to the start of where a next record
/// would be.
fn next<'a, BV, R>(bytes: &mut BV, limit: &mut R::Limit) -> Result<Option<R::Record>, R::Error>
where
    R: RecordsImpl<'a>,
    BV: BufferView<&'a [u8]>,
{
    loop {
        // if we run out of bytes or limit is exhausted, we stop the parsing
        // here.
        if bytes.is_empty() || !limit.has_more() {
            // If only one of the conditions is met and R::EXACT_LIMIT_ERROR is
            // specified, we return the exact limit error.
            return match R::EXACT_LIMIT_ERROR {
                Some(_) if bytes.is_empty() ^ !limit.has_more() => {
                    Err(R::EXACT_LIMIT_ERROR.unwrap())
                }
                _ => Ok(None),
            };
        }

        limit.decrement();
        match R::parse(bytes) {
            Ok(Some(o)) => return Ok(Some(o)),
            Ok(None) => {}
            Err(err) => return Err(err),
        }
    }
}

/// A wrapper around the implementation of `BufferView` for slices.
///
/// `LongLivedBuff` is a thin wrapper around `&[u8]` meant to provide an
/// implementation of `BufferView` that returns slices tied to the same lifetime
/// as the slice that `LongLivedBuff` was created with. This is in contrast to
/// the more widely used `&'b mut &'a [u8]` `BufferView` implementer that
/// returns slice references tied to lifetime `b`.
struct LongLivedBuff<'a>(&'a [u8]);

impl<'a> LongLivedBuff<'a> {
    /// Creates a new `LongLivedBuff` around a slice reference with lifetime
    /// `a`.
    ///
    /// All slices returned by the `BufferView` impl of `LongLivedBuff` are
    /// guaranteed to return slice references tied to the same lifetime `a`.
    pub(crate) fn new(data: &'a [u8]) -> LongLivedBuff<'a> {
        LongLivedBuff::<'a>(data)
    }
}

impl<'a> AsRef<[u8]> for LongLivedBuff<'a> {
    fn as_ref(&self) -> &[u8] {
        self.0
    }
}

impl<'a> packet::BufferView<&'a [u8]> for LongLivedBuff<'a> {
    fn take_front(&mut self, n: usize) -> Option<&'a [u8]> {
        if self.0.len() >= n {
            let (prefix, rest) = std::mem::replace(&mut self.0, &[]).split_at(n);
            std::mem::replace(&mut self.0, rest);
            Some(prefix)
        } else {
            None
        }
    }

    fn take_back(&mut self, n: usize) -> Option<&'a [u8]> {
        if self.0.len() >= n {
            let split = self.0.len() - n;
            let (rest, suffix) = std::mem::replace(&mut self.0, &[]).split_at(n);
            std::mem::replace(&mut self.0, rest);
            Some(suffix)
        } else {
            None
        }
    }

    fn into_rest(self) -> &'a [u8] {
        self.0
    }
}

#[cfg(test)]
mod test {
    use super::*;
    use packet::BufferView;
    use zerocopy::{AsBytes, FromBytes, LayoutVerified, Unaligned};

    const DUMMY_BYTES: [u8; 16] = [
        0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03,
        0x04,
    ];

    #[derive(Debug, AsBytes, FromBytes, Unaligned)]
    #[repr(C)]
    struct DummyRecord {
        a: [u8; 2],
        b: u8,
        c: u8,
    }

    fn parse_dummy_rec<'a, BV>(
        data: &mut BV,
    ) -> Result<Option<LayoutVerified<&'a [u8], DummyRecord>>, ()>
    where
        BV: BufferView<&'a [u8]>,
    {
        match data.take_obj_front::<DummyRecord>() {
            Some(res) => Ok(Some(res)),
            None => Err(()),
        }
    }

    #[derive(Debug)]
    struct LimitlessRecordImpl;

    impl RecordsImplLimit for LimitlessRecordImpl {
        type Limit = ();
    }

    impl RecordsImplErr for LimitlessRecordImpl {
        type Error = ();
    }

    impl<'a> RecordsImpl<'a> for LimitlessRecordImpl {
        type Record = LayoutVerified<&'a [u8], DummyRecord>;

        fn parse<BV: BufferView<&'a [u8]>>(
            data: &mut BV,
        ) -> Result<Option<Self::Record>, Self::Error> {
            parse_dummy_rec(data)
        }
    }

    #[derive(Debug)]
    struct LimitedRecordImpl;

    impl RecordsImplLimit for LimitedRecordImpl {
        type Limit = usize;
    }

    impl RecordsImplErr for LimitedRecordImpl {
        type Error = ();
    }

    impl<'a> RecordsImpl<'a> for LimitedRecordImpl {
        type Record = LayoutVerified<&'a [u8], DummyRecord>;

        fn parse<BV: BufferView<&'a [u8]>>(
            data: &mut BV,
        ) -> Result<Option<Self::Record>, Self::Error> {
            parse_dummy_rec(data)
        }
    }

    #[derive(Debug)]
    struct ExactLimitRecordImpl;

    impl RecordsImplLimit for ExactLimitRecordImpl {
        type Limit = usize;
    }

    impl RecordsImplErr for ExactLimitRecordImpl {
        type Error = ();
    }

    impl<'a> RecordsImpl<'a> for ExactLimitRecordImpl {
        type Record = LayoutVerified<&'a [u8], DummyRecord>;
        const EXACT_LIMIT_ERROR: Option<Self::Error> = Some(());

        fn parse<BV: BufferView<&'a [u8]>>(
            data: &mut BV,
        ) -> Result<Option<Self::Record>, Self::Error> {
            parse_dummy_rec(data)
        }
    }

    fn check_parsed_record(rec: &DummyRecord) {
        assert_eq!(rec.a[0], 0x01);
        assert_eq!(rec.a[1], 0x02);
        assert_eq!(rec.b, 0x03);
    }

    #[test]
    fn all_records_parsing() {
        let parsed = Records::<_, LimitlessRecordImpl>::parse(&DUMMY_BYTES[..]).unwrap();
        assert_eq!(parsed.iter().count(), 4);
        for rec in parsed.iter() {
            check_parsed_record(rec.deref());
        }
    }

    #[test]
    fn limit_records_parsing() {
        let limit: usize = 2;
        let parsed =
            Records::<_, LimitedRecordImpl>::parse_with_limit(&DUMMY_BYTES[..], limit).unwrap();
        assert_eq!(parsed.iter().count(), limit);
        for rec in parsed.iter() {
            check_parsed_record(rec.deref());
        }
    }

    #[test]
    fn exact_limit_records_parsing() {
        Records::<_, ExactLimitRecordImpl>::parse_with_limit(&DUMMY_BYTES[..], 2)
            .expect_err("fails if all the buffer hasn't been parsed");
        Records::<_, ExactLimitRecordImpl>::parse_with_limit(&DUMMY_BYTES[..], 5)
            .expect_err("fails if can't extract enough records");
    }
}

/// Header options for IPv4 and TCP, and NDP.
///
/// This module provides a parser for the options formats used by IPv4, TCP, and
/// NDP. These formats are not identical, but share enough in common that they
/// can be implemented using the same utility with a bit of customization.
pub(crate) mod options {
    use super::*;
    use packet::BufferView;

    /// A parsed set of header options.
    ///
    /// `Options` represents a parsed set of options from an IPv4 or TCP header
    /// or an NDP packet. `Options` uses [`Records`] below the surface.
    ///
    /// [`Records`]: crate::wire::util::records::Records
    pub(crate) type Options<B, O> = Records<B, O>;

    impl<'a, O> RecordsImplErr for O
    where
        O: OptionsImpl<'a>,
    {
        type Error = OptionParseErr<O::Error>;
    }

    impl<'a, O> RecordsImplLimit for O
    where
        O: OptionsImpl<'a>,
    {
        type Limit = ();
    }

    impl<'a, O> RecordsImpl<'a> for O
    where
        O: OptionsImpl<'a>,
    {
        type Record = O::Option;

        fn parse<BV: BufferView<&'a [u8]>>(
            data: &mut BV,
        ) -> Result<Option<Self::Record>, Self::Error> {
            next::<_, O>(data)
        }
    }

    /// Errors returned from parsing options.
    ///
    /// `OptionParseErr` is either `Internal`, which indicates that this module
    /// encountered a malformed sequence of options (likely with a length field
    /// larger than the remaining bytes in the options buffer), or `External`,
    /// which indicates that the `OptionsImpl::parse` callback returned an error.
    #[derive(Debug, Eq, PartialEq)]
    pub(crate) enum OptionParseErr<E> {
        Internal,
        External(E),
    }

    // End of Options List in both IPv4 and TCP
    const END_OF_OPTIONS: u8 = 0;

    // NOP in both IPv4 and TCP
    const NOP: u8 = 1;

    /// Common traits of option parsing and serialization.
    ///
    /// This is split from `OptionsImpl` so that the associated types do not
    /// depend on the lifetime parameter to `OptionsImpl` and provide common
    /// behavior to parsers and serializers.
    pub(crate) trait OptionsImplLayout {
        /// The error type that can be returned in Options parsing.
        type Error;

        /// The value to multiply read lengths by.
        ///
        /// By default, this value is 1, but for some protocols (such as NDP)
        /// this may be different.
        const OPTION_LEN_MULTIPLIER: usize = 1;

        /// The End of options type (if one exists).
        const END_OF_OPTIONS: Option<u8> = Some(END_OF_OPTIONS);

        /// The No-op type (if one exists).
        const NOP: Option<u8> = Some(NOP);
    }

    /// An implementation of an options parser.
    ///
    /// `OptionsImpl` provides functions to parse fixed- and variable-length
    /// options. It is required in order to construct an `Options`.
    pub(crate) trait OptionsImpl<'a>: OptionsImplLayout {
        /// The type of an option; the output from the `parse` function.
        ///
        /// For long or variable-length data, the user is advised to make
        /// `Option` a reference into the bytes passed to `parse`. This is
        /// achievable because of the lifetime parameter to this trait.
        type Option;

        /// Parse an option.
        ///
        /// `parse` takes a kind byte and variable-length data associated and
        /// returns `Ok(Some(o))` if the option successfully parsed as `o`,
        /// `Ok(None)` if the kind byte was unrecognized, and `Err(err)` if the
        /// kind byte was recognized but `data` was malformed for that option
        /// kind. `parse` is allowed to not recognize certain option kinds, as
        /// the length field can still be used to safely skip over them.
        ///
        /// `parse` must be deterministic, or else `Options::parse` cannot
        /// guarantee that future iterations will not produce errors (and
        /// panic).
        fn parse(kind: u8, data: &'a [u8]) -> Result<Option<Self::Option>, Self::Error>;
    }

    fn next<'a, BV, O>(bytes: &mut BV) -> Result<Option<O::Option>, OptionParseErr<O::Error>>
    where
        BV: BufferView<&'a [u8]>,
        O: OptionsImpl<'a>,
    {
        // For an explanation of this format, see the "Options" section of
        // https://en.wikipedia.org/wiki/Transmission_Control_Protocol#TCP_segment_structure
        loop {
            let kind = match bytes.take_front(1).map(|x| x[0]) {
                None => return Ok(None),
                Some(k) => {
                    // Can't do pattern matching with associated constants,
                    // so do it the good-ol' way:
                    if Some(k) == O::NOP {
                        continue;
                    } else if Some(k) == O::END_OF_OPTIONS {
                        return Ok(None);
                    }
                    k
                }
            };
            let len = match bytes.take_front(1).map(|x| x[0]) {
                None => return Err(OptionParseErr::Internal),
                Some(len) => (len as usize) * O::OPTION_LEN_MULTIPLIER,
            };

            if len < 2 || (len - 2) > bytes.len() {
                return debug_err!(Err(OptionParseErr::Internal), "option length {} is either too short or longer than the total buffer length of {}", len, bytes.len());
            }
            // we can safely unwrap here since we verified the correct length above
            let option_data = bytes.take_front(len - 2).unwrap();
            match O::parse(kind, option_data) {
                Ok(Some(o)) => return Ok(Some(o)),
                Ok(None) => {}
                Err(err) => return Err(OptionParseErr::External(err)),
            }
        }
    }

    #[cfg(test)]
    mod tests {
        use super::*;

        #[derive(Debug)]
        struct DummyOptionsImpl;

        impl OptionsImplLayout for DummyOptionsImpl {
            type Error = ();
        }

        impl<'a> OptionsImpl<'a> for DummyOptionsImpl {
            type Option = (u8, Vec<u8>);

            fn parse(kind: u8, data: &'a [u8]) -> Result<Option<Self::Option>, Self::Error> {
                let mut v = Vec::new();
                v.extend_from_slice(data);
                Ok(Some((kind, v)))
            }
        }

        #[derive(Debug)]
        struct AlwaysErrOptionsImpl;

        impl OptionsImplLayout for AlwaysErrOptionsImpl {
            type Error = ();
        }

        impl<'a> OptionsImpl<'a> for AlwaysErrOptionsImpl {
            type Option = ();

            fn parse(_kind: u8, _data: &'a [u8]) -> Result<Option<()>, ()> {
                Err(())
            }
        }

        #[derive(Debug)]
        struct DummyNdpOptionsImpl;

        impl OptionsImplLayout for DummyNdpOptionsImpl {
            type Error = ();

            const OPTION_LEN_MULTIPLIER: usize = 8;

            const END_OF_OPTIONS: Option<u8> = None;

            const NOP: Option<u8> = None;
        }

        impl<'a> OptionsImpl<'a> for DummyNdpOptionsImpl {
            type Option = (u8, Vec<u8>);

            fn parse(kind: u8, data: &'a [u8]) -> Result<Option<Self::Option>, Self::Error> {
                let mut v = Vec::with_capacity(data.len());
                v.extend_from_slice(data);
                Ok(Some((kind, v)))
            }
        }

        #[test]
        fn test_empty_options() {
            // all END_OF_OPTIONS
            let bytes = [END_OF_OPTIONS; 64];
            let options = Options::<_, DummyOptionsImpl>::parse(&bytes[..]).unwrap();
            assert_eq!(options.iter().count(), 0);

            // all NOP
            let bytes = [NOP; 64];
            let options = Options::<_, DummyOptionsImpl>::parse(&bytes[..]).unwrap();
            assert_eq!(options.iter().count(), 0);
        }

        #[test]
        fn test_parse() {
            // Construct byte sequences in the pattern [3, 2], [4, 3, 2], [5, 4,
            // 3, 2], etc. The second byte is the length byte, so these are all
            // valid options (with data [], [2], [3, 2], etc).
            let mut bytes = Vec::new();
            for i in 4..16 {
                // from the user's perspective, these NOPs should be transparent
                bytes.push(NOP);
                for j in (2..i).rev() {
                    bytes.push(j);
                }
                // from the user's perspective, these NOPs should be transparent
                bytes.push(NOP);
            }

            let options = Options::<_, DummyOptionsImpl>::parse(bytes.as_slice()).unwrap();
            for (idx, (kind, data)) in options.iter().enumerate() {
                assert_eq!(kind as usize, idx + 3);
                assert_eq!(data.len(), idx);
                let mut bytes = Vec::new();
                for i in (2..(idx + 2)).rev() {
                    bytes.push(i as u8);
                }
                assert_eq!(data, bytes);
            }

            // Test that we get no parse errors so long as
            // AlwaysErrOptionsImpl::parse is never called.
            let bytes = [NOP; 64];
            let options = Options::<_, AlwaysErrOptionsImpl>::parse(&bytes[..]).unwrap();
            assert_eq!(options.iter().count(), 0);
        }

        #[test]
        fn test_parse_ndp_options() {
            let mut bytes = Vec::new();
            for i in 0..16 {
                bytes.push(i);
                // NDP uses len*8 for the actual length.
                bytes.push(i + 1);
                // Write remaining 6 bytes.
                for j in 2..((i + 1) * 8) {
                    bytes.push(j)
                }
            }

            let options = Options::<_, DummyNdpOptionsImpl>::parse(bytes.as_slice()).unwrap();
            for (idx, (kind, data)) in options.iter().enumerate() {
                assert_eq!(kind as usize, idx);
                assert_eq!(data.len(), ((idx + 1) * 8) - 2);
                let mut bytes = Vec::new();
                for i in (2..((idx + 1) * 8)) {
                    bytes.push(i as u8);
                }
                assert_eq!(data, bytes);
            }
        }

        #[test]
        fn test_parse_err() {
            // the length byte is too short
            let bytes = [2, 1];
            assert_eq!(
                Options::<_, DummyOptionsImpl>::parse(&bytes[..]).unwrap_err(),
                OptionParseErr::Internal
            );

            // the length byte is 0 (similar check to above, but worth
            // explicitly testing since this was a bug in the Linux kernel:
            // https://bugzilla.redhat.com/show_bug.cgi?id=1622404)
            let bytes = [2, 0];
            assert_eq!(
                Options::<_, DummyOptionsImpl>::parse(&bytes[..]).unwrap_err(),
                OptionParseErr::Internal
            );

            // the length byte is too long
            let bytes = [2, 3];
            assert_eq!(
                Options::<_, DummyOptionsImpl>::parse(&bytes[..]).unwrap_err(),
                OptionParseErr::Internal
            );

            // the buffer is fine, but the implementation returns a parse error
            let bytes = [2, 2];
            assert_eq!(
                Options::<_, AlwaysErrOptionsImpl>::parse(&bytes[..]).unwrap_err(),
                OptionParseErr::External(())
            );
        }

        #[test]
        fn test_missing_length_bytes() {
            // Construct a sequence with a valid record followed by an
            // incomplete one, where `kind` is specified but `len` is missing.
            // So we can assert that we'll fail cleanly in that case.
            //
            // Added as part of Change-Id
            // Ibd46ac7384c7c5e0d74cb344b48c88876c351b1a
            //
            // Before the small refactor in the Change-Id above, there was a
            // check during parsing that guaranteed that the length of the
            // remaining buffer was >= 1, but it should've been a check for
            // >= 2, and the case below would have caused it to panic while
            // trying to access the length byte, which was a DoS vulnerability.
            Options::<_, DummyOptionsImpl>::parse(&[0x03, 0x03, 0x01, 0x03][..])
                .expect_err("Can detect malformed length bytes");
        }
    }
}
