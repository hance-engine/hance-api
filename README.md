# Rapidly SDK

The Rapidly SDK is a set of AI models for audio separation, built to process audio on-device and in realtime. By [Rapidly Labs AS](https://rapidly.io).

A lean, cross-platform C++ library built for low latency and small footprint. Customer applications embed the engine binary and load Rapidly's pre-trained models to add real-time audio separation (speech denoise, dereverb, and more) to phone calls, broadcast audio, voice assistants, and other audio pipelines.

## Why use Rapidly?

- Small footprint
- Light on CPU
- No GPU requirements
- Low latency
- Cross-platform
- Easy to integrate

## Install

| Language | Instructions |
|---|---|
| C / C++ | Embed a binary from `bin/<platform>/` and include `include/RapidlyEngine.h`. See `examples/process-file/` for a CLI walkthrough. |
| Swift / iOS / macOS | In Xcode: **File → Add Package Dependencies** → paste `https://github.com/rapidly-labs/rapidly-sdk`, then `import RapidlyEngine` for the Swift-native `RapidlyEngine` class. Or drag `bin/RapidlyEngine.xcframework` into your Xcode project's Frameworks section and `import RapidlyEngineC` for the C API. |
| Kotlin / Android | In Gradle: `implementation("io.rapidly:rapidly-sdk:1.1.0")` from Maven Central, then `import io.rapidly.engine.RapidlyEngine` for the Kotlin-native class. Or download `rapidly-sdk-1.1.0.aar` from the GitHub release into your app's `libs/` and add `implementation(files("libs/rapidly-sdk-1.1.0.aar"))`. Bundles `arm64-v8a`. `minSdk` 26. |
| Python | `pip install rapidly` (PyPI). See `bindings/python/README.md`. |

Documentation: <https://rapidly.io/docs>

## Quickstart

### C / C++

```c
#include "RapidlyEngine.h"

rapidlyAddLicense("lk_...");                       // licence first
auto p = rapidlyCreateProcessor(
    "models/speech-denoise-32ms.v1.1.rapidly",     // model file
    2,                                             // channels
    48000                                          // sample rate
);
rapidlyAddAudioInterleaved(p, input,  numSamples);
rapidlyGetAudioInterleaved(p, output, numSamples);
```

### Swift (iOS / macOS)

```swift
import RapidlyEngine

RapidlyEngine.addLicense("lk_...")               // licence first
let engine = RapidlyEngine(
    modelFilepath: "models/speech-denoise-32ms.v1.1.rapidly",
    numOfChannels: 2,
    sampleRate: 48000
)
engine?.addAudioInterleaved(pcmChannels: inputBuffer, numOfSamples: numSamples)
engine?.getAudioInterleaved(pcmChannels: outputBuffer, numOfSamples: numSamples)
```

### Python

```python
import rapidly

engine = rapidly.RapidlyEngine()
engine.add_license("lk_...")
processor = engine.create_processor("models/speech-denoise-32ms.v1.1.rapidly", 2, 48000)
# feed audio in / out via processor methods
```

For AI-assisted integration, see [`AGENTS.md`](AGENTS.md).

## What's in this distribution

| Folder | Contents |
|---|---|
| `bin/` | Native binaries, one folder per platform, plus `RapidlyEngine.xcframework` for Apple |
| `include/` | Single public header `RapidlyEngine.h` |
| `models/` | Pre-trained Rapidly model files (`.rapidly`) |
| `bindings/swift/` | Swift wrapper source (canonical install: SwiftPM via `Package.swift` at repo root) |
| `bindings/kotlin/` | Kotlin wrapper source (canonical install: `implementation("io.rapidly:rapidly-sdk:1.1.0")` from Maven Central) |
| `bindings/python/` | Python wrapper (`pip install rapidly`) |
| `examples/` | Working integration demos: `process-file`, `raspberry-pi` |
| `Package.swift` | Swift Package Manager manifest (Apple platforms) |
| `LICENSE` | Commercial licence terms |
| `CHANGELOG.md` | Per-version release notes |
| `AGENTS.md` | AI agent integration guide |

## Models

The SDK ships with three model families.

**Speech denoise** removes background noise from speech and outputs cleaned dialogue and noise as separate busses.

**Speech denoise + dereverb** removes both background noise and room reverb, and outputs cleaned dialogue, reverb, and noise as separate busses. Use this when the input has significant room reverb that you want to suppress (or capture as a separate bus).

**Music stem separation** (new in v1.1) splits a music mix into vocals, bass, guitar, drums, piano, and residual busses in real time, for remixing, karaoke, practice, and creative audio applications.

The two speech families ship at four latencies, plus a `micro` size variant of the 32 ms model for CPU-constrained scenarios; the music family ships at 96 ms. The size variant (`micro`) appears between the family name and the latency in the filename — e.g. `speech-denoise-micro-32ms.v1.1.rapidly`.

| Model file | Latency | File size | Real-time factor* | Output busses |
|---|---|---|---|---|
| `speech-denoise-11ms.v1.1.rapidly` | 11 ms | 615 KB | 13x | Dialogue, Noise |
| `speech-denoise-21ms.v1.1.rapidly` | 21 ms | 851 KB | 13x | Dialogue, Noise |
| `speech-denoise-32ms.v1.1.rapidly` | 32 ms | 854 KB | 27x | Dialogue, Noise |
| `speech-denoise-micro-32ms.v1.1.rapidly` | 32 ms (compact) | 241 KB | 125x | Dialogue, Noise |
| `speech-denoise-96ms.v1.1.rapidly` | 96 ms | 925 KB | 29x | Dialogue, Noise |
| `speech-denoise-dereverb-11ms.v1.1.rapidly` | 11 ms | 615 KB | 12x | Dialogue, Reverb, Noise |
| `speech-denoise-dereverb-21ms.v1.1.rapidly` | 21 ms | 851 KB | 12x | Dialogue, Reverb, Noise |
| `speech-denoise-dereverb-32ms.v1.1.rapidly` | 32 ms | 854 KB | 25x | Dialogue, Reverb, Noise |
| `speech-denoise-dereverb-micro-32ms.v1.1.rapidly` | 32 ms (compact) | 241 KB | 115x | Dialogue, Reverb, Noise |
| `speech-denoise-dereverb-96ms.v1.1.rapidly` | 96 ms | 926 KB | 27x | Dialogue, Reverb, Noise |
| `music-stem-separation-96ms.v1.1.rapidly` | 96 ms | 1468 KB | not yet published | vocals, bass, guitar, drums, piano, residual |

*Real-time factor measured on a single core of an AMD RYZEN AI MAX+ 395. The music model's figure will be published once benchmarked.

All models are trained on 48 kHz audio, but the engine accepts other sample rates. Bus names are exactly as reported by `rapidlyGetOutputBusName`.

### Picking a model

Choose by latency budget:

* **11 ms or 21 ms**: live communication, conferencing, real-time monitoring. Lowest delay; trades off some suppression strength.
* **32 ms**: balanced choice for most production use. Strong noise / reverb reduction with moderate latency.
* **`micro` 32 ms**: same latency as 32 ms but a compact 241 KB file with a 4-5x higher real-time factor. Choose this when you need to run many simultaneous streams per CPU.
* **96 ms**: highest fidelity. Best speech clarity; suitable for recording, post-production, and offline processing where latency does not matter.

Choose between `speech-denoise` and `speech-denoise-dereverb` by whether you want room reverb removed (or exposed as a separate bus). The dereverb family is a good fit for desktop microphones in untreated rooms and for archive cleanup; the plain denoise family preserves the room's natural reverb.

## Licences

Every model loads and runs with or without a key. A model not covered by a valid key (or any model used with no key) runs in a watermarked demo mode that audibly degrades its output. Rapidly issues three kinds of key, all registered with `rapidlyAddLicense`:

- **Licence keys** (`lk_...`) are verified on the device and need no network. They encode which models and latency variants your application may use.
- **Subscription keys** (`sk_...`) are for usage-based licensing. The SDK talks to Rapidly's production API for licensing and billing. Bracket each end user's processing with `rapidlyStartSession` and `rapidlyStopSession` so usage is attributed correctly. Keep the key on your own backend and let the SDK, running on the device, obtain short-lived tokens from it (`rapidlySetTokenProvider`).
- **Activation keys** (`ak_...`) are for installs that are fully offline, except when the SDK needs internet to activate or to renew the key for another period. Register storage hooks with `rapidlySetActivationHooks` before adding the key.

Audio never leaves the device with any key type.

For commercial licensing enquiries: <sales@rapidly.io>
For technical support and licence issuance: <support@rapidly.io>

## Supported platforms

Linux x64, Linux arm64, Windows x64, Windows x86, macOS (universal), iOS, Android (arm64-v8a).

WebAssembly is coming in a future release. See `CHANGELOG.md` for the full v1.1 scope.
