/*

This file is part of the HANCE engine for cross-platform model inference.
Copyright (c) 2023 HANCE AS.

You are not allowed to use, distribute or modify this code without
a written permission from HANCE AS.

*/

/** \mainpage HANCE API – C Interface
* 
* The C interface for the HANCE Audio Engine provides developers with simple access to HANCE's
* powerful algorithms and processing capabilities from all languages that offer bindings for
* standard C compatible libraries. The HANCE Audio Engine is a light-weight and cross-platform
* library, and it should be very easy to integrate it into your application. The library can
* load pre-trained AI models and use these for audio processing to perform various tasks such
* as noise reduction and de-reverberation.
*
* The HANCE Audio Engine is delivered with general purpose models for noise reduction and
* de-reverberation. These are designed to meet common requirements in terms of latency and
* CPU usage. However, we can train custom models for lower latencies or less CPU usage
* at the cost of separation quality. Please [contact us](https://hance.ai/contact) for more
* information.
* 
* \section intro_sec Getting Started
* The HANCE API is designed to be as simple as possible. The **ProcessFile** example (see the
* **Examples** folder in the API) illustrates how to create a HANCE processor and process audio
* with it. [CMake 3.0](https://cmake.org/) or later is required to build the example. To build
* **ProcessFile**, open the terminal / command line prompt and locate the Examples/ProcessFile
* subfolder in the HANCE API. Now, please type "./Build.sh" on Mac or Linux, or "Build.bat" on Windows. 
*
* To use the HANCE API, we first need to make sure the "Include" in the HANCE API is added to the
* header search path and include the HanceEngine header file:
* ```
* #include "HanceEngine.h"
* ```
* Now we can create an instance of a HANCE processor by specifying a path to a HANCE model file
* along with the number of channels and sample rate to use:
* ```
* // Create a HANCE processor that loads the pre-trained model from file
* processorHandle = hanceCreateProcessor (modelFilePath, numOfChannels, sampleRate);
* if (processorHandle == nullptr)
*     handleError ("Unable to create the HANCE audio processor.");
* ```
* The processor handle will be nullptr if the processor couldn't be created, e.g., because the
* model file path is invalid.
* 
* Now, we can add audio in floating point format to the HANCE processor. The HANCE API supports
* audio stored either as channel interleaved audio (hanceAddAudioInterleaved and hanceGetAudioInterleaved)
* or as separate channel vectors (hanceAddAudio and hanceGetAudio). We demonstrate how to add
* audio from a std::vector containing channel interleaved float values below:
*
* ```
* // We read PCM audio from the file in the 32-bit floating point format
* hanceAddAudioInterleaved (processorHandle, audioBuffer.data(),
*                           audioBuffer.size() / numOfChannels);
* ```
* 
* The processing introduces latencies, so we need to query how many samples (if any) that are available
* before we can pick up the processed audio using hanceGetAudioInterleaved:
* 
* ```
* int numOfPendingSamples = hanceGetNumOfPendingSamples (processorHandle);
* vector<float> processedBuffer (numOfChannels * numOfPendingSamples);
* if (!hanceGetAudioInterleaved (processorHandle,
*                                processedBuffer.data(),
*                                numOfPendingSamples))
* {
*     handleError ("Unable to get audio from the HANCE audio processor.");
* }
* ```
* 
* You can add silent audio (all values set to zero) to get the processing tail caused by the
* model's latency. When you have completed the processing of the audio stream, please make sure
* to delete the HANCE processor to free its memory:
* ```
* hanceDeleteProcessor (processorHandle);
* ```
*
 * \section Performance Considerations
 *
 * The HANCE Audio Engine is a light-weight and cross-platform library, and it uses either of the
 * following libraries for vector arithmetic if available:
 * - Intel Performance Primitives
 * - Apple vDSP
 * 
 * The HANCE Audio Engine reverts to pure C++ when no compatible vector arithmetic library is available.
 *
 * Copyright (c) 2023 HANCE AS.
 * 
 * @file HanceEngine.h
 */

#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** \cond */
	#ifdef _WIN32
		#ifdef HANCE_EXPORTS
			#define HANCE_API __declspec(dllexport)
		#else
			#define HANCE_API
		#endif
	#else
		#ifdef HANCE_EXPORTS
			#define HANCE_API __attribute__ ((visibility ("default")))
		#else
			#define HANCE_API
		#endif	
	#endif
