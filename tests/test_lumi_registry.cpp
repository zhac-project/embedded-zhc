// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: registry smoke + uniqueness tests.
//
// Every entry in `kLumiRegistry` must be non-null, unique, and carry a
// non-empty `model` + `zigbee_models_count >= 1`. Also acts as the
// "porting dashboard" — prints the port count for CI.

#include <cassert>
#include <cstdio>
#include <cstring>
#include <set>
#include <string_view>

#include "zhc/devices/lumi_registry.hpp"

using namespace zhc;

int main() {
    const auto& reg  = devices::lumi::kLumiRegistry;
    const auto  n    = devices::lumi::kLumiRegistryCount;

    std::set<std::string_view> seen_models;
    std::set<std::string_view> seen_zigbee_models;
    for (std::size_t i = 0; i < n; ++i) {
        const auto* def = reg[i];
        assert(def != nullptr);
        assert(def->model && def->model[0] != '\0');
        assert(def->zigbee_models_count >= 1);
        assert(def->zigbee_models && def->zigbee_models[0]);

        auto [it, inserted] = seen_models.insert(def->model);
        if (!inserted) {
            std::fprintf(stderr, "duplicate model: %s\n", def->model);
            assert(false);
        }

        for (std::uint8_t j = 0; j < def->zigbee_models_count; ++j) {
            auto [zit, zinserted] = seen_zigbee_models.insert(
                def->zigbee_models[j]);
            if (!zinserted) {
                // z2m occasionally declares the same zigbeeModel on
                // multiple entries (whitelabel families where hardware
                // is identical but branding differs). First-match-wins
                // at dispatch time — log but don't abort.
                std::fprintf(stderr,
                             "note: duplicate zigbeeModel: %s (from %s)\n",
                             def->zigbee_models[j], def->model);
            }
        }
    }

    std::printf("lumi registry: %zu devices ported, "
                "%zu unique zigbee_models\n",
                n, seen_zigbee_models.size());
    return 0;
}
