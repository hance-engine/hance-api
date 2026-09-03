/*

This file is part of the Rapidly SDK for cross-platform model inference.
Copyright (c) 2026 Rapidly Labs AS.

You are not allowed to use, distribute or modify this code without
a written permission from Rapidly Labs AS.

*/

#ifndef EngineAdapterObjC_h
#define EngineAdapterObjC_h

#import <Foundation/Foundation.h>

// Must match the layout of `RapidlyProcessorInfo` in engine/RapidlyEngine.h
// exactly. The C engine writes all five fields via rapidlyGetProcessorInfo;
// missing fields here previously caused an 8-byte stack overflow at call
// time (the Swift wrapper still only exposes the three customer-relevant
// fields, but the storage must match the engine's struct size).
struct _RapidlyProcessorInfo {
    double sampleRate;
    int32_t numOfModelChannels;
    int32_t latencyInSamples;
    int32_t blockSize;
    int32_t hopSize;
};

// Exported explicitly. The engine target sets CXX_VISIBILITY_PRESET hidden
// (anti-tamper A5, PR #95), which hides every symbol not marked otherwise.
// The C API stays visible via the RAPIDLY_API macro; this Obj-C++ class is
// not a C API function, so without an explicit default-visibility attribute
// A5 hides _OBJC_CLASS_$_EngineAdapterObjC and the SwiftPM Swift wrapper --
// which bridges through this class -- fails to link against the framework.
__attribute__((visibility("default")))
@interface EngineAdapterObjC : NSObject

/**
 * Adds a license key to the Rapidly SDK.
 * Licence state is global, so this should be called BEFORE creating any processor.
 * Models loaded without a covering licence produce audibly degraded (watermarked) output.
 * @param licenseString                    A string containing a license received from Rapidly
 * @return                                True, if the license check succeeded, otherwise false.
 */
+ (BOOL)addLicense:(NSString *)licenseString;

/**
 * Creates an audio processor, loads a model file and returns a handle to the processor instance if successful.
 * @param modelFilepath                    Pointer to a zero terminated string containing the file path of the model file to load.
 * @param numOfChannels                    The number of channels in the audio to process.
 * @param sampleRate                           The sample rate of the audio to process.
 * @return                      A valid processor handle on success, otherwise nullptr.
 */
- (instancetype)initWithModelFilepath:(NSString *)modelFilepath numOfChannels:(uint32_t)numOfChannels sampleRate:(double)sampleRate;

/**
 * Creates a processor that performs stem separation, loads a set of model files and returns a handle to the processor instance if successful.
 * The number of models is derived from modelFilepaths.count -- the array already carries its length.
 * @param modelFilepaths                An array of strings containing the file paths of the model files to load.
 * @param numOfChannels                    The number of channels in the audio to process.
 * @param sampleRate                    The sample rate of the audio to process.
 * @return                                A valid processor handle on success, otherwise nullptr.
 */
- (instancetype)initWithModelFilepaths:(NSArray<NSString *> *)modelFilepaths numOfChannels:(uint32_t)numOfChannels sampleRate:(double)sampleRate;

/**
 * Adds floating point PCM encoded audio from separate channels to the Rapidly audio processor.
 * @param pcmChannels                    Pointer to an array of channel data pointers, each pointing to sampled PCM values as 32-bit floating point.
 * @param numOfSamples                    The number of samples to add
 */
- (void)addAudio:(void* const* const)pcmChannels numOfSamples:(uint32_t)numOfSamples;

/**
 * Adds floating point PCM encoded audio from a single channel-interleaved buffer to the Rapidly audio processor.
 * @param pcmChannels                Pointer to a 32-bit floating point buffer containing channel-interleaved PCM audio
                                        (stereo audio will be in the form "Left Sample 1", "Right Sample 1", "Left Sample 2"...).
 * @param numOfSamples                    The number of samples to add
 */
- (void)addAudioInterleaved:(void*)pcmChannels numOfSamples:(uint32_t)numOfSamples;

/**
 * Returns the number of samples that are ready after model inference. If the end of the stream has been reached, endOfStream can
 * can be set to true to retrieve the number of remaining samples in the processing queue.
 * @return                                Number of completed samples.
 */
- (uint32_t)getNumOfPendingSamples;

/**
 * Gets floating point PCM encoded audio in separate channels after processing. The number of requested samples must be less or equal to the
 * number of available samples as returned by /ref rapidlyGetNumOfPendingSamples.
 * @param pcmChannels                    Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.
 * @param numOfSamples                    The number of samples to retrieve
 */
- (BOOL)getAudio:(void* const*)pcmChannels numOfSamples:(uint32_t)numOfSamples;

/**
 * Gets floating point PCM encoded audio in a single channel-interleaved buffer after processing. The number of requested samples must be less or equal to the
 * number of available samples as returned by /ref rapidlyGetNumOfPendingSamples.
 * @param pcmChannels                Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.
 * @param numOfSamples                    The number of samples to retrieve
 */
- (BOOL)getAudioInterleaved:(void*)pcmChannels numOfSamples:(uint32_t)numOfSamples;

/**
 * Resets the processor state and clears all delay lines.
 */
- (void)resetProcessorState;

/**
 * Fills a the RapidlyProcessorInfo structure with information about the Rapidly processor.
 */
- (struct _RapidlyProcessorInfo)getProcessorInfo;

/**
 * Returns a parameter value
 * @param index                Index of the parameter to return. The available parameters are
 *                                         defines with RAPIDLY_PARAM as prefix.
 * @return                                The current parameter value.
 */
- (float)getParameterValueFor:(uint32_t)index;

/**
 * Sets a parameter value
 * @param index                Index of the parameter to return. The available parameters are
 *                                         defines with RAPIDLY_PARAM as prefix.
 * @param value                The new parameter value.
 */
- (void)setParameterFor:(uint32_t)index to:(float)value;

/**
 * Releases the underlying processor, freeing its resources. Safe to call
 * multiple times. After close, all audio methods become no-ops and read
 * methods return false / zero. Customers running real-time audio should
 * call this from their audio-stop callback BEFORE releasing references to
 * the adapter, to avoid a race where the audio thread is still inside
 * addAudio/getAudio when ARC destroys the processor. dealloc also calls
 * close, but close gives callers explicit control over timing.
 */
- (void)close;

@end

#endif /* EngineAdapterObjC_h */
