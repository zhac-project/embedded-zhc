// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstdint>

namespace zhc {

// Forward declarations — full definitions live in runtime/dispatch.hpp and
// runtime/definition.hpp, introduced in later tasks.
struct RuntimeContext;
struct InboundApsFrame;
struct PreparedDefinition;

// Hooks the platform adapter must provide. All are C-style function
// pointers — no std::function, no heap.
using NowMsFn          = std::uint32_t (*)();
using LogFn            = void (*)(const char* msg);
using UnhandledFrameFn = void (*)(const InboundApsFrame& frame,
                                   const PreparedDefinition* def,
                                   RuntimeContext& ctx);

}  // namespace zhc
