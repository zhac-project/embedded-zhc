// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Inovelli writeable manuSpec parameter specs. Mirrors the
// CTM / Danfoss pattern: each attribute gets a `ZclWriteSpec` + a
// `TzConverter` that funnels through the generic `tz_zcl_write_attr`
// (foundation writeAttributes encoder, fc=0x14 with mfg=0x122f).
//
// z2m-source: zigbee-herdsman-converters/src/lib/inovelli.ts —
//             COMMON_ATTRIBUTES + COMMON_DIMMER_ATTRIBUTES.
#include "definitions/inovelli/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstddef>
#include <cstdint>

#include "zhc/types.hpp"

namespace zhc::devices::inovelli {

namespace {

// ── Scene / button-tap action decoder ─────────────────────────────
//
// z2m `fzLocal.inovelli` raw branch: when a manufacturer-specific raw
// frame arrives on endpoint 2 with command 0x00, byte `data[5]` is the
// button and byte `data[6]` is the click type. After the 5-byte manuSpec
// ZCL header (fc, mfgLo, mfgHi, tsn, cmd) those land in
// `raw_body[0]` (button) and `raw_body[1]` (click).
//
//   buttonLookup: 1=down 2=up 3=config 4=aux_down 5=aux_up 6=aux_config
//   clickLookup:  0=single 1=release 2=held 3=double 4=triple
//                 5=quadruple 6=quintuple
//
// z2m builds the string as `${button}_${action}`. We can't concatenate
// at runtime (FixedPayload stores a bare char*), so we materialise the
// full 6×7 cross-product as static storage and index into it. Every
// combination of z2m's BUTTON_TAP_SEQUENCES (down/up/config × the seven
// click types) plus the aux_* variants is covered.
constexpr const char* kSceneActionTable[6][7] = {
    // down
    {"down_single", "down_release", "down_held", "down_double",
     "down_triple", "down_quadruple", "down_quintuple"},
    // up
    {"up_single", "up_release", "up_held", "up_double",
     "up_triple", "up_quadruple", "up_quintuple"},
    // config
    {"config_single", "config_release", "config_held", "config_double",
     "config_triple", "config_quadruple", "config_quintuple"},
    // aux_down
    {"aux_down_single", "aux_down_release", "aux_down_held", "aux_down_double",
     "aux_down_triple", "aux_down_quadruple", "aux_down_quintuple"},
    // aux_up
    {"aux_up_single", "aux_up_release", "aux_up_held", "aux_up_double",
     "aux_up_triple", "aux_up_quadruple", "aux_up_quintuple"},
    // aux_config
    {"aux_config_single", "aux_config_release", "aux_config_held",
     "aux_config_double", "aux_config_triple", "aux_config_quadruple",
     "aux_config_quintuple"},
};

bool fz_inovelli_scene_action(const DecodedMessage& msg, const FzConverter&,
                              const PreparedDefinition&, RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // The frame is manufacturer-specific (mfg 0x122f). Command 0x00 also
    // names plenty of standard cluster commands (genOnOff Off, genScenes
    // Add, …) so the guard keeps a non-manu frame from being hijacked.
    if (!msg.manufacturer_specific) return false;
    if (msg.raw_body.size() < 2) return false;  // button + click minimum

    const std::uint8_t button = msg.raw_body[0];  // z2m data[5]
    const std::uint8_t click  = msg.raw_body[1];  // z2m data[6]
    if (button < 1 || button > 6) return false;   // unknown → publish nothing
    if (click > 6) return false;

    Value a{};
    a.type = ValueType::StringRef;
    a.str  = kSceneActionTable[button - 1][click];  // static storage
    out.put("action", a);
    return true;
}

// ── ZCL data-type bytes (per ZCL spec / zigbee-herdsman DataType) ──
constexpr std::uint8_t kZclBool   = 0x10;
constexpr std::uint8_t kZclUint8  = 0x20;
constexpr std::uint8_t kZclUint16 = 0x21;

// Cluster name string used by `tz_zcl_write_attr` to match the
// runtime context cluster. z2m uses
// `manuSpecificInovelli` for cluster 0xFC31. The runtime maps that
// name back to 0xFC31 only if the foundation parser knows it; for a
// pure write path the encoder cares about cluster_id.
constexpr const char* kClusterName = "manuSpecificInovelli";

// COMMON_ATTRIBUTES (cluster 0xFC31, mfg 0x122f) —————————————————
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedUpRemote{
    "dimmingSpeedUpRemote",   1, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedUpLocal{
    "dimmingSpeedUpLocal",    2, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOffToOnRemote{
    "rampRateOffToOnRemote",  3, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOffToOnLocal{
    "rampRateOffToOnLocal",   4, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedDownRemote{
    "dimmingSpeedDownRemote", 5, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedDownLocal{
    "dimmingSpeedDownLocal",  6, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOnToOffRemote{
    "rampRateOnToOffRemote",  7, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOnToOffLocal{
    "rampRateOnToOffLocal",   8, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
// invertSwitch — z2m exposes as enum {No:0, Yes:1}. Accept bool too;
// `tz_zcl_write_attr` Bool→Uint coercion handles it.
constexpr ::zhc::generic::ZclWriteSpec kSpecInvertSwitch{
    "invertSwitch",          11, kZclBool,  kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecAutoTimerOff{
    "autoTimerOff",          12, kZclUint16, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDefaultLevelLocal{
    "defaultLevelLocal",     13, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDefaultLevelRemote{
    "defaultLevelRemote",    14, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecStateAfterPowerRestored{
    "stateAfterPowerRestored", 15, kZclUint8, kInovelliMfgCode, nullptr, 0,
};

// COMMON_DIMMER_ATTRIBUTES (cluster 0xFC31, mfg 0x122f) ———————————
constexpr ::zhc::generic::ZclWriteSpec kSpecMinimumLevel{
    "minimumLevel",           9, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMaximumLevel{
    "maximumLevel",          10, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOutputMode{
    "outputMode",            21, kZclBool, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecSmartBulbMode{
    "smartBulbMode",         22, kZclBool, kInovelliMfgCode, nullptr, 0,
};

}  // namespace

#define ZHC_INO_TZ(var, spec_ref, key_str)                           \
    extern const TzConverter var{                                    \
        .key         = key_str,                                      \
        .cluster     = kClusterName,                                 \
        .cluster_id  = kInovelliClusterId,                           \
        .command_id  = 0x02,                                         \
        .fn          = &::zhc::generic::tz_zcl_write_attr,           \
        .user_config = &spec_ref,                                    \
    };

ZHC_INO_TZ(kTzInoDimmingSpeedUpRemote,    kSpecDimSpeedUpRemote,
           "dimmingSpeedUpRemote")
ZHC_INO_TZ(kTzInoDimmingSpeedUpLocal,     kSpecDimSpeedUpLocal,
           "dimmingSpeedUpLocal")
ZHC_INO_TZ(kTzInoRampRateOffToOnRemote,   kSpecRampOffToOnRemote,
           "rampRateOffToOnRemote")
ZHC_INO_TZ(kTzInoRampRateOffToOnLocal,    kSpecRampOffToOnLocal,
           "rampRateOffToOnLocal")
ZHC_INO_TZ(kTzInoDimmingSpeedDownRemote,  kSpecDimSpeedDownRemote,
           "dimmingSpeedDownRemote")
ZHC_INO_TZ(kTzInoDimmingSpeedDownLocal,   kSpecDimSpeedDownLocal,
           "dimmingSpeedDownLocal")
ZHC_INO_TZ(kTzInoRampRateOnToOffRemote,   kSpecRampOnToOffRemote,
           "rampRateOnToOffRemote")
ZHC_INO_TZ(kTzInoRampRateOnToOffLocal,    kSpecRampOnToOffLocal,
           "rampRateOnToOffLocal")
ZHC_INO_TZ(kTzInoInvertSwitch,            kSpecInvertSwitch,
           "invertSwitch")
ZHC_INO_TZ(kTzInoAutoTimerOff,            kSpecAutoTimerOff,
           "autoTimerOff")
ZHC_INO_TZ(kTzInoDefaultLevelLocal,       kSpecDefaultLevelLocal,
           "defaultLevelLocal")
ZHC_INO_TZ(kTzInoDefaultLevelRemote,      kSpecDefaultLevelRemote,
           "defaultLevelRemote")
ZHC_INO_TZ(kTzInoStateAfterPowerRestored, kSpecStateAfterPowerRestored,
           "stateAfterPowerRestored")
ZHC_INO_TZ(kTzInoMinimumLevel,            kSpecMinimumLevel,
           "minimumLevel")
ZHC_INO_TZ(kTzInoMaximumLevel,            kSpecMaximumLevel,
           "maximumLevel")
ZHC_INO_TZ(kTzInoOutputMode,              kSpecOutputMode,
           "outputMode")
ZHC_INO_TZ(kTzInoSmartBulbMode,           kSpecSmartBulbMode,
           "smartBulbMode")

#undef ZHC_INO_TZ

// Scene / button-tap action. z2m fires on a manufacturer-specific raw
// frame on endpoint 2; we gate on endpoint 2 + command 0x00 + the
// manufacturer_specific guard inside the fn. Server→client (the device
// reports the tap to the coordinator).
extern const FzConverter kFzInovelliSceneAction{
    .family            = FrameFamily::Zcl,
    // cluster left null → id-based dispatch (the decoder doesn't map the
    // 0xFC31 id to a name); command-id + endpoint + direction filter.
    .cluster           = nullptr,
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,   // z2m data[4] === 0x00
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = 2,      // z2m: msg.endpoint.ID === 2
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_inovelli_scene_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::inovelli
