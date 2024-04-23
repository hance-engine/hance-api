/*

This file is part of the HANCE engine for cross-platform model inference.
Copyright (c) 2022 HANCE AS.

You are not allowed to use, distribute or modify this code without
a written permission from HANCE AS.

*/

// Include the header file HanceEngine.h to get access to the C interface for the
// HANCE Audio Engine
#include "HanceEngine.h"

// We include methods to decode and encode audio in the RIFF Wave format.
#include "../RiffWave/RiffWave.h"
#include <vector>

using namespace std;

// Some global handles that we need to close in the event of an error
FILE* g_inputFileHandle  = nullptr;
vector<FILE*> g_outputFileHandles;
HanceProcessorHandle g_processorHandle = nullptr;

// A simple error handler
void handleError (string errorMessage)
{
    cout << errorMessage << endl;

    if (g_inputFileHandle != nullptr)
        fclose (g_inputFileHandle);

    for (auto outputFileHandle : g_outputFileHandles)
        fclose (outputFileHandle);
    if (g_processorHandle != nullptr)
        hanceDeleteProcessor (g_processorHandle);

    exit (-1);
}

int main (int argc, char* argv[])
{
    // Vector of the names of the stems to separate
    vector<string> stemNames    = { "vocals", "bass", "drums" };
    const int numOfModels       = (int) stemNames.size();

    // You can add a license key for a pre-trained HANCE model using the hanceAddLicense method. If
    // you don't have a license, the audio output will include the "Audio enhancement by HANCE" at
    // irregular intervals.
    // hanceAddLicense ("XXXX");

    // Parse the input arguments
    if (argc < 4) {
        cout << "Incorrect number of arguments." << endl
             << "Usage: SeparateStems [model directory] [input filepath] [output directory]" << endl;
        return -1;
    }
    char* modelDirectory  = argv[1];
    char* inputFilePath   = argv[2];
    char* outputDirectory = argv[3];

    // Create file handles for audio input and output
    g_inputFileHandle = fopen (inputFilePath, "rb");
    if (g_inputFileHandle == nullptr)
        handleError ("Unable to open input file.");

    // Parse input file
    int numOfSamplesInSource = 0;
    int numOfChannels        = 0;
    double sampleRate        = 0.0;
    if (!parseRiffHeader (g_inputFileHandle, numOfSamplesInSource, numOfChannels, sampleRate))
        handleError ("The input file does not have a valid format.");

    vector<string> modelFilePaths;
    
    for (auto stemName : stemNames) {
        modelFilePaths.push_back (modelDirectory + string ("/") + stemName + string ("_separation.hance"));
        string outputFilePath = outputDirectory + string ("/") + stemName + string (".wav");
        auto outputFileHandle = fopen (outputFilePath.c_str(), "wb");
        if (outputFileHandle != nullptr) {
            g_outputFileHandles.push_back (outputFileHandle);

            // Write RIFF header to output file
            if (!writeRiffHeader (outputFileHandle, (int32_t) numOfSamplesInSource, numOfChannels, sampleRate))
                handleError ("Unable to write RIFF header to output file.");
        }
        else
            handleError ("Unable to open output file.");
    }

    vector<const char*> modelFilePathPointers;
    for (auto&& filePath : modelFilePaths)
        modelFilePathPointers.push_back (filePath.c_str());

    g_processorHandle = hanceCreateStemSeparator (numOfModels, modelFilePathPointers.data(), numOfChannels, sampleRate);
    if (g_processorHandle == nullptr)
        handleError ("Unable to create the HANCE audio processor.");

    // Adjust the settings of the processing
    // Maximum attenuation in dB in the range [-inf, 0]
    hanceSetParameterValue (g_processorHandle, HANCE_PARAM_MAXATTENUATION, -196.f); 

    // Disable frequency band extrapolation for the mask
    hanceSetParameterValue (g_processorHandle, HANCE_PARAM_MASKEXTRAPOLATION, 0.f); 

    for (int modelIndex = 0; modelIndex < numOfModels; modelIndex++)
        // Sensitivity of the detection in % in the range [-100, 100] with 0% being neutral.
        hanceSetParameterValue (g_processorHandle, HANCE_PARAM_MODEL_SENSITIVITIES + modelIndex, 0.f);

    // Start processing loop
    cout << "Starting processing:" << endl;

    int numOfSamplesWritten              = 0;
    int numOfSamplesRead                 = 0;
    const int maxNumberOfSamplesInBuffer = 2048;
    vector<float> audioBuffer (numOfChannels * maxNumberOfSamplesInBuffer);

    while (numOfSamplesWritten < numOfSamplesInSource) {
        int numOfSamplesInBuffer;
        if (numOfSamplesRead == numOfSamplesInSource) {
            numOfSamplesInBuffer = maxNumberOfSamplesInBuffer;
            fill (audioBuffer.begin(), audioBuffer.end(), 0.f);
        }
        else {
            numOfSamplesInBuffer = min (maxNumberOfSamplesInBuffer, numOfSamplesInSource - numOfSamplesRead);

            // We read PCM audio from the file in the 32-bit floating point format
            if (!readRiffAudio (g_inputFileHandle, audioBuffer.data(), numOfChannels * numOfSamplesInBuffer))
                handleError ("Unable to read audio from file.");

            numOfSamplesRead += numOfSamplesInBuffer;
        }

        // We read PCM audio from the file in the 32-bit floating point format (we skip error handling for simplicity)
        hanceAddAudioInterleaved (g_processorHandle, audioBuffer.data(), numOfSamplesInBuffer);

        // Now, we check how many samples that are ready for pick-up
        int numOfSamplesToWrite = min (hanceGetNumOfPendingSamples (g_processorHandle), numOfSamplesInSource - numOfSamplesWritten);

        // Fetch and save the audio to file if there's anything ready
        if (numOfSamplesToWrite > 0) {
            vector<float> processedBuffer (numOfModels * numOfChannels * numOfSamplesToWrite);
            if (!hanceGetAudioInterleaved (g_processorHandle, processedBuffer.data(), numOfSamplesToWrite))
                handleError ("Unable to get audio from the HANCE audio processor.");

            vector<float> stemBuffer (numOfChannels * numOfSamplesToWrite);

            auto stemBufferPtr      = stemBuffer.data();
            auto processedBufferPtr = processedBuffer.data();
            int stemChannelOffset   = 0;

            for (auto outputFileHandle : g_outputFileHandles) {

                // Copy stem audio data from processed buffer
                for (int sampleIndex = 0; sampleIndex < numOfSamplesToWrite; sampleIndex++) {
                    for (int channelIndex = 0; channelIndex < numOfChannels; channelIndex++) {
                        stemBufferPtr[sampleIndex * numOfChannels + channelIndex] =
                            processedBufferPtr[sampleIndex * numOfChannels * numOfModels + stemChannelOffset + channelIndex];
                    }
                }

                // Write audio to file
                if (!writeRiffAudio (outputFileHandle, stemBufferPtr, (int32_t) stemBuffer.size()))
                    handleError ("Unable to write audio to the output file.");
                stemChannelOffset += numOfChannels;
            }
            numOfSamplesWritten += numOfSamplesToWrite;
        }
        cout << ".";
    }
    cout << endl << "Completed processing." << endl;

    // Close the file handles and delete the HANCE processor
    fclose (g_inputFileHandle);
    for (auto outputFileHandle : g_outputFileHandles)
        fclose (outputFileHandle);
    hanceDeleteProcessor (g_processorHandle);

    return 0;
}
