// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/zcl/foundation.hpp"
#include "zhc/zcl/header.hpp"

using namespace zhc;

static void test_header_basic() {
    // fc=0x18 (global, server->client, disable default) tsn=0x42 cmd=0x0A
    const std::uint8_t frame[] = {0x18, 0x42, 0x0A};
    ZclHeader h{};
    std::size_t hlen = 0;
    bool ok = parse_zcl_header(frame, h, hlen);
    assert(ok);
    assert(hlen == 3);
    assert(h.cluster_specific == false);
    assert(h.direction_server_to_client == true);
    assert(h.disable_default_response == true);
    assert(h.manufacturer_specific == false);
    assert(h.manufacturer_code == 0);
    assert(h.transaction_sequence == 0x42);
    assert(h.command_id == 0x0A);
}

static void test_header_manufacturer_specific() {
    // fc=0x1C (global, manu, s->c, disable default), manu=0x115F (LUMI),
    // tsn=0x01, cmd=0x0A
    const std::uint8_t frame[] = {0x1C, 0x5F, 0x11, 0x01, 0x0A};
    ZclHeader h{};
    std::size_t hlen = 0;
    bool ok = parse_zcl_header(frame, h, hlen);
    assert(ok);
    assert(hlen == 5);
    assert(h.manufacturer_specific == true);
    assert(h.manufacturer_code == 0x115F);
    assert(h.command_id == 0x0A);
}

static void test_report_multi_record() {
    // Two records: (0x0000, bool 0x10, 0x01), (0x0021, u8 0x20, 0x55)
    const std::uint8_t payload[] = {
        0x00, 0x00, 0x10, 0x01,
        0x21, 0x00, 0x20, 0x55,
    };
    const AttrKeyEntry known[] = {
        {0x0000, "state"},
        {0x0021, "battery"},
    };
    char scratch[64];
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    bool ok = parse_report_attributes(payload, known, scratch, sizeof(scratch), out);
    assert(ok);
    assert(out.count == 2);

    const Value* state = out.find("state");
    assert(state && state->type == ValueType::Bool && state->b == true);
    const Value* batt = out.find("battery");
    assert(batt && batt->type == ValueType::Uint && batt->u == 0x55);
}

static void test_report_unknown_attr_is_decimal_key() {
    // Unknown attr_id = 0x8000 (32768) with u8 value 0x03.
    const std::uint8_t payload[] = {0x00, 0x80, 0x20, 0x03};
    char scratch[64];
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    bool ok = parse_report_attributes(payload, {}, scratch, sizeof(scratch), out);
    assert(ok);
    assert(out.count == 1);
    assert(std::strcmp(out.items[0].key, "32768") == 0);
    assert(out.items[0].value.u == 0x03);
}

static void test_read_attr_response_mixes_success_and_failure() {
    // Record 1: attr=0x0000, status=0x00, type=u16(0x21), value=0x1234.
    // Record 2: attr=0x0042, status=0x86 (unsupported) — no type/value.
    const std::uint8_t payload[] = {
        0x00, 0x00, 0x00, 0x21, 0x34, 0x12,
        0x42, 0x00, 0x86,
    };
    const AttrKeyEntry known[] = {
        {0x0000, "model_id_len"},
    };
    char scratch[64];
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    bool ok = parse_read_attr_response(payload, known, scratch, sizeof(scratch), out);
    assert(ok);
    assert(out.count == 1);
    const Value* v = out.find("model_id_len");
    assert(v && v->type == ValueType::Uint && v->u == 0x1234);
}

static void test_mi_struct_parses_tagged_tlv() {
    // Two tags: tag=0x01 type=u16 value=0x0BA8 (2984 mV), tag=0x03 type=i8 value=29.
    const std::uint8_t struct_bytes[] = {
        0x01, 0x21, 0xA8, 0x0B,
        0x03, 0x28, 0x1D,
    };
    char scratch[32];
    FixedPayload<ZHC_MI_STRUCT_CAP> arena{};
    bool ok = parse_mi_struct(struct_bytes, scratch, sizeof(scratch), arena);
    assert(ok);
    assert(arena.count == 2);
    const Value* voltage = arena.find("1");
    assert(voltage && voltage->type == ValueType::Uint && voltage->u == 0x0BA8);
    const Value* temp = arena.find("3");
    assert(temp && temp->type == ValueType::Int && temp->i == 29);
}

static void test_tuya_dp_stream() {
    // dp_id=1, dp_type=0x01 (bool), len=1, value=0x01
    // dp_id=2, dp_type=0x02 (numeric), len=4, value=0x00 0x00 0x00 0x2A
    const std::uint8_t payload[] = {
        0x01, 0x01, 0x00, 0x01, 0x01,
        0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x2A,
    };
    TuyaDpRecord records[8];
    std::size_t count = 0;
    bool ok = parse_tuya_dp_stream(payload, records, count);
    assert(ok);
    assert(count == 2);
    assert(records[0].dp_id == 1 && records[0].dp_type == 0x01 &&
           records[0].value.size() == 1 && records[0].value[0] == 0x01);
    assert(records[1].dp_id == 2 && records[1].dp_type == 0x02 &&
           records[1].value.size() == 4 && records[1].value[3] == 0x2A);
}

int main() {
    test_header_basic();
    test_header_manufacturer_specific();
    test_report_multi_record();
    test_report_unknown_attr_is_decimal_key();
    test_read_attr_response_mixes_success_and_failure();
    test_mi_struct_parses_tagged_tlv();
    test_tuya_dp_stream();
    return 0;
}
