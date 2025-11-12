/*

This file is part of the HANCE engine for cross-platform model inference.
Copyright (c) 2022 HANCE AS.

You are not allowed to use, distribute or modify this code without
a written permission from HANCE AS.

*/

#include <iostream>
#include <cstdint>

#ifdef _WIN32
	#pragma pack (push, 1)
    #define RIFF_PACKED
#else
    #define RIFF_PACKED __attribute__((packed))
#endif

// List of supported wave formats
enum WaveFormatTags
{
	WAVE_FORMAT_PCM = 1,
};

struct WaveFormatEx
{
	uint16_t m_formatTag;				/* format type */
	uint16_t m_numOfChannels;			/* number of channels (i.e. mono, stereo...) */
	uint32_t m_sampleRate;				/* sample rate */
	uint32_t m_numOfAvgBytesPerSec;		/* for buffer estimation */
	uint16_t m_blockAlign;				/* block size of data */
	uint16_t m_numOfBitsPerSample;		/* number of bits per sample of mono data */
	uint16_t m_numOfExtraBytes;			/* the count in bytes of the size of extra information (after cbSize) */
} RIFF_PACKED;

bool parseRiffHeader (FILE* fileHandle, int& numOfSamplesInSource, int& numOfChannels, double& sampleRate);
bool readRiffAudio (FILE* fileHandle, float* audioData, int numOfValuesToRead);
bool writeRiffHeader (FILE* fileHandle, int numOfSamplesToWrite, int numOfChannels, double sampleRate);
bool writeRiffAudio (FILE* fileHandle, const float* audioData, int numOfValuesToWrite);

#ifdef _WIN32
	#pragma pack (pop)
#endif
