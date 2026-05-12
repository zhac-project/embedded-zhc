// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Exercises find_definition_by_model with overlapping zigbeeModel
// prefixes — confirms exact match wins over substring, and that
// substring fallback picks the longest matching entry.

#include <cassert>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"

using namespace zhc;

namespace {

// Three definitions with deliberately overlapping zigbeeModel prefixes.
// "lumi.sensor_motion"       — short string, exact match for a hypothetical
//                              ancient model id.
// "lumi.sensor_motion.aq2"   — longer, exact match for RTCGQ11LM.
// "lumi.sensor_motion.ac01"  — longer, matches a theoretical newer device.

const char* const kZmMotion[]     = { "lumi.sensor_motion" };
const char* const kZmMotionAq2[]  = { "lumi.sensor_motion.aq2" };
const char* const kZmMotionAc01[] = { "lumi.sensor_motion.ac01",
                                      "lumi.sensor_motion.ac01_v2" };

constexpr PreparedDefinition kDefMotion{
    .zigbee_models       = kZmMotion,
    .zigbee_models_count = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model               = "OLD",
    .vendor              = "Aqara",
    .meta                = nullptr,
    .exposes             = nullptr, .exposes_count = 0,
    .white_labels        = nullptr, .white_labels_count = 0,
    .from_zigbee         = nullptr, .from_zigbee_count = 0,
    .to_zigbee           = nullptr, .to_zigbee_count = 0,
    .configure           = nullptr,
    .on_event            = nullptr,
};
constexpr PreparedDefinition kDefMotionAq2{
    .zigbee_models       = kZmMotionAq2,
    .zigbee_models_count = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model               = "RTCGQ11LM",
    .vendor              = "Aqara",
    .meta                = nullptr,
    .exposes             = nullptr, .exposes_count = 0,
    .white_labels        = nullptr, .white_labels_count = 0,
    .from_zigbee         = nullptr, .from_zigbee_count = 0,
    .to_zigbee           = nullptr, .to_zigbee_count = 0,
    .configure           = nullptr,
    .on_event            = nullptr,
};
constexpr PreparedDefinition kDefMotionAc01{
    .zigbee_models       = kZmMotionAc01,
    .zigbee_models_count = 2,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model               = "RTCGQ16LM",
    .vendor              = "Aqara",
    .meta                = nullptr,
    .exposes             = nullptr, .exposes_count = 0,
    .white_labels        = nullptr, .white_labels_count = 0,
    .from_zigbee         = nullptr, .from_zigbee_count = 0,
    .to_zigbee           = nullptr, .to_zigbee_count = 0,
    .configure           = nullptr,
    .on_event            = nullptr,
};

const PreparedDefinition* const kRegistry[] = {
    &kDefMotion, &kDefMotionAq2, &kDefMotionAc01,
};

}  // namespace

static void test_exact_match_wins_over_substring() {
    // Exact match for the shortest model id should bind to kDefMotion
    // even though both other defs' zigbeeModels contain this string
    // as a substring.
    const auto* d = find_definition_by_model("lumi.sensor_motion", kRegistry);
    assert(d == &kDefMotion);
}

static void test_longer_exact_match_wins() {
    const auto* d = find_definition_by_model("lumi.sensor_motion.aq2", kRegistry);
    assert(d == &kDefMotionAq2);
}

static void test_second_alias_in_multi_model_def() {
    // The second string of kZmMotionAc01 — verifies array iteration.
    const auto* d = find_definition_by_model("lumi.sensor_motion.ac01_v2", kRegistry);
    assert(d == &kDefMotionAc01);
}

static void test_substring_fallback_longest_wins() {
    // Input carries extra suffix; no exact match. Both kDefMotion and
    // kDefMotionAq2 zigbeeModels are substrings. Longer one wins.
    const auto* d = find_definition_by_model("lumi.sensor_motion.aq2_xxx",
                                              kRegistry);
    assert(d == &kDefMotionAq2);
}

static void test_unknown_model_id_returns_null() {
    const auto* d = find_definition_by_model("vendor.something_unrelated",
                                              kRegistry);
    assert(d == nullptr);
}

static void test_empty_or_null_model_id_returns_null() {
    assert(find_definition_by_model(nullptr, kRegistry) == nullptr);
    assert(find_definition_by_model("",      kRegistry) == nullptr);
}

int main() {
    test_exact_match_wins_over_substring();
    test_longer_exact_match_wins();
    test_second_alias_in_multi_model_def();
    test_substring_fallback_longest_wins();
    test_unknown_model_id_returns_null();
    test_empty_or_null_model_id_returns_null();
    return 0;
}
