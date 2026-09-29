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

// The epoch of the answer: the def's own field, else z2m v26.105.0's
// tuyaBase({timeStart}) fingerprints, else none (z2m default "off").
// Generated defs never carry the field, so the fingerprint decides for them.
static void test_time_start_follows_z2m() {
    PreparedDefinition def{};
    // Tuya ZTH08 LCD sensor: z2m TS0601_temperature_humidity_sensor_2, "1970".
    assert(tuya::time_start(def, "_TZE204_d7lpruvi", "TS0601") == 1);
    assert(tuya::time_start(def, "_TZE284_d7lpruvi", "TS0601") == 1);
    // Avatto TRV26: "2000".
    assert(tuya::time_start(def, "_TZE204_xdtnpp1a", "TS0601") == 2);
    // Immax 07703L matches on zigbeeModel "losfena": "2000".
    assert(tuya::time_start(def, "_TYST11_other", "losfena") == 2);
    // Not in any timeStart definition: no answer.
    assert(tuya::time_start(def, "_TZ3000_notlisted", "TS0601") == 0);
    assert(tuya::time_start(def, nullptr, nullptr) == 0);
    // A hand-set field wins.
    def.tuya_time_start = 2;
    assert(tuya::time_start(def, "_TZE204_d7lpruvi", "TS0601") == 2);
}

int main() {
    test_mcu_sync_time_fires_hook();
    test_no_hook_is_safe();
    test_time_start_follows_z2m();
    return 0;
}
