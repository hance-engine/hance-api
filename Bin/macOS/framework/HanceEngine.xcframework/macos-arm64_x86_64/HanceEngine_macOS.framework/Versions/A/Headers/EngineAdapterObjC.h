/*

This file is part of the HANCE engine for cross-platform model inference.
Copyright (c) 2024 HANCE AS.

You are not allowed to use, distribute or modify this code without
a written permission from HANCE AS.

*/

#ifndef EngineAdapterObjC_h
#define EngineAdapterObjC_h

#import <Foundation/Foundation.h>

struct _HanceProcessorInfo {
    double sampleRate;
    int32_t numOfModelChannels;
    int32_t latencyInSamples;
};

@interface EngineAdapterObjC : NSObject

/**
 * Adds a license key to the HANCE engine to remove audio watermarking on the output
 * @param licenseString                    A string containing a license received from HANCE
 * @return                                True, if the license check succeeded, otherwise false.
 */
- (BOOL)addLicense:(NSString *)licenseString;

/**
 * Creates an audio processor, loads a model file and returns a handle to the processor instance if successful.
 * @param modelFilepath                    Pointer to a zero terminated string containing the file path of the model file to load.
 * @param numOfChannels                    The number of channels in the audio to process.
 * @param sampleRate                           The sample rate of the audio to process.
 * @return                      A valid processor handle on success, otherwise nullptr.
 */
- (instancetype)initWithModelFilepath:(NSArray *)modelFilepath numOfChannels:(uint32_t)numOfChannels sampleRate:(double)sampleRate;

/**
 * Creates a processor that performs stem separation, loads a set of model files and returns a handle to the processor instance if successful.
 * @param numOfStems                    The number of model files to load
 * @param modelFilepaths                Pointer to an array of pointers to zero terminated strings containing the file paths of the model files to load.
 * @param numOfChannels                    The number of channels in the audio to process.
 * @param sampleRate                    The sample rate of the audio to process.
 * @return                                A valid processor handle on success, otherwise nullptr.
 */
- (instancetype)initWithNumOfStems:(uint32_t)numOfStems modelFilepaths:(NSArray *)modelFilepaths numOfChannels:(uint32_t)numOfChannels sampleRate:(double)sampleRate;

/**
 * Adds floating point PCM encoded audio from separate channels to the HANCE audio processor.
 * @param pcmChannels                    Pointer to an array of channel data pointers, each pointing to sampled PCM values as 32-bit floating point.
 * @param numOfSamples                    The number of samples to add
 */
- (void)addAudio:(void* const* const)pcmChannels numOfSamples:(uint32_t)numOfSamples;

/**
 * Adds floating point PCM encoded audio from a single channel-interleaved buffer to the HANCE audio processor.
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
 * number of available samples as returned by /ref hanceGetNumOfPendingSamples.
 * @param pcmChannels                    Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.
 * @param numOfSamples                    The number of samples to retrieve
 */
- (BOOL)getAudio:(void* const*)pcmChannels numOfSamples:(uint32_t)numOfSamples;

/**
 * Gets floating point PCM encoded audio in a single channel-interleaved buffer after processing. The number of requested samples must be less or equal to the
 * number of available samples as returned by /ref hanceGetNumOfPendingSamples.
 * @param pcmChannels                Pointer to an array of channel data pointers, each receiving sampled PCM values as 32 bit floating point.
 * @param numOfSamples                    The number of samples to retrieve
 */
- (BOOL)getAudioInterleaved:(void*)pcmChannels numOfSamples:(uint32_t)numOfSamples;

/**
 * Resets the processor state and clears all delay lines.
 */
- (void)resetProcessorState;

/**
 * Fills a the HanceProcessorInfo structure with information about the HANCE processor.
 */
- (struct _HanceProcessorInfo)getProcessorInfo;

/**
 * Returns a parameter value
 * @param index                Index of the parameter to return. The available parameters are
 *                                         defines with HANCE_PARAM as prefix.
 * @return                                The current parameter value.
 */
- (float)getParameterValueFor:(uint32_t)index;

/**
 * Sets a parameter value
 * @param index                Index of the parameter to return. The available parameters are
 *                                         defines with HANCE_PARAM as prefix.
 * @param value                The new parameter value.
 */
- (void)setParameterFor:(uint32_t)index to:(float)value;

@end

#endif /* EngineAdapterObjC_h */
