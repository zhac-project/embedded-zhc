// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aubess ZXZIR-02 — universal smart IR remote control.
//
// z2m-source: aubess.ts #ZXZIR-02. The z2m record carries NO standard
// clusters — it is a pure Zosung IR blaster/learner built entirely from
// `zosung.zosungExtend.addZosungIRTransmitCluster()` +
// `addZosungIRControlCluster()`, the six `fzZosung.zosung_send_ir_code_00..05`
// decoders, the `tzZosung.zosung_ir_code_to_send` / `zosung_learn_ir_code`
// writers, and the three IR exposes from `zosung.presetsZosung`.
//
// The auto-generated Tier-1 port mis-classified it as a bare on/off +
// battery device (phantom `kFzOnOff` + `kFzBattery`, dead `state`/`battery`/
// `voltage` exposes, bindings to genOnOff 0x0006 + genPowerCfg 0x0001) — the
// device emits NONE of those clusters. Graduated to wire the existing Zosung
// IR runtime (definitions/zosung/_shared) instead, matching the Moes UFO-R11
// sibling (same TS1201 IR remote). Unlike the Moes record, z2m's Aubess def
// exposes NO battery, so no genPowerCfg binding / battery+voltage exposes.
//
// HARDWARE TESTING REQUIRED — the Zosung protocol implementation is faithful
// to z2m's reference but has not been exercised on real hardware.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zosung/_shared.hpp"

namespace zhc::devices::aubess {
namespace {

const FzConverter* const kFz_ZXZIR_02[] = {
    &::zhc::zosung::kFzZosungSendIrCode_00,
    &::zhc::zosung::kFzZosungSendIrCode_01,
    &::zhc::zosung::kFzZosungSendIrCode_02,
    &::zhc::zosung::kFzZosungSendIrCode_03,
    &::zhc::zosung::kFzZosungSendIrCode_04,
    &::zhc::zosung::kFzZosungSendIrCode_05,
};

const TzConverter* const kTz_ZXZIR_02[] = {
    &::zhc::zosung::kTzZosungIrCodeToSend,
    &::zhc::zosung::kTzZosungLearnIrCode,
};

constexpr const char* kModels_ZXZIR_02[] = { "TS1201" };
constexpr const char* kManus_ZXZIR_02[] = { "_TZ3290_acv1iuslxi3shaaj" };

// z2m presetsZosung: learn_ir_code (binary SET), learned_ir_code (text
// STATE), ir_code_to_send (text SET). No battery — the device reports none.
constexpr Expose kExp_ZXZIR_02[] = {
    { "learn_ir_code",   ExposeType::Binary, ::zhc::Access::Set,   nullptr, nullptr, nullptr, 0 },
    { "learned_ir_code", ExposeType::String, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "ir_code_to_send", ExposeType::String, ::zhc::Access::Set,   nullptr, nullptr, nullptr, 0 },
};

// Bind both Zosung clusters so the device can push IRTransmit / IRControl
// frames to the coordinator. No genPowerCfg — z2m exposes no battery.
constexpr BindingSpec kBind_ZXZIR_02[] = {
    { 1, ::zhc::zosung::kZosungIRTransmitCluster },
    { 1, ::zhc::zosung::kZosungIRControlCluster },
};
}  // namespace

extern const PreparedDefinition kDef_ZXZIR_02{
    .zigbee_models=kModels_ZXZIR_02, .zigbee_models_count=sizeof(kModels_ZXZIR_02)/sizeof(kModels_ZXZIR_02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZXZIR_02, .manufacturer_names_count=sizeof(kManus_ZXZIR_02)/sizeof(kManus_ZXZIR_02[0]),
    .model="ZXZIR-02", .vendor="Aubess",
    .meta=nullptr, .exposes=kExp_ZXZIR_02, .exposes_count=sizeof(kExp_ZXZIR_02)/sizeof(kExp_ZXZIR_02[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZXZIR_02, .from_zigbee_count=sizeof(kFz_ZXZIR_02)/sizeof(kFz_ZXZIR_02[0]),
    .to_zigbee=kTz_ZXZIR_02,   .to_zigbee_count=sizeof(kTz_ZXZIR_02)/sizeof(kTz_ZXZIR_02[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_ZXZIR_02,
    .bindings_count           = sizeof(kBind_ZXZIR_02)/sizeof(kBind_ZXZIR_02[0]),
};

}  // namespace zhc::devices::aubess
