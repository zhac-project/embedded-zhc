// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara ZNMHLDJ01LM smart vertical blinds motor H1
// (zigbeeModel "lumi.curtain.acn011"; z2m v26.105.0, #13093).
//
// Plain closuresWindowCovering with lift + tilt. Upstream marks the device
// `coverInverted` (it reports 100 = open natively); the generic lift/tilt
// decoders pass the reported percentage straight through, which is exactly
// the value z2m publishes for this device. identify() is upstream surface
// with no ZHC counterpart.
// z2m-source: lumi.ts #ZNMHLDJ01LM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::lumi::kFzLumiBasic,
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzCoverTilt,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPositionLift,
    &::zhc::generic::kTzCoverPositionTilt,
};
constexpr const char* kModels[] = { "lumi.curtain.acn011" };
constexpr const char* kStateOpts[] = { "OPEN", "CLOSE", "STOP" };
constexpr Expose kExposes[] = {
    {"state",    ExposeType::Enum,    Access::Set,      nullptr, "Open, close or stop the blinds", kStateOpts, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", "Lift position", nullptr, 0},
    {"tilt",     ExposeType::Numeric, Access::StateSet, "%", "Tilt position", nullptr, 0},
};
constexpr BindingSpec kBindings[] = {
    {1, 0x0102},
};
}  // namespace

extern const PreparedDefinition kDefZNMHLDJ01LM{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="ZNMHLDJ01LM",.vendor="Aqara",
    .meta=nullptr,.exposes=kExposes,.exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};
}  // namespace zhc::devices::lumi
