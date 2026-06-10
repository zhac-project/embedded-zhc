// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Parity fixture for the WMUN ZS05 universal IR remote (TS1201).
//
// The auto-port reduced this IR remote to a battery-only device — it
// kept only kFzBattery and dropped the entire Zosung IR channel that
// z2m wires (wmun.ts #ZS05: addZosungIRTransmitCluster/IRControlCluster
// + the six fzZosung.zosung_send_ir_code_00..05 + tzZosung
// ir_code_to_send / learn_ir_code, exposing learn_ir_code /
// learned_ir_code / ir_code_to_send). After graduating the def to a
// parent override and wiring the shared Zosung runtime, this test pins
// the restored def shape.
//
// z2m-source: zigbee-herdsman-converters/src/devices/wmun.ts #ZS05.

#include <cassert>
#include <cstring>
#include <string_view>

#include "definitions/zosung/_shared.hpp"
#include "zhc/devices/wmun_registry.hpp"
#include "zhc/runtime/definition.hpp"

using namespace zhc;

namespace {

const PreparedDefinition& find_zs05() {
    for (std::size_t i = 0; i < devices::wmun::kWmunRegistryCount; ++i) {
        const PreparedDefinition* d = devices::wmun::kWmunRegistry[i];
        if (d != nullptr && d->model != nullptr &&
            std::strcmp(d->model, "ZS05") == 0) {
            return *d;
        }
    }
    assert(false && "ZS05 not found in wmun registry");
    static PreparedDefinition dummy{};
    return dummy;
}

bool has_expose(const PreparedDefinition& d, const char* name) {
    for (std::uint8_t i = 0; i < d.exposes_count; ++i) {
        if (d.exposes[i].name != nullptr &&
            std::strcmp(d.exposes[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

bool has_fz_cluster(const PreparedDefinition& d, const char* cluster,
                    std::uint16_t cmd) {
    for (std::uint8_t i = 0; i < d.from_zigbee_count; ++i) {
        const FzConverter* c = d.from_zigbee[i];
        if (c != nullptr && c->cluster != nullptr &&
            std::strcmp(c->cluster, cluster) == 0 && c->command_id == cmd) {
            return true;
        }
    }
    return false;
}

bool has_tz_key(const PreparedDefinition& d, const char* key) {
    for (std::uint8_t i = 0; i < d.to_zigbee_count; ++i) {
        const TzConverter* c = d.to_zigbee[i];
        if (c != nullptr && c->key != nullptr &&
            std::strcmp(c->key, key) == 0) {
            return true;
        }
    }
    return false;
}

bool has_binding(const PreparedDefinition& d, std::uint16_t cluster_id) {
    for (std::uint8_t i = 0; i < d.bindings_count; ++i) {
        if (d.bindings[i].cluster_id == cluster_id) return true;
    }
    return false;
}

}  // namespace

int main() {
    const PreparedDefinition& d = find_zs05();

    // ── Battery channel preserved (z2m m.battery()). ─────────────────
    assert(has_expose(d, "battery"));
    assert(has_expose(d, "voltage"));

    // ── IR exposes restored (were dropped by the auto-port). ─────────
    assert(has_expose(d, "learn_ir_code"));
    assert(has_expose(d, "learned_ir_code"));
    assert(has_expose(d, "ir_code_to_send"));

    // ── All six Zosung IRTransmit fz handlers (cmd 00..05) wired. ────
    using namespace zhc::zosung;
    assert(has_fz_cluster(d, kZosungIRTransmitName, cmd::kSendIRCode00));
    assert(has_fz_cluster(d, kZosungIRTransmitName, cmd::kSendIRCode01));
    assert(has_fz_cluster(d, kZosungIRTransmitName, cmd::kSendIRCode02));
    assert(has_fz_cluster(d, kZosungIRTransmitName, cmd::kSendIRCode03));
    assert(has_fz_cluster(d, kZosungIRTransmitName, cmd::kSendIRCode04));
    assert(has_fz_cluster(d, kZosungIRTransmitName, cmd::kSendIRCode05));

    // ── Both tz handlers (outbound user writes) wired. ───────────────
    assert(d.to_zigbee_count == 2);
    assert(has_tz_key(d, "ir_code_to_send"));
    assert(has_tz_key(d, "learn_ir_code"));

    // ── Both Zosung clusters bound + battery cluster. ────────────────
    assert(has_binding(d, 0x0001));  // genPowerCfg
    assert(has_binding(d, kZosungIRTransmitCluster));  // 0xED00
    assert(has_binding(d, kZosungIRControlCluster));   // 0xE004

    // ── Manufacturer fingerprint restored (z2m _TZ3290_u9xac5rv). ────
    bool found_manu = false;
    for (std::uint8_t i = 0; i < d.manufacturer_names_count; ++i) {
        if (d.manufacturer_names[i] != nullptr &&
            std::string_view(d.manufacturer_names[i]) == "_TZ3290_u9xac5rv") {
            found_manu = true;
        }
    }
    assert(found_manu);

    return 0;
}
