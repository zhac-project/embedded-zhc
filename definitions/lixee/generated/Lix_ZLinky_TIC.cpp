// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lixee ZLinky_TIC.
// Lixee ZLinky — French ENEDIS Linky meter ↔ Zigbee bridge.
// z2m-source: lixee.ts #ZLinky_TIC.
//
// v1 covers the auto-tariff core: meter id (ADSC/PRM), instantaneous
// electrical (URMS1/IRMS1/SINSTS/CCASN), cumulative kWh (EAST/EAIT
// + EASF01-04), and contract metadata (current_tarif/tariff_period
// /relais). Tariff-mode-specific subsets (BBR/EJP/TEMPO daily
// counters, three-phase phB/phC) defer to v2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lixee/_shared.hpp"

namespace zhc::devices::lixee {
namespace {

constexpr const char* kModels_ZLinky_TIC[] = { "ZLinky_TIC" };

const FzConverter* const kFz_ZLinky_TIC[] = {
    &::zhc::lixee::kFzLixeePrivate,
    &::zhc::lixee::kFzLixeeElec,
    &::zhc::lixee::kFzLixeeMetering,
};

constexpr Expose kExp_ZLinky_TIC[] = {
    // Meter identity
    { "meter_serial_number",  ExposeType::String,  ::zhc::Access::State, nullptr,  "Serial Number (ADSC)",                       nullptr, 0 },
    { "site_id",              ExposeType::String,  ::zhc::Access::State, nullptr,  "PRM number",                                 nullptr, 0 },
    // Contract / tariff
    { "current_tarif",        ExposeType::String,  ::zhc::Access::State, nullptr,  "Supplier pricing schedule name (NGTF)",      nullptr, 0 },
    { "tariff_period",        ExposeType::String,  ::zhc::Access::State, nullptr,  "Current tariff period (modern firmware)",    nullptr, 0 },
    { "current_index_tarif",  ExposeType::Numeric, ::zhc::Access::State, nullptr,  "Current tariff index number (NTARF)",        nullptr, 0 },
    { "active_register_tier", ExposeType::Numeric, ::zhc::Access::State, nullptr,  "Active register tier (legacy)",              nullptr, 0 },
    { "tomorrow_color",       ExposeType::String,  ::zhc::Access::State, nullptr,  "Tomorrow's color (TEMPO)",                   nullptr, 0 },
    { "schedule_hphc",        ExposeType::Numeric, ::zhc::Access::State, nullptr,  "HC/HP schedule",                             nullptr, 0 },
    { "current_date",         ExposeType::String,  ::zhc::Access::State, nullptr,  "Current date and time (DATE)",               nullptr, 0 },
    // Instantaneous electrical (haElectricalMeasurement)
    { "rms_voltage",          ExposeType::Numeric, ::zhc::Access::State, "V",      "RMS voltage (URMS1)",                        nullptr, 0 },
    { "rms_current",          ExposeType::Numeric, ::zhc::Access::State, "A",      "RMS current (IRMS1)",                        nullptr, 0 },
    { "rms_current_max",      ExposeType::Numeric, ::zhc::Access::State, "A",      "Max RMS current today",                      nullptr, 0 },
    { "active_power",         ExposeType::Numeric, ::zhc::Access::State, "W",      "Current active load (CCASN)",                nullptr, 0 },
    { "active_power_max",     ExposeType::Numeric, ::zhc::Access::State, "W",      "Max active load",                            nullptr, 0 },
    { "apparent_power",       ExposeType::Numeric, ::zhc::Access::State, "VA",     "Apparent power (SINSTS)",                    nullptr, 0 },
    { "reactive_power",       ExposeType::Numeric, ::zhc::Access::State, "VAr",    "Reactive power",                             nullptr, 0 },
    { "total_reactive_power", ExposeType::Numeric, ::zhc::Access::State, "VArh",   "Total reactive power Q1 (ERQ1)",             nullptr, 0 },
    // Cumulative energy (seMetering, Wh→kWh internally)
    { "current_summ_delivered", ExposeType::Numeric, ::zhc::Access::State, "kWh",  "Total active energy delivered (EAST)",       nullptr, 0 },
    { "current_summ_received",  ExposeType::Numeric, ::zhc::Access::State, "kWh",  "Total active energy injected (EAIT)",        nullptr, 0 },
    { "tier1_summ_delivered",   ExposeType::Numeric, ::zhc::Access::State, "kWh",  "Energy index tier 1 (EASF01)",               nullptr, 0 },
    { "tier2_summ_delivered",   ExposeType::Numeric, ::zhc::Access::State, "kWh",  "Energy index tier 2 (EASF02)",               nullptr, 0 },
    { "tier3_summ_delivered",   ExposeType::Numeric, ::zhc::Access::State, "kWh",  "Energy index tier 3 (EASF03)",               nullptr, 0 },
    { "tier4_summ_delivered",   ExposeType::Numeric, ::zhc::Access::State, "kWh",  "Energy index tier 4 (EASF04)",               nullptr, 0 },
    // Contract physical
    { "relais",               ExposeType::Numeric, ::zhc::Access::State, nullptr,  "Virtual dry-contact relay bitmap",           nullptr, 0 },
    { "status_register",      ExposeType::String,  ::zhc::Access::State, nullptr,  "Linky status register (hex string)",         nullptr, 0 },
};

// Bind the three TIC clusters on EP1 — bind happens at join, no
// reporting setup attempted in v1 (z2m polls many of these).
constexpr BindingSpec kBind_ZLinky_TIC[] = {
    { 1, 0x0702 },  // seMetering
    { 1, 0x0B04 },  // haElectricalMeasurement
    { 1, 0xFF66 },  // liXeePrivate (manuSpec)
};

}  // namespace

extern const PreparedDefinition kDef_ZLinky_TIC{
    .zigbee_models             = kModels_ZLinky_TIC,
    .zigbee_models_count       = sizeof(kModels_ZLinky_TIC) / sizeof(kModels_ZLinky_TIC[0]),
    .manufacturer_name_prefix  = nullptr,
    .manufacturer_names        = nullptr,
    .manufacturer_names_count  = 0,
    .model                     = "ZLinky_TIC",
    .vendor                    = "Lixee",
    .meta                      = nullptr,
    .exposes                   = kExp_ZLinky_TIC,
    .exposes_count             = sizeof(kExp_ZLinky_TIC) / sizeof(kExp_ZLinky_TIC[0]),
    .white_labels              = nullptr,
    .white_labels_count        = 0,
    .from_zigbee               = kFz_ZLinky_TIC,
    .from_zigbee_count         = sizeof(kFz_ZLinky_TIC) / sizeof(kFz_ZLinky_TIC[0]),
    .to_zigbee                 = nullptr,
    .to_zigbee_count           = 0,
    .configure                 = nullptr,
    .on_event                  = nullptr,
    .bindings                  = kBind_ZLinky_TIC,
    .bindings_count            = sizeof(kBind_ZLinky_TIC) / sizeof(kBind_ZLinky_TIC[0]),
};

}  // namespace zhc::devices::lixee
