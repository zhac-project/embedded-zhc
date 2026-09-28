// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Registry parity for bare TS0601 fingerprints. A definition with zigbeeModel
// TS0601 and no manufacturer list matches every TS0601 nothing else claims.
// Two generated Lincukoo stubs (SZLR08, SZW08) did exactly that, so an unknown
// Tuya DP device — the soil sensor `_TZE284_2nhqasjh` among them — showed up
// as a Lincukoo radar with "state + battery". Their manufacturer-listed twins
// already carry z2m's fingerprints. This pins:
//   * no TS0601 catch-all in the Tuya, Moes, Lincukoo and tier-E registries;
//   * the Lincukoo fingerprints, and their _TZE28C1000000_ / _TZE2841000000_
//     twins (z2m v26.105.0), still resolve to their Lincukoo definitions;
//   * `_TZE284_2nhqasjh` is a TS0601_soil like the other eight z2m lists.
//
// z2m-source: tuya.ts #TS0601_soil, lincukoo.ts #SZLR08 / #SZLR08T / #SZW08.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/devices/lincukoo_registry.hpp"
#include "zhc/devices/moes_registry.hpp"
#include "zhc/devices/tier_e_registries.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::vector<const PreparedDefinition*> g_reg;

void add(const PreparedDefinition* const* reg, std::size_t n) { g_reg.insert(g_reg.end(), reg, reg + n); }

const PreparedDefinition* resolve(const char* manu) {
    return find_definition("TS0601", manu, std::span<const PreparedDefinition* const>(g_reg.data(), g_reg.size()));
}

bool is_model(const PreparedDefinition* d, const char* model) {
    return d && std::strcmp(d->model, model) == 0;
}

bool exposes(const PreparedDefinition& d, const char* key) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (std::strcmp(d.exposes[i].name, key) == 0) return true;
    return false;
}

// One datapoint through the definition; `key` must come out.
Value decode(const PreparedDefinition& d, std::uint8_t dp, std::uint8_t type,
             std::vector<std::uint8_t> bytes, const char* key) {
    DecodedMessage msg{};
    msg.family = FrameFamily::TuyaDp;
    msg.type = MessageType::Command;
    msg.cluster = "manuSpecificTuya";
    msg.direction = Direction::ServerToClient;
    msg.command_id = 0x02;
    msg.src_endpoint = msg.dst_endpoint = 1;
    InboundApsFrame raw{};
    raw.cluster_id = 0xEF00;
    raw.src_endpoint = raw.dst_endpoint = 1;
    const TuyaDpRecord rec[] = {{dp, type, std::span<const std::uint8_t>(bytes)}};
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(rec, 1), d, raw, ctx);
    const Value* v = r.merged.find(key);
    assert(v);
    return *v;
}

}  // namespace

int main() {
    // Adapter order for these vendors: tuya, moes, …, lincukoo; tier E last.
    add(devices::tuya::kTuyaRegistry, devices::tuya::kTuyaRegistryCount);
    add(devices::moes::kMoesRegistry, devices::moes::kMoesRegistryCount);
    add(devices::lincukoo::kLincukooRegistry, devices::lincukoo::kLincukooRegistryCount);
    for (std::size_t i = 0; i < devices::tier_e::kTierERegistriesCount; ++i)
        add(devices::tier_e::kTierERegistries[i].reg, devices::tier_e::kTierERegistries[i].count);

    // No definition claims TS0601 without a manufacturer list …
    for (const auto* d : g_reg) {
        if (!d || d->manufacturer_name_prefix || d->manufacturer_names_count) continue;
        for (std::size_t k = 0; k < d->zigbee_models_count; ++k)
            assert(std::strcmp(d->zigbee_models[k], "TS0601") != 0);
    }
    // … so an unknown one stays unknown (the adapter's generic fallback takes it).
    assert(resolve("_TZE200_zzzzzzzz") == nullptr);
    assert(resolve("_TZE284_zzzzzzzz") == nullptr);

    // Lincukoo keeps its devices, twins included.
    const PreparedDefinition* szlr08 = resolve("_TZE204_lw5ny7tp");
    assert(is_model(szlr08, "SZLR08") && szlr08->manufacturer_names_count && exposes(*szlr08, "presence"));
    assert(resolve("_TZE28C1000000_lw5ny7tp") == szlr08);
    const PreparedDefinition* szlr08t = resolve("_TZE204_b8vxct9l");
    assert(is_model(szlr08t, "SZLR08T") && resolve("_TZE28C1000000_b8vxct9l") == szlr08t);
    const PreparedDefinition* szw08 = resolve("_TZE284_ajhu0zqb");
    assert(is_model(szw08, "SZW08") && szw08->manufacturer_names_count);
    assert(resolve("_TZE2841000000_ajhu0zqb") == szw08);

    // All nine z2m TS0601_soil IDs land on a soil definition.
    for (const char* id : {"_TZE200_myd45weu", "_TZE200_ga1maeof", "_TZE200_2se8efxh", "_TZE204_myd45weu",
                           "_TZE284_myd45weu", "_TZE284_oitavov2", "_TZE284_2nhqasjh", "_TZE284_2se8efxh",
                           "_TZE200_9cqcpkgb"}) {
        const PreparedDefinition* d = resolve(id);
        assert(d && std::strcmp(d->vendor, "Tuya") == 0 && exposes(*d, "soil_moisture") &&
               exposes(*d, "temperature") && exposes(*d, "battery"));
    }

    // `_TZE284_2nhqasjh`: z2m's datapoints, all raw.
    const PreparedDefinition* soil = resolve("_TZE284_2nhqasjh");
    assert(is_model(soil, "TS0601_soil"));
    const Value m = decode(*soil, 3, 0x02, {0, 0, 0, 45}, "soil_moisture");
    assert(m.type == ValueType::Int && m.i == 45);
    const Value t = decode(*soil, 5, 0x02, {0, 0, 0, 21}, "temperature");
    assert(t.type == ValueType::Int && t.i == 21);
    const Value u = decode(*soil, 9, 0x04, {1}, "temperature_unit");
    assert(u.type == ValueType::StringRef && std::strcmp(u.str, "fahrenheit") == 0);
    const Value s = decode(*soil, 14, 0x04, {2}, "battery_state");
    assert(s.type == ValueType::StringRef && std::strcmp(s.str, "high") == 0);
    const Value b = decode(*soil, 15, 0x02, {0, 0, 0, 80}, "battery");
    assert(b.type == ValueType::Int && b.i == 80);
    // tuyaBase: magic packet on pairing.
    assert(soil->config_steps_count == 1 && soil->config_steps[0].cluster_id == 0x0000);
    return 0;
}
