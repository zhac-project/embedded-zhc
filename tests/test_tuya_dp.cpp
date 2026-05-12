// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T0.2: Tuya DP value decode tests.
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/zcl/tuya_dp.hpp"

using namespace zhc;

namespace {
TuyaDpRecord make(std::uint8_t dp, std::uint8_t type,
                   std::span<const std::uint8_t> v) {
    return TuyaDpRecord{ dp, type, v };
}
}  // namespace

static void test_bool() {
    Value out{};
    const std::uint8_t b[] = { 0x01 };
    assert(decode_tuya_dp(make(1, 0x01, b), out));
    assert(out.type == ValueType::Bool && out.b == true);

    const std::uint8_t b0[] = { 0x00 };
    assert(decode_tuya_dp(make(1, 0x01, b0), out));
    assert(out.b == false);
}

static void test_numeric_big_endian() {
    // 0x01020304 — non-palindromic so endian flip would not silently pass.
    const std::uint8_t b[] = { 0x01, 0x02, 0x03, 0x04 };
    Value out{};
    assert(decode_tuya_dp(make(2, 0x02, b), out));
    assert(out.type == ValueType::Int);
    assert(out.i == 0x01020304);
}

static void test_numeric_negative() {
    // 0xFFFFFFFF → -1 signed.
    const std::uint8_t b[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    Value out{};
    assert(decode_tuya_dp(make(3, 0x02, b), out));
    assert(out.type == ValueType::Int && out.i == -1);
}

static void test_enum() {
    const std::uint8_t b[] = { 0x03 };
    Value out{};
    assert(decode_tuya_dp(make(4, 0x04, b), out));
    assert(out.type == ValueType::Uint && out.u == 3);
}

static void test_bitmap_1_2_4() {
    const std::uint8_t b1[] = { 0x81 };
    const std::uint8_t b2[] = { 0x12, 0x34 };
    const std::uint8_t b4[] = { 0xAA, 0xBB, 0xCC, 0xDD };
    Value out{};
    assert(decode_tuya_dp(make(5, 0x05, b1), out));
    assert(out.type == ValueType::Uint && out.u == 0x81);

    assert(decode_tuya_dp(make(5, 0x05, b2), out));
    assert(out.u == 0x1234);

    assert(decode_tuya_dp(make(5, 0x05, b4), out));
    assert(out.u == 0xAABBCCDDull);
}

static void test_string_surfaces_as_bytes() {
    const std::uint8_t b[] = { 'h', 'i' };
    Value out{};
    assert(decode_tuya_dp(make(6, 0x03, b), out));
    assert(out.type == ValueType::BytesRef);
    assert(out.bytes.size() == 2);
    assert(out.bytes[0] == 'h');
}

static void test_raw_surfaces_as_bytes() {
    const std::uint8_t b[] = { 0xDE, 0xAD };
    Value out{};
    assert(decode_tuya_dp(make(7, 0x00, b), out));
    assert(out.type == ValueType::BytesRef);
    assert(out.bytes.size() == 2 && out.bytes[1] == 0xAD);
}

static void test_malformed_rejected() {
    Value out{};
    // bool with 2 bytes
    const std::uint8_t b[] = { 0x01, 0x00 };
    assert(!decode_tuya_dp(make(8, 0x01, b), out));
    // numeric with 3 bytes
    const std::uint8_t b3[] = { 0x01, 0x02, 0x03 };
    assert(!decode_tuya_dp(make(8, 0x02, b3), out));
    // bitmap with 3 bytes (not in {1,2,4})
    assert(!decode_tuya_dp(make(8, 0x05, b3), out));
}

int main() {
    test_bool();
    test_numeric_big_endian();
    test_numeric_negative();
    test_enum();
    test_bitmap_1_2_4();
    test_string_surfaces_as_bytes();
    test_raw_surfaces_as_bytes();
    test_malformed_rejected();
    return 0;
}
