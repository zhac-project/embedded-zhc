// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Tuya WSD500A (TS0201 temperature & humidity sensor). The
// auto-generated def wired battery + genOnOff and DROPPED temperature +
// humidity, so msTemperatureMeasurement (0x0402) / msRelativeHumidity (0x0405)
// reports came in as "(no match)". This guards the graduated def: temp/humidity
// decode via the generic ZCL converters (/100 → Float → VAL_FLOAT), battery
// present, and NO spurious onOff `state` (z2m: [TS0201_battery, fz.temperature,
// fz.humidity]). Frame values are from a real _TZ3000_fllyghyj capture.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tuya.ts #WSD500A.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDef_WSD500A;
}  // namespace zhc::devices::tuya

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL global attribute report (cluster-specific=false, Report=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

}  // namespace

int main() {
    const auto& def = devices::tuya::kDef_WSD500A;

    // A temp/humidity sensor exposes temperature + humidity + battery, NOT onOff.
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "state"));   // TS0201 has no onOff

    // msTemperatureMeasurement (0x0402) attr 0x0000, int16 2825 → 28.25 °C (/100).
    const std::uint8_t temp[] = {0x09, 0x0b};   // 0x0B09 = 2825 LE
    auto rt = dispatch_zcl(def, 0x0402, 1, attr_report(0x0000, 0x29, temp));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float && tv->f > 28.24f && tv->f < 28.26f);

    // msRelativeHumidity (0x0405) attr 0x0000, uint16 3690 → 36.90 % (/100).
    const std::uint8_t hum[] = {0x6a, 0x0e};    // 0x0E6A = 3690 LE
    auto rh = dispatch_zcl(def, 0x0405, 1, attr_report(0x0000, 0x21, hum));
    assert(rh.any_matched);
    const Value* hv = rh.merged.find("humidity");
    assert(hv && hv->type == ValueType::Float && hv->f > 36.89f && hv->f < 36.91f);

    return 0;
}
