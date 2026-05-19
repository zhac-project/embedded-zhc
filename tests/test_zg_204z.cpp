// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Fixture: HOBEIAN ZG-204Z (Tuya zigbeeModel "ZG-204Z") motion sensor.
// Pins the IAS-Zone wire format the device actually sends, captured
// from a real hardware run:
//
//     I (..) zhc_adapter: 0x...77a7cea6 Tuya/ZG-204Z (no match)
//     cluster=0x0500(ssIasZone) family=Zcl type=4 cmd=0x00
//     zcl[9]=09 fa 00 01 00 00 00 00 00
//
// Before the def was rewritten this dispatcher returned `no match`
// because the generator emitted Tuya MCU converters on cluster
// 0xEF00. After: kFzBattery + kFzIasMotionAlarm bound to clusters
// 0x0001 + 0x0500. These tests lock that pairing.

// Force assert() to remain enabled even when the rest of the tree is
// built with -DNDEBUG; without this Release builds elide every check
// and the test reports a false pass.
#undef NDEBUG
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/types.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefGen_ZG_204Z;
}  // namespace zhc::devices::tuya

namespace {

InboundApsFrame make_frame(std::uint16_t cluster,
                            std::uint8_t   src_ep,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// Build the 9-byte Zone Status Change Notification body. The frame
// matches HOBEIAN's wire format exactly: cluster-specific, S→C,
// disable-default-response, no manufacturer code.
//
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame {
    std::array<std::uint8_t, 9> bytes;
};

IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(),
                                                          f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, devices::tuya::kDefGen_ZG_204Z,
                                raw, ctx);
}

bool b(const Value* v) { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

}  // namespace

// Motion detected: zoneStatus = 0x0001 (alarm_1 set, all other bits clear).
// This is the canonical "movement seen" packet captured on the user's
// network. Expect occupancy=true, tamper=false, battery_low=false.
static void test_motion_present() {
    auto r = dispatch_ias(ias_notif(0x0001));
    assert(r.any_matched);
    assert(b(r.merged.find("occupancy")));
    assert(b_false(r.merged.find("tamper")));
    assert(b_false(r.merged.find("battery_low")));
}

// Motion cleared: zoneStatus = 0x0000.
static void test_motion_clear() {
    auto r = dispatch_ias(ias_notif(0x0000));
    assert(r.any_matched);
    assert(b_false(r.merged.find("occupancy")));
    assert(b_false(r.merged.find("tamper")));
    assert(b_false(r.merged.find("battery_low")));
}

// Tamper alarm: bit 2 set without bit 0 (motion).
static void test_tamper_only() {
    auto r = dispatch_ias(ias_notif(0x0004));
    assert(r.any_matched);
    assert(b_false(r.merged.find("occupancy")));
    assert(b(r.merged.find("tamper")));
    assert(b_false(r.merged.find("battery_low")));
}

// Battery low: bit 3 — sensors report this when cell drops below the
// internal threshold. Motion may or may not be co-asserted.
static void test_battery_low_with_motion() {
    auto r = dispatch_ias(ias_notif(0x0009));   // bit0 + bit3
    assert(r.any_matched);
    assert(b(r.merged.find("occupancy")));
    assert(b_false(r.merged.find("tamper")));
    assert(b(r.merged.find("battery_low")));
}

// Sanity: kFzIasMotionAlarm only fires on cmd 0x00 (Zone Status Change
// Notification). A bogus cluster-specific cmd id should not produce
// any decoded fields. After P2-era unification kFzIasZoneConfig is in
// the fz list too but it filters to AttributeReport / ReadResponse, so
// a cluster-specific cmd frame can't be misclaimed by it either.
static void test_other_cmd_does_not_match() {
    const std::uint8_t kFrame[] = { 0x09, 0x01, 0x01,   // cluster-specific cmd 0x01
                                     0x00 };
    auto raw = make_frame(0x0500, 1, std::span<const std::uint8_t>(kFrame, 4));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, devices::tuya::kDefGen_ZG_204Z,
                                   raw, ctx);
    assert(!r.any_matched);
}

