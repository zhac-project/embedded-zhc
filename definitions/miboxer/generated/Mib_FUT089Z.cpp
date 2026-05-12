// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Miboxer FUT089Z — RGB+CCT battery remote.
//
// z2m-source: zigbee-herdsman-converters/src/devices/miboxer.ts (FUT089Z).
//
// The device is a Tuya-OEM TS1002 relabelled by MiBoxer. It sends
// cluster-specific commands on genOnOff / genLevelCtrl /
// lightingColorCtrl, plus the Tuya-private `tuyaAction` (0xFD) on
// genOnOff for scene selection. We map each to a plain `action`
// string — zone-aware suffixes (z2m's `zone_N` option) are deferred
// since they rely on runtime group-to-zone config.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::miboxer {
namespace {

// ── fz_fut089z_zone ─────────────────────────────────────────────────
//
// Synthesises a `zone` numeric attribute (1..8) from the frame's
// group ID whenever the remote emits a command under one of the
// default zone groups 101..108 (matches the fixed mapping programmed
// via `miboxerSetZones`). Runs alongside the action-emitting generic
// converters — dispatch merges every matching converter's payload, so
// a single button press publishes { action, zone } together.
//
// No-op on frames that carry group ID 0 (unicast) or one outside the
// 101..108 band, so forwarding commands that don't originate from a
// zone button don't grow a bogus `zone` key.
bool fz_fut089z_zone(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::uint16_t gid = msg.group_id;
    if (gid < 101 || gid > 108) return false;
    Value v{}; v.type = ValueType::Uint; v.u = gid - 100;
    out.put("zone", v);
    return true;
}

constexpr FzConverter kFz_FUT089Z_Zone{
    .family            = FrameFamily::Zcl,
    .cluster           = nullptr,                     // any cluster
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_fut089z_zone },
    .user_config       = nullptr,
};

constexpr const char* kModels_FUT089Z[] = { "TS1002" };
constexpr const char* kManus_FUT089Z[]  = { "_TZ3000_xwh1e22x", "_TZ3000_zwszqdpy" };

constexpr const char* kActions_FUT089Z[] = {
    "on",
    "off",
    "brightness_move_to_level",
    "color_temperature_move",
    "move_to_hue_and_saturation",
    "switch_scene",
};

