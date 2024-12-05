# Welcome to the HANCE Engine

The HANCE Engine is a model inference library built with audio in mind. A large set of pre-trained models ranging from speech noise supression and de-reverberation to stem separation and recovery of missing frequency content are available.

Integrating machine learning inference directly into audio callback functions has traditionally been a challenge. Built from scratch in cross-platform C++, the HANCE Engine enables low latency and lock-free operation, optimized for seamless audio processing.

Our models are trained specifically for real-time usage, achieving low latencies down to 20 milliseconds in speech enhancement applications. Furthermore, the models are designed to be small and resource-efficient, with model file sizes down to 242 KB for the smallest noise suppression model.

https://github.com/hance-engine/hance-api/assets/3242951/b8c06232-1633-49cf-b30e-c659ad0afb2e

## Trying out the HANCE Engine

### Using the HANCE Model Player
The easiest way to try out the HANCE Engine is to use the HANCE Model Player and load one of the models from the [Models](Models) subdirectory in this repository. This allows you to adjust the parameters of the model in real-time, so that you can optimize these parameters for your use case.

The HANCE model player is available for MacOS and Windows, and installers can be found [here](https://hance.ai/downloads). The model player is an audio plugin (AudioUnit for MacOS, VST3 for Windows), that can be used in the audio editing software of your choice for testing.

### Using Python
See documentation [here](PythonAPI/README.md) for instructions to test with python.


### Models
The models in the [Models](Models) folder have semantic names. For example, [speech-denoise-48kHz-32ms.hance](Models/speech-denoise-48kHz-32ms.hance), signifies that this is a model that _denoises_ speech, expects an input samplerate of 48kHz, and has a latency of 32ms. For product information, take a look at [this page](https://hance.ai/products)

## Multiplatform
The HANCE Engine supports a wide range of platforms from embedded systems to
browser-based processing with WebAssembly. The use of vector arithmetic through
Intel IPP, Apple vDSP, or NEON intrinsics ensures maximum performance across
platforms.

- Windows 32 and 64 bit (Intel / AMD)
- Linux (Intel / AMD and ARM64)
- Mac / iOS (Intel and ARM64)

Learn more and listen to examples at [HANCE.ai](https://hance.ai)

[Contact us](https://hance.ai/contact/)

## Why use HANCE?

- Small footprint
- Light on CPU
- No GPU requirements
- Low latency
- Cross-platform
- Easy to integrate

## Documentation

Please see the online API documentation here for integrating with HANCE Engine: [https://hance-engine.github.io/hance-api/Documentation/](https://hance-engine.github.io/hance-api/Documentation/)