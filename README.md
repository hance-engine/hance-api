# Welcome to HANCE 26

The HANCE Engine is a model inference library built with audio in mind. A large set of pre-trained models ranging from speech noise suppression and de-reverberation to stem separation and recovery of missing frequency content are available.

Integrating machine learning inference directly into audio callback functions has traditionally been a challenge. Built from scratch in cross-platform C++, the HANCE Engine enables low latency and lock-free operation, optimized for seamless audio processing.

Our models are trained specifically for real-time usage, achieving low latencies down to 11 milliseconds in speech enhancement applications. Furthermore, the models are designed to be small and resource-efficient, with model file sizes down to 241 KB for the smallest noise suppression model.

https://github.com/hance-engine/hance-api/assets/3242951/b8c06232-1633-49cf-b30e-c659ad0afb2e

## Trying out the HANCE Engine

### Using the HANCE Model Player

The easiest way to try out the HANCE Engine is to use the HANCE Model Player and load one of the models from the [Models](Models) subdirectory in this repository. This allows you to adjust the parameters of the model in real-time, so that you can optimize parameters for your use case.

The HANCE Model Player is available as an AudioUnit for MacOS [here](https://files.hance.ai/plugin/v26.1/HanceModelPlayer_macOS_26_1_1.pkg.zip) and as VST3 for Windows [here](https://files.hance.ai/plugin/v26.1/HanceModelPlayer_Win64_26_1_1.exe). They can be tested in the audio editing software of your choice, like [Reaper](https://www.reaper.fm/).

### Using Python
See documentation [here](PythonAPI/README.md) for instructions to test with Python.

### Models
The models in the [Models](Models) folder have semantic names. For example, [speech-denoise-32ms.v26.1.hance](Models/speech-denoise-32ms.v26.1.hance), signifies that this is a model that *denoises* speech, and has a latency of 32ms.

All models within a family, e.g., the speech-denoise family, have similar characteristics. If you have special requirements for particular audio circumstances, we offer can build models to better suit those circumstances. Contact us if this is the case.

We offer both speech noise reduction models and models that combine noise and reverb reduction for speech. The latter will output cleaned dialogue, noise and reverb in separate output busses.

The following table shows the currently available models along with CPU efficiency, file size and latencies:

| Model File Name                               | File Size | Real-time Factor* |
|-----------------------------------------------|----------:|------------------:|
| speech-denoise-dereverb-96ms.v26.1.hance      |    926 KB |               27x |
| speech-denoise-dereverb-32ms.v26.1.hance      |    854 KB |               25x |
| speech-denoise-dereverb-32ms-tiny.v26.1.hance |    241 KB |              115x |
| speech-denoise-dereverb-21ms.v26.1.hance      |    851 KB |               12x |
| speech-denoise-dereverb-11ms.v26.1.hance      |    615 KB |               12x |
| speech-denoise-96ms.v26.1.hance               |    925 KB |               29x |
| speech-denoise-32ms.v26.1.hance               |    854 KB |               27x |
| speech-denoise-32ms-tiny.v26.1.hance          |    241 KB |              125x |
| speech-denoise-21ms.v26.1.hance               |    851 KB |               13x |
| speech-denoise-11ms.v26.1.hance               |    615 KB |               13x |

\*The real-time factor is measured on a single core of an AMD RYZEN AI MAX+ 395.

Read more details about the model [here](Models/README.md).

## Multiplatform

The HANCE Engine supports a wide range of platforms from embedded systems to browser-based processing with WebAssembly. The use of vector arithmetic through Intel IPP, Apple vDSP, or NEON intrinsics ensures maximum performance across platforms.

- Windows 32 and 64 bit (Intel / AMD)
- Linux (Intel / AMD and ARM64)
- Mac / iOS (Intel and ARM64)

Learn more and listen to examples at [HANCE.ai](https://hance.ai/)

[Contact Us](https://hance.ai/contact/)

## Why use HANCE?

- Small footprint
- Light on CPU
- No GPU requirements
- Low latency
- Cross-platform
- Easy to integrate

## Documentation

Please see the online API documentation here for integrating with HANCE Engine: [https://hance-engine.github.io/hance-api/Documentation/](https://hance-engine.github.io/hance-api/Documentation/)



