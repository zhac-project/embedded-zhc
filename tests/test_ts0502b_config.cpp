// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Port-gap test for TS0502B (Tuya CCT light). z2m's `TS0502B` ships a
// `configure:` (tuyaLight `configureReporting:true` + magic packet); the
// ZHC parent def previously had `config_steps=nullptr`. This asserts the
// configure pipeline is now wired on BOTH registrations and drives the
// three light clusters plus the genBasic magic-packet read.

// Keep assert() live even under -DNDEBUG (Release).
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdint>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0502B;
extern const PreparedDefinition kDefTS0502B_v2;
}  // namespace zhc::devices::tuya

using namespace zhc;

namespace {

// True if the def has a Read step targeting `cluster` on endpoint 1.
bool has_read_on_cluster(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::uint8_t i = 0; i < def.config_steps_count; ++i) {
        const ConfigStep& s = def.config_steps[i];
        if (s.op == ConfigStepOp::Read && s.cluster_id == cluster) return true;
    }
    return false;
}

bool has_wait(const PreparedDefinition& def) {
    for (std::uint8_t i = 0; i < def.config_steps_count; ++i) {
        if (def.config_steps[i].op == ConfigStepOp::Wait) return true;
    }
    return false;
}

void check_def(const PreparedDefinition& def) {
    // config_steps must now be present.
    assert(def.config_steps != nullptr);
    assert(def.config_steps_count >= 4);

    // Settle wait before any radio traffic.
    assert(has_wait(def));

    // Magic packet — Read on genBasic (0x0000).
    assert(has_read_on_cluster(def, 0x0000));

    // Reporting kick on the three CCT-light clusters (binds already in
    // kAutoBindings; these Reads start the device streaming).
    assert(has_read_on_cluster(def, 0x0006));   // genOnOff
    assert(has_read_on_cluster(def, 0x0008));   // genLevelCtrl
    assert(has_read_on_cluster(def, 0x0300));   // lightingColorCtrl

    // The binds themselves stay declared in bindings[].
    assert(def.bindings != nullptr);
    bool bind_onoff = false, bind_level = false, bind_color = false;
    for (std::uint8_t i = 0; i < def.bindings_count; ++i) {
        switch (def.bindings[i].cluster_id) {
            case 0x0006: bind_onoff = true; break;
            case 0x0008: bind_level = true; break;
            case 0x0300: bind_color = true; break;
            default: break;
        }
    }
    assert(bind_onoff && bind_level && bind_color);
}

}  // namespace

int main() {
    check_def(devices::tuya::kDefTS0502B);
    check_def(devices::tuya::kDefTS0502B_v2);
    return 0;
}