// ── sensitivity / keep_time configuration attrs ─────────────────────
//
// Wire format (Report Attributes, cmd 0x0A on ssIasZone):
//   fc=0x18 (profile-wide, S→C, disable default response)
//   tsn
//   cmd=0x0A
//   record(s): attr_id_lo, attr_id_hi, type, value_bytes
//
// For ENUM8 (type 0x30) the value is a single byte. attr 0x0013 maps
// 0/1/2 → "low"/"medium"/"high"; attr 0xF001 (61441) maps 0/1/2 →
// "30"/"60"/"120" seconds.

namespace {

DispatchResult dispatch_attr_report(std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(0x0500, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, devices::tuya::kDefGen_ZG_204Z,
                                 raw, ctx);
}

bool str_eq(const Value* v, const char* expected) {
    return v && v->type == ValueType::StringRef
        && std::strcmp(v->str, expected) == 0;
}

}  // namespace

// Single sensitivity record (medium).
static void test_sensitivity_report_medium() {
    const std::uint8_t kFrame[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x13, 0x00, 0x30, 0x01,  // attr 0x0013, ENUM8, value 1=medium
    };
    auto r = dispatch_attr_report(std::span<const std::uint8_t>(kFrame, sizeof(kFrame)));
    assert(r.any_matched);
    assert(str_eq(r.merged.find("sensitivity"), "medium"));
    assert(r.merged.find("keep_time") == nullptr);
}

// Single keep_time record (120 s — the longest setting).
static void test_keep_time_report_120s() {
    const std::uint8_t kFrame[] = {
        0x18, 0x42, 0x0A,
        0x01, 0xF0, 0x30, 0x02,  // attr 0xF001, ENUM8, value 2=120s
    };
    auto r = dispatch_attr_report(std::span<const std::uint8_t>(kFrame, sizeof(kFrame)));
    assert(r.any_matched);
    assert(str_eq(r.merged.find("keep_time"), "120"));
    assert(r.merged.find("sensitivity") == nullptr);
}

// Combined report — both attrs in one frame, both decoded.
static void test_combined_sensitivity_and_keep_time() {
    const std::uint8_t kFrame[] = {
        0x18, 0x42, 0x0A,
        0x13, 0x00, 0x30, 0x00,  // sensitivity = "low"
        0x01, 0xF0, 0x30, 0x01,  // keep_time   = "60"
    };
    auto r = dispatch_attr_report(std::span<const std::uint8_t>(kFrame, sizeof(kFrame)));
    assert(r.any_matched);
    assert(str_eq(r.merged.find("sensitivity"), "low"));
    assert(str_eq(r.merged.find("keep_time"),   "60"));
}

// All three keep_time z2m values round-trip through the lookup table.
static void test_keep_time_all_values() {
    static const struct { std::uint8_t enum_v; const char* label; } cases[] = {
        {0, "30"}, {1, "60"}, {2, "120"},
    };
    for (auto& c : cases) {
        const std::uint8_t kFrame[] = {
            0x18, 0x42, 0x0A,
            0x01, 0xF0, 0x30, c.enum_v,
        };
        auto r = dispatch_attr_report(std::span<const std::uint8_t>(kFrame, sizeof(kFrame)));
        assert(r.any_matched);
        assert(str_eq(r.merged.find("keep_time"), c.label));
    }
}

// All three sensitivity z2m values round-trip.
static void test_sensitivity_all_values() {
    static const struct { std::uint8_t enum_v; const char* label; } cases[] = {
        {0, "low"}, {1, "medium"}, {2, "high"},
    };
    for (auto& c : cases) {
        const std::uint8_t kFrame[] = {
            0x18, 0x42, 0x0A,
            0x13, 0x00, 0x30, c.enum_v,
        };
        auto r = dispatch_attr_report(std::span<const std::uint8_t>(kFrame, sizeof(kFrame)));
        assert(r.any_matched);
        assert(str_eq(r.merged.find("sensitivity"), c.label));
    }
}

