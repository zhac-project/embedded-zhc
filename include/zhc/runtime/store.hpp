// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "zhc/config.hpp"

namespace zhc {

// Per-device runtime state slot. Carries the common hand-rolled state
// converters need (button timing, counters). Arbitrary per-converter
// state uses `scratch` — interpret the bytes in a struct local to the
// converter.
struct DeviceRuntimeState {
    std::uint32_t counter{0};          // generic counter (test scaffold)
    std::uint32_t press_start_ms{0};
    std::uint32_t hold_start_ms{0};
    std::array<std::uint8_t, 32> scratch{};
};

// Fixed-capacity runtime store. Template so capacity is picked at
// link time by the platform, but dispatch consumes an erased view
// via StoreGetFn so the core stays untemplated.
template <std::size_t MaxDevices>
struct RuntimeStore {
    std::array<DeviceRuntimeState, MaxDevices> devices{};

    // Type-erased accessor — stored in RuntimeContext so dispatch code
    // doesn't have to know `MaxDevices`.
    static DeviceRuntimeState* get(void* self, std::uint16_t idx) {
        auto* s = static_cast<RuntimeStore<MaxDevices>*>(self);
        return idx < MaxDevices ? &s->devices[idx] : nullptr;
    }
};

using StoreGetFn = DeviceRuntimeState* (*)(void* store, std::uint16_t idx);

}  // namespace zhc
