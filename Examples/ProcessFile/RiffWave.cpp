/*

This file is part of the HANCE engine for cross-platform model inference.
Copyright (c) 2022 HANCE AS.

You are not allowed to use, distribute or modify this code without
a written permission from HANCE AS.

*/

#include "RiffWave.h"
#include <string>
#include <vector>

using namespace std;

int getRiffSpecifier (string tag)
{
    if (tag.size() < 4)
        return 0;

    int specifier;
    memcpy (&specifier, tag.c_str(), 4);
    return specifier;
}

bool parseRiffHeader (FILE* fileHandle, int& numOfSamplesInSource, int& numOfChannels, double& sampleRate)
{
    fseek (fileHandle, 0, SEEK_SET);
  
    int32_t specifier;
    fread (&specifier, 4, 1, fileHandle);
    if (specifier != getRiffSpecifier ("RIFF"))
        return false;

    uint32_t riffSize;
    fread (&riffSize, 4, 1, fileHandle);

    fread (&specifier, 4, 1, fileHandle);
    if (specifier != getRiffSpecifier ("WAVE"))
        return false;

    uint32_t audioDataOffset = 0;
    uint32_t audioDataSize = 0;

    vector<uint8_t> formatData;

	while ((audioDataSize == 0) || (formatData.size() == 0)) {
		uint32_t chunkSize = 0;

        fread (&specifier, 4, 1, fileHandle);
        fread (&chunkSize, 4, 1, fileHandle);

        uint32_t chunkFileOffset = ftell (fileHandle);

        if (specifier == getRiffSpecifier ("data")) {
            audioDataOffset = ftell (fileHandle);
            audioDataSize = chunkSize;
        }
        else if (specifier == getRiffSpecifier ("fmt ")) {
            formatData.resize (chunkSize);
            fread (formatData.data(), 1, formatData.size(), fileHandle);
        }
		if (chunkSize == 0)
            return false;

        uint32_t paddedChunkSize = 2 * ((chunkSize + 1) / 2);
        if (fseek (fileHandle, chunkFileOffset + paddedChunkSize, SEEK_SET) != 0)
            return false;
	}

    if ((audioDataSize == 0) || (formatData.size() == 0))
        return false;

    auto waveFormat = (WaveFormatEx*) (formatData.data());
    if (waveFormat->m_formatTag != WAVE_FORMAT_PCM)
        return false;

    if (waveFormat->m_numOfBitsPerSample != 16)
        return false;

    numOfSamplesInSource = audioDataSize / waveFormat->m_blockAlign;
    numOfChannels = waveFormat->m_numOfChannels;
    sampleRate = (double) waveFormat->m_sampleRate;

    if (fseek (fileHandle, audioDataOffset, SEEK_SET) != 0)
        return false;

	return true;
}

bool readRiffAudio (FILE* fileHandle, float* audioData, int numOfValuesToRead)
{
    // We only support 16 bit files for simplicity
    vector<int16_t> intBuffer (numOfValuesToRead);

    // Read integer data
    auto numOfValuesRead = fread (intBuffer.data(), sizeof (int16_t), numOfValuesToRead, fileHandle);
    if (numOfValuesRead != numOfValuesToRead)
        return false;

    // Convert to floating point
    auto intBufferPtr = intBuffer.data();
    for (int valueIndex = 0; valueIndex < numOfValuesToRead; valueIndex++)
        audioData[valueIndex] = intBufferPtr[valueIndex] / 32768.f;

    return true;
}

bool writeRiffHeader (FILE* fileHandle, int numOfSamplesToWrite, int numOfChannels, double sampleRate)
{
    if (fseek (fileHandle, 0, SEEK_SET) != 0)
        return false;
  
    int32_t specifier = getRiffSpecifier ("RIFF");
    fwrite (&specifier, 4, 1, fileHandle);

    uint32_t fmtChunkSize = sizeof (WaveFormatEx);
    uint32_t dataChunkSize = sizeof (int16_t) * numOfChannels * numOfSamplesToWrite;

    uint32_t riffSize = fmtChunkSize + dataChunkSize + 16;
    fwrite (&riffSize, 4, 1, fileHandle);

    specifier = getRiffSpecifier ("WAVE");
    fwrite (&specifier, 4, 1, fileHandle);

    specifier = getRiffSpecifier ("fmt ");
    fwrite (&specifier, 4, 1, fileHandle);

    fwrite (&fmtChunkSize, 4, 1, fileHandle);

    WaveFormatEx wfx;
    memset (&wfx, 0, sizeof (WaveFormatEx));
    wfx.m_blockAlign = (uint16_t) (sizeof (int16_t) * numOfChannels);
    wfx.m_formatTag = WAVE_FORMAT_PCM;
    wfx.m_numOfBitsPerSample = 16;
    wfx.m_numOfChannels = (uint16_t) numOfChannels;
    wfx.m_sampleRate = (int) (sampleRate + 0.5);
    wfx.m_numOfAvgBytesPerSec = wfx.m_blockAlign * wfx.m_sampleRate;

    fwrite (&wfx, sizeof (WaveFormatEx), 1, fileHandle);

    specifier = getRiffSpecifier ("data");
    fwrite (&specifier, 4, 1, fileHandle);

    if (fwrite (&dataChunkSize, 4, 1, fileHandle) != 1)
        return false;

    return true;
}

bool writeRiffAudio (FILE* fileHandle, const float* audioData, int numOfValuesToWrite)
{
    // We only support 16 bit files for simplicity
    vector<int16_t> intBuffer (numOfValuesToWrite);

    // Convert from floating point
    auto intBufferPtr = intBuffer.data();
    for (int valueIndex = 0; valueIndex < numOfValuesToWrite; valueIndex++)
        intBufferPtr[valueIndex] = (int16_t) (max (min (audioData[valueIndex] * 32768.f + 0.5f, 32767.f), -32768.f));

    // Read integer data
    auto numOfValuesWritten = fwrite (intBuffer.data(), sizeof (int16_t), numOfValuesToWrite, fileHandle);
    if (numOfValuesWritten != numOfValuesToWrite)
        return false;

    return true;
}
