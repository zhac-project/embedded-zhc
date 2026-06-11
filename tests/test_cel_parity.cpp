// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the CEL (California Eastern Labs / Cortet) family.
// Pins the gap the auto-generated port carried against the z2m wire shape:
//
//   * CGW-Z-0010 ("Z10", Cortet range extender) is a pure router: z2m
//     wires fromZigbee:[fz.linkquality_from_basic] (linkquality is
//     stack-level metadata, not a device expose), toZigbee:[], exposes:[].
//     The auto-port mis-ported it as a controllable on/off switch
//     (kFzOnOff + kTzOnOff + phantom `state` expose + 0x0006 OnOff bind)
//     — a dead control: the extender never speaks genOnOff. Fixed to the
//     canonical router shape: no converters, no exposes, no bindings.
//
// z2m-source: zigbee-herdsman-converters/src/devices/cel.ts.

#include <cassert>
#include <cstddef>
#include <span>

#include "zhc/devices/cel_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> cel_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::cel::kCelRegistry,
        devices::cel::kCelRegistryCount);
}

const PreparedDefinition* range_extender() {
    const auto* d = find_definition("Z10", nullptr, cel_registry());
    assert(d && "Z10 must resolve to the CGW-Z-0010 range extender def");
    return d;
}

}  // namespace

// ── CGW-Z-0010: pure router, no phantom on/off control ──────────────
static void test_range_extender_is_router() {
    const auto* d = range_extender();
    // z2m exposes:[] / toZigbee:[] / fromZigbee only reads linkquality —
    // the phantom on/off bundle (state expose + write path + decoder +
    // OnOff bind) must be gone.
    assert(d->exposes_count == 0);
    assert(d->from_zigbee_count == 0);
    assert(d->to_zigbee_count == 0);
    assert(d->bindings_count == 0);
}

int main() {
    test_range_extender_is_router();
    return 0;
}
