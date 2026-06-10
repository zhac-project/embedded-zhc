// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Blitzwolf BW-IS3 — Tuya-MCU (0xEF00) DP-stream PIR motion sensor.
// Rechargeable Zigbee PIR motion sensor.
//
// Bug fixed: the auto-port wired the generic kFzIasZone (ssIasZone 0x0500)
// and exposed phantom alarm/tamper/battery_low with a dead 0x0500 binding.
// The device speaks NONE of that — z2m decodes it via
// legacy.fz.blitzwolf_occupancy_with_timeout, a manuSpecificTuya (0xEF00)
// commandDataResponse DP stream where DP3 (`dataPoints.occupancy`) carries
// presence. The IAS-vs-DP misroute left the single real channel (occupancy)
// dead. Graduated to a Tier-2 override wiring fz_tuya_datapoints with the
// DP3 -> occupancy map, exposing occupancy, binding 0xEF00.
//
// z2m-source: blitzwolf.ts #BW-IS3 + lib/legacy.ts
//             fz.blitzwolf_occupancy_with_timeout (dataPoints.occupancy = 3).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::blitzwolf {
namespace {
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry entries[] = {
        ::zhc::tuya::dp::binary(3, "occupancy"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{entries, 1};
};
using FX = ::zhc::tuya::factory::TuyaOnOff<cfg>;

constexpr const char* kModels_BW_IS3[] = { "5j6ifxj" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};

extern const PreparedDefinition kDef_BW_IS3{
    .zigbee_models=kModels_BW_IS3, .zigbee_models_count=sizeof(kModels_BW_IS3)/sizeof(kModels_BW_IS3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BW-IS3", .vendor="Blitzwolf",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::blitzwolf
