// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Enkin family. One real z2m<->ez gap caught here:
//
//   * ZDM150 — 150W dimmer module. z2m drives it with
//     extend=[m.light({powerOnBehavior:false, effect:false}),
//             m.forcePowerSource({powerSource:"Mains (single phase)"})].
//     m.light() gives on/off + brightness (genLevelCtrl 0x0008) plus the
//     matching to-zigbee writers. The auto-generated port mis-classified the
//     device as battery-only on/off: it wired kFzBattery + phantom
//     battery/voltage exposes + a genPowerCfg (0x0001) binding and DROPPED
//     brightness entirely. The fix re-ports it as a proper light:
//     kFzOnOff + kFzBrightness (+ tz), state + brightness exposes, bindings on
//     genOnOff (0x0006) + genLevelCtrl (0x0008), no phantom battery, and
//     power_source_override = Mains (single phase).
//
// z2m-source: zigbee-herdsman-converters/src/devices/enkin.ts +
//             lib/modernExtend.ts light() + forcePowerSource().

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

namespace zhc::devices::enkin {
extern const PreparedDefinition kDef_ZDM150;
}  // namespace zhc::devices::enkin

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
constexpr std::uint16_t GEN_LEVEL  = 0x0008;

// ── ZDM150: dimmer — on/off + brightness, no phantom battery ─────────
void check_zdm150() {
    const auto& def = devices::enkin::kDef_ZDM150;

    // It IS a controllable light.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));

    // The phantom battery bundle must be gone (mains-powered dimmer).
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));

    // to-zigbee writers present (it's controllable, not read-only).
    assert(def.to_zigbee != nullptr);
    assert(def.to_zigbee_count == 2);

    // forcePowerSource → Mains (single phase) override.
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

    // genLevelCtrl currentLevel (attr 0x0000, u8) → brightness — the dropped
    // half that the fix restores.
    const std::uint8_t lvl[] = {0xAA};  // 170
    auto rl = dispatch_zcl(def, GEN_LEVEL, 1, attr_report(0x0000, 0x20, lvl));
    assert(rl.any_matched);
    const Value* bv = rl.merged.find("brightness");
    assert(bv && bv->type == ValueType::Uint && bv->u == 170);
}

}  // namespace

int main() {
    check_zdm150();
    return 0;
}
