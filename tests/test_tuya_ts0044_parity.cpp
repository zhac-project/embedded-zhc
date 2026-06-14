// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Tuya TS0044 (4-button scene switch). z2m drives it with
// `fromZigbee: [tuya.fz.on_off_action, fz.battery]` and exposes `action` +
// `battery`. The auto-port had trimmed the def to action-only, dropping the
// genPowerCfg battery half for EVERY TS0044 (the def matches all manufacturers
// via an unrestricted zigbeeModel). This guards the re-added battery decode.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tuya.ts #TS0044.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0044;
}  // namespace zhc::devices::tuya

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL global attribute report (cluster-specific=false, Report=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

constexpr std::uint16_t GEN_POWER_CFG = 0x0001;

}  // namespace

int main() {
    const auto& def = devices::tuya::kDefTS0044;

    // The action half stayed; the battery half was restored (z2m parity).
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    // genPowerCfg must be bound for the battery reports to route home.
    assert(def_binds(def, GEN_POWER_CFG));

    // genPowerCfg batteryPercentageRemaining (attr 0x0021, u8 half-percent) → battery %.
    const std::uint8_t pct[] = {200};   // 200 half-percent = 100 %
    auto rb = dispatch_zcl(def, GEN_POWER_CFG, 1, attr_report(0x0021, 0x20, pct));
    assert(rb.any_matched);
    const Value* batt = rb.merged.find("battery");
    assert(batt && batt->type == ValueType::Uint && batt->u == 100);

    // genPowerCfg batteryVoltage (attr 0x0020, u8 units of 100 mV) → voltage mV.
    const std::uint8_t volt[] = {30};   // 30 * 100 mV = 3000 mV
    auto rv = dispatch_zcl(def, GEN_POWER_CFG, 1, attr_report(0x0020, 0x20, volt));
    assert(rv.any_matched);
    const Value* mv = rv.merged.find("voltage");
    assert(mv && mv->type == ValueType::Uint && mv->u == 3000);

    return 0;
}
