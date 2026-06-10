// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Dlink DCH-B112 — door/window sensor with vibration.
// Wireless smart door window sensor with vibration.
//
// z2m-source: dlink.ts #DCH-B112 (fzLocal.DCH_B112 + fz.battery).
//
// Why a vendor converter: z2m's fzLocal.DCH_B112 decodes the ssIasZone
// ZoneStatusChangeNotification (cmd 0x00) into FOUR semantic keys off a
// single zoneStatus word:
//   contact     = !(bit0)   (inverted — magnet present / door closed => true)
//   vibration   =  bit1
//   tamper      =  bit2
//   battery_low =  bit3
// The auto port wired the generic kFzIasZone, which emits a bare `alarm`
// (bit0, NOT inverted) and never produces `contact` or `vibration`. The
// generic typed IAS converters each emit exactly ONE alarm key, so neither
// can cover the contact+vibration pair this device reports together. Hence
// a dedicated decoder here.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::dlink {
namespace {

// z2m fzLocal.DCH_B112 (ssIasZone commandStatusChangeNotification).
// Payload after the 3-byte ZCL header (5 if manufacturer-specific):
// zoneStatus:u16 LE, extendedStatus:u8, zoneId:u8, delay:u16.
bool fz_dch_b112_zone(const ::zhc::DecodedMessage& msg, const ::zhc::FzConverter&,
                      const ::zhc::PreparedDefinition&, ::zhc::RuntimeContext&,
                      ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    const std::uint16_t status =
        static_cast<std::uint16_t>(msg.raw_data[hdr]) |
        (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);

    ::zhc::Value v{}; v.type = ::zhc::ValueType::Bool;
    v.b = (status & 0x0001) == 0;  out.put("contact",     v);  // inverted bit0
    v.b = (status & 0x0002) != 0;  out.put("vibration",   v);  // bit1
    v.b = (status & 0x0004) != 0;  out.put("tamper",      v);  // bit2
    v.b = (status & 0x0008) != 0;  out.put("battery_low", v);  // bit3
    return true;
}

const ::zhc::FzConverter kFzDchB112Zone{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // ZoneStatusChangeNotification flows server (device) -> client (coord).
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_dch_b112_zone },
    .user_config       = nullptr,
};

const FzConverter* const kFz_DCH_B112[] = {
    &::zhc::generic::kFzBattery,
    &kFzDchB112Zone,
};

constexpr const char* kModels_DCH_B112[] = { "DCH-B112" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"vibration", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_DCH_B112{
    .zigbee_models=kModels_DCH_B112, .zigbee_models_count=sizeof(kModels_DCH_B112)/sizeof(kModels_DCH_B112[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DCH-B112", .vendor="Dlink",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DCH_B112, .from_zigbee_count=sizeof(kFz_DCH_B112)/sizeof(kFz_DCH_B112[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::dlink