// Out-of-range value (firmware glitch). Converter must abstain rather
// than index past the 3-entry label table.
static void test_sensitivity_out_of_range() {
    const std::uint8_t kFrame[] = {
        0x18, 0x42, 0x0A,
        0x13, 0x00, 0x30, 0x05,  // value 5 — beyond low/medium/high
    };
    auto r = dispatch_attr_report(std::span<const std::uint8_t>(kFrame, sizeof(kFrame)));
    assert(r.merged.find("sensitivity") == nullptr);
}

// Read Attributes Response (cmd 0x01) record format differs from cmd
// 0x0A: { attr_id(2), status(1), [type(1), value(N) if status=0] }.
// The decoder normalises both into msg.payload, so the same FZ logic
// should work without changes.
static void test_keep_time_read_response() {
    const std::uint8_t kFrame[] = {
        0x18, 0x42, 0x01,        // fc, tsn, cmd=Read Attributes Response
        0x01, 0xF0,              // attr 0xF001
        0x00,                    // status = SUCCESS
        0x30,                    // type = ENUM8
        0x01,                    // value = 1 → "60"
    };
    auto r = dispatch_attr_report(std::span<const std::uint8_t>(kFrame, sizeof(kFrame)));
    assert(r.any_matched);
    assert(str_eq(r.merged.find("keep_time"), "60"));
}

// ── run_configure: bind / report / read pipeline at join ───────────
//
// Lock the contract that joining a ZG-204Z fires:
//   2 binds   — genPowerCfg + ssIasZone on EP1
//   2 reports — batteryPercentageRemaining (0x0021) + batteryVoltage (0x0020)
//   1 read    — ssIasZone attrs 0x0013, 0xF001, 0x0002 (LE packed, manu=0)
//
// Without the configure-time read, sensitivity / keep_time stay
// invisible in the shadow until the user first sets them.

namespace {

struct CapturedBind {
    std::uint16_t device_index;
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
};
struct CapturedReport {
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
    std::uint16_t attr_id;
    std::uint16_t min_interval;
    std::uint16_t max_interval;
};
struct CapturedRead {
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
    std::uint8_t  attr_id_count;
    std::array<std::uint16_t, 8> attr_ids;
    std::uint16_t manu_code;
};

// Single-instance captures — test scaffold lives on the test's main
// task; no concurrency, no atomics needed.
std::array<CapturedBind,   4> g_binds;   std::size_t g_bind_n   = 0;
std::array<CapturedReport, 4> g_reports; std::size_t g_report_n = 0;
std::array<CapturedRead,   4> g_reads;   std::size_t g_read_n   = 0;

bool capture_bind(std::uint16_t idx, std::uint8_t ep, std::uint16_t cl) {
    if (g_bind_n < g_binds.size()) g_binds[g_bind_n++] = {idx, ep, cl};
    return true;
}
bool capture_report(std::uint16_t /*idx*/, std::uint8_t ep, std::uint16_t cl,
                     std::uint16_t attr, std::uint8_t /*type*/,
                     std::uint16_t mn, std::uint16_t mx,
                     std::uint32_t /*chg*/, std::uint16_t /*manu*/) {
    if (g_report_n < g_reports.size())
        g_reports[g_report_n++] = {ep, cl, attr, mn, mx};
    return true;
}
bool capture_read(std::uint16_t /*idx*/, std::uint8_t ep, std::uint16_t cl,
                   const std::uint8_t* attrs_le, std::uint8_t count,
                   std::uint16_t manu) {
    if (g_read_n >= g_reads.size()) return true;
    CapturedRead& r = g_reads[g_read_n++];
    r.endpoint      = ep;
    r.cluster_id    = cl;
    r.attr_id_count = count;
    r.manu_code     = manu;
    for (std::uint8_t i = 0; i < count && i < r.attr_ids.size(); ++i) {
        r.attr_ids[i] = static_cast<std::uint16_t>(attrs_le[i * 2]) |
                         (static_cast<std::uint16_t>(attrs_le[i * 2 + 1]) << 8);
    }
    return true;
}

void reset_capture() {
    g_bind_n = 0; g_report_n = 0; g_read_n = 0;
}

}  // namespace

