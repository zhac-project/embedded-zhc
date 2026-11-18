// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/fontastic_registry.hpp"

namespace zhc::devices::fontastic {

extern const PreparedDefinition kDef_yfmeg60f;

const PreparedDefinition* const kFontasticRegistry[] = {
    &kDef_yfmeg60f,
};

const std::size_t kFontasticRegistryCount = sizeof(kFontasticRegistry) / sizeof(kFontasticRegistry[0]);

}  // namespace zhc::devices::fontastic
