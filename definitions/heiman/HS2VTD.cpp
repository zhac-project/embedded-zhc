// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HEIMAN HS2VTD 4-in-1 door sensor (contact + vibration + T/H) (z2m v26.95.0 parity).
// z2m-source: heiman.ts #HS2VTD (added v26.94.0)
// v26.125.0 gates the two IAS zones by endpoint (iasZoneAlarm endpointId):
// EP1 contact (alarm_1, inverted) + battery_low, EP2 vibration (alarm_2 = bit 1).
// Before, the wildcard contact converter turned an EP2 vibration notification
// into contact = true.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
// zoneStatus from commandStatusChangeNotification (first u16 after the ZCL header).
bool zone_status(const DecodedMessage& msg, std::uint16_t& status) {
    const std::size_t hdr = msg.manufacturer_specific ? 5u : 3u;
    if (msg.raw_data.size() < hdr + 2) return false;
    status = static_cast<std::uint16_t>(msg.raw_data[hdr] | (msg.raw_data[hdr + 1] << 8));
    return true;
}
bool fz_contact_ep1(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                    RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t st = 0;
    if (!zone_status(msg, st)) return false;
    Value v{}; v.type = ValueType::Bool;
    v.b = (st & 0x1) == 0; out.put("contact", v);
    v.b = (st & 0x8) != 0; out.put("battery_low", v);
    return true;
}
bool fz_vibration_ep2(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                      RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t st = 0;
    if (!zone_status(msg, st)) return false;
    Value v{}; v.type = ValueType::Bool; v.b = (st & 0x2) != 0;
    return out.put("vibration", v);
}
constexpr FzConverter kFzContactEp1{
    .family = FrameFamily::Zcl, .cluster = "ssIasZone",
    .type_mask = type_bit(MessageType::Command), .command_id = 0x00,
    .attr_id = WILDCARD_ATTR_ID, .endpoint = 1, .frame_flags_mask = 0, .frame_flags_value = 0,
    .direction = Direction::ServerToClient, .fn = { .zcl_fn = fz_contact_ep1 }, .user_config = nullptr,
};
constexpr FzConverter kFzVibrationEp2{
    .family = FrameFamily::Zcl, .cluster = "ssIasZone",
    .type_mask = type_bit(MessageType::Command), .command_id = 0x00,
    .attr_id = WILDCARD_ATTR_ID, .endpoint = 2, .frame_flags_mask = 0, .frame_flags_value = 0,
    .direction = Direction::ServerToClient, .fn = { .zcl_fn = fz_vibration_ep2 }, .user_config = nullptr,
};
const FzConverter* const kFz_HS2VTD[] = {
    &kFzContactEp1,
    &kFzVibrationEp2,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_HS2VTD[] = { "HS2VTD" };

constexpr Expose kExposes_HS2VTD[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, "Contact state", nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, "Battery low", nullptr, 0},
    {"vibration", ExposeType::Binary, Access::State, nullptr, "Vibration state", nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\u00b0C", "Measured temperature", nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", "Measured relative humidity", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", "Battery percentage", nullptr, 0},
};

constexpr BindingSpec kBindings_HS2VTD[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0402},
    {1, 0x0405},
};
}  // namespace

extern const PreparedDefinition kDef_HS2VTD{
    .zigbee_models=kModels_HS2VTD, .zigbee_models_count=sizeof(kModels_HS2VTD)/sizeof(kModels_HS2VTD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS2VTD", .vendor="HEIMAN",
    .meta=nullptr, .exposes=kExposes_HS2VTD, .exposes_count=sizeof(kExposes_HS2VTD)/sizeof(kExposes_HS2VTD[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS2VTD, .from_zigbee_count=sizeof(kFz_HS2VTD)/sizeof(kFz_HS2VTD[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HS2VTD, .bindings_count=sizeof(kBindings_HS2VTD)/sizeof(kBindings_HS2VTD[0]),
};

}  // namespace zhc::devices::heiman
