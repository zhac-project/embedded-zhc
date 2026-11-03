// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Shelly BLU DoorWindow Mini ZB (SBDW-203C) and Outdoor ZB (SBDW-302C)
// (z2m v26.125.0 window). Fingerprints: modelID "BLU DoorWindow Mini ZB" /
// "BLU DoorWindow Outdoor ZB", manufacturerName "Shelly".
// fzLocal.blu_door_window reads ssIasZone zoneStatus (zone type 0x0016, door/
// window handle) from the status-change command and from attribute reports:
// alarm1=0 alarm2=0 closed, alarm1=1 alarm2=0 tilted, alarm1=1 alarm2=1 open;
// contact = closed; battery_low = bit3. Plus m.battery and identify.
// The Mini adds shellyLightLevel: Shelly cluster 0xFC21 (manufacturer code
// 0x1490), lightLevel 0x0000 u8 dark/twilight/bright, reported every 60-900 s.
// Its dark/bright_threshold attributes are UINT24 writes, which ZclWriteSpec
// cannot encode; deferred.
// z2m-source: shelly.ts #SBDW-203C #SBDW-302C + fzLocal.blu_door_window.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::shelly {
namespace {

constexpr std::uint16_t kShellyManufacturerCode = 0x1490;

bool fz_blu_door_window(const DecodedMessage& msg, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint32_t status = 0;
    if (msg.type == MessageType::Command) {
        if (msg.command_id != 0x00) return false;   // statusChangeNotification
        const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
        if (msg.raw_data.size() < hdr + 2) return false;
        status = static_cast<std::uint32_t>(msg.raw_data[hdr]) |
                 (static_cast<std::uint32_t>(msg.raw_data[hdr + 1]) << 8);
    } else {
        const Value* v = msg.payload.find("2");     // attr 0x0002 zoneStatus
        if (!v || v->type != ValueType::Uint) return false;
        status = static_cast<std::uint32_t>(v->u);
    }
    const bool alarm1 = (status & 0x01u) != 0;
    const bool alarm2 = (status & 0x02u) != 0;
    const char* position = !alarm1 && !alarm2 ? "closed"
                         : alarm1 && !alarm2  ? "tilted"
                                              : "open";
    Value b{}; b.type = ValueType::Bool;
    b.b = !alarm1 && !alarm2;          out.put("contact", b);
    Value s{}; s.type = ValueType::StringRef; s.str = position;
    out.put("handle_position", s);
    b.b = (status & 0x08u) != 0;       out.put("battery_low", b);
    return true;
}

constexpr FzConverter kFzBluDoorWindow{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command) |
                         type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_blu_door_window },
    .user_config       = nullptr,
};

// 0xFC21 is manufacturer-specific and unnamed in the cluster table, so the
// converter matches any cluster and the map's manufacturer code gates it.
constexpr ::zhc::generic::ZclWriteLookup kLightLevels[] = {
    {"dark", 0}, {"twilight", 1}, {"bright", 2} };
constexpr ::zhc::generic::ZclAttrRow kRowsLightLevel[] = {
    {0x0000, "light_level", 1, kLightLevels, 3},
};
constexpr ::zhc::generic::ZclAttrMap kMapLightLevel{
    kRowsLightLevel, sizeof(kRowsLightLevel)/sizeof(kRowsLightLevel[0]), kShellyManufacturerCode };
constexpr FzConverter kFzLightLevel = ::zhc::generic::zcl_attr_fz(nullptr, &kMapLightLevel);

const FzConverter* const kFz_SBDW_203C[] = {
    &kFzBluDoorWindow,
    &::zhc::generic::kFzBattery,
    &kFzLightLevel,
};
const FzConverter* const kFz_SBDW_302C[] = {
    &kFzBluDoorWindow,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kHandlePositions[] = { "closed", "tilted", "open" };
constexpr const char* kLightLevelOpts[] = { "dark", "twilight", "bright" };
constexpr Expose kExp_SBDW_203C[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"handle_position", ExposeType::Enum, Access::State, nullptr,
     "Handle position: closed, tilted (partly open), or open", kHandlePositions, 3},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"light_level", ExposeType::Enum, Access::State, nullptr, "Coarse light level", kLightLevelOpts, 3},
};
constexpr Expose kExp_SBDW_302C[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"handle_position", ExposeType::Enum, Access::State, nullptr,
     "Handle position: closed, tilted (partly open), or open", kHandlePositions, 3},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SBDW_203C[] = {
    {1, 0x0001},
    {1, 0xFC21},
};
constexpr BindingSpec kBindings_SBDW_302C[] = {
    {1, 0x0001},
};
constexpr ReportingSpec kReports_SBDW_203C[] = {
    {1, 0xFC21, 0x0000, 0x20, 60, 900, 0, kShellyManufacturerCode},
};

constexpr const char* kModels_SBDW_203C[] = { "BLU DoorWindow Mini ZB" };
constexpr const char* kModels_SBDW_302C[] = { "BLU DoorWindow Outdoor ZB" };
constexpr const char* kManus_Shelly[] = { "Shelly" };

}  // namespace

extern const PreparedDefinition kDef_SBDW_203C{
    .zigbee_models=kModels_SBDW_203C, .zigbee_models_count=sizeof(kModels_SBDW_203C)/sizeof(kModels_SBDW_203C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_Shelly, .manufacturer_names_count=sizeof(kManus_Shelly)/sizeof(kManus_Shelly[0]),
    .model="SBDW-203C", .vendor="Shelly",
    .meta=nullptr, .exposes=kExp_SBDW_203C, .exposes_count=sizeof(kExp_SBDW_203C)/sizeof(kExp_SBDW_203C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SBDW_203C, .from_zigbee_count=sizeof(kFz_SBDW_203C)/sizeof(kFz_SBDW_203C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SBDW_203C, .bindings_count=sizeof(kBindings_SBDW_203C)/sizeof(kBindings_SBDW_203C[0]),
    .reports=kReports_SBDW_203C, .reports_count=sizeof(kReports_SBDW_203C)/sizeof(kReports_SBDW_203C[0]),
};

extern const PreparedDefinition kDef_SBDW_302C{
    .zigbee_models=kModels_SBDW_302C, .zigbee_models_count=sizeof(kModels_SBDW_302C)/sizeof(kModels_SBDW_302C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_Shelly, .manufacturer_names_count=sizeof(kManus_Shelly)/sizeof(kManus_Shelly[0]),
    .model="SBDW-302C", .vendor="Shelly",
    .meta=nullptr, .exposes=kExp_SBDW_302C, .exposes_count=sizeof(kExp_SBDW_302C)/sizeof(kExp_SBDW_302C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SBDW_302C, .from_zigbee_count=sizeof(kFz_SBDW_302C)/sizeof(kFz_SBDW_302C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SBDW_302C, .bindings_count=sizeof(kBindings_SBDW_302C)/sizeof(kBindings_SBDW_302C[0]),
};

}  // namespace zhc::devices::shelly
