// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Xenon Smart X7633 natural gas detector, TS0601 `_TZE204_0corc0ph`
// (z2m v26.133.0); white label Zorro Alert JT-ZL2010Z on the same fingerprint.
// DPs: 1 gas (trueFalseEnum0), 10 preheat, 11 fault bitmap -> fault_alarm (any
// bit) + one Bool per bit (fault, serious_fault, sensor_fault, probe_fault,
// power_fault), 12 end_of_life (trueFalseInvert).
// z2m-source: tuya.ts #X7633.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
// DP11 fault bitmap, upstream's inline `from`: fault_alarm = v != 0, then
// bit 0..4 each to its own Bool key.
bool expand_faults(const ::zhc::tuya::TuyaDpMapEntry&, const Value& raw, RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint64_t bits = 0;
    if      (raw.type == ValueType::Uint) bits = raw.u;
    else if (raw.type == ValueType::Int)  bits = static_cast<std::uint64_t>(raw.i);
    else return false;
    static constexpr const char* kBitKeys[] = { "fault", "serious_fault", "sensor_fault", "probe_fault", "power_fault" };
    Value v{}; v.type = ValueType::Bool;
    v.b = bits != 0; out.put("fault_alarm", v);
    for (std::size_t i = 0; i < sizeof(kBitKeys)/sizeof(kBitKeys[0]); ++i) {
        v.b = ((bits >> i) & 1u) != 0; out.put(kBitKeys[i], v);
    }
    return true;
}
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "gas"),
        ::zhc::tuya::dp::binary(10, "preheat"),
        { 11, nullptr, ::zhc::TuyaDpType::Bitmap, 1, nullptr, 0, 0, 0.0f, &expand_faults, nullptr },
        ::zhc::tuya::dp::binary_inv(12, "end_of_life"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr Expose kExp[] = {
    {"gas", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected gas", nullptr, 0},
    {"preheat", ExposeType::Binary, Access::State, nullptr, "Indicates sensor preheat is active", nullptr, 0},
    {"fault_alarm", ExposeType::Binary, Access::State, nullptr, "Indicates whether a fault was detected", nullptr, 0},
    {"fault", ExposeType::Binary, Access::State, nullptr, "Indicates whether a general fault was detected", nullptr, 0},
    {"serious_fault", ExposeType::Binary, Access::State, nullptr, "Indicates whether a serious fault was detected", nullptr, 0},
    {"sensor_fault", ExposeType::Binary, Access::State, nullptr, "Indicates whether a sensor fault was detected", nullptr, 0},
    {"probe_fault", ExposeType::Binary, Access::State, nullptr, "Indicates whether a probe fault was detected", nullptr, 0},
    {"power_fault", ExposeType::Binary, Access::State, nullptr, "Indicates whether a power fault was detected", nullptr, 0},
    {"end_of_life", ExposeType::Binary, Access::State, nullptr, "Indicates whether the sensor has reached its end of life and should be replaced", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_0corc0ph" };
constexpr WhiteLabel kWL[] = { {"Zorro Alert", "JT-ZL2010Z"} };
}  // namespace

extern const PreparedDefinition kDef_X7633{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="X7633",.vendor="Xenon Smart",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
