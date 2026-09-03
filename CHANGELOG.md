# Rapidly SDK — Changelog

All notable changes to the Rapidly SDK are documented here.
Versions follow [semantic versioning](https://semver.org).

## [1.1.0] - 2026-09-03

### Added
- Additional licensing options alongside offline licence keys:
  - Subscription keys, for usage-based licensing with per-session reporting
    (bracket each end user's processing with `rapidlyStartSession` /
    `rapidlyStopSession`).
  - Activation keys, for offline-capable installs with periodic activation
    (register `rapidlySetActivationHooks` before adding the key).
- Music stem separation model family, launching with a 96 ms latency variant.
- Hardened runtime integrity protection: interference with the running engine
  results in watermarked output.

### Changed
- Model files updated to v1.1 with versioned model identifiers. v1.0 model
  files are not loadable by this release; use the v1.1 model files included
  in this distribution.

### Fixed
- An audio artifact affecting output on Apple platforms.
- A sample rate handling issue.

### Coming in a future release
- WebAssembly build.

## [1.0.0] — 2026-05-27

Initial public release.

### Added
- Cross-platform native binaries: Linux x64, Linux arm64, Windows x64, Windows x86, macOS universal (x86_64 + arm64), iOS arm64, Android arm64-v8a.
- Signed `RapidlyEngine.xcframework` bundling the macOS, iOS device, and iOS Simulator slices for Apple integrations.
- Swift Package Manager support via `Package.swift` at the distribution root (Xcode → Add Package Dependencies). Swift wrapper source mirrored at `bindings/swift/`.
- Swift-native `RapidlyEngine` class on Apple platforms (via SwiftPM).
- Kotlin / Android binding `rapidly-sdk-1.0.aar`, available as `implementation("io.rapidly:rapidly-sdk:1.0")` from Maven Central or as a downloadable artefact in the GitHub release. Kotlin wrapper source mirrored at `bindings/kotlin/`. Bundles `arm64-v8a`. `minSdk` 26.
- Kotlin-native `io.rapidly.engine.RapidlyEngine` class on Android, mirroring the Swift API surface (single- and multi-model construction, interleaved or per-channel audio I/O via direct `FloatBuffer`, processor info, parameter get/set, idempotent close).
- Speech denoise and speech denoise + dereverb model families, in 11 ms / 21 ms / 32 ms / 96 ms latency variants, plus a `micro` size variant of the 32 ms model.
- Offline licence-key verification, with per-model and per-latency entitlement enforcement at model load.
- Python bindings (`bindings/python/`), available via `pip install rapidly`.
- Native C / C++ examples: `process-file`, `raspberry-pi`.

### Coming in a future release
- WebAssembly build.

### Notes for integrators
- See `AGENTS.md` for AI-assisted integration guidance and `LICENSE` for the licence terms.