/** \endcond */

	typedef void* HanceProcessorHandle;			/**< The processor handle refers to an audio processor that can
													 process PCM audio in 32-bit floating point using a specified
													 inference model. The handle created using \ref hanceCreateProcessor. */

	/** \struct HanceProcessorInfo
		\brief The HanceProcessorInfo contains information about a neural network based audio processor.

		Call the \ref hanceGetProcessorInfo function to fill in the structure.
	*/
	struct HanceProcessorInfo {						
		double sampleRate;					/**< \brief Sample rate used when model was trained. The processor will automatically
												 convert sample rates to match the model. */
		int32_t numOfModelChannels;			/**< \brief The true number of channels used in the processing. The processor will
												 automatically convert the channel format to match the model. */
		int32_t latencyInSamples;			/**< \brief The maximum latency of the model in samples */
	};

	/**
	 * Adds a license key to the HANCE engine to remove audio watermarking on the output
	 * @param licenseString					A string containing a license received from HANCE
	 * @return								True, if the license check succeeded, otherwise false.
	 */
	HANCE_API bool hanceAddLicense (const char* licenseString);

	/**
	 * Creates an audio processor, loads a model file and returns a handle to the processor instance if successful.
	 * @param modelFilepath					Pointer to a zero terminated string containing the file path of the model file to load.
	 * @param numOfChannels					The number of channels in the audio to process.
	 * @param sampleRate					The sample rate of the audio to process.
	 * @return								A valid processor handle on success, otherwise nullptr.
	 */
	HANCE_API HanceProcessorHandle hanceCreateProcessor (const char* modelFilepath, int32_t numOfChannels, double sampleRate);

	/**
	 * Deletes a processor instance.
	 * @param processorHandle				Handle to the processor to delete.
	 */
	HANCE_API void hanceDeleteProcessor (HanceProcessorHandle processorHandle);

	/**
	 * Adds floating point PCM encoded audio from separate channels to the HANCE audio processor.
	 * @param processorHandle				Handle to the audio processor.
	 * @param pcmChannels					Pointer to an array of channel data pointers, each pointing to sampled PCM values as 32-bit floating point.
	 * @param numOfSamples					The number of samples to add
	 */
	HANCE_API void hanceAddAudio (HanceProcessorHandle processorHandle, const float** pcmChannels, int32_t numOfSamples);

	/**
	 * Adds floating point PCM encoded audio from a single channel-interleaved buffer to the HANCE audio processor.
	 * @param processorHandle				Handle to the audio processor.
	 * @param interleavedPCM				Pointer to a 32-bit floating point buffer containing channel-interleaved PCM audio
											(stereo audio will be in the form "Left Sample 1", "Right Sample 1", "Left Sample 2"...).
	 * @param numOfSamples					The number of samples to add
	 */
	HANCE_API void hanceAddAudioInterleaved (HanceProcessorHandle processorHandle, const float* interleavedPCM, int32_t numOfSamples);

	/**
	 * Returns the number of samples that are ready after model inference. If the end of the stream has been reached, endOfStream can
	 * can be set to true to retrieve the number of remaining samples in the processing queue.
	 * @param processorHandle				Handle to the audio processor.
	 * @return								Number of completed samples.
	 */
	HANCE_API int32_t hanceGetNumOfPendingSamples (HanceProcessorHandle processorHandle);

	/**
	 * Gets floating point PCM encoded audio in separate channels after processing. The number of requested samples must be less or equal to the
	 * number of available samples as returned by /ref hanceGetNumOfPendingSamples.
	 * @param processorHandle				Handle to the audio processor.
	 * @param pcmChannels					Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.
	 * @param numOfSamples					The number of samples to retrieve
	 */
	HANCE_API bool hanceGetAudio (HanceProcessorHandle processorHandle, float* const* pcmChannels, int32_t numOfSamples);

	/**
	 * Gets floating point PCM encoded audio in a single channel-interleaved buffer after processing. The number of requested samples must be less or equal to the
	 * number of available samples as returned by /ref hanceGetNumOfPendingSamples.
	 * @param processorHandle				Handle to the audio processor.
	 * @param interleavedPCM				Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.
	 * @param numOfSamples					The number of samples to retrieve
	 */
	HANCE_API bool hanceGetAudioInterleaved (HanceProcessorHandle processorHandle, float* interleavedPCM, int32_t numOfSamples);

	/**
	 * Resets the processor state and clears all delay lines.
	 * @param processorHandle				Handle to the audio processor.
	 */
	HANCE_API void hanceResetProcessorState (HanceProcessorHandle processorHandle);

	/**
	 * Fills a the ModelInfo structure with information about the inference model.
	 * @param processorHandle				Handle to the audio processor.
	 * @param modelInfo						Pointer to a ModelInfo struct that will receive the model information.
	 */
	HANCE_API void hanceGetProcessorInfo (HanceProcessorHandle processorHandle, HanceProcessorInfo* modelInfo);


#ifdef __cplusplus
}
#endif /* __cplusplus */
