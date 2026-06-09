// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ctm CTM_MBD_Dim — graduated from generated/ for the occupancy fix.
// MBD Dim, motion detector with dimmer.
//
// Parity fix: same dead-`occupancy` gap as MBD-S — now decoded by the
// generic `kFzOccupancy` (msOccupancySensing 0x0406, z2m fz.occupancy).
// z2m: occupancy + ctm_mbd_device_enabled + ctm_relay_state + brightness
// + lighting_ballast_configuration + m.illuminance(). No IAS.
// z2m-source: ctm.ts #CTM_MBD_Dim.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ctm/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
const FzConverter* const kFz_CTM_MBD_Dim[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
};
const TzConverter* const kTz_CTM_MBD_Dim[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::ctm::kTzCtmDeviceEnabled,
    &::zhc::ctm::kTzCtmRelayState,
};
constexpr const char* kModels_CTM_MBD_Dim[] = { "MBD Dim" };

}  // namespace


// Manual: drop spurious IAS exposes; occupancy now decoded by
// kFzOccupancy + illuminance + device_enabled.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"device_enabled", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0400},
    {1, 0x0406},
};

extern const PreparedDefinition kDef_CTM_MBD_Dim{
    .zigbee_models=kModels_CTM_MBD_Dim, .zigbee_models_count=sizeof(kModels_CTM_MBD_Dim)/sizeof(kModels_CTM_MBD_Dim[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CTM_MBD_Dim", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CTM_MBD_Dim, .from_zigbee_count=sizeof(kFz_CTM_MBD_Dim)/sizeof(kFz_CTM_MBD_Dim[0]),
    .to_zigbee=kTz_CTM_MBD_Dim, .to_zigbee_count=sizeof(kTz_CTM_MBD_Dim)/sizeof(kTz_CTM_MBD_Dim[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
