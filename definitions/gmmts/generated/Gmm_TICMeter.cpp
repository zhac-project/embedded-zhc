// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2 (PARTIAL): GammaTroniques TICMeter — hand-tuned.
// TICMeter pour Linky — French Linky electricity-meter reader.
// z2m-source: gmmts.ts #TICMeter (sole device in vendor file, lines 2089-2410).
//
// Coverage in this port:
//   * vendor "GammaTroniques" (was incorrectly "Gmmts") + zigbeeModel ["TICMeter"]
//   * generic seMetering + haElectricalMeasurement decoders
//     (~50 of the ~105 z2m attributes; see docs/GMMTS_PARITY.md)
//   * bindings on ep1 to 0x0702 / 0x0B04 / 0x0B01 / 0xFF42
//
// TODO (runtime blockers, see docs/GMMTS_PARITY.md):
//   * Custom 0xFF42 manuSpecificGmmts cluster (~55 attrs) — needs a
//     ZHC_GMMTS_TZ runtime handler with mfg-code writes (z2m passes
//     manufacturerCode: null, so a non-manu-spec write is acceptable
//     once a converter is wired up).
//   * Dynamic exposes builder (filter by tic_mode / contract_type /
//     elec_mode / producer / translation user options).
//   * fz.meter_identification (cluster 0x0B01) — no shared converter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gmmts {
namespace {

constexpr const char* kModels_TICMeter[] = { "TICMeter" };

const FzConverter* const kFz_TICMeter[] = {
    &::zhc::generic::kFzMetering,              // seMetering 0x0702 — covers ~19 attrs
    &::zhc::generic::kFzElectricalMeasurement, // haElectricalMeasurement 0x0B04 — covers ~31 attrs
};

// to_zigbee left empty: every writeable attribute lives in the custom
// manuSpecificGmmts (0xFF42) cluster. See doc for runtime patch plan.

// Conservative static exposes — the standard-cluster attrs that are active
// for every contract / tic / elec combination. Names match z2m exactly
// (z2m exposes property snake_case via toSnakeCase()).
constexpr Expose kExp_TICMeter[] = {
    { "energy",       ExposeType::Numeric, ::zhc::Access::State, "kWh", nullptr, nullptr, 0 },
    { "power",        ExposeType::Numeric, ::zhc::Access::State, "W",   nullptr, nullptr, 0 },
    { "current",      ExposeType::Numeric, ::zhc::Access::State, "A",   nullptr, nullptr, 0 },
    { "voltage",      ExposeType::Numeric, ::zhc::Access::State, "V",   nullptr, nullptr, 0 },
    { "ac_frequency", ExposeType::Numeric, ::zhc::Access::State, "Hz",  nullptr, nullptr, 0 },
};

// z2m configure() (line 2330) binds these four clusters to coordinator.
constexpr BindingSpec kBind_TICMeter[] = {
    { 1, 0x0702 },  // seMetering
    { 1, 0x0B04 },  // haElectricalMeasurement
    { 1, 0x0B01 },  // seMeterIdentification
    { 1, 0xFF42 },  // manuSpecificGmmts (custom)
};

}  // namespace

extern const PreparedDefinition kDef_TICMeter{
    .zigbee_models=kModels_TICMeter,
    .zigbee_models_count=sizeof(kModels_TICMeter)/sizeof(kModels_TICMeter[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TICMeter", .vendor="GammaTroniques",
    .meta=nullptr,
    .exposes=kExp_TICMeter,
    .exposes_count=sizeof(kExp_TICMeter)/sizeof(kExp_TICMeter[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TICMeter,
    .from_zigbee_count=sizeof(kFz_TICMeter)/sizeof(kFz_TICMeter[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_TICMeter,
    .bindings_count=sizeof(kBind_TICMeter)/sizeof(kBind_TICMeter[0]),
};

}  // namespace zhc::devices::gmmts
