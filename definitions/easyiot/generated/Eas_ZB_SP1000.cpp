// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-SP1000 — auto-generated.
// MP3 player that supports up to 1,000 voice clips.
// z2m-source: easyiot.ts #ZB-SP1000.
//
// RUNTIME BLOCKER: play_voice / set_volume go via
// seTunneling.transferData (tunnelId 0x0000) using the data-point
// frame layout (cmd-byte | sub-byte | len | type | value). Status
// arrives via commandTransferData. Vendor-specific encoder TBD.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
constexpr const char* kModels_ZB_SP1000[] = { "ZB-SP1000" };

// TODO(easyiot/ZB-SP1000): seTunneling data-point encoder.

constexpr Expose kExposes_ZB_SP1000[] = {
    {"last_received_status", ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"play_voice",           ExposeType::Numeric, Access::Set,   nullptr, nullptr, nullptr, 0},
    {"set_volume",           ExposeType::Numeric, Access::Set,   nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_SP1000[] = {
    {1, 0x1000},  // seTunneling
};

}  // namespace

extern const PreparedDefinition kDef_ZB_SP1000{
    .zigbee_models=kModels_ZB_SP1000, .zigbee_models_count=sizeof(kModels_ZB_SP1000)/sizeof(kModels_ZB_SP1000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-SP1000", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_SP1000, .exposes_count=sizeof(kExposes_ZB_SP1000)/sizeof(kExposes_ZB_SP1000[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_SP1000, .bindings_count=sizeof(kBindings_ZB_SP1000)/sizeof(kBindings_ZB_SP1000[0]),
};

}  // namespace zhc::devices::easyiot
