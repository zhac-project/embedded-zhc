// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// exposes_from_dp_map: the expose table a generated Tuya definition gets
// when it ships only a datapoint map. Pins types, the access guess, units,
// enum labels, the fan-out flags, duplicate keys and the skipped DP types.
#include <cassert>
#include <cstdint>
#include <cstring>

#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"

using namespace zhc;

static constexpr tuya::TuyaEnumEntry kModes[] = {{0, "off"}, {1, "heat"}, {2, "auto"}};
static constexpr tuya::TuyaEnumEntry kActs[]  = {{0, "single"}, {1, "double"}};
static constexpr tuya::TuyaEnumEntry kAlarm[] = {{1, "alarm"}};

static const tuya::TuyaDpMapEntry kEntries[] = {
    tuya::dp::binary(1, "state"),
    tuya::dp::temperature(2),
    tuya::dp::numeric(3, "current_heating_setpoint", 10),
    tuya::dp::enum_lookup(4, "system_mode", kModes, 3),
    tuya::dp::binary(5, "water_leak"),
    tuya::dp::action(6, kActs, 2),
    {7, "blob", TuyaDpType::Raw, 1, nullptr, 0, 0},
    {8, "bits", TuyaDpType::Bitmap, 1, nullptr, 0, 0},
    tuya::dp::numeric(9, "humidity", 1),
    tuya::dp::numeric(10, "temperature", 10),        // second DP for a key already seen
    {11, "smoke", TuyaDpType::Enum, 1, kAlarm, 1, tuya::kTuyaDpFlagEnumBool},   // enum -> bool
    {12, "heating", TuyaDpType::Bool, 1, kModes, 2, tuya::kTuyaDpFlagBoolEnum}, // bool -> enum
    tuya::dp::numeric(13, "battery", 1),
    tuya::dp::numeric(14, "soil_temperature", 10),
    tuya::dp::numeric(15, "sensitivity", 1),
    {16, "", TuyaDpType::Bool, 1, nullptr, 0, 0},   // nameless: skipped
};

static const Expose* find(const Expose* ex, std::size_t n, const char* name) {
    for (std::size_t i = 0; i < n; i++) if (std::strcmp(ex[i].name, name) == 0) return &ex[i];
    return nullptr;
}

int main() {
    const tuya::TuyaDatapointMap map{kEntries, sizeof(kEntries) / sizeof(kEntries[0])};
    Expose ex[32];
    const char* labels[32];
    const std::size_t n = tuya::exposes_from_dp_map(map, ex, 32, labels, 32);
    // 16 rows: minus Raw, Bitmap, the duplicate temperature and the nameless one
    assert(n == 12);

    const Expose* st = find(ex, n, "state");
    assert(st && st->type == ExposeType::Binary && st->access == Access::StateSet);

    const Expose* t = find(ex, n, "temperature");
    assert(t && t->type == ExposeType::Numeric && t->access == Access::State);
    assert(t->unit && std::strcmp(t->unit, "°C") == 0);

    const Expose* sp = find(ex, n, "current_heating_setpoint");
    assert(sp && sp->access == Access::StateSet && sp->unit && std::strcmp(sp->unit, "°C") == 0);

    const Expose* m = find(ex, n, "system_mode");
    assert(m && m->type == ExposeType::Enum && m->access == Access::StateSet);
    assert(m->enum_count == 3 && std::strcmp(m->enum_values[0], "off") == 0 &&
           std::strcmp(m->enum_values[2], "auto") == 0);

    const Expose* wl = find(ex, n, "water_leak");
    assert(wl && wl->type == ExposeType::Binary && wl->access == Access::State);

    const Expose* a = find(ex, n, "action");
    assert(a && a->type == ExposeType::Enum && a->access == Access::State && a->enum_count == 2);
    assert(std::strcmp(a->enum_values[1], "double") == 0);

    assert(!find(ex, n, "blob") && !find(ex, n, "bits") && !find(ex, n, ""));

    const Expose* h = find(ex, n, "humidity");
    assert(h && h->access == Access::State && std::strcmp(h->unit, "%") == 0);

    const Expose* sm = find(ex, n, "smoke");          // EnumBool fan-out: a boolean
    assert(sm && sm->type == ExposeType::Binary && sm->access == Access::State);

    const Expose* ht = find(ex, n, "heating");        // BoolEnum fan-out: an enum
    assert(ht && ht->type == ExposeType::Enum && ht->enum_count == 2 &&
           std::strcmp(ht->enum_values[1], "heat") == 0);

    const Expose* b = find(ex, n, "battery");
    assert(b && b->category == ExposeCategory::Diagnostic && b->access == Access::State);

    const Expose* soil = find(ex, n, "soil_temperature");   // suffix rule
    assert(soil && soil->access == Access::State && std::strcmp(soil->unit, "°C") == 0);

    const Expose* sens = find(ex, n, "sensitivity");   // unknown key: writable, no unit
    assert(sens && sens->access == Access::StateSet && sens->unit == nullptr);

    // capacity: stops at `cap`, labels never overflow
    Expose few[3];
    const char* few_labels[2];
    assert(tuya::exposes_from_dp_map(map, few, 3, few_labels, 2) == 3);
    assert(std::strcmp(few[2].name, "current_heating_setpoint") == 0);
    Expose one[4];
    const std::size_t n1 = tuya::exposes_from_dp_map(map, one, 4, few_labels, 2);   // 3 labels do not fit 2
    assert(n1 == 4 && one[3].type == ExposeType::Enum && one[3].enum_count == 0);
    return 0;
}
