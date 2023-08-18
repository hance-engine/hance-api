<a href="https://hance.ai">
    <img src="https://hance.ai/images/hance-logo-branding-color.svg" alt="Aimeos logo" title="Aimeos" align="right" height="40" />
</a>

# Welcome to the HANCE APIs

HANCE offers top-quality signal-processing techniques developed by machine learning specialists, sound engineers, and audio processing experts. Our technology is designed to provide users with the highest possible audio experience by removing noise, reverb, and other signal impairments. HANCE can also provide sound classification and signal recovery – all in realtime. 

https://github.com/hance-engine/hance-api/assets/3242951/b8c06232-1633-49cf-b30e-c659ad0afb2e

The HANCE APIs are designed to make our cutting-edge algorithms accessible to everyone, regardless of their device or application. Our highly trainable and adaptable Web, Python, and C++ APIs, with more platforms to come, give developers all they need to get started. With CPU and memory efficiency in mind, the HANCE APIs allow easy access to our revolutionary audio algorithms without sacrificing performance.

The HANCE algorithms are built on a unique combination of deep learning and evolutionary computing techniques. This enables our systems to learn quickly, accurately, and efficiently from auditory data.

Learn more and listen to examples at [HANCE.ai](https://hance.ai)

[Contact us](https://hance.ai/contact/)

## Why use HANCE?

- Small footprint
- Light on CPU
- No GPU requirements
- Low latency
- Cross-platform
- Easy to integrate

<!-- 
<details>
      <summary>
<h2>Listen to the Amazing Examples</h2>
    </summary>
- Example 1
- Example 2
- Example 3

</details>
+ -->

<details>
    <summary>
<h1>HANCE Python API</h1>
    </summary>

HANCE is a powerful audio enhancement engine that can drastically improve the quality of audio signals in your Python projects. This document explains how to install and use the HANCE Python wrapper.

To learn more about HANCE and its capabilities, visit [HANCE.ai](https://hance.ai).

## Installation

Installing the HANCE Python wrapper is easy:

* Use _pip_, the standard Python package installer. 
* Run the command in the example below from the _terminal_ on macOS or the _command line_ on Windows. 

> 
> Note that HANCE is compatible with Python 3 and later. If your Python version is older than Python 3, update it to use the HANCE Python API.

### Install HANCE with Pip

```bash
python -m pip install hance
```

## How to use

The HANCE Python API is essentially a wrapper around the underlying C++ library. To use the API, you can import it and then list the available models:

### Importing HANCE

```python
import hance
models = hance.list_models()
print(models)
```

> 
> Use the `list_models` function to get a list of available models you can use with HANCE, such as noise reduction and reverb removal models.

## Basic audio processing
To process an audio file using HANCE, use the `process_file` function, as shown below.

> 
> In this example, we use _PySoundFile_ to read and write audio files. While _PySoundFile_ is not a requirement for using HANCE, it is a convenient library for handling audio files in Python. If you wish to use the process_file function shown in the example, you must install PySoundFile by running the command `python -m pip install soundfile`

### File processing
```python
import hance
models = hance.list_models()
hance.process_file(models[0], input_file_path, output_file_path)
```


The HANCE engine works without a license but it will leave a watermark on the sound. You can obtain a license for the HANCE engine [here](https://hance.ai/contact).
This is an example of processing a file using a license.

### Using with a license
```python
import hance

license_string = "example_license"

models = hance.list_models()
hance.process_file(models[0], input_file_path, output_file_path,
                    license_string=license_string)
```


## Realtime processing

In addition to processing audio files, HANCE can also be used on realtime audio streams. The following example demonstrates how to use HANCE with PyAudio to record audio from a microphone, process it in realtime, and output it to headphones. At the very bottom you can find a minimal example of this with only the HANCE related parts.


### Complete Python example of realtime processing

```python
 # We capture the input of your microphone and process it in real-time. 
 # We then send it to the output device.

import threading
import numpy as np
import hance
import pyaudio

engine = hance.HanceEngine()
p = pyaudio.PyAudio()

FORMAT = pyaudio.paFloat32
CHANNELS = 1
RATE = 44100
CHUNK = 512

print("\nRecord audio from a microphone and process it in realtime with HANCE.")
print("PyAudio will induce some latency with the roundtrip to the soundcard,")
print("but the HANCE engine runs in realtime.\n")

# We get a list of available input devices.
input_devices = []
for i in range(p.get_device_count()):
    device_info = p.get_device_info_by_index(i)
    if device_info["maxInputChannels"] > 0:
        input_devices.append(device_info)

# Print the list of available input devices and ask the user to select one.
print("Available input devices:")
for i, device in enumerate(input_devices):
    print(f"{i}: {device['name']}")
input_device_index = int(input("\nSelect an input device by entering its number: "))
input_device_info = input_devices[input_device_index]

# We get a list of available output devices.
output_devices = []
for i in range(p.get_device_count()):
    device_info = p.get_device_info_by_index(i)
    if device_info["maxOutputChannels"] > 0:
        output_devices.append(device_info)

# We print the list of available input devices and ask the user to select one.
print("\nAvailable output devices:")
print("To prevent feedback, select a headphones output.\n")

for i, device in enumerate(output_devices):
    print(f"{i}: {device['name']}")
output_device_index = int(input("\nSelect an output device by entering its number: "))
output_device_info = output_devices[output_device_index]

models = hance.list_models()
processor = engine.create_processor(models[0], CHANNELS, RATE)

stop_thread = False
processor_active = True

def record_and_playback_thread():
    stream_record = p.open(format=FORMAT, channels=CHANNELS,
                           rate=RATE, input=True,
                           input_device_index=input_device_info['index'],
                           frames_per_buffer=CHUNK)

    stream_play = p.open(format=pyaudio.paFloat32,
                         channels=1,
                         rate=RATE,
                         frames_per_buffer=CHUNK,
                         output=True,
                         output_device_index=output_device_info['index']
                         )
    while not stop_thread:
        data = stream_record.read(CHUNK, exception_on_overflow=False)
        audio_buffer = np.frombuffer(data, dtype=np.float32)
        if processor_active:
            audio_buffer = processor.process(audio_buffer)
        stream_play.write(audio_buffer.astype(np.float32).tobytes())

    # We stop recording.
    stream_record.stop_stream()
    stream_record.close()

    stream_play.stop_stream()
    stream_play.close()

t = threading.Thread(target=record_and_playback_thread)
t.start()

print("\nThe microphone and processing is active")
while True:
    user_input = input("Enter 'p' to toggle processing on and off or 'q' to quit: ")
    if user_input.lower() == "p":
        # Bypass processing and continue the loop
        if processor_active:
            processor_active = False
            print("The processing is bypassed")
        else:
            processor_active = True
            print("The processing is active")
    elif user_input.lower() == "q":
        # Stop the thread
        stop_thread = True
        break

t.join
```

Here is a minimal example of the HANCE-related code for you to look at:

> 
> Please note that this example does not run, as the PCM_AUDIO_DATA is not set, and is only used to demonstrate the usage of the API.

### Minimal Python example

```python
import hance
import numpy as np

engine = hance.HanceEngine()
models = hance.list_models()
processor = engine.create_processor(models[0], CHANNELS, RATE)
audio_buffer = np.frombuffer(PCM_AUDIO_DATA, dtype=np.float32)
audio_buffer = processor.process(audio_buffer)
```

The HANCE Python wrapper is a powerful tool for audio processing and enhancement. With the ability to remove noise, decrease reverb, and be highly CPU and memory-efficient, HANCE stands at the cutting edge of audio enhancement technology. 

We hope this guide has helped get you started with the HANCE Python wrapper, and we encourage you to visit the [HANCE ](https://hance.ai) website to learn more about HANCE and its capabilities.
</details>

<details>
<summary>
<h1>HANCE C API</h1>
</summary>

The C interface for the HANCE Audio Engine provides developers with simple access to HANCE's powerful algorithms and processing capabilities from all languages that offer bindings for standard C-compatible libraries. The HANCE Audio Engine is a light-weight and cross-platform library, and it should be very easy to integrate it into your application. The library can load pre-trained AI models and use these for audio processing to perform various tasks such as noise reduction and de-reverberation.

> 
> The HANCE Audio Engine is delivered with general purpose models for
> noise reduction and de-reverberation. These are designed to meet common 
> requirements in terms of latency and CPU usage. However, we can train custom
> models for lower latencies or less CPU usage at the cost of separation
> quality. Please [contact us](https://hance.ai/contact) for more information.

## Getting Started

The HANCE API is designed to be as simple as possible. The **ProcessFile** example (see the **Examples** folder in the API) illustrates how to create a HANCE processor and process audio with it. [CMake 3.0](https://cmake.org/) or later is required to build the example. To build **ProcessFile**, open the Terminal (on Mac and Linux) or the Command Line Prompt (on Windows) and locate the **Examples/ProcessFile** subfolder in the HANCE API. Please type ```./Build.sh``` on Mac or Linux, or ```Build.bat``` on Windows.

To use the HANCE API, we first need to make sure the "Include" in the HANCE API is added to the header search path and include the HanceEngine header file:

### Include the HANCE Engine Header File
```c++
#include "HanceEngine.h"
```

Now we can create an instance of a HANCE processor by specifying a path to a HANCE model file along with the number of channels and sample rate to use:

### Creating a HANCE Processor Instance
```c++
// Create a HANCE processor that loads the pre-trained model from file
processorHandle = hanceCreateProcessor (modelFilePath, numOfChannels, sampleRate);
if (processorHandle == nullptr)
    handleError ("Unable to create the HANCE audio processor.");
```

The returned processor handle will be **nullptr** if the processor couldn't be created, e.g., because the model file path is invalid.

Now, we can add audio in floating point format to the HANCE processor. The HANCE API supports audio stored either as channel interleaved audio (hanceAddAudioInterleaved and hanceGetAudioInterleaved) or as separate channel vectors (hanceAddAudio and hanceGetAudio). We demonstrate how to add audio from a std::vector containing channel interleaved float values below:

### Adding Channel Interleaved Audio to a HANCE Processor
```c++
// We read PCM audio from the file in the 32-bit floating point format
hanceAddAudioInterleaved (processorHandle, audioBuffer.data(),
                          audioBuffer.size() / numOfChannels);
```

The processing introduces latency, so we need to query how many samples (if any) that are available before we can pick up the processed audio using hanceGetAudioInterleaved:

### Getting Processed Audio from a HANCE Processor
```c++
int numOfPendingSamples = hanceGetNumOfPendingSamples (processorHandle);
vector <float> processedBuffer (numOfChannels * numOfPendingSamples);
if (!hanceGetAudioInterleaved (processorHandle,
                               processedBuffer.data(),
                               numOfPendingSamples))
{
    handleError ("Unable to get audio from the HANCE audio processor.");
}
```

You can add silent audio (all values set to zero) to get the processing tail caused by the model's latency. When you have completed the processing of the audio stream, please make sure to delete the HANCE processor to free its memory:

### Deleting the HANCE Processor and Free Memory
```c++
hanceDeleteProcessor (processorHandle);
```

## Performance Considerations

The HANCE Audio Engine is a light-weight and cross-platform library, and it uses either of the following libraries for vector arithmetic if available:

* Intel Performance Primitives
* Apple vDSP

> 
> The HANCE Audio Engine reverts to pure C++ when no compatible vector arithmetic library is available.

## Datatypes

### HanceProcessorHandle
```c++
typedef void* HanceProcessorHandle;
```
The processor handle refers to an audio processor that can process PCM audio in 32-bit floating point using a specified inference model. The handle created using [hanceCreateProcessor](#hanceCreateProcessor).

### HanceProcessorInfo
```c++
struct HanceProcessorInfo
{
    double sampleRate;
    int32_t numOfModelChannels;
    int32_t latencyInSamples;
};
```

The HanceProcessorInfo contains information about a neural network based audio processor.

|Return Type|Member name|Description|
|-----------|-----------|-----------|
|double|sampleRate|Sample rate used when model was trained. The processor will automatically convert sample rates to match the model.|
|int32_t|numOfModelChannels|The true number of channels used in the processing. The processor will automatically convert the channel format to match the model.|
|int32_t|latencyInSamples|The maximum latency of the model in samples.|

## Functions

### hanceAddAudio
```c++
void hanceAddAudio (HanceProcessorHandle processorHandle,
                    const float **pcmChannels,
                    int32_t numOfSamples)
```

Adds floating point PCM encoded audio from separate channels to the HANCE audio processor.

|Parameters| Description |
|--------------|-|
|processorHandle|Handle to the audio processor.|
|pcmChannels|Pointer to an array of channel data pointers, each pointing to sampled PCM values as 32-bit floating point.|
|numOfSamples|The number of samples to add|

### hanceAddAudioInterleaved
```c++
void hanceAddAudioInterleaved (HanceProcessorHandle processorHandle,
                               const float* interleavedPCM,
                               int32_t numOfSamples)
```

Adds floating point PCM encoded audio from a single channel-interleaved buffer to the HANCE audio processor.

|Parameters|Description|
|--------------|-|
|processorHandle|Handle to the audio processor.|
|interleavedPCM|Pointer to a 32-bit floating point buffer containing channel-interleaved PCM audio (stereo audio will be in the form "Left Sample 1", "Right Sample 1", "Left Sample 2"...).|
|numOfSamples|The number of samples to add|

### hanceAddLicense
```c++
bool hanceAddLicense (const char* licenseString)
```

Adds a license key to the HANCE engine to remove audio watermarking on the output.

|Parameters|Description |
|--------------|-|
|licenseString|A string containing a license received from HANCE.|
|**Returns**|True, if the license check succeeded, otherwise false.|

### hanceCreateProcessor
```c++
HanceProcessorHandle hanceCreateProcessor (const char* modelFilepath,
                                           int32_t numOfChannels,
                                           double sampleRate)
```

Creates an audio processor, loads a model file and returns a handle to the processor instance if successful.

|Parameters|Description |
|--------------|-|
|modelFilepath|Pointer to a zero terminated string containing the file path of the model file to load.|
|numOfChannels|The number of channels in the audio to process.|
|sampleRate|The sample rate of the audio to process.|
|**Returns**|A valid processor handle on success, otherwise nullptr.|

### hanceDeleteProcessor
```c++
void hanceDeleteProcessor (HanceProcessorHandle processorHandle)
```

Deletes a processor instance.

|Parameters|Description |
|--------------|-|
|processorHandle|Handle to the processor to delete.|

### hanceGetAudio
```c++
bool hanceGetAudio (HanceProcessorHandle processorHandle,
                    float* const* pcmChannels,
                    int32_t numOfSamples)
```

Gets floating point PCM encoded audio in separate channels after processing. The number of requested samples must be less or equal to the number of available samples as returned by /ref hanceGetNumOfPendingSamples.

|Parameters|Description |
|--------------|-|
|processorHandle|Handle to the audio processor.|
|pcmChannels|Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.|
|numOfSamples|The number of samples to retrieve|

### hanceGetAudioInterleaved
```c++
bool hanceGetAudioInterleaved (HanceProcessorHandle processorHandle,
                               float* interleavedPCM,
                               int32_t	numOfSamples)
```

Gets floating point PCM encoded audio in a single channel-interleaved buffer after processing. The number of requested samples must be less or equal to the number of available samples as returned by /ref hanceGetNumOfPendingSamples.

|Parameters|Description|
|--------------|-|
|processorHandle|Handle to the audio processor.|
|interleavedPCM|Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.|
|numOfSamples|The number of samples to retrieve|

### hanceGetNumOfPendingSamples
```c++
int32_t hanceGetNumOfPendingSamples (HanceProcessorHandle processorHandle)
```

Returns the number of samples that are ready after model inference. If the end of the stream has been reached, endOfStream can can be set to true to retrieve the number of remaining samples in the processing queue.

|Parameters|Description|
|--------------|-|
|processorHandle|Handle to the audio processor.|
|**Returns**|Number of completed samples.|

### hanceGetProcessorInfo
```c++
void hanceGetProcessorInfo (HanceProcessorHandle processorHandle,
                            HanceProcessorInfo* processorInfo)
```

Fills a the [HanceProcessorInfo](#HanceProcessorInfo) structure with information about the HANCE processor.

|Parameters| Description|
|--------------|-|
|processorHandle|Handle to the audio processor.|
|processorInfo|Pointer to a HanceProcessorInfo struct that will receive the model information.|

### hanceResetProcessorState
```c++
void hanceResetProcessorState (HanceProcessorHandle processorHandle)
```

Resets the processor state and clears all delay lines.

|Parameters|Description |
|--------------|-|
|processorHandle|Handle to the audio processor.|

</details>

<details>
  <summary>
    <h1>HANCE Web API</h1>
  </summary>


The HANCE Web API provides developers with a simple, programmatic interface to access HANCE's powerful algorithms and processing capabilities. By making HTTP(S) requests to the HANCE Web API, developers can process audio files, explore the various [processing modules](#get-models) HANCE offers, and find the best configuration for their needs.

The HANCE Web API offers developers a taste of the impressive results generated by the HANCE Audio Engine. However, it should be noted that this API does not demonstrate realtime processing. This is because files must be uploaded to the HANCE servers, processed, and downloaded to the user's browser.

We recommend reviewing our [C++](https://hance.ai/api/c) and [Python](https://hance.ai/api/python) API documentation or [contacting us directly](https://hance.ai/contact) if you are interested in exploring realtime audio processing or training custom models.

This document provides a comprehensive guide to the HANCE Web API, with endpoints, parameters, options, and JavaScript (ES6) code examples. It is designed to make learning the HANCE Web API quick, easy and fun.

> 
> Using the HANCE Web API without an API key will limit the processing time for each file to 30 seconds, and the returned file format is limited to 128kbps mp3. You can purchase an API key from [HANCE](https://hance.ai/contact) to remove these limitations and access additional features. With an API key, you can process files for longer periods of time and receive the file in more formats.

## Get Upload URL

To start uploading files from your code, request an upload **ID** and **URL** from the server. These will securely handle the file and ensure a successful upload.

> 
> **Important HANCE Web API endpoint A**<br>
> https://server.hance.ai/api/get_upload_url

### Get Upload URL: JavaScript Code Example

```javascript
// First, we store the address from "Important endpoint A" as a constant:
const URL_ID_ENDPOINT =
  "https://server.hance.ai/api/get_upload_url"

// Using ES6 syntax, this one-liner downloads and converts our data from JSON
// to native JavaScript objects.

const getData = async () => await (await fetch(URL_ID_ENDPOINT)).json()

getData()
  .then(({ upload_url, upload_id }) => {
    // Once the data is available, the .then() method will be called,
    // providing access to the upload_url and upload_id values. Here we would
    // typically place an upload function, passing in the now available data.

    // Do something here...

    // We are also returning the values so that they can be used in the next
    // steps.
    return { upload_url, upload_id }
  })
  .catch(console.error)
```

### Get upload URL: query parameters

| Field  | Type   | Description                                                                                                         |
| ------ | ------ | ------------------------------------------------------------------------------------------------------------------- |
| origin | String | Use the **origin** parameter with your website URL to handle potential CORS issues during web browser file uploads. |

> 
> CORS is a security feature that blocks web pages from making requests to a different domain than the one that served the web page. By specifying the origin parameter, the API server can validate that the request is coming from an authorized domain and allow the request to proceed. If the upload process fails later, this might be the cause, so please come back and apply an **origin** query parameter.

### Get upload URL: setting the origin

```javascript
// Alternative 1: Getting the origin from the current browser URL.
let origin = window.location.origin

// Alternative 2: Manually setting the origin.
let origin = "https://example.com"

// Add the origin defined in alternative 1 or 2 as a query parameter to the
// endpoint, using the encodeURI method for compatibility when used as a
// query parameter.
const URL_ID_ENDPOINT = `https://server.hance.ai/api/get_upload_url?origin=${encodeURI(origin)}`
```

### Get upload URL: return values

| Field      | Type   | Description                                                               |
| ---------- | ------ | ------------------------------------------------------------------------- |
| upload_url | String | The URL used for uploading the file to be processed by the API.           |
| upload_id  | String | A unique identifier that allows the API to identify and process the file. |

## Get models

The **get_models endpoint** provides a list of models you can select to process uploaded audio files. Each model addresses a certain audio-related problem, such as noise reduction, reverb attenuation, signal recovery and so forth.

> 
> Hance specializes in creating lightweight, effective models optimized for all kinds of realtime audio processing. The HANCE Audio Engine is built from scratch to be highly CPU and memory efficient. It can be customized and trained to tackle various audio-related issues. For information on custom-developed models, please [contact us](https://hance.ai/contact).


> 
> **Important HANCE Web API endpoint B**<br>
> https://server.hance.ai/api/get_models


### Get models: example of returned JSON data


```json
[
  {"model_description": "De-noise", "model_id": "speech-denoise"}, 
  {"model_description": "De-reverb", "model_id": "speech-dereverb"}
]
```


### Get models: retrieving and selecting a model

```javascript
// First, we store the address from Important endpoint B as a constant:
const MODELS_ENDPOINT =
  "https://server.hance.ai/api/get_models"

const getModelsData = async () => await (await fetch(MODELS_ENDPOINT)).json()

getModelsData()
  .then((models) => {
    // The array of objects is now available to the getModelData function
    // and ready to be utilized.

    // We can do something with the models data here. To check if we have
    // received the data, we log it to the console.
    console.dir(models)

    // We also return the models data so the values can be used in the next
    // .then() call, like in the example below.
    return models
  })
  .then((models) => console.log(models[1].model_id))//real_time_noise_reverb_v1
  .catch(console.error)
```

### Get models: return values

| Field  | Type             | Description                                                                             |
| ------ | ---------------- | --------------------------------------------------------------------------------------- |
| models | Array of objects | Each object in the models array contain a **model_description** and a **model_id** key. |

## Processing audio files

 Once you have obtained the upload URL and unique ID, as explained in the [Get upload URL](#get-upload-url) section, and [selected the appropriate model](#get-models), we can upload audio files to HANCE's servers. To do this, make a POST request to the generated upload URL and include some of the query parameters described below with the request.

> 
> The WEB API does not demonstrate the realtime capabilities of the HANCE audio engine, as files must be uploaded to the server, processed, and then downloaded back. It does, however, demonstrate the excellent quality of the results. For realtime processing, please refer to our [C++](https://hance.ai/api/c) and [Python](https://hance.ai/api/python) documentation or [contact us](https://hance.ai/contact) with any questions.

### Processing audio files: query parameters

| Field           | Type   | Description                                                                                                                                          |
| --------------- | ------ | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| upload_id       | String | The upload_id returned from the [Get upload URL](#get-upload-url) endpoint.                                                                          |
| model_id        | String | The model used for processing. This can be obtained by using the [Get models](#get-models) endpoint detailed above.                                  |
| file_type       | String | The parameter is optional, and it accepts the two values **audio_file** or **audio_sprite**. If no value is provided, it defaults to **audio_file**. |
| return_file_url | String | Option to return the file directly and improve speed by eliminating extra transfer. Set the value to true/false with false as the default value.     |
| output_format   | String | An optional output file format for the processed file. Accepted values are **wav** (default), **mp3**, and **ogg**.                                  |

> *hint*
> The HANCE Web API enables faster file uploads by accepting compressed files. This can dramatically reduce upload time, accelerating the whole process. However, the quality of the returned files may be diminished when using compressed audio formats, so it is recommended to experiment with different file formats before settling on one.

### Processing audio files: return values

The HANCE Web API will return a redirect to the enhanced file once it has completed processing. If the redirect argument is not specified or set to **false**, the API will return a URL pointing to the processed file instead. The request may take a while to complete, depending on how large and complex the file is.

### A minimal demo

A minmal demo of the web api is available [here](https://server.hance.ai/demo)
</details>
