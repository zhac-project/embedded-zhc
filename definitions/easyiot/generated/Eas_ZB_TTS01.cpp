// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-TTS01 — auto-generated.
// TTS Converter for Simplified Chinese GB2312 encoded text.
// z2m-source: easyiot.ts #ZB-TTS01.
//
// RUNTIME BLOCKER: send_tts encodes the payload as GB2312 and pushes
// it through seTunneling.transferData (tunnelId 0x0000). Status
// arrives via commandTransferData. Both directions need a
// vendor-specific encoder/decoder; exposed here as schema only.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
constexpr const char* kModels_ZB_TTS01[] = { "ZB-TTS01" };

// TODO(easyiot/ZB-TTS01): seTunneling transferData encoder + GB2312
// payload converter.

constexpr Expose kExposes_ZB_TTS01[] = {
    {"last_received_status", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
    {"send_tts",             ExposeType::String, Access::Set,   nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_TTS01[] = {
    {1, 0x1000},  // seTunneling
};

}  // namespace

extern const PreparedDefinition kDef_ZB_TTS01{
    .zigbee_models=kModels_ZB_TTS01, .zigbee_models_count=sizeof(kModels_ZB_TTS01)/sizeof(kModels_ZB_TTS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-TTS01", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_TTS01, .exposes_count=sizeof(kExposes_ZB_TTS01)/sizeof(kExposes_ZB_TTS01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_TTS01, .bindings_count=sizeof(kBindings_ZB_TTS01)/sizeof(kBindings_ZB_TTS01[0]),
};

}  // namespace zhc::devices::easyiot
