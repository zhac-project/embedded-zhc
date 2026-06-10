// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Onesti Products AS (EasyAccess / Nimly / Relax)
// family.
//
// The vendor has three defs. The two door locks (easyCodeTouch_v1 and
// Nimly) are functionally clean against z2m for their core surface:
// fz.lock (lock state), fz.battery, fz.lock_operation_event +
// fz.lock_programming_event (the `action` stream), and tz.lock /
// tz.pincode_lock / tz.lock_sound_volume all map onto the generic
// closuresDoorLock runtime (kFzLock / kFzLockOperationEvent /
// kFzLockProgrammingEvent / kTzLock / kTzLockPinCode / kTzLockSoundVolume).
// z2m additionally layers two VENDOR-SPECIFIC decoders with no generic
// equivalent — fzLocal.nimly_pro_lock_actions (closuresDoorLock attrs
// 256/257 -> last_action_source / last_action_user / last_used_pin_code)
// and fz.easycodetouch_action (a `type:"raw"` closuresDoorLock frame
// indexed into constants.easyCodeTouchActions). Both need fresh
// attribute / raw-cluster infra + a lookup table; INFRA-deferred, not
// half-built here.
//
// The one real, buildable z2m<->ZHC gap was S4RX-110 ("Relax" smart
// plug). z2m wires fz.on_off + fz.electrical_measurement + fz.metering +
// fz.device_temperature and binds genDeviceTempCfg (0x0002), but the
// auto-generated port carried only kFzOnOff + kFzMetering +
// kFzElectricalMeasurement and dropped device_temperature entirely (same
// gap class as dawon_dns PM-B540-ZB). All of the plug's clusters live on
// endpoint 2 (z2m `endpoint: () => ({default: 2})`). Fixed by graduating
// One_S4RX_110.cpp to a Tier-2 parent override that wires the generic
// kFzDeviceTemperature (0x0002 attr 0x0000 currentTemperature, raw whole
// °C — mirrors fromZigbee.ts fz.device_temperature), adds the
// device_temperature expose + the 0x0002 bind, retargets the bindings to
// endpoint 2, and sets default_endpoint=2 so outbound on/off routes there.
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * a genDeviceTempCfg (0x0002) report yields device_temperature (Int °C,
//     signed, no scaling),
//   * the metering (0x0702) + electrical-measurement (0x0B04) halves still
//     yield energy/power and voltage/current (regression),
//   * S4RX-110 declares the full expose set incl. device_temperature, binds
//     all four clusters on endpoint 2, and routes outbound to ep 2.
//
// z2m-source: zigbee-herdsman-converters/src/devices/onesti.ts
//             #S4RX-110 + converters/fromZigbee.ts device_temperature.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::onesti {
extern const PreparedDefinition kDef_S4RX_110;
extern const PreparedDefinition kDef_Nimly;
extern const PreparedDefinition kDef_easyCodeTouch_v1;
}  // namespace zhc::devices::onesti

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

bool def_binds_ep(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
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

// ── S4RX-110: the real fix. device_temperature + metering/electrical
//    regression, all on endpoint 2. ──
void check_s4rx110() {
    using namespace zhc::devices::onesti;
    const PreparedDefinition& def = kDef_S4RX_110;

    // Static shape: device_temperature added without dropping any of the
    // power-measurement channels.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "device_temperature"));

    // All clusters bound on endpoint 2 (z2m default:2), incl. the new 0x0002.
    assert(def_binds_ep(def, 2, 0x0006));   // genOnOff
    assert(def_binds_ep(def, 2, 0x0702));   // seMetering
    assert(def_binds_ep(def, 2, 0x0B04));   // haElectricalMeasurement
    assert(def_binds_ep(def, 2, 0x0002));   // genDeviceTempCfg
    assert(def.default_endpoint == 2);

    // genDeviceTempCfg (0x0002) attr 0x0000 currentTemperature, int16s.
    // 0x0021 = 33 °C, returned raw (no scaling).
    const std::uint8_t temp_b[] = {0x21, 0x00};
    auto rt = dispatch_zcl(def, 0x0002, "genDeviceTempCfg", 2,
                           attr_report(0x0000, 0x29, temp_b));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("device_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 33);
    // Must NOT masquerade as the 0.01 °C measurement channel.
    assert(rt.merged.find("temperature") == nullptr);

    // Negative die temperature round-trips as signed (int16s = -4 °C).
    const std::uint8_t neg_b[] = {0xFC, 0xFF};
    auto rn = dispatch_zcl(def, 0x0002, "genDeviceTempCfg", 2,
                           attr_report(0x0000, 0x29, neg_b));
    assert(rn.any_matched);
    const Value* nv = rn.merged.find("device_temperature");
    assert(nv && nv->type == ValueType::Int && nv->i == -4);

    // Regression: seMetering (0x0702) still yields energy + power.
    // attr 0x0000 CurrentSummationDelivered, uint48 = 1234.
    const std::uint8_t energy_b[] = {0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto re = dispatch_zcl(def, 0x0702, "seMetering", 2,
                           attr_report(0x0000, 0x25, energy_b));
    assert(re.any_matched);
    assert(re.merged.find("energy") != nullptr);

    // Regression: haElectricalMeasurement (0x0B04) still yields voltage/current.
    // attr 0x0505 rmsVoltage, uint16 = 230.
    const std::uint8_t volt_b[] = {0xE6, 0x00};
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 2,
                           attr_report(0x0505, 0x21, volt_b));
    assert(rv.any_matched);
    assert(rv.merged.find("voltage") != nullptr);

    // attr 0x0508 rmsCurrent, uint16 = 1500.
    const std::uint8_t cur_b[] = {0xDC, 0x05};
    auto rc = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 2,
                           attr_report(0x0508, 0x21, cur_b));
    assert(rc.any_matched);
    assert(rc.merged.find("current") != nullptr);

    // onOff (0x0006) attr 0x0000 bool → state still decodes (on ep 2).
    const std::uint8_t on_b[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0006, "genOnOff", 2,
                           attr_report(0x0000, 0x10, on_b));
    assert(ro.any_matched);
    assert(ro.merged.find("state") != nullptr);
}

// ── Locks stay whole: core lock/battery/action/pincode surface intact.
//    (The vendor-specific last_action / last_used_pin_code extras are
//    INFRA-deferred, so we only assert the generic surface here.) ──
void check_locks() {
    using namespace zhc::devices::onesti;

    for (const PreparedDefinition* def : {&kDef_easyCodeTouch_v1, &kDef_Nimly}) {
        assert(def_exposes(*def, "battery"));
        assert(def_exposes(*def, "lock_state"));
        assert(def_exposes(*def, "sound_volume"));
        assert(def_exposes(*def, "pin_code"));
        assert(def_exposes(*def, "action"));
        // No phantom power-measurement / light bundle on a battery lock.
        assert(!def_exposes(*def, "power"));
        assert(!def_exposes(*def, "brightness"));
        assert(!def_exposes(*def, "device_temperature"));
        // closuresDoorLock binding present (lock state + action stream).
        assert(def_binds_ep(*def, 1, 0x0101));
    }
}

}  // namespace

int main() {
    check_s4rx110();
    check_locks();
    return 0;
}
