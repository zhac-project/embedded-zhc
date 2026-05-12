// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include <cstdio>

#include "zhc/config.hpp"
#include "zhc/platform.hpp"
#include "zhc/types.hpp"

// Bootstrap test: proves the core headers compile as a unit and the
// capacity macros resolve to sane values. Later tasks replace this with
// real unit tests per subsystem.
int main() {
    static_assert(ZHC_FIXED_PAYLOAD_CAP > 0, "payload cap must be > 0");
    static_assert(ZHC_MAX_DEVICES > 0,       "device cap must be > 0");

    zhc::FixedPayload<4> payload{};
    zhc::Value v{};
    v.type = zhc::ValueType::Uint;
    v.u    = 42;
    if (!payload.put("answer", v)) return 1;

    const auto* found = payload.find("answer");
    if (!found) return 2;
    if (found->type != zhc::ValueType::Uint) return 3;
    if (found->u != 42) return 4;

    std::printf("zhc test runner bootstrap OK — fixed payload cap=%u\n",
                ZHC_FIXED_PAYLOAD_CAP);
    return 0;
}
