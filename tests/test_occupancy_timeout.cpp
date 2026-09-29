// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// PreparedDefinition::occupancy_timeout — the hub-side "no motion" interval.
//
// z2m (v26.105.0) offers the per-device option `occupancy_timeout` through the
// host-timer occupancy converters, for sensors that report motion but never
// "no motion":
//   fz.ias_occupancy_alarm_1_with_timeout  default 90 s
//   fz.ias_ace_occupancy_with_timeout      default 90 s
//   fz.occupancy_with_timeout              default 90 s
//   lumi.fz.lumi_occupancy[_illuminance]   default detection_interval + 2
//                                          (60 + 2; RTCGQ14LM 30 + 2)
// Pins: every such def that decodes `occupancy` here carries z2m's default;
// the defs z2m gives no such option keep 0 (they report "no motion"
// themselves, or have a device-side delay like the Hue sensors).

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstring>
#include <span>

#include "definitions/_generic/_shared.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"

namespace zhc::devices {
namespace bitron { extern const PreparedDefinition kDef_AV2010_14, kDef_AV2010_22, kDef_AV2010_22A, kDef_AV2010_22B; }
namespace hive { extern const PreparedDefinition kDef_MOT003, kDef_KEYPAD001; }
namespace hommyn { extern const PreparedDefinition kDef_MS_20_Z; }
namespace jxuan { extern const PreparedDefinition kDef_PRZ01; }
namespace konke { extern const PreparedDefinition kDef_D2AJZ4KPBS, kDef_KK_BS_J01W; }
namespace livingwise { extern const PreparedDefinition kDef_LVS_SN10ZW_SN11; }
namespace lumi { extern const PreparedDefinition kDefRTCGQ01LM, kDefRTCGQ11LM, kDefRTCGQ12LM,
                                                  kDefRTCGQ13LM, kDefRTCGQ14LM, kDefRTCGQ15LM; }
namespace orvibo { extern const PreparedDefinition kDef_SN10ZW; }
namespace technicolor { extern const PreparedDefinition kDef_XHK1_TC; }
namespace terncy { extern const PreparedDefinition kDef_TERNCY_PP01; }
namespace tuya { extern const PreparedDefinition kDefSM0202, kDefTS0202, kDefTS0202_1; }
namespace universal_electronics_inc { extern const PreparedDefinition kDef_XHK1_UE, kDef_UEHK2AZ0; }
namespace philips { extern const PreparedDefinition kDef_D9290012607; }
}  // namespace zhc::devices

using namespace zhc;
using namespace zhc::devices;

namespace {

bool exposes(const PreparedDefinition& d, const char* key) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (d.exposes[i].name && std::strcmp(d.exposes[i].name, key) == 0) return true;
    return false;
}

bool decodes_with(const PreparedDefinition& d, const FzConverter& fz) {
    for (std::size_t i = 0; i < d.from_zigbee_count; ++i)
        if (d.from_zigbee[i] == &fz) return true;
    return false;
}

// A def that gets the interval must also emit and expose `occupancy`.
void check(const PreparedDefinition& d, std::uint16_t want) {
    assert(d.occupancy_timeout == want);
    assert(exposes(d, "occupancy"));
}

}  // namespace

int main() {
    // fz.ias_occupancy_alarm_1_with_timeout — 90 s.
    for (const auto* d : {&bitron::kDef_AV2010_14, &bitron::kDef_AV2010_22, &bitron::kDef_AV2010_22A,
                          &bitron::kDef_AV2010_22B, &hive::kDef_MOT003, &hommyn::kDef_MS_20_Z,
                          &jxuan::kDef_PRZ01, &konke::kDef_D2AJZ4KPBS, &konke::kDef_KK_BS_J01W,
                          &livingwise::kDef_LVS_SN10ZW_SN11, &orvibo::kDef_SN10ZW,
                          &tuya::kDefSM0202, &tuya::kDefTS0202_1})
        check(*d, 90);

    // fz.ias_ace_occupancy_with_timeout — 90 s (keypads that also decode the
    // IAS zone here; 3400-D and ZS130000078 decode no occupancy, so no option).
    for (const auto* d : {&hive::kDef_KEYPAD001, &technicolor::kDef_XHK1_TC,
                          &universal_electronics_inc::kDef_XHK1_UE, &universal_electronics_inc::kDef_UEHK2AZ0})
        check(*d, 90);

    // fz.occupancy_with_timeout — 90 s.
    for (const auto* d : {&lumi::kDefRTCGQ01LM, &lumi::kDefRTCGQ11LM, &terncy::kDef_TERNCY_PP01})
        check(*d, 90);

    // lumi_occupancy / lumi_occupancy_illuminance — detection_interval + 2.
    check(lumi::kDefRTCGQ12LM, 62);
    check(lumi::kDefRTCGQ13LM, 62);
    check(lumi::kDefRTCGQ14LM, 32);
    check(lumi::kDefRTCGQ15LM, 62);

    // SM0202: z2m decodes `occupancy` (ias_occupancy_alarm_1_with_timeout); the
    // port used the bare IAS zone decoder, which emits `alarm`.
    assert(decodes_with(tuya::kDefSM0202, generic::kFzIasMotionAlarm));
    assert(!exposes(tuya::kDefSM0202, "alarm"));

    // TS0202_1: z2m's five TS0202 manufacturers that never send "no motion"
    // resolve to their own def; any other TS0202 keeps the generic one, which
    // gets no interval (z2m: plain fz.ias_occupancy_alarm_1).
    const std::span<const PreparedDefinition* const> tuya_reg(tuya::kTuyaRegistry, tuya::kTuyaRegistryCount);
    for (const char* m : {"_TYZB01_jytabjkb", "_TZ3000_lltemgsf", "_TYZB01_5nr7ncpl",
                          "_TZ3000_mg4dy6z6", "_TZ3000_bsvqrxru"})
        assert(find_definition("TS0202", m, tuya_reg) == &tuya::kDefTS0202_1);
    assert(find_definition("TS0202", "_TZ3000_hgu1dlak", tuya_reg) == &tuya::kDefTS0202);
    assert(tuya::kDefTS0202.occupancy_timeout == 0);
    assert(decodes_with(tuya::kDefTS0202_1, generic::kFzIasMotionAlarm));
    assert(decodes_with(tuya::kDefTS0202_1, generic::kFzBattery));

    // A device-side delay stays the device's: Hue motion sensors keep 0.
    assert(philips::kDef_D9290012607.occupancy_timeout == 0);

    // Opt-in only: the rest of the Tuya registry (generated defs included)
    // carries no interval.
    std::size_t tuya_with = 0;
    for (const auto* d : tuya_reg) tuya_with += (d && d->occupancy_timeout) ? 1 : 0;
    assert(tuya_with == 2);
    return 0;
}
