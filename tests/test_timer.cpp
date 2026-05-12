// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// HostTimerFake — deterministic firing order + cancel-before-fire.
// Task 0.2 exit criterion.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include "zhc/runtime/host_timer_fake.hpp"
#include "zhc/runtime/timer.hpp"

using zhc::test::HostTimerFake;

namespace {

struct Recorded {
    std::uint16_t device_index;
    std::uint32_t user_tag;
};

struct Sink {
    std::array<Recorded, 8> entries{};
    std::size_t count{0};
    void record(std::uint16_t idx, std::uint32_t tag) {
        if (count < entries.size()) entries[count++] = {idx, tag};
    }
};

void cb(std::uint16_t device_index, std::uint32_t user_tag, void* user_data) {
    static_cast<Sink*>(user_data)->record(device_index, user_tag);
}

}  // namespace

static void test_two_timers_fire_in_delay_order() {
    HostTimerFake fake;
    Sink sink;

    // Schedule B before A, but A has a shorter delay.
    const auto idB = zhc::timer_schedule(fake.scheduler(), /*ms=*/500,
                                          /*dev=*/2, /*tag=*/0xB00,
                                          cb, &sink);
    const auto idA = zhc::timer_schedule(fake.scheduler(), /*ms=*/100,
                                          /*dev=*/1, /*tag=*/0xA00,
                                          cb, &sink);
    assert(idA != zhc::kInvalidTimerId);
    assert(idB != zhc::kInvalidTimerId);
    assert(fake.pending_count() == 2);

    fake.advance(1000);
    assert(fake.pending_count() == 0);
    assert(sink.count == 2);
    assert(sink.entries[0].user_tag == 0xA00);   // A fired first
    assert(sink.entries[0].device_index == 1);
    assert(sink.entries[1].user_tag == 0xB00);
    assert(sink.entries[1].device_index == 2);
}

static void test_cancel_before_fire_suppresses_callback() {
    HostTimerFake fake;
    Sink sink;

    const auto id = zhc::timer_schedule(fake.scheduler(), 1000, 0, 0xDEAD,
                                         cb, &sink);
    zhc::timer_cancel(fake.scheduler(), id);
    fake.advance(2000);

    assert(sink.count == 0);
    assert(fake.pending_count() == 0);
}

// Schedule two, cancel one, confirm the other still fires.
static void test_cancel_one_of_two() {
    HostTimerFake fake;
    Sink sink;

    const auto idA = zhc::timer_schedule(fake.scheduler(), 500, 0, 0xA, cb, &sink);
    const auto idB = zhc::timer_schedule(fake.scheduler(), 500, 0, 0xB, cb, &sink);
    (void)idA;
    zhc::timer_cancel(fake.scheduler(), idB);
    fake.advance(1000);

    assert(sink.count == 1);
    assert(sink.entries[0].user_tag == 0xA);
}

// Null scheduler (platforms without timer support) — schedule returns
// invalid id, cancel is a no-op.
static void test_null_scheduler_safe() {
    Sink sink;
    const auto id = zhc::timer_schedule(nullptr, 100, 0, 0, cb, &sink);
    assert(id == zhc::kInvalidTimerId);
    zhc::timer_cancel(nullptr, 42);  // no crash
    assert(sink.count == 0);
}

int main() {
    test_two_timers_fire_in_delay_order();
    test_cancel_before_fire_suppresses_callback();
    test_cancel_one_of_two();
    test_null_scheduler_safe();
    return 0;
}
