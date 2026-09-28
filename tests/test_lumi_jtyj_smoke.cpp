// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Xiaomi JTYJ-GD-01LM/BW (lumi.sensor_smoke). Pins what the
// alarm-only port lacked: `test` (zoneStatus bit 1), `smoke_density`
// (MI-struct tag 100), and the two writes — `sensitivity` and `selftest` —
// both a u32 on ssIasZone attr 0xFFF1 with the Lumi manufacturer code.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lumi.ts #JTYJ-GD-01LM/BW;
//             lib/lumi.ts lumi_smoke, numericAttributes2Payload case "100",
//             lumi_sensitivity, lumi_selftest.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefJTYJGD01LM;
}  // namespace zhc::devices::lumi

using namespace zhc;

namespace {

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

// Encodes `key` = `label`; checks the manu-specific ssIasZone 0xFFF1 u32 write.
void expect_fff1_write(const char* key, const char* label, std::uint32_t want) {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::StringRef; v.str = label;
    const auto r = dispatch_to_zigbee(devices::lumi::kDefJTYJGD01LM, key, v, ctx, frame);
    assert(r.ok && r.cluster_id == 0x0500);
    const std::uint8_t hdr[] = {0x14, 0x5F, 0x11, 0x00, 0x02, 0xF1, 0xFF, 0x23};
    assert(r.frame_size == sizeof(hdr) + 4);
    assert(std::memcmp(frame, hdr, sizeof(hdr)) == 0);
    const std::uint32_t got = frame[8] | (frame[9] << 8) | (frame[10] << 16) |
                              (static_cast<std::uint32_t>(frame[11]) << 24);
    assert(got == want);
}

}  // namespace

int main() {
    const auto& def = devices::lumi::kDefJTYJGD01LM;

    for (const char* k : {"smoke", "test", "smoke_density", "sensitivity", "selftest"})
        assert(find_expose(def, k));
    const Expose* sens = find_expose(def, "sensitivity");
    assert(sens->type == ExposeType::Enum && sens->enum_count == 3 &&
           sens->access == Access::StateSet);
    assert(find_expose(def, "selftest")->access == Access::Set);

    // zoneStatusChangeNotification, zoneStatus = 0x0003 (alarm_1 + test bit).
    {
        const std::uint8_t zcl[] = {0x19, 0x42, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};
        auto r = dispatch_zcl(def, 0x0500, zcl);
        const Value* s = r.merged.find("smoke");
        const Value* t = r.merged.find("test");
        assert(s && s->type == ValueType::Bool && s->b);
        assert(t && t->type == ValueType::Bool && t->b);
    }
    {
        const std::uint8_t zcl[] = {0x19, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
        auto r = dispatch_zcl(def, 0x0500, zcl);
        const Value* t = r.merged.find("test");
        assert(t && t->type == ValueType::Bool && !t->b);
    }

    // genBasic 0xFF01 MI-struct: tag 1 battery mV 2950, tag 0x64 u8 density 5.
    {
        const std::uint8_t zcl[] = {0x18, 0x44, 0x0A, 0x01, 0xFF, 0x42, 0x07,
                                    0x01, 0x21, 0x86, 0x0B,     // tag 1 u16 2950
                                    0x64, 0x20, 0x05};          // tag 100 u8 5
        auto r = dispatch_zcl(def, 0x0000, zcl);
        const Value* d = r.merged.find("smoke_density");
        assert(d && d->type == ValueType::Uint && d->u == 5);
        const Value* b = r.merged.find("battery");
        assert(b && b->u == 66);
    }

    // Writes (35 s timeout in z2m is a transport detail, not ported).
    expect_fff1_write("sensitivity", "low",    0x04010000);
    expect_fff1_write("sensitivity", "medium", 0x04020000);
    expect_fff1_write("sensitivity", "high",   0x04030000);
    expect_fff1_write("selftest",    "",       0x03010000);
    return 0;
}
