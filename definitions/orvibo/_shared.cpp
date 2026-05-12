// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/orvibo/_shared.hpp"

#include "definitions/_generic/_shared.hpp"

namespace zhc::orvibo {
namespace {

// 0xFF00 attr 0x0001 uint8: 1 = off, 2 = previous (no `on`).
//
// z2m-source: orvibo.ts `orviboSwitchPowerOnBehavior` —
//   const powerOnLookup2: {[k: string]: number} = {off: 1, previous: 2};
//
// herdsman writes this without a manuCode because the cluster itself
// is already vendor-private (declared via deviceAddCustomCluster
// without a manufacturerCode field).
constexpr ::zhc::generic::ZclWriteLookup kPowerOnBehaviorLookup[] = {
    {"off",      1},
    {"previous", 2},
};

constexpr ::zhc::generic::ZclWriteSpec kSpecPowerOnBehavior{
    "power_on_behavior",
    /* attr_id           = */ 0x0001,
    /* attr_type         = */ 0x20,            // ZCL uint8
    /* manufacturer_code = */ 0x0000,
    /* lookup            = */ kPowerOnBehaviorLookup,
    /* lookup_count      = */ sizeof(kPowerOnBehaviorLookup) /
                              sizeof(kPowerOnBehaviorLookup[0]),
};

constexpr const char* kPowerOnBehaviorEnum[] = {"off", "previous"};

}  // namespace

extern const ::zhc::Expose kExposePowerOnBehaviorOrvibo{
    /* name        = */ "power_on_behavior",
    /* type        = */ ::zhc::ExposeType::Enum,
    /* access      = */ ::zhc::Access::StateSet,
    /* unit        = */ nullptr,
    /* description = */ nullptr,
    /* enum_values = */ kPowerOnBehaviorEnum,
    /* enum_count  = */ 2,
    /* category    = */ ::zhc::ExposeCategory::Config,
};

extern const ::zhc::TzConverter kTzOrviboPowerOnBehavior{
    /* key         = */ "power_on_behavior",
    /* cluster     = */ "manuSpecificOrvibo2",
    /* cluster_id  = */ 0xFF00,
    /* command_id  = */ 0x02,                  // writeAttributes
    /* fn          = */ &::zhc::generic::tz_zcl_write_attr,
    /* user_config = */ &kSpecPowerOnBehavior,
};

}  // namespace zhc::orvibo
