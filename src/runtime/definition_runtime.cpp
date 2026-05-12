// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/runtime/definition_runtime.hpp"

#include <cstring>

namespace zhc {

namespace {

bool has_exact_model(const PreparedDefinition& def, const char* wanted) {
    if (!def.zigbee_models) return false;
    for (std::uint8_t i = 0; i < def.zigbee_models_count; ++i) {
        const char* zm = def.zigbee_models[i];
        if (zm && std::strcmp(zm, wanted) == 0) return true;
    }
    return false;
}

// Longest substring match against any of def's zigbeeModels. Returns
// 0 when no entry is a substring match.
std::size_t longest_substr_score(const PreparedDefinition& def,
                                  const char* wanted) {
    if (!def.zigbee_models || !wanted) return 0;
    std::size_t best = 0;
    for (std::uint8_t i = 0; i < def.zigbee_models_count; ++i) {
        const char* zm = def.zigbee_models[i];
        if (!zm) continue;
        if (std::strstr(wanted, zm) == nullptr) continue;  // zm must appear in wanted
        const std::size_t len = std::strlen(zm);
        if (len > best) best = len;
    }
    return best;
}

}  // namespace

const PreparedDefinition* find_definition_by_model(
    const char* model_id,
    std::span<const PreparedDefinition* const> registry) {
    if (!model_id || !*model_id) return nullptr;

    // Pass 1: exact match.
    for (const auto* def : registry) {
        if (!def) continue;
        if (has_exact_model(*def, model_id)) return def;
    }

    // Pass 2: longest substring. `wanted` contains `zm` — covers z2m's
    // tolerance for devices reporting e.g. "lumi.sensor_switch.aq3"
    // when the registry only lists "lumi.sensor_switch".
    const PreparedDefinition* best_def = nullptr;
    std::size_t best_score = 0;
    for (const auto* def : registry) {
        if (!def) continue;
        const std::size_t score = longest_substr_score(*def, model_id);
        if (score > best_score) {
            best_score = score;
            best_def   = def;
        }
    }
    return best_def;
}

namespace {

bool has_manufacturer_match(const PreparedDefinition& def,
                             const char* manu) {
    // No Tuya-style discriminator → candidate doesn't care about manu.
    if (!def.manufacturer_name_prefix &&
        (!def.manufacturer_names || def.manufacturer_names_count == 0)) {
        return true;
    }
    if (!manu) return false;

    if (def.manufacturer_names && def.manufacturer_names_count > 0) {
        for (std::uint8_t i = 0; i < def.manufacturer_names_count; ++i) {
            const char* m = def.manufacturer_names[i];
            if (m && std::strcmp(m, manu) == 0) return true;
        }
        // When an exact list is present, a miss disqualifies the
        // candidate — don't fall through to prefix.
        return false;
    }
    if (def.manufacturer_name_prefix) {
        const std::size_t plen = std::strlen(def.manufacturer_name_prefix);
        if (std::strncmp(def.manufacturer_name_prefix, manu, plen) == 0) {
            return true;
        }
    }
    return false;
}

bool is_tuya_styled(const PreparedDefinition& def) {
    return def.manufacturer_name_prefix ||
           (def.manufacturer_names && def.manufacturer_names_count > 0);
}

}  // namespace

const PreparedDefinition* find_definition(
    const char* model_id,
    const char* manufacturer_name,
    std::span<const PreparedDefinition* const> registry) {
    if (!model_id || !*model_id) return nullptr;

    // Pass 1: Tuya-style candidates (manu-discriminated). Walk first so
    // a matching TS0601 + _TZE200_xxx wins over a bare TS0601 fallback.
    for (const auto* def : registry) {
        if (!def || !is_tuya_styled(*def)) continue;
        if (!has_exact_model(*def, model_id)) continue;
        if (has_manufacturer_match(*def, manufacturer_name)) return def;
    }

    // Pass 2: non-Tuya candidates by exact model.
    for (const auto* def : registry) {
        if (!def || is_tuya_styled(*def)) continue;
        if (has_exact_model(*def, model_id)) return def;
    }

    // Pass 2b: a Tuya-style def may also declare a plain zigbeeModel
    // alongside its fingerprint list (e.g. Neo NAS-AB02B0 has
    // `zigbeeModel: ["0yu2xgi"]` + `fingerprint: [TS0601 + _TZE200_…]`).
    // When the incoming model matches a non-"TS0601" zigbeeModel but
    // the manu fails Pass 1, accept the def — z2m behaves the same.
    for (const auto* def : registry) {
        if (!def || !is_tuya_styled(*def)) continue;
        if (!has_exact_model(*def, model_id)) continue;
        if (std::strcmp(model_id, "TS0601") == 0) continue;
        return def;
    }

    // Pass 3: substring fallback (lumi-style).
    const PreparedDefinition* best_def = nullptr;
    std::size_t best_score = 0;
    for (const auto* def : registry) {
        if (!def || is_tuya_styled(*def)) continue;
        const std::size_t score = longest_substr_score(*def, model_id);
        if (score > best_score) {
            best_score = score;
            best_def   = def;
        }
    }
    return best_def;
}

}  // namespace zhc
