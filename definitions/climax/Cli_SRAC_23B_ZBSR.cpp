// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Climax SRAC-23B-ZBSR — IAS dead-key + missing-channel fix 2026-06-10.
// Smart siren. z2m fz: battery, ias_wd, ias_enroll, ias_siren.
//   * ias_siren is a commandStatusChangeNotification decoder (cmd 0x00) that
//     emits `alarm` from zoneStatus bit0 (+ tamper bit2 / battery_low bit3).
//     The auto-port wired the generic kFzIasZone, which only reads the
//     ssIasZone ZoneStatus *attribute* (attr 0x0002 report/readResponse) and
//     produces nothing on the status-change-notification *command* a siren
//     actually sends → `alarm` was dead. Swapped to kFzIasGenericAlarm, the
//     command-path bit0→`alarm` converter (z2m m.iasZoneAlarm{zoneType:alarm}).
//   * ias_wd (ssIasWd maxDuration, attr 0x0000) was dropped entirely — the
//     `max_duration` channel and expose were missing. Added kFzIasWdMaxDuration
//     (decode) + kTzIasWdMaxDuration (write) and bound ssIasWd (0x0502).
//   * ias_enroll is global IAS enrollment in ZHC — no per-device converter.
// Exposes brought to z2m parity: warning + squawk (the tz already existed,
// the exposes were missing) + max_duration.
// z2m-source: climax.ts #SRAC-23B-ZBSR.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::climax {
namespace {
const FzConverter* const kFz_SRAC_23B_ZBSR[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasGenericAlarm,
    &::zhc::generic::kFzIasWdMaxDuration,
};
const TzConverter* const kTz_SRAC_23B_ZBSR[] = {
    &::zhc::generic::kTzWarning,
    &::zhc::generic::kTzIasWdWarningSimple,
    &::zhc::generic::kTzIasWdMaxDuration,
    &::zhc::generic::kTzIasWdSquawk,
};
constexpr const char* kModels_SRAC_23B_ZBSR[] = { "SRACBP5_00.00.03.06TC", "SRAC_00.00.00.16TC", "SRACBP5_00.00.05.10TC", "SRACB_00.00.03.07TC" };

constexpr Expose kExp_SRAC_23B_ZBSR[] = {
    {"battery",      ExposeType::Numeric, Access::State,    "%", nullptr, nullptr, 0},
    {"voltage",      ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"alarm",        ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"tamper",       ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"battery_low",  ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"warning",      ExposeType::Binary,  Access::Set,      nullptr, nullptr, nullptr, 0},
    {"squawk",       ExposeType::Binary,  Access::Set,      nullptr, nullptr, nullptr, 0},
    {"max_duration", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_SRAC_23B_ZBSR[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0502},
};

}  // namespace

extern const PreparedDefinition kDef_SRAC_23B_ZBSR{
    .zigbee_models=kModels_SRAC_23B_ZBSR, .zigbee_models_count=sizeof(kModels_SRAC_23B_ZBSR)/sizeof(kModels_SRAC_23B_ZBSR[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SRAC-23B-ZBSR", .vendor="Climax",
    .meta=nullptr, .exposes=kExp_SRAC_23B_ZBSR, .exposes_count=sizeof(kExp_SRAC_23B_ZBSR)/sizeof(kExp_SRAC_23B_ZBSR[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SRAC_23B_ZBSR, .from_zigbee_count=sizeof(kFz_SRAC_23B_ZBSR)/sizeof(kFz_SRAC_23B_ZBSR[0]),
    .to_zigbee=kTz_SRAC_23B_ZBSR, .to_zigbee_count=sizeof(kTz_SRAC_23B_ZBSR)/sizeof(kTz_SRAC_23B_ZBSR[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SRAC_23B_ZBSR, .bindings_count=sizeof(kBind_SRAC_23B_ZBSR)/sizeof(kBind_SRAC_23B_ZBSR[0]),
};

}  // namespace zhc::devices::climax