constexpr Expose kExposes_FUT089Z[] = {
    { "action",            ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_FUT089Z, sizeof(kActions_FUT089Z)/sizeof(kActions_FUT089Z[0]) },
    { "action_scene",      ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    // Companion numerics parsed from the moved-to command payloads.
    // Always exposed — the upstream z2m device gates them behind an
    // `expose_values=true` device option we don't have a runtime
    // framework for yet. Consumers can ignore the keys they don't want.
    { "level",             ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "color_temperature", ExposeType::Numeric, Access::State,
      "mired", nullptr, nullptr, 0 },
    { "hue",               ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "saturation",        ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    // `zone` (1..8) is synthesised by `fz_fut089z_zone` from the
    // frame's groupID — the remote sends each zone's commands under
    // its own group, which maps to zone numbers via miboxerSetZones.
    { "zone",              ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "battery",           ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage",           ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_FUT089Z[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzBattery,
    &::zhc::tuya::kFzTuyaSwitchScene,
    &kFz_FUT089Z_Zone,   // synthetic `zone = groupID - 100`
};

constexpr WhiteLabel kWhiteLabels_FUT089Z[] = {
    { "Ledron", "YK-16" },   // same firmware, sold under Ledron branding
};

// Bind each source cluster to the coordinator so the remote actually
// routes its outgoing command frames our way. genPowerCfg gets a bind
// so battery reports arrive — the device doesn't report spontaneously
// without it.
constexpr BindingSpec kBindings_FUT089Z[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
    { 1, 0x0300 },   // lightingColorCtrl
};

// Tuya "magic packet" attrs (genBasic): manufacturerName, zclVersion,
// appVersion, modelIdentifier, powerSource, attributeReportingStatus.
// LE-encoded: two bytes per attr.
constexpr std::uint8_t kMagicAttrs_FUT089Z[] = {
    0x04, 0x00,    // 0x0004
    0x00, 0x00,    // 0x0000
    0x01, 0x00,    // 0x0001
    0x05, 0x00,    // 0x0005
    0x07, 0x00,    // 0x0007
    0xFE, 0xFF,    // 0xFFFE
};

// miboxerSetZones payload: u8 count + 8 × (u16 groupId LE, u8 zoneNum).
// Default mapping per z2m: zoneNum 1..8 → groupId 101..108.
constexpr std::uint8_t kZones_FUT089Z[] = {
    8,                           // zone count
    0x65, 0x00, 0x01,            // zone 1 → group 101
    0x66, 0x00, 0x02,            // zone 2 → group 102
    0x67, 0x00, 0x03,            // zone 3 → group 103
    0x68, 0x00, 0x04,            // zone 4 → group 104
    0x69, 0x00, 0x05,            // zone 5 → group 105
    0x6A, 0x00, 0x06,            // zone 6 → group 106
    0x6B, 0x00, 0x07,            // zone 7 → group 107
    0x6C, 0x00, 0x08,            // zone 8 → group 108
};

// Ordered configure pipeline for FUT089Z:
//   1. 800 ms wait — bindings[] fire sync from run_configure via
//      ZDO_BIND_REQ → SRSP only. The actual ZDO_BIND_RSP AREQ from the
//      device lands several hundred ms later. Without this settle
//      window the magic packet read races with bind processing and
//      comes back 0xF0 ZMacTransactionExpired.
//   2. genBasic Read "magic packet" — unlocks Tuya firmware.
//   3. 500 ms wait — device is busy streaming back a 6-attribute
//      ReadAttributesResponse; without this pause the follow-up
//      setZones frame gets dropped at the MAC.
//   4. genGroups cmd 0xF0 miboxerSetZones — programs zone-to-group map.
//   5. 800 ms wait — observed AF_DATA_CONFIRM arrives ~450 ms after
//      the setZones TX. Wait long enough that the device finishes
//      processing zones before we send tuyaSetup; both frames
//      MAC-failing cost the user the magic-packet unlock.
//   6. genBasic  cmd 0xF0 tuyaSetup — empty body, default-response off.
constexpr ConfigStep kConfigSteps_FUT089Z[] = {
    { ConfigStepOp::Wait, 0, 0,      0,    0, nullptr, 0, 800 },
    { ConfigStepOp::Read, 1, 0x0000, 0x00, 0,
      kMagicAttrs_FUT089Z, sizeof(kMagicAttrs_FUT089Z), 2000 },
    { ConfigStepOp::Wait, 0, 0,      0,    0, nullptr, 0, 500 },
    { ConfigStepOp::Cmd,  1, 0x0004, 0xF0, 0,
      kZones_FUT089Z,      sizeof(kZones_FUT089Z),      2000 },
    { ConfigStepOp::Wait, 0, 0,      0,    0, nullptr, 0, 800 },
    { ConfigStepOp::Cmd,  1, 0x0000, 0xF0,
      kStepFlagDisableDefaultResponse,
      nullptr, 0, 2000 },
};

}  // namespace

extern const PreparedDefinition kDef_FUT089Z{
    .zigbee_models            = kModels_FUT089Z,
    .zigbee_models_count      = sizeof(kModels_FUT089Z)/sizeof(kModels_FUT089Z[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = kManus_FUT089Z,
    .manufacturer_names_count = sizeof(kManus_FUT089Z)/sizeof(kManus_FUT089Z[0]),
    .model                    = "FUT089Z",
    .vendor                   = "Miboxer",
    .meta                     = nullptr,
    .exposes                  = kExposes_FUT089Z,
    .exposes_count            = sizeof(kExposes_FUT089Z)/sizeof(kExposes_FUT089Z[0]),
    .white_labels             = kWhiteLabels_FUT089Z,
    .white_labels_count       = sizeof(kWhiteLabels_FUT089Z)/sizeof(kWhiteLabels_FUT089Z[0]),
    .from_zigbee              = kFz_FUT089Z,
    .from_zigbee_count        = sizeof(kFz_FUT089Z)/sizeof(kFz_FUT089Z[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_FUT089Z,
    .bindings_count           = sizeof(kBindings_FUT089Z)/sizeof(kBindings_FUT089Z[0]),
    .reports                  = nullptr,
    .reports_count            = 0,
    .config_steps             = kConfigSteps_FUT089Z,
    .config_steps_count       = sizeof(kConfigSteps_FUT089Z)/sizeof(kConfigSteps_FUT089Z[0]),
};

}  // namespace zhc::devices::miboxer
