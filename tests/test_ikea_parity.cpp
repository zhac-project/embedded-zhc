// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the IKEA non-light device family.
// z2m-source: ikea.ts + lib/ikea.ts.
//
// Bugs fixed (auto-generated defs wired the full kFzIkeaLight bundle —
// state/brightness/color_temp + OnOff/Level/Color bindings — onto
// devices that are NOT lights, leaving their real sensor channels dead):
//   * E1525/E1745 TRADFRI motion sensor   -> phantom light  (now battery)
//   * E2112       VINDSTYRKA air quality  -> phantom light  (now temp+humidity)
//   * E1746       TRADFRI signal repeater -> phantom light  (now empty, z2m exposes:[])
//   * E2007       STARKVIND air purifier  -> phantom light  (now empty; channels are
//                                            manuSpecific 0xfc7d, INFRA)
//   * E2134       VALLHORN motion sensor  -> battery-only; occupancy + illuminance
//                                            (msOccupancySensing 0x0406 /
//                                             msIlluminanceMeasurement 0x0400) were dead
//   * E1757/E1926/E2102/E2103 blinds      -> cover position only; ikeaBattery()
//                                            (genPowerCfg) was dropped
//
// These tests pin: (a) the phantom-light keys are gone from the
// non-light defs; (b) VALLHORN decodes occupancy + illuminance from
// real wire frames; (c) VINDSTYRKA decodes temperature + humidity; and
// (d) the blinds/motion now expose battery.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ikea {
extern const PreparedDefinition kDef_E1525_E1745;  // TRADFRI motion (was phantom light)
extern const PreparedDefinition kDef_E2112;        // VINDSTYRKA     (was phantom light)
extern const PreparedDefinition kDef_E1746;        // signal repeater(was phantom light)
extern const PreparedDefinition kDef_E2007;        // STARKVIND      (was phantom light)
extern const PreparedDefinition kDef_E2134;        // VALLHORN motion
extern const PreparedDefinition kDef_E1757;        // FYRTUR blind
extern const PreparedDefinition kDef_E1926;        // KADRILJ blind
extern const PreparedDefinition kDef_E2102;        // PRAKTLYSING blind
extern const PreparedDefinition kDef_E2103;        // TREDANSEN blind
}  // namespace zhc::devices::ikea

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }

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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster,
                        const char* cluster_name, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// No light keys must survive on a non-light device.
void assert_not_light(const PreparedDefinition& def) {
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
    // And it must not bind the colour cluster.
    assert(!def_binds(def, 0x0300));
}

}  // namespace

// ── E2134 VALLHORN — occupancy (0x0406) + illuminance (0x0400) ───────
static void test_vallhorn_occupancy_illuminance() {
    const auto& def = devices::ikea::kDef_E2134;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "illuminance"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0406));
    assert(def_binds(def, 0x0400));
    // It must NOT be a light anymore.
    assert_not_light(def);

    // msOccupancySensing Report Attributes: attr 0x0000 (occupancy, bitmap8),
    // bit 0 = occupied.
    const std::uint8_t kOcc[] = {
        0x18, 0x10, 0x0A,  // fc, tsn, cmd=Report Attributes
        0x00, 0x00,        // attr 0x0000
        0x18,              // type bitmap8
        0x01,              // value 1 = occupied
    };
    auto occ = dispatch(def, 0x0406, "msOccupancySensing", kOcc);
    assert(occ.any_matched);
    assert(b_true(occ.merged.find("occupancy")));

    // msIlluminanceMeasurement Report Attributes: attr 0x0000 (measuredValue,
    // uint16). Value 0x0064 = 100. Just assert the key decodes (any numeric).
    const std::uint8_t kIllu[] = {
        0x18, 0x11, 0x0A,  // fc, tsn, cmd=Report Attributes
        0x00, 0x00,        // attr 0x0000
        0x21,              // type uint16
        0x64, 0x00,        // value 100
    };
    auto illu = dispatch(def, 0x0400, "msIlluminanceMeasurement", kIllu);
    assert(illu.any_matched);
    assert(illu.merged.find("illuminance") != nullptr);
}

