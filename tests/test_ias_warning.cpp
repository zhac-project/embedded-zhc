// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// IAS WD `warning` (ssIasWd 0x0502, startWarning cmd 0x00) sends what z2m sends.
// ez has no composite expose, so `warning` is a settable binary: true is z2m's
// default warning, a number is that warning with this duration, and false is
// m.iasWarning's stop.
//   start: mode emergency, level medium, strobe on, 10 s, strobe duty cycle 0,
//          strobe level 1 -> info 0x35 = (mode << 4) | (strobe << 2) | level
//   stop:  mode stop, level low, strobe off, 0 s, strobe level 1 -> info 0x00
// Six sirens take the reversed info byte, mode | (strobe << 4) | (level << 6) = 0x53:
// AV2010/24A and AV2010/29A (tz.warning's model list) and SMSZB-120, HESZB-120,
// SIRZB-110, SIRZB-111 (m.iasWarning({reversePayload: true})).
//
// z2m-source: converters/toZigbee.ts warning, lib/modernExtend.ts iasWarning (v26.133.0).

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/devices/adeo_registry.hpp"
#include "zhc/devices/adurosmart_registry.hpp"
#include "zhc/devices/bitron_registry.hpp"
#include "zhc/devices/develco_registry.hpp"
#include "zhc/devices/heiman_registry.hpp"
#include "zhc/devices/smartthings_registry.hpp"
#include "zhc/devices/tier_e_registries.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/types.hpp"
#include "test_ctx.hpp"

using namespace zhc;

namespace {

constexpr const char* kReversed[] = {
    "AV2010/24A", "AV2010/29A", "SMSZB-120", "HESZB-120", "SIRZB-110", "SIRZB-111",
};

bool reversed(const char* model) {
    for (const char* m : kReversed)
        if (std::strcmp(m, model) == 0) return true;
    return false;
}

// startWarning payload: info, duration lo, duration hi, strobe duty cycle, strobe level.
// Empty when no converter turns `warning` into an ssIasWd startWarning.
std::vector<std::uint8_t> start_warning(const PreparedDefinition& def, const Value& v) {
    RuntimeContext& ctx = test_ctx();
    std::uint8_t buf[96] = {};
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* c = def.to_zigbee[i];
        if (!c || (c->key && std::strcmp(c->key, "warning") != 0)) continue;   // null key = wildcard
        std::size_t n = 0;
        if (c->fn("warning", v, *c, def, ctx, std::span<std::uint8_t>(buf, sizeof(buf)), n) &&
            c->cluster_id == 0x0502 && n == 8 && buf[2] == 0x00)
            return {buf + 3, buf + n};
    }
    return {};
}

Value boolean(bool b) { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }
Value seconds(std::uint64_t s) { Value v{}; v.type = ValueType::Uint; v.u = s; return v; }

const Expose* expose(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (std::strcmp(d.exposes[i].name, name) == 0) return &d.exposes[i];
    return nullptr;
}

using Bytes = std::vector<std::uint8_t>;

}  // namespace

int main() {
    const Bytes stop{0x00, 0x00, 0x00, 0x00, 0x01};
    // Tier-E vendors plus the siren vendors outside that aggregator.
    std::vector<std::span<const PreparedDefinition* const>> regs{
        {devices::adeo::kAdeoRegistry, devices::adeo::kAdeoRegistryCount},
        {devices::adurosmart::kAdurosmartRegistry, devices::adurosmart::kAdurosmartRegistryCount},
        {devices::bitron::kBitronRegistry, devices::bitron::kBitronRegistryCount},
        {devices::develco::kDevelcoRegistry, devices::develco::kDevelcoRegistryCount},
        {devices::heiman::kHeimanRegistry, devices::heiman::kHeimanRegistryCount},
        {devices::smartthings::kSmartthingsRegistry, devices::smartthings::kSmartthingsRegistryCount},
    };
    for (std::size_t r = 0; r < devices::tier_e::kTierERegistriesCount; ++r)
        regs.emplace_back(devices::tier_e::kTierERegistries[r].reg, devices::tier_e::kTierERegistries[r].count);

    std::size_t sirens = 0, rev = 0, bad = 0;
    for (const auto& vendor : regs) {
        for (const PreparedDefinition* d : vendor) {
            if (!d) continue;
            const Bytes start = start_warning(*d, boolean(true));
            if (start.empty()) continue;
            ++sirens;
            const bool is_rev = reversed(d->model);
            rev += is_rev;
            const Bytes want{static_cast<std::uint8_t>(is_rev ? 0x53 : 0x35), 0x0A, 0x00, 0x00, 0x01};
            // A number is the start warning with that duration; 0 stops.
            const Bytes timed = start_warning(*d, seconds(300));
            if (start != want || start_warning(*d, boolean(false)) != stop ||
                timed != Bytes{want[0], 0x2C, 0x01, 0x00, 0x01} || start_warning(*d, seconds(0)) != stop) {
                std::fprintf(stderr, "%s: info 0x%02X\n", d->model, start[0]);
                ++bad;
            }
            // Where `warning` is exposed, the UI and the HA bridge drive it as a
            // settable binary: the encoder takes a bool or a duration, not text.
            const Expose* w = expose(*d, "warning");
            if (w && (w->type != ExposeType::Binary || w->access != Access::Set)) {
                std::fprintf(stderr, "%s: warning expose is not a settable binary\n", d->model);
                ++bad;
            }
        }
    }
    std::printf("%zu sirens, %zu reversed\n", sirens, rev);
    assert(bad == 0);
    assert(rev == sizeof(kReversed) / sizeof(kReversed[0]));
    assert(sirens >= 30);
    return 0;
}
