// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstdint>

namespace zhc {

using TimerId = std::uint32_t;
inline constexpr TimerId kInvalidTimerId = 0;

// Callback fired when a scheduled timer matures.
//
//   device_index  — which device's slot the timer was scheduled against
//   user_tag      — free integer chosen by the scheduling converter
//                   (e.g. kind = press-timeout vs hold-expiry)
//   user_data     — opaque pointer also supplied at schedule time
//
// The callback is intentionally minimal: it can read/write per-device
// state by calling back into the store via the `user_data` pointer,
// and can publish an event via a platform-supplied fan-out hook. The
// scheduler itself holds no dispatch context — converters that need
// the full RuntimeContext should stash a pointer in `user_data`.
using TimerFiredFn = void (*)(std::uint16_t device_index,
                               std::uint32_t user_tag,
                               void* user_data);

// Type-erased scheduler. The ESP-IDF adapter wraps `esp_timer_*`; the
// host test harness wraps `HostTimerFake`. Platforms that never need
// timers leave `RuntimeContext::timers` null and the converter falls
// back to whatever retroactive detection it can manage.
struct TimerScheduler {
    TimerId (*schedule)(void* impl,
                         std::uint32_t delay_ms,
                         std::uint16_t device_index,
                         std::uint32_t user_tag,
                         TimerFiredFn fn,
                         void* user_data);
    void (*cancel)(void* impl, TimerId id);
    void* impl;
};

// Convenience wrappers with null guards.
inline TimerId timer_schedule(TimerScheduler* s,
                               std::uint32_t delay_ms,
                               std::uint16_t device_index,
                               std::uint32_t user_tag,
                               TimerFiredFn fn,
                               void* user_data) {
    return (s && s->schedule)
        ? s->schedule(s->impl, delay_ms, device_index, user_tag, fn, user_data)
        : kInvalidTimerId;
}

inline void timer_cancel(TimerScheduler* s, TimerId id) {
    if (s && s->cancel && id != kInvalidTimerId) s->cancel(s->impl, id);
}

}  // namespace zhc
