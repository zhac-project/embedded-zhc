# Third-party libraries

## JSON reader

**Shipped:** in-tree minimal JSON reader at `embedded-zhc/tools/json_mini.hpp` (~150 LOC, library-private, test-harness only).

**Why not vendored `nlohmann/json`:** the reference spec + plan called for the single-header nlohmann drop, but the bootstrap environment had no copy available and network fetch is not reliable here. A focused in-tree reader covers the fixture format (objects, arrays, strings, integers, booleans, null) without pulling 24k lines of upstream code into a test-only build target.

**Swap path:** the reader is header-only inside `tools/` and used only by `tools/fixture_runner.cpp`. To swap in `nlohmann/json`:

1. Drop `json.hpp` here as `embedded-zhc/third_party/json.hpp`.
2. Rewrite the ~30-line fixture loader in `tools/fixture_runner.cpp` against the nlohmann API.
3. Delete `tools/json_mini.hpp`.

No public headers or tests depend on the reader's identity.
