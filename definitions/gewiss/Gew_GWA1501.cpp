// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Gewiss GWA1501 — graduated.
// Contact interface - 2 channels
// z2m-source: gewiss.ts #GWA1501.
// z2m extend = [deviceEndpoints{1,2}, m.battery(), binary(input) x2 via
// genBinaryInput.presentValue]. Battery was ported correctly, but the
// two `input` channels (genBinaryInput presentValue on ep1/ep2) were
// dropped. Add kFzBinaryInput → `input` (dispatch suffixes to input_1 /
// input_2 via endpoint_map), keep battery.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gewiss {
namespace {
const FzConverter* const kFz_GWA1501[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzBinaryInput,
};

constexpr const char* kModels_GWA1501[] = { "GWA1501_BinaryInput_FC" };

constexpr ::zhc::EndpointLabel kEndpoints_GWA1501[] = { {"1", 1}, {"2", 2} };

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"input", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x000F},
    {2, 0x000F},
};
}  // namespace

extern const PreparedDefinition kDef_GWA1501{
    .zigbee_models=kModels_GWA1501, .zigbee_models_count=sizeof(kModels_GWA1501)/sizeof(kModels_GWA1501[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GWA1501", .vendor="Gewiss",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_GWA1501, .from_zigbee_count=sizeof(kFz_GWA1501)/sizeof(kFz_GWA1501[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map             = kEndpoints_GWA1501,
    .endpoint_map_count       = sizeof(kEndpoints_GWA1501)/sizeof(kEndpoints_GWA1501[0]),
};

}  // namespace zhc::devices::gewiss
