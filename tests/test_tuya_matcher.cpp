// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T0.1: manufacturer-prefix fingerprint matcher tests.
//
// Covers the Tuya case where many devices share modelId="TS0601" and
// only the manufacturerName prefix discriminates.

#include <cassert>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"

using namespace zhc;

namespace {

constexpr const char* kTS0601[] = { "TS0601" };

constexpr const char* kDevA_manu[] = { "_TZE200_aaa1", "_TZE200_aaa2" };

// Device A: TS0601 + exact manufacturer list.
constexpr PreparedDefinition kDevA{
    .zigbee_models            = kTS0601,
    .zigbee_models_count      = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = kDevA_manu,
    .manufacturer_names_count = 2,
    .model                    = "DEV_A",
    .vendor                   = "TuyaVendor",
    .meta                     = nullptr,
    .exposes                  = nullptr,
    .exposes_count            = 0,
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = nullptr,
    .from_zigbee_count        = 0,
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
};

// Device B: TS0601 + prefix match.
constexpr PreparedDefinition kDevB{
    .zigbee_models            = kTS0601,
    .zigbee_models_count      = 1,
    .manufacturer_name_prefix = "_TZE200_bbb",
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "DEV_B",
    .vendor                   = "TuyaVendor",
    .meta                     = nullptr,
    .exposes                  = nullptr,
    .exposes_count            = 0,
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = nullptr,
    .from_zigbee_count        = 0,
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
};

// Device C: TS0601 bare (no manufacturer discrimination; fallback).
constexpr PreparedDefinition kDevC{
    .zigbee_models            = kTS0601,
    .zigbee_models_count      = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "DEV_C",
    .vendor                   = "Generic",
    .meta                     = nullptr,
    .exposes                  = nullptr,
    .exposes_count            = 0,
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = nullptr,
    .from_zigbee_count        = 0,
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
};

constexpr const char* kLumiModels[] = { "lumi.sensor_switch" };
constexpr PreparedDefinition kDevLumi{
    .zigbee_models            = kLumiModels,
    .zigbee_models_count      = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "WXKG01LM",
    .vendor                   = "Xiaomi",
    .meta                     = nullptr,
    .exposes                  = nullptr,
    .exposes_count            = 0,
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = nullptr,
    .from_zigbee_count        = 0,
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
};

const PreparedDefinition* const kRegistry[] = {
    &kDevC, &kDevA, &kDevB, &kDevLumi,   // intentionally mixed order
};

}  // namespace

static void test_exact_manu_list_wins() {
    const auto* d = find_definition("TS0601", "_TZE200_aaa1", kRegistry);
    assert(d == &kDevA);
    d = find_definition("TS0601", "_TZE200_aaa2", kRegistry);
    assert(d == &kDevA);
}

static void test_prefix_manu_wins() {
    const auto* d = find_definition("TS0601", "_TZE200_bbbXYZ", kRegistry);
    assert(d == &kDevB);
    d = find_definition("TS0601", "_TZE200_bbb", kRegistry);
    assert(d == &kDevB);
}

static void test_bare_fallback_when_no_manu_match() {
    // manu doesn't match A or B → fall to bare TS0601 kDevC.
    const auto* d = find_definition("TS0601", "_TZE200_zzz", kRegistry);
    assert(d == &kDevC);
}

static void test_bare_fallback_when_manu_null() {
    const auto* d = find_definition("TS0601", nullptr, kRegistry);
    assert(d == &kDevC);
}

static void test_lumi_style_still_resolves() {
    // legacy find_definition_by_model path works through find_definition.
    const auto* d = find_definition("lumi.sensor_switch", nullptr, kRegistry);
    assert(d == &kDevLumi);
}

static void test_exact_list_miss_does_not_fall_into_prefix() {
    // A has exact list only, no prefix. "_TZE200_aaa1" matches A;
    // "_TZE200_aaa" alone is a prefix match against A's list? No —
    // exact list requires full strcmp. So a partial miss goes to C.
    const auto* d = find_definition("TS0601", "_TZE200_aaa", kRegistry);
    assert(d == &kDevC);
}

int main() {
    test_exact_manu_list_wins();
    test_prefix_manu_wins();
    test_bare_fallback_when_no_manu_match();
    test_bare_fallback_when_manu_null();
    test_lumi_style_still_resolves();
    test_exact_list_miss_does_not_fall_into_prefix();
    return 0;
}
