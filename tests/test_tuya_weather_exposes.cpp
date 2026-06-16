// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Guards the two Tuya-DP weather defs that now ship EXPOSES WITH UNITS so the local
// web-ui and the cloud (both render units from the device's exposes) show temperature/
// humidity units — previously only WSD500A did, because the others shipped
// `.exposes=nullptr` (device.get → empty array → no units anywhere).
//   _TZE204_upagmta9 : temperature "C", humidity "%", battery_state (enum) — pure T/H.
//   air_house_keeper : temperature "C", humidity "%", voc, co2, pm25 — multi-sensor.
// The DP decode itself is unchanged (covered by the _TZE204_upagmta9 fixture test); this
// asserts only the declared exposes + units (the fix).

#include <cassert>
#include <cstddef>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDef_TS0601_TZE204_upagmta9;
extern const PreparedDefinition kDefTS0601_air_house_keeper;
}  // namespace zhc::devices::tuya

using namespace zhc;

namespace {
const Expose* find_expose(const PreparedDefinition& d, const char* key) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (d.exposes[i].name && std::strcmp(d.exposes[i].name, key) == 0) return &d.exposes[i];
    return nullptr;
}
bool numeric_with_unit(const Expose* e, const char* unit) {
    return e && e->type == ExposeType::Numeric && e->unit && std::strcmp(e->unit, unit) == 0;
}
}  // namespace

int main() {
    // _TZE204_upagmta9 — pure temperature/humidity sensor → climate_sensor in the cloud.
    {
        const auto& d = devices::tuya::kDef_TS0601_TZE204_upagmta9;
        assert(numeric_with_unit(find_expose(d, "temperature"), "C"));
        assert(numeric_with_unit(find_expose(d, "humidity"), "%"));
        const Expose* bs = find_expose(d, "battery_state");
        assert(bs && bs->type == ExposeType::Enum && bs->enum_count == 3);  // low/medium/high
        assert(!find_expose(d, "state"));   // not an on/off device
    }

    // air_house_keeper — multi-sensor; must keep ALL five readouts (with units on temp/hum).
    {
        const auto& d = devices::tuya::kDefTS0601_air_house_keeper;
        assert(numeric_with_unit(find_expose(d, "temperature"), "C"));
        assert(numeric_with_unit(find_expose(d, "humidity"), "%"));
        assert(find_expose(d, "voc"));
        assert(find_expose(d, "co2"));
        assert(find_expose(d, "pm25"));
        assert(!find_expose(d, "state"));
    }
    return 0;
}
