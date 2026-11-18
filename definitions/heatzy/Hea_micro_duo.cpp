// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heatzy micro-duo pilot wire controller with relay, energy metering and
// Bluetooth OTA (new vendor in z2m v26.126.0..v26.133.0). Fingerprint: modelID
// micro-duo + manufacturerName Heatzy.
// m.deviceEndpoints {relay: 10, pilot_wire: 1, ble_ota: 11}: keys carry the
// endpoint name, as z2m's (state_relay, power_relay, energy_relay,
// pilot_wire_mode_pilot_wire, ble_ota_status_ble_ota).
//   relay (10)      m.onOff (no power-on behaviour) + m.electricityMeter power
//                   (haElectricalMeasurement) and energy (seMetering)
//   pilot_wire (1)  heatzyPilotWire 0xFC00 attr 0x0000 u8: pilot_wire_mode
//                   {off, comfort, eco, frost_protection, comfort_-1, comfort_-2}
//   ble_ota (11)    heatzyBleOta 0xFC01 attr 0x0000 u8: ble_ota_status (read-only)
//                   {disabled, advertising, connected, receiving, verifying}
// 0xFC00 is labelled "manuSpecificPhilips" in zhc/cluster_names.hpp (cosmetic).
// 0xFC01 has no name there (non-manufacturer frames), so its frames reach every
// converter of the def ("fail open"): its attr 0 would also land in genOnOff
// state, seMetering energy and pilot_wire_mode. gated() keeps those off unnamed
// frames and the ble_ota_status map on them (endpoint 11) only.
// Meter reporting: z2m scales each change by the divisor/multiplier it reads at
// configure time; the rows assume 1/1 (power 5, energy 1 raw unit).
// Not ported: the bleOtaControl extend (ble_ota switch: startBleOta /
// stopBleOta commands on 0xFC01 + re-read); ota.
// z2m-source: heatzy.ts #micro-duo.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heatzy {
namespace {

namespace g = ::zhc::generic;

// A named wrapper skips frames of unnamed clusters; an unnamed one (cluster
// nullptr) takes only those, non-manufacturer-specific.
// ponytail: copied per def (also edflab, hzc_electric, repenic_ltd); hoist into
// _generic if more defs need it.
bool fz_gated(const DecodedMessage& msg, const FzConverter& self, const PreparedDefinition& def,
              RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const bool named = msg.cluster != nullptr;
    if (self.cluster ? !named : (named || msg.manufacturer_specific)) return false;
    const auto* inner = static_cast<const FzConverter*>(self.user_config);
    return inner->fn.zcl_fn(msg, *inner, def, ctx, out);
}
constexpr FzConverter gated(const char* cluster, const FzConverter* inner,
                            std::uint8_t endpoint = WILDCARD_ENDPOINT) {
    FzConverter c = g::zcl_attr_fz(cluster, nullptr);
    c.endpoint = endpoint;
    c.fn.zcl_fn = fz_gated;
    c.user_config = inner;
    return c;
}

constexpr std::uint8_t kEpRelay = 10;
constexpr std::uint8_t kEpPilotWire = 1;
constexpr std::uint8_t kEpBleOta = 11;

constexpr g::ZclWriteLookup kPilotWireModes[] = {
    {"off", 0}, {"comfort", 1}, {"eco", 2}, {"frost_protection", 3}, {"comfort_-1", 4}, {"comfort_-2", 5} };
constexpr g::ZclAttrRow kPilotRows[] = {
    { 0x0000, "pilot_wire_mode", 1, kPilotWireModes, sizeof(kPilotWireModes)/sizeof(kPilotWireModes[0]) },
};
constexpr g::ZclAttrMap kPilotMap{ kPilotRows, sizeof(kPilotRows)/sizeof(kPilotRows[0]) };
constexpr FzConverter kFzPilotRaw = g::zcl_attr_fz("manuSpecificPhilips", &kPilotMap);
constexpr FzConverter kFzPilot = gated("manuSpecificPhilips", &kFzPilotRaw, kEpPilotWire);
constexpr g::ZclWriteSpec kPilotSpec{
    "pilot_wire_mode", 0x0000, 0x20, 0, kPilotWireModes, sizeof(kPilotWireModes)/sizeof(kPilotWireModes[0]) };
constexpr TzConverter kTzPilot = g::zcl_write_tz("heatzyPilotWire", 0xFC00, &kPilotSpec, kEpPilotWire);

constexpr g::ZclWriteLookup kBleOtaStates[] = {
    {"disabled", 0}, {"advertising", 1}, {"connected", 2}, {"receiving", 3}, {"verifying", 4} };
constexpr g::ZclAttrRow kBleOtaRows[] = {
    { 0x0000, "ble_ota_status", 1, kBleOtaStates, sizeof(kBleOtaStates)/sizeof(kBleOtaStates[0]) },
};
constexpr g::ZclAttrMap kBleOtaMap{ kBleOtaRows, sizeof(kBleOtaRows)/sizeof(kBleOtaRows[0]) };
constexpr FzConverter kFzBleOtaRaw = g::zcl_attr_fz(nullptr, &kBleOtaMap);
constexpr FzConverter kFzBleOta = gated(nullptr, &kFzBleOtaRaw, kEpBleOta);

constexpr FzConverter kFzOnOff = gated("genOnOff", &g::kFzOnOff);
constexpr FzConverter kFzMetering = gated("seMetering", &g::kFzMetering);
constexpr g::ZclAttrRow kPowerRows[] = { { 0x050B, "power" } };
constexpr g::ZclAttrMap kPowerMap{ kPowerRows, sizeof(kPowerRows)/sizeof(kPowerRows[0]) };
constexpr FzConverter kFzPower = g::zcl_attr_fz("haElectricalMeasurement", &kPowerMap);

const FzConverter* const kFz[] = {
    &kFzOnOff,
    &kFzPower,
    &kFzMetering,
    &kFzPilot,
    &kFzBleOta,
};
const TzConverter* const kTz[] = {
    &g::kTzOnOff,
    &kTzPilot,
};
constexpr const char* kPilotWireLabels[] = {
    "off", "comfort", "eco", "frost_protection", "comfort_-1", "comfort_-2" };
constexpr const char* kBleOtaLabels[] = { "disabled", "advertising", "connected", "receiving", "verifying" };
constexpr Expose kExp[] = {
    {"state_relay", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_relay", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy_relay", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"pilot_wire_mode_pilot_wire", ExposeType::Enum, Access::StateSet, nullptr, "Pilot wire mode",
     kPilotWireLabels, 6},
    {"ble_ota_status_ble_ota", ExposeType::Enum, Access::State, nullptr, "Current Bluetooth OTA state",
     kBleOtaLabels, 5},
};
constexpr ::zhc::EndpointLabel kEndpoints[] = {
    {"relay", kEpRelay}, {"pilot_wire", kEpPilotWire}, {"ble_ota", kEpBleOta} };
constexpr const char* kM[] = { "micro-duo" };
constexpr const char* kManu[] = { "Heatzy" };
constexpr BindingSpec kBind[] = {
    {kEpRelay, 0x0006},
    {kEpRelay, 0x0B04},
    {kEpRelay, 0x0702},
    {kEpPilotWire, 0xFC00},
    {kEpBleOta, 0xFC01},
};
constexpr ReportingSpec kRep[] = {
    {kEpRelay, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},    // onOff MIN..MAX
    {kEpRelay, 0x0B04, 0x050B, 0x29, 10, 65000, 5, 0},   // activePower
    {kEpRelay, 0x0702, 0x0000, 0x25, 10, 65000, 1, 0},   // currentSummDelivered
    {kEpPilotWire, 0xFC00, 0x0000, 0x20, 0, 65000, 1, 0},
    {kEpBleOta, 0xFC01, 0x0000, 0x20, 0, 65000, 1, 0},
};
}  // namespace

extern const PreparedDefinition kDef_micro_duo{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManu, .manufacturer_names_count=sizeof(kManu)/sizeof(kManu[0]),
    .model="micro-duo", .vendor="Heatzy",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::heatzy
