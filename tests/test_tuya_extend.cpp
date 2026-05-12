// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T1.2/T1.3: smoke tests for the Tuya extend helpers.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "definitions/tuya/extend.hpp"
#include "zhc/runtime/dispatch.hpp"

using namespace zhc;

static void test_configure_hooks_unique() {
    auto a = tuya::extend::tuya_base_configure();
    auto b = tuya::extend::register_cluster();
    auto c = tuya::extend::magic_packet();
    auto d = tuya::extend::electricity_meter_poll();
    assert(a && b && c && d);
    assert(a != b && a != c && a != d);
    assert(b != c && b != d && c != d);
}

static void test_configure_is_callable() {
    RuntimeContext ctx{};
    tuya::extend::tuya_base_configure()(0, ctx);
    tuya::extend::register_cluster()(0, ctx);
    tuya::extend::magic_packet()(0, ctx);
}

static void test_magic_packet_payloads() {
    assert(tuya::extend::kTuyaMagicPacket_readAttrs.len == 12);
    assert(tuya::extend::kTuyaMagicPacket_readAttrs.cluster_id == 0x0000);
    assert(tuya::extend::kTuyaMagicPacket_zoneMode.len == 2);
    assert(tuya::extend::kTuyaMagicPacket_zoneMode.cluster_id == 0xE001);

    // First byte of each canned sequence is documented.
    assert(tuya::extend::kTuyaMagicPacket_readAttrs.bytes[0] == 0x00);
    assert(tuya::extend::kTuyaMagicPacket_zoneMode.bytes[0] == 0x01);
}

int main() {
    test_configure_hooks_unique();
    test_configure_is_callable();
    test_magic_packet_payloads();
    return 0;
}
