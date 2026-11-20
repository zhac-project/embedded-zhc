// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// NoDieby ND-01: the five 0xFC00 settings carry state. z2m gives them access
// ALL and its convertSet hands the value back as state, so the hub page lists
// them with the value set last, not as a Commands-tab form that starts on the
// first value. There is no read-back decoder (z2m reads them only on a get).
//
// z2m-source: nodieby.ts #ND-01.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <initializer_list>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::nodieby {
extern const PreparedDefinition kDef_ND_01;
}  // namespace zhc::devices::nodieby

using namespace zhc;

int main() {
    const PreparedDefinition& def = devices::nodieby::kDef_ND_01;
    int seen = 0;
    for (const char* key : {"led_brightness", "volume", "sensitivity", "alarm_duration", "alarm_delay"})
        for (std::uint8_t i = 0; i < def.exposes_count; ++i)
            if (std::strcmp(def.exposes[i].name, key) == 0) {
                assert(def.exposes[i].access == Access::StateSet);
                ++seen;
            }
    assert(seen == 5);
    return 0;
}
