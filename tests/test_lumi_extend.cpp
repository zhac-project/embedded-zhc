// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: smoke tests for the Lumi utility extend helpers. Each must
// return a distinct, non-null ConfigureFn so the platform adapter can
// match on identity.

#include <cassert>
#include <set>

#include "definitions/lumi/extend.hpp"
#include "zhc/runtime/dispatch.hpp"

using namespace zhc;

static void test_each_helper_yields_unique_fn() {
    const ConfigureFn fns[] = {
        lumi::extend::prevent_reset(),
        lumi::extend::flip_indicator_light(),
        lumi::extend::led_disabled_night(),
        lumi::extend::add_manu_specific_lumi_cluster(),
        lumi::extend::zigbee_ota(),
        lumi::extend::electricity_meter_config(),
    };
    std::set<ConfigureFn> unique;
    for (auto f : fns) {
        assert(f != nullptr);
        unique.insert(f);
    }
    assert(unique.size() == sizeof(fns) / sizeof(fns[0]));
}

static void test_same_helper_is_stable() {
    assert(lumi::extend::prevent_reset() == lumi::extend::prevent_reset());
}

static void test_helpers_are_callable_without_crash() {
    RuntimeContext ctx{};
    lumi::extend::prevent_reset()(0, ctx);
    lumi::extend::flip_indicator_light()(0, ctx);
    lumi::extend::led_disabled_night()(0, ctx);
    lumi::extend::add_manu_specific_lumi_cluster()(0, ctx);
    lumi::extend::zigbee_ota()(0, ctx);
    lumi::extend::electricity_meter_config()(0, ctx);
}

static void test_modern_extend_bundle() {
    constexpr auto bundle = lumi::extend::lumi_modern_extend_default();
    assert(bundle.count == 5);
    for (std::uint8_t i = 0; i < bundle.count; ++i) {
        assert(bundle.steps[i] != nullptr);
    }
    // All five must be distinct.
    std::set<ConfigureFn> unique;
    for (std::uint8_t i = 0; i < bundle.count; ++i) unique.insert(bundle.steps[i]);
    assert(unique.size() == bundle.count);
}

int main() {
    test_each_helper_yields_unique_fn();
    test_same_helper_is_stable();
    test_helpers_are_callable_without_crash();
    test_modern_extend_bundle();
    return 0;
}
