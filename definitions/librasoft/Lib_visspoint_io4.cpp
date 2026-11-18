// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: LibraSoft visspoint-io4, I/O module with 4 relays and 4 dry-contact
// inputs (new vendor in z2m v26.126.0..v26.133.0). Endpoints 1-4.
// Ported: m.onOff({powerOnBehavior: false}) per endpoint -> state_1..state_4
// (genOnOff bound and onOff reported on each endpoint); m.binary input =
// genBinaryInput presentValue per endpoint -> input_1..input_4 (true = contact
// closed; upstream reads it at configure, no binding or reporting).
// Not ported (partial port): the per-channel `trigger` pulse time. Upstream
// stores it in the device's database meta (nothing is sent to the device) and
// turns a plain ON into On With Timed Off while it is non-zero, then reads the
// relay back after the pulse; that needs persisted host-side config and a
// timer. Here ON is a plain ON.
// z2m-source: librasoft.ts #visspoint-io4.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::librasoft {
namespace {

const FzConverter* const kFz_visspoint_io4[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBinaryInput,
};
const TzConverter* const kTz_visspoint_io4[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_visspoint_io4[] = { "visspoint-io4" };

constexpr ::zhc::EndpointLabel kEndpoints_visspoint_io4[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4} };

constexpr const char* kInputDesc = "Dry contact input: true = contact closed";
constexpr Expose kExposes_visspoint_io4[] = {
    {"state_1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"input_1", ExposeType::Binary, Access::State, nullptr, kInputDesc, nullptr, 0},
    {"input_2", ExposeType::Binary, Access::State, nullptr, kInputDesc, nullptr, 0},
    {"input_3", ExposeType::Binary, Access::State, nullptr, kInputDesc, nullptr, 0},
    {"input_4", ExposeType::Binary, Access::State, nullptr, kInputDesc, nullptr, 0},
};

constexpr BindingSpec kBindings_visspoint_io4[] = {
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006}, {4, 0x0006},
};
constexpr ReportingSpec kReports_visspoint_io4[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
    {2, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
    {3, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
    {4, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
};

}  // namespace

extern const PreparedDefinition kDef_visspoint_io4{
    .zigbee_models=kModels_visspoint_io4, .zigbee_models_count=sizeof(kModels_visspoint_io4)/sizeof(kModels_visspoint_io4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="visspoint-io4", .vendor="LibraSoft",
    .meta=nullptr, .exposes=kExposes_visspoint_io4, .exposes_count=sizeof(kExposes_visspoint_io4)/sizeof(kExposes_visspoint_io4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_visspoint_io4, .from_zigbee_count=sizeof(kFz_visspoint_io4)/sizeof(kFz_visspoint_io4[0]),
    .to_zigbee=kTz_visspoint_io4, .to_zigbee_count=sizeof(kTz_visspoint_io4)/sizeof(kTz_visspoint_io4[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_visspoint_io4, .bindings_count=sizeof(kBindings_visspoint_io4)/sizeof(kBindings_visspoint_io4[0]),
    .reports=kReports_visspoint_io4, .reports_count=sizeof(kReports_visspoint_io4)/sizeof(kReports_visspoint_io4[0]),
    .endpoint_map=kEndpoints_visspoint_io4,
    .endpoint_map_count=sizeof(kEndpoints_visspoint_io4)/sizeof(kEndpoints_visspoint_io4[0]),
};

}  // namespace zhc::devices::librasoft
