// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Makegood MG-AUZG01 — graduated from generated/.
// Double Zigbee power point (with power monitoring)
// z2m-source: makegood.ts #MG-AUZG01.
//
// Gap class (b) — MISSING decoder/channel. In z2m the device uses
// `tuya.modernExtend.tuyaOnOff({endpoints:["l1","l2"], electricalMeasurements:true})`.
// The `electricalMeasurements:true` branch (tuya.ts tuyaOnOff) pushes
// `fz.electrical_measurement, fz.metering` and exposes power/current/
// voltage/energy. The auto-port did not expand that modernExtend, so the
// generated def carried on/off ONLY — the metering surface (0x0702 energy
// + 0x0B04 power/voltage/current) was entirely dropped. Wire the generic
// seMetering + haElectricalMeasurement decoders + the 4 exposes + bindings
// (z2m configure binds genOnOff, haElectricalMeasurement, seMetering on EP1).
//
// DEFERRED (INFRA, gap class f — dual-endpoint metering split): z2m declares
// two switch endpoints (l1:1, l2:2) and keeps power/current/voltage/energy
// global via meta.multiEndpointSkip while suffixing only the switch state per
// endpoint. The runtime suffix rewrite (src/runtime/dispatch.cpp) has no
// per-def "metering-skip" hook — adding an endpoint_map here would wrongly
// suffix power/current/energy to `_l1` (only `voltage` is in
// kAlwaysGlobalKeys[]), and adding those keys to that global list would
// de-suffix genuine per-socket metering on other devices. Same defer as the
// structurally-identical Honyar U86Z223A10-ZJU01(GD) (HY0157). The device
// therefore ships a single bare `state` + a single (EP1) metering surface,
// which is correct for the metered socket; the second-gang state split waits
// on the dispatch infra.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::makegood {
namespace {
const FzConverter* const kFz_MG_AUZG01[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_MG_AUZG01[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_MG_AUZG01[] = { "TS011F" };
constexpr const char* kManus_MG_AUZG01[] = { "_TZ3000_dd8wwzcy" };
}  // namespace


// --- exposes + bindings (z2m tuyaOnOff electricalMeasurements + configure) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
    {2, 0x0006},
};
// --- end block ---

extern const PreparedDefinition kDef_MG_AUZG01{
    .zigbee_models=kModels_MG_AUZG01, .zigbee_models_count=sizeof(kModels_MG_AUZG01)/sizeof(kModels_MG_AUZG01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_MG_AUZG01, .manufacturer_names_count=sizeof(kManus_MG_AUZG01)/sizeof(kManus_MG_AUZG01[0]),
    .model="MG-AUZG01", .vendor="Makegood",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MG_AUZG01, .from_zigbee_count=sizeof(kFz_MG_AUZG01)/sizeof(kFz_MG_AUZG01[0]),
    .to_zigbee=kTz_MG_AUZG01, .to_zigbee_count=sizeof(kTz_MG_AUZG01)/sizeof(kTz_MG_AUZG01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::makegood
