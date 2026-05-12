// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T0.3: mcuSyncTime hook fires when cluster 0xEF00 cmd 0x24 arrives.

#include <cassert>
#include <cstdint>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {
struct Probe {
    bool          called{false};
    std::uint16_t device_index{0xFFFF};
    std::uint8_t  trans_seq{0};
};
Probe g_probe;
void capture(std::uint16_t idx, std::uint8_t tsn) {
    g_probe.called = true;
    g_probe.device_index = idx;
    g_probe.trans_seq = tsn;
}
}  // namespace

static void test_mcu_sync_time_fires_hook() {
    // ZCL header: fc = 0x01 (cluster-specific c→s), tsn = 0x42, cmd = 0x24
    // No payload.
    // fc=0x09 (cluster-specific, server→client), tsn=0x42, cmd=0x24.
    constexpr std::uint8_t kFrame[] = { 0x09, 0x42, 0x24 };
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = std::span<const std::uint8_t>(kFrame, 3);

    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "manuSpecificTuya";

    const FzConverter* const fz_list[] = { &tuya::kFzTuyaMcuSyncTime };
    PreparedDefinition def{};
    static const char* models[] = { "SYNTH" };
    def.zigbee_models       = models;
    def.zigbee_models_count = 1;
    def.model               = "SYNTH";
    def.vendor              = "Test";
    def.from_zigbee         = fz_list;
    def.from_zigbee_count   = 1;

    g_probe = Probe{};
    RuntimeContext ctx{};
    ctx.device_index   = 7;
    ctx.tuya_sync_time = &capture;

    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);
    assert(g_probe.called);
    assert(g_probe.device_index == 7);
    assert(g_probe.trans_seq == 0x42);
}

// Without a hook, converter still matches (prevents unhandled warning)
// but no callback fires.
static void test_no_hook_is_safe() {
    constexpr std::uint8_t kFrame[] = { 0x09, 0x05, 0x24 };
    InboundApsFrame raw{};
    raw.cluster_id = 0xEF00;
    raw.src_endpoint = 1; raw.dst_endpoint = 1; raw.linkquality = 0xC8;
    raw.data = std::span<const std::uint8_t>(kFrame, 3);

    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "manuSpecificTuya";

    const FzConverter* const fz_list[] = { &tuya::kFzTuyaMcuSyncTime };
    PreparedDefinition def{};
    static const char* models[] = { "SYNTH" };
    def.zigbee_models = models; def.zigbee_models_count = 1;
    def.model = "SYNTH"; def.vendor = "Test";
    def.from_zigbee = fz_list; def.from_zigbee_count = 1;

    g_probe = Probe{};
    RuntimeContext ctx{};   // no tuya_sync_time
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);
    assert(!g_probe.called);
}

int main() {
    test_mcu_sync_time_fires_hook();
    test_no_hook_is_safe();
    return 0;
}