static void test_configure_pipeline() {
    reset_capture();

    RuntimeContext ctx{};
    ctx.device_index     = 7;
    ctx.configure_bind   = &capture_bind;
    ctx.configure_report = &capture_report;
    ctx.configure_read   = &capture_read;

    const bool ok = zhc::run_configure(devices::tuya::kDefGen_ZG_204Z, ctx);
    assert(ok);

    // Bindings: genPowerCfg (0x0001) + ssIasZone (0x0500) on EP1.
    assert(g_bind_n == 2);
    assert(g_binds[0].device_index == 7);
    assert(g_binds[0].endpoint == 1);
    assert(g_binds[0].cluster_id == 0x0001);
    assert(g_binds[1].endpoint == 1);
    assert(g_binds[1].cluster_id == 0x0500);

    // Reports: batteryPercentageRemaining (0x0021) + batteryVoltage (0x0020),
    // both on genPowerCfg with the 3600/65535 default.
    assert(g_report_n == 2);
    assert(g_reports[0].cluster_id == 0x0001);
    assert(g_reports[0].attr_id    == 0x0021);
    assert(g_reports[0].min_interval == 3600);
    assert(g_reports[0].max_interval == 65535);
    assert(g_reports[1].cluster_id == 0x0001);
    assert(g_reports[1].attr_id    == 0x0020);

    // Read: ssIasZone attrs 0x0013, 0xF001, 0x0002 — exactly the triple
    // z2m's tz.ZM35HQ_attr.convertGet pulls.
    assert(g_read_n == 1);
    assert(g_reads[0].endpoint      == 1);
    assert(g_reads[0].cluster_id    == 0x0500);
    assert(g_reads[0].attr_id_count == 3);
    assert(g_reads[0].attr_ids[0]   == 0x0013);
    assert(g_reads[0].attr_ids[1]   == 0xF001);
    assert(g_reads[0].attr_ids[2]   == 0x0002);
    assert(g_reads[0].manu_code     == 0);
}

// ── Per-report failure tolerance (kReportFlagOptional) ──────────────
//
// Synthesise a mini def with three reports — the middle one fails.
// Without the optional flag → pipeline aborts after the failure (only
// the first report fires, none after).
// With the optional flag on the middle entry → the failure is logged
// and the loop keeps going (all three callbacks fire; config_steps
// still runs).

namespace zhc { bool run_configure(const PreparedDefinition& def,
                                    RuntimeContext& ctx); }

namespace {

// Capture of report attempts + a knob for forcing failure on a given
// attr_id. The pipeline test above keeps using `capture_report` which
// always returns true; this scaffold is a separate fixture.
std::uint16_t g_failing_attr = 0;

bool report_fails_on(std::uint16_t /*idx*/, std::uint8_t ep, std::uint16_t cl,
                      std::uint16_t attr, std::uint8_t /*type*/,
                      std::uint16_t mn, std::uint16_t mx,
                      std::uint32_t /*chg*/, std::uint16_t /*manu*/) {
    if (g_report_n < g_reports.size())
        g_reports[g_report_n++] = {ep, cl, attr, mn, mx};
    return attr != g_failing_attr;   // forced failure on this attr id
}

// Read step shared by both flavors below.
constexpr std::uint8_t kReadAttrs_Fixture[] = { 0xAA, 0xBB };  // attr 0xBBAA
constexpr zhc::ConfigStep kConfigSteps_Fixture[] = {
    {
        .op          = zhc::ConfigStepOp::Read,
        .endpoint    = 1,
        .cluster_id  = 0x0001,
        .cmd_id      = 0,
        .flags       = 0,
        .payload     = kReadAttrs_Fixture,
        .payload_len = sizeof(kReadAttrs_Fixture),
        .wait_ms     = 0,
        .manu_code   = 0,
    },
};

}  // namespace

