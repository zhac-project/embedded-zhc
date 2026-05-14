// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0502B tunable-white v2. z2m-source: tuya.ts #TS0502B.
//
// Two registrations:
//   - kDefTS0502B      — primary, matches any device reporting
//                        zigbeeModel="TS0502B" regardless of manu.
//   - kDefTS0502B_v2   — fingerprint variant for the MiBoxer FUT035Z+
//                        / E2-ZR / PZ2 family that reports zigbeeModel
//                        ="TS0503B" but is actually a CT light. Without
//                        this variant the device matches `tuya/TS0503B.cpp`
//                        (RGB light) and loses color_temp.
#include "definitions/_generic/_shared.hpp"
namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff, &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature };
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff, &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp };

constexpr const char* kModels[]    = { "TS0502B" };
constexpr const char* kModels_v2[] = { "TS0503B" };
// z2m: tuya.fingerprint("TS0503B", ["_TZB210_lmqquxus"]).
constexpr const char* kManus_v2[]  = { "_TZB210_lmqquxus" };
}

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

// z2m whiteLabel entries — UI metadata, runtime ignores. Manus from
// the per-label tuya.whitelabel(...) calls are NOT folded into the
// primary def's manus[] because the primary already matches any manu
// via zigbeeModel="TS0502B" — the labels are pure cosmetic display.
constexpr WhiteLabel kWhiteLabels_TS0502B[] = {
    {"Mercator Ikuü", "SMI7040"},        // Ford Batten Light
    {"Mercator Ikuü", "SMD9300"},        // Donovan Panel Light
    {"Aldi",          "F122SB62H22A4.5W"},
    {"Sibling",       "Light-ZSLL"},
    {"MiBoxer",       "FUT035Z+"},
    {"MiBoxer",       "E2-ZR"},
    {"MiBoxer",       "PZ2"},
    {"Lidl",          "14156408L"},
    {"EcoDim",        "ED-10032"},
    {"Mercator Ikuü", "SMCL01-ZB"},
    {"LUUMR",         "10024773"},
};

extern const PreparedDefinition kDefTS0502B{
    .zigbee_models=kModels,
    .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="TS0502B",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_TS0502B,
    .white_labels_count=sizeof(kWhiteLabels_TS0502B)/sizeof(kWhiteLabels_TS0502B[0]),
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

extern const PreparedDefinition kDefTS0502B_v2{
    .zigbee_models=kModels_v2,
    .zigbee_models_count=sizeof(kModels_v2)/sizeof(kModels_v2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_v2,
    .manufacturer_names_count=sizeof(kManus_v2)/sizeof(kManus_v2[0]),
    .model="TS0502B",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_TS0502B,
    .white_labels_count=sizeof(kWhiteLabels_TS0502B)/sizeof(kWhiteLabels_TS0502B[0]),
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}
