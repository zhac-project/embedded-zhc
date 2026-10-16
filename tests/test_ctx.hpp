// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// A RuntimeContext for a test helper that dispatches a frame and returns the
// DispatchResult. The result's suffixed keys (`state_l2`) and alloc_str()
// texts point into its ctx's ep_scratch arena (see RuntimeContext), so a ctx
// declared inside the helper dies before the caller reads the result: ASan's
// stack-use-after-return, which plain builds never notice. Each call hands out
// the next of 64 static contexts, freshly reset, so the last 64 results stay
// readable.
#pragma once

#include "zhc/runtime/dispatch.hpp"

inline zhc::RuntimeContext& test_ctx() {
    static zhc::RuntimeContext ring[64];
    static unsigned next = 0;
    zhc::RuntimeContext& ctx = ring[next++ % 64];
    ctx = zhc::RuntimeContext{};
    return ctx;
}
