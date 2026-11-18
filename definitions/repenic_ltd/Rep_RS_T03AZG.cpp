// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Repenic Ltd. RS-T03AZG smart plug with power monitoring (zigbeeModels
// RS-T03AZG, RS-R03AZG; new in z2m v26.126.0..v26.133.0).
// m.onOff (no power-on behaviour) + m.electricityMeter() (power, voltage,
// current from haElectricalMeasurement, energy from seMetering) +
// remote_mode: repenicPrivateE00B 0xE00B attr 0x0000 enum8 {load 0, remote 1},
// read and written without a manufacturer code.
// 0xE00B has no name in zhc/cluster_names.hpp, so its frames reach every
// converter of the def ("fail open"): its attr 0 would also land in genOnOff
// state and seMetering energy. gated() keeps those two converters off unnamed
// frames and the remote_mode map on them only.
// Meter reporting: z2m scales each change by the divisor/multiplier it reads at
// configure time; the rows below assume 1/1 (power 5, voltage 5, current and
// energy 1 raw unit).
// z2m-source: repenic_ltd.ts #RS-T03AZG.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::repenic_ltd {
namespace {

namespace g = ::zhc::generic;

// A named wrapper skips frames of unnamed clusters; an unnamed one (cluster
// nullptr) takes only those, non-manufacturer-specific.
// ponytail: copied per def (also heatzy, edflab, hzc_electric); hoist into
// _generic if more defs need it.
bool fz_gated(const DecodedMessage& msg, const FzConverter& self, const PreparedDefinition& def,
              RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const bool named = msg.cluster != nullptr;
    if (self.cluster ? !named : (named || msg.manufacturer_specific)) return false;
    const auto* inner = static_cast<const FzConverter*>(self.user_config);
    return inner->fn.zcl_fn(msg, *inner, def, ctx, out);
}
constexpr FzConverter gated(const char* cluster, const FzConverter* inner) {
    FzConverter c = g::zcl_attr_fz(cluster, nullptr);
    c.fn.zcl_fn = fz_gated;
    c.user_config = inner;
    return c;
}

constexpr g::ZclWriteLookup kRemoteModes[] = { {"load", 0}, {"remote", 1} };
constexpr g::ZclAttrRow kRemoteRows[] = {
    { 0x0000, "remote_mode", 1, kRemoteModes, sizeof(kRemoteModes)/sizeof(kRemoteModes[0]) },
};
constexpr g::ZclAttrMap kRemoteMap{ kRemoteRows, sizeof(kRemoteRows)/sizeof(kRemoteRows[0]) };
constexpr FzConverter kFzRemoteRaw = g::zcl_attr_fz(nullptr, &kRemoteMap);
constexpr FzConverter kFzRemote = gated(nullptr, &kFzRemoteRaw);
constexpr FzConverter kFzOnOff = gated("genOnOff", &g::kFzOnOff);
constexpr FzConverter kFzMetering = gated("seMetering", &g::kFzMetering);
constexpr g::ZclWriteSpec kRemoteSpec{
    "remote_mode", 0x0000, 0x30, 0, kRemoteModes, sizeof(kRemoteModes)/sizeof(kRemoteModes[0]) };
constexpr TzConverter kTzRemote = g::zcl_write_tz("repenicPrivateE00B", 0xE00B, &kRemoteSpec);

const FzConverter* const kFz[] = {
    &kFzOnOff,
    &kFzMetering,
    &g::kFzElectricalMeasurement,
    &kFzRemote,
};
const TzConverter* const kTz[] = {
    &g::kTzOnOff,
    &kTzRemote,
};
constexpr const char* kRemoteLabels[] = { "load", "remote" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"remote_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "Remote controller mode: with load or remote only", kRemoteLabels, 2},
};
constexpr const char* kM[] = { "RS-T03AZG", "RS-R03AZG" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},    // onOff MIN..MAX
    {1, 0x0B04, 0x050B, 0x29, 10, 65000, 5, 0},   // activePower
    {1, 0x0B04, 0x0505, 0x21, 10, 65000, 5, 0},   // rmsVoltage
    {1, 0x0B04, 0x0508, 0x21, 10, 65000, 1, 0},   // rmsCurrent
    {1, 0x0702, 0x0000, 0x25, 10, 65000, 1, 0},   // currentSummDelivered
};
}  // namespace

extern const PreparedDefinition kDef_RS_T03AZG{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RS-T03AZG", .vendor="RepenicLtd",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::repenic_ltd
