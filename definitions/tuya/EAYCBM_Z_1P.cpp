// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: EARU EAYCBM-Z-1P smart circuit breaker with energy metering
// (`_TZE204_davzgqq0`; white label EAYCBM-Z-2P with RCD), new upstream in
// z2m v26.133.0.
//
//   dp6   phaseVariant2 — upstream's NARROW read (low two bytes of current
//         and power), reproduced as on ZBN-DJ-63.
//   dp17  thresholdBlocks([[4, "leakage"]]) and dp18 eaycbmThresholds
//         ([[1, over_current], [3, over_voltage], [4, under_voltage]]) — the
//         same 4-byte [code, on, value_be16] records dp::thresholds decodes,
//         publishing `<name>_breaker` / `<name>_threshold`.
//
// NOT PORTED:
//   * dp18 writes — upstream's thresholdBlocks.to rebuilds every record from
//     the merged device state (multi-key read-modify-write), which a ZHC tz
//     converter cannot do. The breaker / threshold keys are read-only here.
//   * dp19 `breaker_id` — a string datapoint; no string DP codec here.
//   * queryOnDeviceAnnounce — the 2P variant only reports dp17/18 when
//     queried; no DP query on announce here.
//
// z2m-source: tuya.ts #EAYCBM-Z-1P.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaThresholdDef kLeakageDefs[] = {
    { 4, "leakage_breaker", "leakage_threshold" },
};
constexpr ::zhc::tuya::TuyaThresholdTable kLeakage{ kLeakageDefs, 1 };

constexpr ::zhc::tuya::TuyaThresholdDef kEaycbmDefs[] = {
    { 1, "over_current_breaker",  "over_current_threshold"  },
    { 3, "over_voltage_breaker",  "over_voltage_threshold"  },
    { 4, "under_voltage_breaker", "under_voltage_threshold" },
};
constexpr ::zhc::tuya::TuyaThresholdTable kEaycbm{
    kEaycbmDefs, sizeof(kEaycbmDefs)/sizeof(kEaycbmDefs[0]) };

struct cfg_eaycbm {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::phase_variant2(6, &::zhc::tuya::kTuyaPhaseKeysPlain),
        ::zhc::tuya::dp::binary(16, "state"),
        ::zhc::tuya::dp::thresholds(17, &kLeakage),
        ::zhc::tuya::dp::thresholds(18, &kEaycbm),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_eaycbm>;

constexpr Expose kExposes[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "On/off state of the switch", nullptr, 0 },
    { "energy",  ExposeType::Numeric, ::zhc::Access::State, "kWh", nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, ::zhc::Access::State, "W",   nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "V",   nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, ::zhc::Access::State, "A",   nullptr, nullptr, 0 },
    // Protection — read-only, see the header note.
    { "over_current_breaker",    ExposeType::Binary,  ::zhc::Access::State, nullptr,
      "Switch off when current exceeds the threshold", nullptr, 0 },
    { "over_current_threshold",  ExposeType::Numeric, ::zhc::Access::State, "A",
      "Overcurrent threshold", nullptr, 0, ExposeCategory::State, 1, 63, 1 },
    { "over_voltage_breaker",    ExposeType::Binary,  ::zhc::Access::State, nullptr,
      "Switch off when voltage exceeds the threshold", nullptr, 0 },
    { "over_voltage_threshold",  ExposeType::Numeric, ::zhc::Access::State, "V",
      "Overvoltage threshold", nullptr, 0, ExposeCategory::State, 230, 300, 1 },
    { "under_voltage_breaker",   ExposeType::Binary,  ::zhc::Access::State, nullptr,
      "Switch off when voltage drops below the threshold", nullptr, 0 },
    { "under_voltage_threshold", ExposeType::Numeric, ::zhc::Access::State, "V",
      "Undervoltage threshold", nullptr, 0, ExposeCategory::State, 80, 220, 1 },
    { "leakage_breaker",   ExposeType::Binary,  ::zhc::Access::State, nullptr,
      "Residual current protection enabled (EAYCBM-Z-2P with RCD only)", nullptr, 0 },
    { "leakage_threshold", ExposeType::Numeric, ::zhc::Access::State, "mA",
      "Residual current trip threshold (EAYCBM-Z-2P with RCD only)", nullptr, 0 },
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE204_davzgqq0" };
constexpr WhiteLabel kWL[] = { {"EARU", "EAYCBM-Z-2P"} };

}  // namespace

extern const PreparedDefinition kDef_EAYCBM_Z_1P{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="EAYCBM-Z-1P", .vendor="EARU",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=kWL, .white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
