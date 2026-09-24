// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Smoke detectors report the fire alarm as an IAS zone status change. Two
// definitions (Tuya TS0205, Xiaomi JTYJ-GD-01LM/BW) had no IAS converter wired,
// so a real alarm decoded to nothing. Feed each the frame the device sends.
#include <cassert>
#include <cstdint>
#include <span>

#include "zhc/zcl/decoder.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya { extern const PreparedDefinition kDefTS0205; }
namespace zhc::devices::lumi { extern const PreparedDefinition kDefJTYJGD01LM; }

using namespace zhc;

static bool smoke_for(const PreparedDefinition& def, std::uint16_t zonestatus, bool& smoke) {
    // ZCL: cluster-specific, server->client, disable default response; cmd 0x00
    // zoneStatusChangeNotification: zonestatus u16 LE, ext status, zone id, delay u16.
    const std::uint8_t zcl[] = { 0x19, 0x42, 0x00,
        static_cast<std::uint8_t>(zonestatus), static_cast<std::uint8_t>(zonestatus >> 8),
        0x00, 0x00, 0x00, 0x00 };
    InboundApsFrame raw{};
    raw.cluster_id = 0x0500; raw.src_endpoint = 1; raw.dst_endpoint = 1;
    raw.data = std::span<const std::uint8_t>(zcl, sizeof(zcl));
    DecodedMessage msg{};
    if (!decode_frame(raw, {}, msg)) return false;
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    const Value* v = r.merged.find("smoke");
    if (!v || v->type != ValueType::Bool) return false;
    smoke = v->b;
    return true;
}

int main() {
    for (const auto* def : { &devices::tuya::kDefTS0205, &devices::lumi::kDefJTYJGD01LM }) {
        bool smoke = false;
        assert(smoke_for(*def, 0x0001, smoke) && smoke);     // alarm_1 set
        assert(smoke_for(*def, 0x0000, smoke) && !smoke);    // clear
    }
    return 0;
}
