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

    std::set<std::string_view> models;
    for (std::size_t i = 0; i < n; ++i) {
        const auto* d = reg[i];
        assert(d && d->model && d->model[0]);
        assert(d->zigbee_models_count >= 1);
        assert(d->from_zigbee && d->from_zigbee_count >= 1);
        for (std::uint8_t j = 0; j < d->from_zigbee_count; ++j) {
            assert(d->from_zigbee[j] != nullptr);
        }
        auto [it, inserted] = models.insert(d->model);
        if (!inserted) {
            std::fprintf(stderr, "duplicate tuya model: %s\n", d->model);
            assert(false);
        }
    }
    std::printf("tuya registry: %zu devices ported\n", n);
    return 0;
}
