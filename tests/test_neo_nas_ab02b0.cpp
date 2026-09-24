// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Neo NAS-AB02B0 (temperature/humidity alarm): both fingerprints z2m serves
// resolve to it -- TS0601 + _TZE200_d0yu2xgi and the _TYST11_ unit that
// reports zigbeeModel "0yu2xgi" -- and no Tuya definition claims them first.
// power_type (DP 101) also yields battery_low, as z2m's value === 3.
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/devices/neo_registry.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::neo {
extern const PreparedDefinition kDef_NAS_AB02B0;
}

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> neo_reg() {
    return {devices::neo::kNeoRegistry, devices::neo::kNeoRegistryCount};
}
std::span<const PreparedDefinition* const> tuya_reg() {
    return {devices::tuya::kTuyaRegistry, devices::tuya::kTuyaRegistryCount};
}

const Value* decode_dp101(std::uint8_t v, const char* key, RuntimeContext& ctx, DispatchResult& out) {
    const std::uint8_t data[1] = { v };
    const TuyaDpRecord recs[] = { { 101, 0x04, std::span<const std::uint8_t>(data, 1) } };
    DecodedMessage msg{};
    msg.family = FrameFamily::TuyaDp; msg.type = MessageType::Command;
    msg.cluster = "manuSpecificTuya"; msg.direction = Direction::ServerToClient;
    msg.command_id = 0x02; msg.src_endpoint = 1; msg.dst_endpoint = 1;
    InboundApsFrame raw{};
    raw.cluster_id = 0xEF00; raw.src_endpoint = 1; raw.dst_endpoint = 1;
    out = dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, 1),
                               devices::neo::kDef_NAS_AB02B0, raw, ctx);
    return out.merged.find(key);
}

}  // namespace

int main() {
    const auto* ab = &devices::neo::kDef_NAS_AB02B0;
    assert(find_definition("TS0601", "_TZE200_d0yu2xgi", neo_reg()) == ab);
    assert(find_definition("0yu2xgi", "_TYST11_d0yu2xgi", neo_reg()) == ab);
    // Nothing in the Tuya registry may win these first (a slim-sensor
    // stand-in did, exposing only `state`).
    const auto* t = find_definition("TS0601", "_TZE200_d0yu2xgi", tuya_reg());
    assert(t == nullptr || t->manufacturer_names_count == 0);   // only a generic TS0601 fallback

    // volume is a writable enum: its values must reach the UI as a list.
    bool vol = false;
    for (std::size_t i = 0; i < ab->exposes_count; ++i) {
        const auto& e = ab->exposes[i];
        if (std::strcmp(e.name, "volume") == 0) {
            vol = e.enum_count == 3 && std::strcmp(e.enum_values[1], "medium") == 0;
        }
    }
    assert(vol);

    RuntimeContext ctx{};
    DispatchResult r{};
    const Value* low = decode_dp101(3, "battery_low", ctx, r);
    assert(low && low->type == ValueType::Bool && low->b);
    const Value* pt = r.merged.find("power_type");
    assert(pt && pt->type == ValueType::StringRef && std::strcmp(pt->str, "battery_low") == 0);

    const Value* usb = decode_dp101(4, "battery_low", ctx, r);
    assert(usb && usb->type == ValueType::Bool && !usb->b);
    return 0;
}
