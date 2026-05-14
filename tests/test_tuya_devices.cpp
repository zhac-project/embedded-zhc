// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Phase 4 smoke — every entry in kTuyaRegistry has a valid
// PreparedDefinition shape. Acts as the "N ported" dashboard.

#include <cassert>
#include <cstdio>
#include <cstring>
#include <set>
#include <string_view>

#include "zhc/devices/tuya_registry.hpp"

using namespace zhc;

int main() {
    const auto& reg = devices::tuya::kTuyaRegistry;
    const auto  n   = devices::tuya::kTuyaRegistryCount;

    // Track (model, first-manu) pairs. A model is allowed to appear in
    // multiple registry entries when each entry has a distinct manu set
    // (z2m mirrors this — e.g. TS0502B has both a zigbeeModel match and
    // a tuya.fingerprint("TS0503B", [manu]) variant under the same
    // user-facing `model`).
    std::set<std::pair<std::string_view, std::string_view>> seen;
    for (std::size_t i = 0; i < n; ++i) {
        const auto* d = reg[i];
        assert(d && d->model && d->model[0]);
        assert(d->zigbee_models_count >= 1);
        // Empty fz is permitted for match-only stubs (e.g. SNTZ009,
        // U86KCJ_ZP) that exist in registry to claim a zigbeeModel but
        // contribute no decoders. Either both pointer and count are
        // zero, OR pointer is non-null and every slot is non-null.
        if (d->from_zigbee_count > 0) {
            assert(d->from_zigbee != nullptr);
            for (std::uint8_t j = 0; j < d->from_zigbee_count; ++j) {
                assert(d->from_zigbee[j] != nullptr);
            }
        } else {
            assert(d->from_zigbee == nullptr);
        }
        std::string_view manu0 = (d->manufacturer_names_count > 0 &&
                                    d->manufacturer_names &&
                                    d->manufacturer_names[0])
            ? std::string_view(d->manufacturer_names[0])
            : std::string_view("");
        auto [it, inserted] = seen.insert({d->model, manu0});
        if (!inserted) {
            std::fprintf(stderr,
                         "duplicate tuya (model, first-manu): %s / %s\n",
                         d->model, manu0.data());
            assert(false);
        }
    }
    std::printf("tuya registry: %zu devices ported\n", n);
    return 0;
}
