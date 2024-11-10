# Welcome to the HANCE Engine

The HANCE Engine is a model inference library built with audio in mind. A large set of pre-trained models ranging from speech noise supression and de-reverberation to stem separation and recovery of missing frequency content are available.

Integrating machine learning inference directly into audio callback functions has traditionally been a challenge. Built from scratch in cross-platform C++, the HANCE Engine enables low latency and lock-free operation, optimized for seamless audio processing.

Our models are trained specifically for real-time usage, achieving low latencies down to 20 milliseconds in speech enhancement applications. Furthermore, the models are designed to be small and resource-efficient, with model file sizes down to 242 KB for the smallest noise suppression model.

https://github.com/hance-engine/hance-api/assets/3242951/b8c06232-1633-49cf-b30e-c659ad0afb2e

## Trying out the HANCE Engine

### Using the HANCE Model Player
The easiest way to try out the HANCE Engine is to use the HANCE Model Player and load one of the models from the Models subdirectory in the repository:
- [HANCE Model Player (Mac - AU)](https://143687363.fs1.hubspotusercontent-eu1.net/hubfs/143687363/HANCEModelPlayer.component.zip)
- [HANCE Model Player (Windows 64 bit - VST3)](https://143687363.fs1.hubspotusercontent-eu1.net/hubfs/143687363/HanceModelPlayer_Win64_2_9_90.exe)

To use the AU component on Mac, copy the HANCEModelPlayer.component file to `/Library/Audio/Plug-Ins/Components`. On Windows, simply run the installer.

### Using Python
The Python API makes it easy to install and test our models in your own prototypes.

## Multiplatform
The HANCE Engine supports a wide range of platforms from embedded systems to
browser-based processing with WebAssembly. The use of vector arithmetic through
Intel IPP, Apple vDSP, or NEON intrinsics ensures maximum performance across
platforms.

- Windows 32 and 64 bit (Intel / AMD)
- Linux (Intel / AMD and ARM64)
- Mac / iOS (Intel and ARM64)
- WebAssembly (WASM)

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

Please see the online API documentation on our website:
[HANCE API Documentation](https://hance.ai/docs/welcome)