// ── E2112 VINDSTYRKA — temperature (0x0402) + humidity (0x0405) ──────
static void test_vindstyrka_temp_humidity() {
    const auto& def = devices::ikea::kDef_E2112;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_binds(def, 0x0402));
    assert(def_binds(def, 0x0405));
    assert_not_light(def);

    // msTemperatureMeasurement measuredValue (int16) 0x0834 = 2100 = 21.00 °C.
    const std::uint8_t kTemp[] = {
        0x18, 0x20, 0x0A, 0x00, 0x00, 0x29, 0x34, 0x08,
    };
    auto t = dispatch(def, 0x0402, "msTemperatureMeasurement", kTemp);
    assert(t.any_matched);
    assert(t.merged.find("temperature") != nullptr);

    // msRelativeHumidity measuredValue (uint16) 0x1388 = 5000 = 50.00 %.
    const std::uint8_t kHum[] = {
        0x18, 0x21, 0x0A, 0x00, 0x00, 0x21, 0x88, 0x13,
    };
    auto h = dispatch(def, 0x0405, "msRelativeHumidity", kHum);
    assert(h.any_matched);
    assert(h.merged.find("humidity") != nullptr);
}

// ── E1746 signal repeater + E2007 air purifier — no phantom light ────
static void test_repeater_purifier_no_phantom_light() {
    const auto& rep = devices::ikea::kDef_E1746;
    assert_not_light(rep);
    assert(rep.exposes_count == 0);
    assert(rep.from_zigbee_count == 0);
    assert(rep.to_zigbee_count == 0);

    const auto& pur = devices::ikea::kDef_E2007;
    assert_not_light(pur);
    assert(pur.exposes_count == 0);
    assert(pur.from_zigbee_count == 0);
    // STARKVIND keeps its white-label (E2006) but no functional channels.
    assert(pur.white_labels_count == 1);
}

// ── E1525/E1745 TRADFRI motion — phantom light gone, battery present ─
static void test_tradfri_motion_battery_no_light() {
    const auto& def = devices::ikea::kDef_E1525_E1745;
    assert_not_light(def);
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0001));

    // genPowerCfg batteryPercentageRemaining (attr 0x0021, uint8). 0xC8 = 200 -> 100%.
    const std::uint8_t kBatt[] = {
        0x18, 0x30, 0x0A, 0x21, 0x00, 0x20, 0xC8,
    };
    auto r = dispatch(def, 0x0001, "genPowerCfg", kBatt);
    assert(r.any_matched);
    assert(r.merged.find("battery") != nullptr);
}

// ── Blinds — cover position retained + battery added ─────────────────
static void check_blind(const PreparedDefinition& def) {
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0102));  // closuresWindowCovering
    assert(def_binds(def, 0x0001));  // genPowerCfg

    // genPowerCfg battery report still decodes on the blind.
    const std::uint8_t kBatt[] = {
        0x18, 0x40, 0x0A, 0x21, 0x00, 0x20, 0xC8,
    };
    auto r = dispatch(def, 0x0001, "genPowerCfg", kBatt);
    assert(r.any_matched);
    assert(r.merged.find("battery") != nullptr);
}

static void test_blinds_battery() {
    check_blind(devices::ikea::kDef_E1757);  // FYRTUR
    check_blind(devices::ikea::kDef_E1926);  // KADRILJ
    check_blind(devices::ikea::kDef_E2102);  // PRAKTLYSING
    check_blind(devices::ikea::kDef_E2103);  // TREDANSEN
}

int main() {
    test_vallhorn_occupancy_illuminance();
    test_vindstyrka_temp_humidity();
    test_repeater_purifier_no_phantom_light();
    test_tradfri_motion_battery_no_light();
    test_blinds_battery();
    return 0;
}
