// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Innr smart-plug family with power monitoring.
//
// Innr is almost entirely a lighting vendor (m.light(...) -> on/off +
// brightness +/- color_temp +/- color), and the remotes (RC 110/210/250)
// already surface command_* presses on the `action` channel. The one real
// z2m<->ZHC gap was the metering plugs: SP 120 / SP 234 / SP 240 / OSP 240
// are built in z2m with
//   m.onOff() + m.electricityMeter({...})
// and electricityMeter() defaults to cluster:"both" with
// electricalMeasurementType:"ac" -- i.e. it decodes BOTH seMetering
// (0x0702 -> energy/instantaneousDemand) AND haElectricalMeasurement
// (0x0B04 -> rmsCurrent/rmsVoltage/activePower). The auto-generated ports
// wired only kFzMetering (0x0702) and so dropped the current + voltage
// channels entirely. Fixed by graduating each def and wiring the generic
// kFzElectricalMeasurement converter + current/voltage exposes + 0x0B04
// bind (mirrors the Lidl HG08673 power-plug override).
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * a haElectricalMeasurement (0x0B04) report yields power/voltage/current,
//   * a seMetering (0x0702) report still yields energy,
//   * the def declares all of state/power/current/voltage/energy exposes
//     and binds genOnOff/haElectricalMeasurement/seMetering.
//
// The plain-onOff plugs (SP 110/220/222/224, OSP 210) and the light/remote
// categories are intentionally NOT re-tested here -- they were verified
// CLEAN against z2m in the parity pass and exercised by the generic suites.
//
// z2m-source: zigbee-herdsman-converters/src/devices/innr.ts
//             #SP 120 / #SP 234 / #SP 240 / #OSP 240 +
//             lib/modernExtend.ts electricityMeter() +
//             converters/fromZigbee.ts electrical_measurement / metering.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <utility>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::innr {
extern const PreparedDefinition kDef_SP_120;
extern const PreparedDefinition kDef_SP_234;
extern const PreparedDefinition kDef_SP_240;
extern const PreparedDefinition kDef_OSP_240;
}  // namespace zhc::devices::innr

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A=ReportAttributes,
// then one record: <attr_id LE><type><value LE>.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

void check_plug(const PreparedDefinition& def) {
    // ── Static shape: full electricityMeter expose + bind surface. ──
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "energy"));
    assert(def_binds(def, 0x0006));   // genOnOff
    assert(def_binds(def, 0x0B04));   // haElectricalMeasurement
    assert(def_binds(def, 0x0702));   // seMetering

    // ── haElectricalMeasurement (0x0B04) → power / voltage / current. ──
    // attr 0x050B ActivePower, int16s = 0x00E6 = 230 W.
    const std::uint8_t power_b[] = {0xE6, 0x00};
    auto rp = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x050B, 0x29, power_b));
    assert(rp.any_matched);
    const Value* pv = rp.merged.find("power");
    assert(pv && pv->type == ValueType::Int && pv->i == 230);

    // attr 0x0505 RMSVoltage, uint16 = 0x00E9 = 233 V.
    const std::uint8_t volt_b[] = {0xE9, 0x00};
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0505, 0x21, volt_b));
    assert(rv.any_matched);
    const Value* vv = rv.merged.find("voltage");
    assert(vv && vv->type == ValueType::Uint && vv->u == 233);

    // attr 0x0508 RMSCurrent, uint16 = 0x03DE = 990 mA.
    const std::uint8_t curr_b[] = {0xDE, 0x03};
    auto rc = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0508, 0x21, curr_b));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("current");
    assert(cv && cv->type == ValueType::Uint && cv->u == 990);

    // ── Regression: seMetering (0x0702) still yields energy. ──
    // attr 0x0000 CurrentSummationDelivered, uint48 = 1234.
    const std::uint8_t energy_b[] = {0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto re = dispatch_zcl(def, 0x0702, "seMetering", 1,
                           attr_report(0x0000, 0x25, energy_b));
    assert(re.any_matched);
    assert(re.merged.find("energy") != nullptr);
}

}  // namespace

int main() {
    using namespace zhc::devices::innr;
    for (const PreparedDefinition* def :
         {&kDef_SP_120, &kDef_SP_234, &kDef_SP_240, &kDef_OSP_240}) {
        check_plug(*def);
    }
    return 0;
}
