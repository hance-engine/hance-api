# Rapidly SDK — Kotlin / Android

Real-time audio separation for Android apps, via Maven Central.

## Install

In `app/build.gradle.kts`:

```kotlin
dependencies {
    implementation("io.rapidly:rapidly-sdk:1.1.0")
}
```

`minSdk` 26 (Android 8.0). Bundles `arm64-v8a`. The `.aar` is also attached
as a downloadable artefact in the GitHub Release for `libs/`-based installs:

```kotlin
dependencies {
    implementation(files("libs/rapidly-sdk-1.1.0.aar"))
}
```

## Use

```kotlin
import io.rapidly.engine.RapidlyEngine

RapidlyEngine.addLicense("lk_...")                            // licence first
val engine = RapidlyEngine.create(
    modelFilepath = modelPathOnDisk,                          // copy from assets to cacheDir first
    numOfChannels = 2,
    sampleRate = 48000.0,
)
engine?.addAudioInterleaved(inputBuffer, numSamples)
engine?.getAudioInterleaved(outputBuffer, numSamples)
engine?.close()
```

Audio buffers must be direct
(`ByteBuffer.allocateDirect(n * 4).order(ByteOrder.nativeOrder()).asFloatBuffer()`)
for the zero-copy JNI path. The model path must be a filesystem path; ship
the `.rapidly` model in `assets/` and copy it to `context.cacheDir` on
first use.

See the root `README.md` for the model list and the licence model, and
`AGENTS.md` for AI-assisted integration guidance.
