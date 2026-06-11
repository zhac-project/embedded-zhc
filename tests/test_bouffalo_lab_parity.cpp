// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Bouffalo Lab family. One def, one real gap pair:
//
//   * RMC002 — US plug smart socket. z2m drives it with
//     extend=[m.onOff(), m.forcePowerSource({powerSource:"Mains (single
//     phase)"})]. m.onOff() defaults powerOnBehavior=true, so it wires
//     fz.power_on_behavior (genOnOff 0x4003 startUpOnOff) + tz.power_on_behavior
//     + the power_on_behavior enum expose. The Tier-1 auto-port collapsed the
//     device to bare on/off, DROPPING the power_on_behavior axis AND the
//     forcePowerSource override. The fix restores kFzPowerOnBehavior /
//     kTzPowerOnBehavior1 + the enum expose, and sets
//     power_source_override = 0x01 (Mains, single phase).
//
// z2m-source: zigbee-herdsman-converters/src/devices/bouffalo_lab.ts +
//             lib/modernExtend.ts onOff() + forcePowerSource().

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

namespace zhc::devices::bouffalo_lab {
extern const PreparedDefinition kDef_RMC002;
}  // namespace zhc::devices::bouffalo_lab

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

// ZCL attribute report (cluster-specific=false, Report=0x0A, one record).
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

constexpr std::uint16_t GEN_ON_OFF = 0x0006;

// ── RMC002: smart socket — on/off + power_on_behavior + mains override ─
void check_rmc002() {
    const auto& def = devices::bouffalo_lab::kDef_RMC002;

    // It is an on/off socket with a restored power_on_behavior axis.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power_on_behavior"));

    // Plain socket — no phantom light/sensor bundle.
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "battery"));

    // to-zigbee writers present for both axes (on/off + power_on_behavior).
    assert(def.to_zigbee != nullptr);
    assert(def.to_zigbee_count == 2);

    // forcePowerSource → Mains (single phase) override (ZCL Basic 0x0007 = 0x01).
    assert(def.power_source_override == 0x01);

    // genOnOff onOff (attr 0x0000, bool) → state.
    const std::uint8_t on[] = {0x01};
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, attr_report(0x0000, 0x10, on));
    assert(ron.any_matched);
    const Value* sv = ron.merged.find("state");
    assert(sv && sv->type == ValueType::Bool && sv->b == true);

    const std::uint8_t off[] = {0x00};
    auto roff = dispatch_zcl(def, GEN_ON_OFF, 1, attr_report(0x0000, 0x10, off));
    const Value* sv2 = roff.merged.find("state");
    assert(sv2 && sv2->type == ValueType::Bool && sv2->b == false);

    // genOnOff startUpOnOff (attr 0x4003 = 16387, enum8) → power_on_behavior —
    // the dropped half that the fix restores. value 1 → "on".
    const std::uint8_t pob[] = {0x01};
    auto rp = dispatch_zcl(def, GEN_ON_OFF, 1, attr_report(0x4003, 0x30, pob));
    assert(rp.any_matched);
    const Value* pv = rp.merged.find("power_on_behavior");
    assert(pv && pv->type == ValueType::StringRef &&
           std::strcmp(pv->str, "on") == 0);
}

}  // namespace

int main() {
    check_rmc002();
    return 0;
}
