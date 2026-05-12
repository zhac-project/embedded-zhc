// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm MBD-S — auto-generated.
// MBD-S, motion detector with 16A relay
// z2m-source: ctm.ts #MBD-S.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ctm/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
// MBD-S z2m: fz.occupancy + fz.ctm_mbd_device_enabled + fz.ctm_relay_state
// + m.illuminance(). BLOCKED: msOccupancySensing 0x0406 has no generic
// decoder in `_generic/_shared.cpp` (only `kFzIgnoreOccupancyReport`
// today). Use the shared CTM relay-state writer for the manuSpec write.
const FzConverter* const kFz_MBD_S[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzIlluminance,
};
const TzConverter* const kTz_MBD_S[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::ctm::kTzCtmDeviceEnabled,
    &::zhc::ctm::kTzCtmRelayState,
};
constexpr const char* kModels_MBD_S[] = { "MBD-S" };

}  // namespace


// Manual: drop spurious IAS exposes (device has no zone); add occupancy
// (decode TODO) + illuminance + device_enabled. Bindings retain 0x0406
// so the device still sends reports — they'll surface as raw attribute
// keys until ZHC gains a generic occupancy decoder.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"device_enabled", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0400},
    {1, 0x0406},
};

extern const PreparedDefinition kDef_MBD_S{
    .zigbee_models=kModels_MBD_S, .zigbee_models_count=sizeof(kModels_MBD_S)/sizeof(kModels_MBD_S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MBD-S", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MBD_S, .from_zigbee_count=sizeof(kFz_MBD_S)/sizeof(kFz_MBD_S[0]),
    .to_zigbee=kTz_MBD_S, .to_zigbee_count=sizeof(kTz_MBD_S)/sizeof(kTz_MBD_S[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
