// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include <cassert>
#include <cstdint>

#include "zhc/runtime/expose_range.hpp"

using namespace zhc;

static void test_known_attrs() {
    std::int32_t lo = 0, hi = 0, st = 0;

    assert(default_numeric_range("brightness", lo, hi, st));
    assert(lo == 0 && hi == 254 && st == 1);

    assert(default_numeric_range("color_temp", lo, hi, st));
    assert(lo == 150 && hi == 500);

    assert(default_numeric_range("position", lo, hi, st));
    assert(lo == 0 && hi == 100);
}

static void test_unknown_leaves_outputs_untouched() {
    std::int32_t lo = -7, hi = -3, st = 9;
    assert(!default_numeric_range("temperature", lo, hi, st)); // a sensor attr — no UI range table
    assert(lo == -7 && hi == -3 && st == 9);                   // untouched on miss
    assert(!default_numeric_range(nullptr, lo, hi, st));
}

int main() {
    test_known_attrs();
    test_unknown_leaves_outputs_untouched();
    return 0;
}
