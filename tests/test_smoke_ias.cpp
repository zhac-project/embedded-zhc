// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Smoke detectors report the fire alarm as an IAS zone status change. Two
// definitions (Tuya TS0205, Xiaomi JTYJ-GD-01LM/BW) had no IAS converter wired,
// so a real alarm decoded to nothing. Feed each the frame the device sends.
//
// TS0205 `_TZ3210_up3pngle` (TS0205_smoke_2) also needs genPowerCfg
// batteryPercentageRemaining reporting configured, which z2m does for that
// manufacturer only (issue 22421 / PR 8004); other TS0205 get none.
#include <cassert>
#include <cstdint>
#include <span>

#include "zhc/devices/tuya_registry.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0205;
extern const PreparedDefinition kDefTS0205_up3pngle;
}
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
    for (const auto* def : { &devices::tuya::kDefTS0205, &devices::tuya::kDefTS0205_up3pngle,
                             &devices::lumi::kDefJTYJGD01LM }) {
        bool smoke = false;
        assert(smoke_for(*def, 0x0001, smoke) && smoke);     // alarm_1 set
        assert(smoke_for(*def, 0x0000, smoke) && !smoke);    // clear
    }

    // _TZ3210_up3pngle gets its own def; the other TS0205 keep the generic one.
    const std::span<const PreparedDefinition* const> reg(devices::tuya::kTuyaRegistry,
                                                         devices::tuya::kTuyaRegistryCount);
    const auto& up = devices::tuya::kDefTS0205_up3pngle;
    assert(find_definition("TS0205", "_TZ3210_up3pngle", reg) == &up);
    assert(find_definition("TS0205", "_TYZB01_wqcac7lo", reg) == &devices::tuya::kDefTS0205);
    assert(devices::tuya::kDefTS0205.reports_count == 0);

    // z2m reporting.batteryPercentageRemaining: genPowerCfg 0x0021 u8,
    // min 3600 s, max 65000 s, change 0; bind genPowerCfg first, read after.
    bool bound = false;
    for (std::size_t i = 0; i < up.bindings_count; ++i) bound |= up.bindings[i].cluster_id == 0x0001;
    assert(bound);
    assert(up.reports_count == 1);
    const ReportingSpec& r = up.reports[0];
    assert(r.endpoint == 1 && r.cluster_id == 0x0001 && r.attr_id == 0x0021 && r.attr_type == 0x20);
    assert(r.min_interval == 3600 && r.max_interval == 65000 && r.reportable_change == 0);
    assert(r.manufacturer_code == 0);
    assert(up.config_steps_count == 1);
    const ConfigStep& rd = up.config_steps[0];
    assert(rd.op == ConfigStepOp::Read && rd.cluster_id == 0x0001 &&
           rd.payload_len == 2 && rd.payload[0] == 0x21 && rd.payload[1] == 0x00);
    return 0;
}