// Mandatory failure aborts the rest of the pipeline.
static void test_report_mandatory_failure_aborts() {
    reset_capture();
    g_failing_attr = 0x0020;   // 2nd report fails

    constexpr zhc::ReportingSpec kReports[] = {
        {1, 0x0001, 0x0021, 0x20, 3600, 65535, 1, 0, /*flags=*/0},
        {1, 0x0001, 0x0020, 0x20, 3600, 65535, 1, 0, /*flags=*/0},
        {1, 0x0001, 0x0019, 0x20, 3600, 65535, 1, 0, /*flags=*/0},
    };
    PreparedDefinition def{};
    def.reports             = kReports;
    def.reports_count       = sizeof(kReports) / sizeof(kReports[0]);
    def.config_steps        = kConfigSteps_Fixture;
    def.config_steps_count  = 1;

    RuntimeContext ctx{};
    ctx.device_index     = 99;
    ctx.configure_report = &report_fails_on;
    ctx.configure_read   = &capture_read;

    const bool ok = zhc::run_configure(def, ctx);
    assert(!ok);                              // overall result reflects abort
    assert(g_report_n == 2);                  // 0x0021 succeeded, 0x0020 attempted+failed
    assert(g_reports[1].attr_id == 0x0020);
    assert(g_read_n == 0);                    // config_steps must NOT have run
}

// Optional failure: middle report is flagged optional → loop continues
// → all three reports fire → config_steps still runs → overall ok=true.
static void test_report_optional_failure_continues() {
    reset_capture();
    g_failing_attr = 0x0020;

    constexpr zhc::ReportingSpec kReports[] = {
        {1, 0x0001, 0x0021, 0x20, 3600, 65535, 1, 0, /*flags=*/0},
        {1, 0x0001, 0x0020, 0x20, 3600, 65535, 1, 0,
         /*flags=*/zhc::kReportFlagOptional},
        {1, 0x0001, 0x0019, 0x20, 3600, 65535, 1, 0, /*flags=*/0},
    };
    PreparedDefinition def{};
    def.reports             = kReports;
    def.reports_count       = sizeof(kReports) / sizeof(kReports[0]);
    def.config_steps        = kConfigSteps_Fixture;
    def.config_steps_count  = 1;

    RuntimeContext ctx{};
    ctx.device_index     = 99;
    ctx.configure_report = &report_fails_on;
    ctx.configure_read   = &capture_read;

    const bool ok = zhc::run_configure(def, ctx);
    assert(ok);                               // overall success — optional miss
    assert(g_report_n == 3);                  // all three attempted
    assert(g_reports[0].attr_id == 0x0021);
    assert(g_reports[1].attr_id == 0x0020);   // attempted, returned false
    assert(g_reports[2].attr_id == 0x0019);
    assert(g_read_n == 1);                    // config_steps ran
    assert(g_reads[0].cluster_id == 0x0001);
}

int main() {
    test_motion_present();
    test_motion_clear();
    test_tamper_only();
    test_battery_low_with_motion();
    test_other_cmd_does_not_match();
    test_sensitivity_report_medium();
    test_keep_time_report_120s();
    test_combined_sensitivity_and_keep_time();
    test_keep_time_all_values();
    test_sensitivity_all_values();
    test_sensitivity_out_of_range();
    test_keep_time_read_response();
    test_configure_pipeline();
    test_report_mandatory_failure_aborts();
    test_report_optional_failure_continues();
    return 0;
}
