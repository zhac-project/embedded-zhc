// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RepenicLtd RD-250ZG — dimmer + electricity meter.
// Dimmer
// z2m-source: repenic_ltd.ts #RD-250ZG.
//
// z2m wires `m.light({configureReporting:true})` + `m.electricityMeter()`.
// electricityMeter() defaults to cluster:"both" (seMetering 0x0702 energy +
// haElectricalMeasurement 0x0B04 power/voltage/current), ac type, with
// acFrequency/powerFactor OFF — so z2m exposes power, voltage, current,
// energy. The auto-port wired only kFzMetering (0x0702) and exposed
// energy+power, dropping the 0x0B04 half: voltage + current had no decoder
// and no expose, and power had no decoder either because z2m sources power
// from haElectricalMeasurement (it deletes seMetering.power in "both" mode).
// Fix: add the generic kFzElectricalMeasurement (0x0B04) converter, the
// voltage/current exposes, and the 0x0B04 reporting bind.
// (The genLevelCtrl manuSpecific config attrs — min/max/start brightness,
// boost, dimming_mode, default_move_rate — need a write/read of arbitrary
// attribute IDs with no generic converter; INFRA, deferred.)
//
// z2m v26.125.0->v26.133.0: `scene_mode` (private cluster 0xE003
// repenicPrivateE003, attr 0x0003 enum8, m.binary ON=1 / OFF=0, no
// manufacturer code, no reporting) and the button `action` from raw frames on
// cluster 0xE004: the LAST byte of the whole frame (header included, as z2m's
// msg.data) that is 2 / 4 / 8 / 16 gives hold / release / double / triple.
// 0xE003 has no entry in cluster_names.hpp, so its frames carry a null cluster
// name and reach every converter of this def; the scene_mode decoder in turn
// matches only unnamed-cluster frames (no named cluster is "repenicPrivateE003")
// and no other converter here reads attribute 0x0003. Upstream adds no bind for
// either cluster. Not ported: the genTime write at configure (needs the hub's
// local clock at configure time), the sleep / wakeup / night pattern composites
// (0xE003 commands 0x00..0x02 with a raw buffer payload).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::repenic_ltd {
namespace {
constexpr ::zhc::generic::ZclWriteLookup kOnOff_RD_250ZG[] = { {"ON", 1}, {"OFF", 0} };
constexpr ::zhc::generic::ZclAttrRow kSceneRows_RD_250ZG[] = {
    { 0x0003, "scene_mode", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
};
constexpr ::zhc::generic::ZclAttrMap kSceneMap_RD_250ZG{
    kSceneRows_RD_250ZG, sizeof(kSceneRows_RD_250ZG)/sizeof(kSceneRows_RD_250ZG[0]) };
constexpr FzConverter kFzSceneMode_RD_250ZG =
    ::zhc::generic::zcl_attr_fz("repenicPrivateE003", &kSceneMap_RD_250ZG);   // 0xE003, unnamed
constexpr ::zhc::generic::ZclWriteSpec kSceneModeSpec_RD_250ZG{
    "scene_mode", 0x0003, 0x30, 0, kOnOff_RD_250ZG, sizeof(kOnOff_RD_250ZG)/sizeof(kOnOff_RD_250ZG[0]) };
constexpr TzConverter kTzSceneMode_RD_250ZG =
    ::zhc::generic::zcl_write_tz("repenicPrivateE003", 0xE003, &kSceneModeSpec_RD_250ZG);

// z2m: msg.data.findLast((byte) => byte in actionLookup) over the raw frame.
bool fz_rd250zg_button(const DecodedMessage& msg, const FzConverter&,
                       const PreparedDefinition&, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    for (std::size_t i = msg.raw_data.size(); i-- > 0;) {
        const char* label = nullptr;
        switch (msg.raw_data[i]) {
            case 2:  label = "hold";    break;
            case 4:  label = "release"; break;
            case 8:  label = "double";  break;
            case 16: label = "triple";  break;
            default: continue;
        }
        Value a{}; a.type = ValueType::StringRef; a.str = label;
        return out.put("action", a);
    }
    return false;
}

// Cluster 0xE004 is named "zosungIRControl" in cluster_names.hpp (the id the
// UFO-R11 IR blaster uses); the name only selects the cluster id here. z2m's
// "raw" type is a frame herdsman could not parse, i.e. a cluster-specific
// command it has no schema for, so only Command frames are taken, in either
// direction. Foundation frames (our MessageType::Raw) stay out: the 0xE003
// write response (18 tsn 04 00, null cluster name, so it passes the selector)
// would otherwise read as "release".
constexpr FzConverter rd250zg_button_fz(Direction dir) {
    return FzConverter{
        .family            = FrameFamily::Zcl,
        .cluster           = "zosungIRControl",   // 0xE004
        .type_mask         = type_bit(MessageType::Command),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = dir,
        .fn                = { .zcl_fn = fz_rd250zg_button },
        .user_config       = nullptr,
    };
}
constexpr FzConverter kFzButtonS2C_RD_250ZG = rd250zg_button_fz(Direction::ServerToClient);
constexpr FzConverter kFzButtonC2S_RD_250ZG = rd250zg_button_fz(Direction::ClientToServer);

const FzConverter* const kFz_RD_250ZG[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &kFzSceneMode_RD_250ZG,
    &kFzButtonS2C_RD_250ZG,
    &kFzButtonC2S_RD_250ZG,
};
const TzConverter* const kTz_RD_250ZG[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &kTzSceneMode_RD_250ZG,
};
constexpr const char* kActions_RD_250ZG[] = { "hold", "release", "double", "triple" };
constexpr const char* kModels_RD_250ZG[] = { "RD-250ZG" };

}  // namespace


// --- auto-generated by emit_expose_bindings.py ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"scene_mode", ExposeType::Binary, Access::StateSet, nullptr, "Scene mode", nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, "Triggered action (e.g. a button click)",
     kActions_RD_250ZG, sizeof(kActions_RD_250ZG)/sizeof(kActions_RD_250ZG[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_RD_250ZG{
    .zigbee_models=kModels_RD_250ZG, .zigbee_models_count=sizeof(kModels_RD_250ZG)/sizeof(kModels_RD_250ZG[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RD-250ZG", .vendor="RepenicLtd",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RD_250ZG, .from_zigbee_count=sizeof(kFz_RD_250ZG)/sizeof(kFz_RD_250ZG[0]),
    .to_zigbee=kTz_RD_250ZG, .to_zigbee_count=sizeof(kTz_RD_250ZG)/sizeof(kTz_RD_250ZG[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::repenic_ltd
