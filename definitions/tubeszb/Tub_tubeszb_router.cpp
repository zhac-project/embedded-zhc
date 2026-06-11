// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tubeszb tubeszb.router — graduated from generated/.
// CC2652 Router.
//
// z2m-source: tubeszb.ts #tubeszb.router.
//   The z2m definition is a pure router: fromZigbee: [fz.linkquality_from_basic],
//   toZigbee: [], exposes: []. Its only configure() binds genBasic on
//   endpoint 8 and configures zclVersion reporting. The auto-generated Tier-1
//   port carried a PHANTOM on/off bundle: a kFzOnOff/kTzOnOff converter pair,
//   a "state" Binary expose, and a bind to genOnOff (0x0006) — none of which
//   exist in z2m. A CC2652 router is a mains-powered repeater with no
//   controllable on/off endpoint, so those were dead keys (a phantom toggle
//   in the shadow that no firmware/SPA action could ever drive). Stripped to
//   match z2m: no exposes, no fz/tz, only the genBasic bind from configure().
//   z2m fingerprints on zigbeeModel "tubeszb.router" alone (no manufacturer
//   gate), so manufacturer_names stays null.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tubeszb {
namespace {

constexpr const char* kModels_tubeszb_router[] = { "tubeszb.router" };

// z2m configure(): reporting.bind(getEndpoint(8), coordinatorEndpoint, ["genBasic"]).
constexpr BindingSpec kBind_tubeszb_router[] = {
    { 8, 0x0000 },  // genBasic on endpoint 8
};

}  // namespace

extern const PreparedDefinition kDef_tubeszb_router{
    .zigbee_models=kModels_tubeszb_router, .zigbee_models_count=sizeof(kModels_tubeszb_router)/sizeof(kModels_tubeszb_router[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="tubeszb.router", .vendor="Tubeszb",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_tubeszb_router,
    .bindings_count=sizeof(kBind_tubeszb_router)/sizeof(kBind_tubeszb_router[0]),
};

}  // namespace zhc::devices::tubeszb
