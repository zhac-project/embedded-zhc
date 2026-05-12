// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Header-only HostTimerFake — deterministic timer backend for host
// tests. Not compiled into the ESP-IDF build. Uses <vector>, heap
// allocation, and other host-only facilities intentionally.

#include <algorithm>
#include <cstdint>
#include <vector>

#include "zhc/runtime/timer.hpp"

namespace zhc::test {

class HostTimerFake {
public:
    HostTimerFake() : scheduler_{ &HostTimerFake::schedule_thunk,
                                   &HostTimerFake::cancel_thunk,
                                   this } {}

    TimerScheduler* scheduler() { return &scheduler_; }

    std::uint32_t now_ms() const { return clock_ms_; }

    // Advance the fake clock. Any timer whose `due_at_ms <= new_now`
    // fires in chronological order. Concurrent schedule() calls from
    // within a firing callback are honoured on subsequent ticks.
    void advance(std::uint32_t delta_ms) {
        const std::uint32_t target = clock_ms_ + delta_ms;
        while (true) {
            auto it = std::min_element(pending_.begin(), pending_.end(),
                [](const Entry& a, const Entry& b) {
                    return a.due_at_ms < b.due_at_ms;
                });
            if (it == pending_.end() || it->due_at_ms > target) break;
            const Entry e = *it;
            pending_.erase(it);
            clock_ms_ = e.due_at_ms;
            if (e.fn) e.fn(e.device_index, e.user_tag, e.user_data);
        }
        clock_ms_ = target;
    }

    std::size_t pending_count() const { return pending_.size(); }

private:
    struct Entry {
        TimerId        id;
        std::uint32_t  due_at_ms;
        std::uint16_t  device_index;
        std::uint32_t  user_tag;
        TimerFiredFn   fn;
        void*          user_data;
    };

    static TimerId schedule_thunk(void* impl, std::uint32_t delay_ms,
                                   std::uint16_t device_index,
                                   std::uint32_t user_tag,
                                   TimerFiredFn fn, void* user_data) {
        auto* self = static_cast<HostTimerFake*>(impl);
        const TimerId id = ++self->next_id_;
        self->pending_.push_back(Entry{
            id, self->clock_ms_ + delay_ms,
            device_index, user_tag, fn, user_data,
        });
        return id;
    }

    static void cancel_thunk(void* impl, TimerId id) {
        auto* self = static_cast<HostTimerFake*>(impl);
        self->pending_.erase(
            std::remove_if(self->pending_.begin(), self->pending_.end(),
                [id](const Entry& e) { return e.id == id; }),
            self->pending_.end());
    }

    TimerScheduler     scheduler_;
    std::vector<Entry> pending_;
    std::uint32_t      clock_ms_  = 0;
    TimerId            next_id_   = kInvalidTimerId;
};

}  // namespace zhc::test
