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

// Substring candidates must be long enough to mean something. The registry
// carries ~110 zigbeeModels of four characters or fewer ("CCT", "RGB",
// "RGBW", "2PM", "H1", ...). As substrings they match a large share of
// unrelated modelIDs -- "CCT" alone appears in most colour-temperature
// lighting ids -- and because the substring pass runs before the generic
// fallback, a hit there also pre-empts synth_definition. The exact-match
// passes still serve those short definitions. (Review 2026-09, EZ-02.)
constexpr std::size_t kMinSubstrModelLen = 5;

// U+FFFD in UTF-8: a model string that was mangled upstream. It must never
// act as a substring candidate.
bool has_replacement_char(const char* s) {
    return std::strstr(s, "\xEF\xBF\xBD") != nullptr;
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
        const std::size_t len = std::strlen(zm);
        if (len < kMinSubstrModelLen) continue;
        if (has_replacement_char(zm)) continue;
        if (std::strstr(wanted, zm) == nullptr) continue;  // zm must appear in wanted
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

// Tuya's newer manufacturerName families. Devices now report
// `_TZE2841000000_<sfx>` or `_TZE28C1000000_<sfx>` for products that were
// registered as `_TZE284_<sfx>`, `_TZE204_<sfx>` or `_TZE200_<sfx>`; upstream
// appends the twin to each definition as it surfaces (35 of them in z2m
// v26.104.0 + v26.105.0 alone, and the mapping to a legacy prefix is not
// fixed -- `_TZE284_ncc7uahd` gained `_TZE28C1000000_`, `_TZE284_7qc2wlqr`
// gained `_TZE2841000000_`). The eight-character suffix is the product id
// and the prefix the radio/firmware family, so a name that misses under a
// new prefix is retried under the legacy ones. Only consulted when no
// definition carries the exact name, so an explicit twin still wins.
constexpr const char* kTwinPrefixes[]   = { "_TZE2841000000_", "_TZE28C1000000_" };
constexpr const char* kLegacyPrefixes[] = { "_TZE284_", "_TZE204_", "_TZE200_" };
constexpr std::size_t kLegacyPrefixCount =
    sizeof(kLegacyPrefixes) / sizeof(kLegacyPrefixes[0]);
constexpr std::size_t kManuAliasCap = 24;   // "_TZE284_" + suffix + NUL

// Fills `out` with the legacy spellings of a twin-prefixed name. Returns 0
// when `manu` is not one (or the suffix would not fit).
std::size_t manufacturer_aliases(const char* manu,
                                 char (&out)[kLegacyPrefixCount][kManuAliasCap]) {
    if (!manu) return 0;
    const char* sfx = nullptr;
    for (const char* tp : kTwinPrefixes) {
        const std::size_t tl = std::strlen(tp);
        if (std::strncmp(manu, tp, tl) == 0) { sfx = manu + tl; break; }
    }
    if (!sfx || !*sfx) return 0;
    const std::size_t sl = std::strlen(sfx);
    std::size_t n = 0;
    for (const char* lp : kLegacyPrefixes) {
        const std::size_t ll = std::strlen(lp);
        if (ll + sl + 1 > kManuAliasCap) continue;
        std::memcpy(out[n], lp, ll);
        std::memcpy(out[n] + ll, sfx, sl + 1);
        ++n;
    }
    return n;
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

    // Pass 1b: the same walk under the legacy spellings of a twin-prefixed
    // manufacturer name (see manufacturer_aliases). Runs after Pass 1 so a
    // definition that lists the exact twin is preferred.
    {
        char alias[kLegacyPrefixCount][kManuAliasCap];
        const std::size_t n = manufacturer_aliases(manufacturer_name, alias);
        for (std::size_t a = 0; a < n; ++a) {
            for (const auto* def : registry) {
                if (!def || !is_tuya_styled(*def)) continue;
                if (!has_exact_model(*def, model_id)) continue;
                if (has_manufacturer_match(*def, alias[a])) return def;
            }
        }
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
    // Manufacturer check still enforced so two defs sharing the same
    // non-TS0601 model with different mfgs don't collide.
    for (const auto* def : registry) {
        if (!def || !is_tuya_styled(*def)) continue;
        if (!has_exact_model(*def, model_id)) continue;
        if (std::strcmp(model_id, "TS0601") == 0) continue;
        if (!has_manufacturer_match(*def, manufacturer_name)) continue;
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
