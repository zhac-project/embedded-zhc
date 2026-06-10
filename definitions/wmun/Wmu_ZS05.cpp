// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Wmun ZS05 — graduated parent override.
// Universal smart IR remote control on batteries.
//
// z2m-source: wmun.ts #ZS05.
//
// The auto-port (Tier 1) reduced this IR remote to a battery-only
// device — it kept only m.battery() and dropped the entire Zosung IR
// channel. z2m wires the full chunked-IR-transfer state machine via
// zosung.zosungExtend.addZosungIRTransmitCluster() (0xED00) +
// addZosungIRControlCluster() (0xE004), the six fzZosung.zosung_send_
// ir_code_00..05 fz handlers, and the two tzZosung handlers
// (ir_code_to_send, learn_ir_code), exposing learn_ir_code /
// learned_ir_code / ir_code_to_send.
//
// This def is the WMUN-branded TS1201 sibling of the Moes UFO-R11 and
// reuses the identical Zosung IR runtime (definitions/zosung/_shared).
// HARDWARE TESTING REQUIRED — protocol implementation faithful to z2m's
// reference but not yet exercised on real hardware (see zosung runtime).
#include "definitions/_generic/_shared.hpp"
#include "definitions/zosung/_shared.hpp"

namespace zhc::devices::wmun {
namespace {
const FzConverter* const kFz_ZS05[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::zosung::kFzZosungSendIrCode_00,
    &::zhc::zosung::kFzZosungSendIrCode_01,
    &::zhc::zosung::kFzZosungSendIrCode_02,
    &::zhc::zosung::kFzZosungSendIrCode_03,
    &::zhc::zosung::kFzZosungSendIrCode_04,
    &::zhc::zosung::kFzZosungSendIrCode_05,
};

const TzConverter* const kTz_ZS05[] = {
    &::zhc::zosung::kTzZosungIrCodeToSend,
    &::zhc::zosung::kTzZosungLearnIrCode,
};

constexpr const char* kModels_ZS05[] = { "TS1201" };
constexpr const char* kManus_ZS05[] = { "_TZ3290_u9xac5rv" };

}  // namespace


// --- expose bindings — extended for Zosung IR (mirrors Moes UFO-R11) ---
//
// learn_ir_code   — binary Set,   write triggers learn-mode magic.
// learned_ir_code — string State, populated when device returns code.
// ir_code_to_send — string Set,   base64 IR blob to transmit.
// battery, voltage — std genPowerCfg (m.battery()).
constexpr Expose kAutoExposes[] = {
    {"learn_ir_code",   ExposeType::Binary,  ::zhc::Access::Set,   nullptr, nullptr, nullptr, 0},
    {"learned_ir_code", ExposeType::String,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0},
    {"ir_code_to_send", ExposeType::String,  ::zhc::Access::Set,   nullptr, nullptr, nullptr, 0},
    {"battery",         ExposeType::Numeric, ::zhc::Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",         ExposeType::Numeric, ::zhc::Access::State, "mV",    nullptr, nullptr, 0},
};

// Bind genPowerCfg for battery reports + both Zosung clusters so the
// device can emit IRTransmit / IRControl frames to the coordinator.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, ::zhc::zosung::kZosungIRTransmitCluster},
    {1, ::zhc::zosung::kZosungIRControlCluster},
};
// --- end block ---

extern const PreparedDefinition kDef_ZS05{
    .zigbee_models=kModels_ZS05, .zigbee_models_count=sizeof(kModels_ZS05)/sizeof(kModels_ZS05[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZS05, .manufacturer_names_count=sizeof(kManus_ZS05)/sizeof(kManus_ZS05[0]),
    .model="ZS05", .vendor="Wmun",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZS05, .from_zigbee_count=sizeof(kFz_ZS05)/sizeof(kFz_ZS05[0]),
    .to_zigbee=kTz_ZS05,   .to_zigbee_count=sizeof(kTz_ZS05)/sizeof(kTz_ZS05[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::wmun
