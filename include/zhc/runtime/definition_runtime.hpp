// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include <span>

#include "zhc/runtime/definition.hpp"

namespace zhc {

// Resolve a device's Basic-cluster `modelIdentifier` to a prepared
// definition. Walks `registry` twice:
//
//   1. Exact `strcmp` against every entry's `zigbee_models`. Wins on
//      first hit — clean, unambiguous, O(N·M) worst case.
//   2. Fallback: `strstr(reg_entry_model, input_model)` against every
//      entry's `zigbee_models`. The longest entry string wins so that
//      `lumi.sensor_motion` doesn't accidentally match a definition
//      for a device whose zigbeeModel is the shorter prefix.
//
// Returns nullptr when no definition matches.
//
// Thread-safety: pure function, no state. Caller owns the registry.
const PreparedDefinition* find_definition_by_model(
    const char* model_id,
    std::span<const PreparedDefinition* const> registry);

// Extended resolver that also consults `manufacturer_name`.
//
// Match rules, in order:
//   1. zigbee_models exact-match on `model_id`. Skip on miss.
//   2. If the candidate has neither `manufacturer_name_prefix` nor
//      `manufacturer_names`, the zigbeeModel match alone wins.
//   3. If `manufacturer_names_count > 0`, `manufacturer_name` must
//      appear verbatim in the list.
//   4. Else if `manufacturer_name_prefix` is set, `manufacturer_name`
//      must begin with that prefix.
//
// Returns the first candidate that passes. When `manufacturer_name` is
// null, Tuya-discriminated entries are skipped. Falls back to
// `find_definition_by_model` if no Tuya-style entry matches.
const PreparedDefinition* find_definition(
    const char* model_id,
    const char* manufacturer_name,
    std::span<const PreparedDefinition* const> registry);

}  // namespace zhc
