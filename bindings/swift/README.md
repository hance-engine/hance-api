# Rapidly SDK — Swift / Apple

Real-time audio separation for iOS and macOS apps, via Swift Package Manager.

## Install

In Xcode: **File → Add Package Dependencies** → paste:

```
https://github.com/rapidly-labs/rapidly-sdk
```

Pick the latest version. Supports iOS 14+ and macOS 11+.

Or in a `Package.swift`-based project:

```swift
.package(url: "https://github.com/rapidly-labs/rapidly-sdk", from: "1.1.0")
```

## Use

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

See the root `README.md` for the model list and the licence model, and
`AGENTS.md` for AI-assisted integration guidance.
