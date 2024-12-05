/*

This file is part of the HANCE engine for cross-platform model inference.
Copyright (c) 2024 HANCE AS.

You are not allowed to use, distribute or modify this code without
a written permission from HANCE AS.

*/

// Include the header file HanceEngine.h to get access to the C interface for the
// HANCE Audio Engine
#include "HanceEngine.h"

// We include methods to decode and encode audio in the RIFF Wave format.
#include "../RiffWave/RiffWave.h"
#include <vector>
#include <set>
#include <iomanip>

using namespace std;

// Some global handles that we need to close in the event of an error
FILE* g_inputFileHandle  = nullptr;
FILE* g_outputFileHandle = nullptr;
HanceProcessorHandle g_processorHandle = nullptr;

// A simple error handler
void handleError (string errorMessage)
{
    cout << errorMessage << endl;

    if (g_inputFileHandle != nullptr)
        fclose (g_inputFileHandle);
    if (g_outputFileHandle != nullptr)
        fclose (g_outputFileHandle);
    if (g_processorHandle != nullptr)
        hanceDeleteProcessor (g_processorHandle);

    exit (-1);
}

int main (int argc, char* argv[])
{
    // Parse the input arguments
    if (argc < 4) {
        cout << "Incorrect number of arguments." << endl
             << "Usage: ProcessFile [model filepath] [input filepath] [output filepath] [license key (optional)]" << endl;
        return -1;
    }
    char* modelFilePath  = argv[1];
    char* inputFilePath  = argv[2];
    char* outputFilePath = argv[3];

    if (argc == 5) {
        if (!hanceAddLicense (argv[4]))
            handleError ("License key not accepted.");
    }

    // Create file handles for audio input and output
    g_inputFileHandle = fopen (inputFilePath, "rb");
    if (g_inputFileHandle == nullptr)
        handleError ("Unable to open input file.");

    g_outputFileHandle = fopen (outputFilePath, "wb");
    if (g_outputFileHandle == nullptr)
        handleError ("Unable to open output file.");

    // Parse input file
    int numOfSamplesInSource = 0;
    int numOfChannels        = 0;
    double sampleRate        = 0.0;
    if (!parseRiffHeader (g_inputFileHandle, numOfSamplesInSource, numOfChannels, sampleRate))
        handleError ("The input file does not have a valid format.");

    // Write RIFF header to output file
    if (!writeRiffHeader (g_outputFileHandle, (int32_t) numOfSamplesInSource, numOfChannels, sampleRate))
        handleError ("Unable to write RIFF header to output file.");

    // Create a HANCE processor that loads the pre-trained model from file
    g_processorHandle = hanceCreateProcessor (modelFilePath, numOfChannels, sampleRate);
    if (g_processorHandle == nullptr)
        handleError ("Unable to create the HANCE audio processor.");

    // Print model information
    HanceProcessorInfo processorInfo;
    hanceGetProcessorInfo (g_processorHandle, &processorInfo);
    cout << "Model information:" << endl;
    cout << " - block size:         " << processorInfo.blockSize << " samples" << endl;
    cout << " - hop size:           " << processorInfo.hopSize << " samples" << endl;
    cout << " - latency:            " << processorInfo.latencyInSamples << " samples" << endl;
    cout << " - number of channels: " << processorInfo.numOfModelChannels << endl;
    cout << " - sample rate:        " << processorInfo.sampleRate << " Hz" << endl << endl;

    // Print output bus information
    int numOfOutputBusses = hanceGetNumOfOutputBusses (g_processorHandle);
    cout << "Output busses:" << endl;

    // Here's a set of output busses we'd like to include in the output. Other buses will
    // be muted.
    set<string> outputsToIsolate = { "Processed", "Voice", "Vocals", "Snare" };

    // Always show floating point values with two decimals
    cout << fixed << setprecision (2);

    for (int busIndex = 0; busIndex < numOfOutputBusses; busIndex++) {
        vector<char> nameBuffer (255, '\0');
        hanceGetOutputBusName (g_processorHandle, busIndex, (char*) nameBuffer.data(), nameBuffer.size());
        string outputBusName = nameBuffer.data();

        // Set the gain parameters for this output, 1.0 if bus should be included in output, otherwise 0.0
        if (outputsToIsolate.find (outputBusName) != outputsToIsolate.end())
            hanceSetParameterValue (g_processorHandle, HANCE_PARAM_BUS_GAINS + busIndex, 1.f);
        else
            hanceSetParameterValue (g_processorHandle, HANCE_PARAM_BUS_GAINS + busIndex, 0.f);

        // Set the sensitivity in percent for this output
        hanceSetParameterValue (g_processorHandle, HANCE_PARAM_BUS_SENSITIVITIES + busIndex, 0.f);

        // Read back the parameter values for demonstration
        float gain          = hanceGetParameterValue (g_processorHandle, HANCE_PARAM_BUS_GAINS + busIndex);
        float sensitivity   = hanceGetParameterValue (g_processorHandle, HANCE_PARAM_BUS_SENSITIVITIES + busIndex);

    // Make sure that we display floating point values with two decimals
        cout << busIndex << ": " << outputBusName << ", gain = " << gain
             << " (linear scaling), sensitivity = " << sensitivity << " (%)" << endl;
    }
    cout << endl;

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
            vector<float> processedBuffer (numOfChannels * numOfSamplesToWrite);
            if (!hanceGetAudioInterleaved (g_processorHandle, processedBuffer.data(), numOfSamplesToWrite))
                handleError ("Unable to get audio from the HANCE audio processor.");

            // Write audio to file
            if (!writeRiffAudio (g_outputFileHandle, processedBuffer.data(), (int32_t) processedBuffer.size()))
                handleError ("Unable to write audio to the output file.");
            numOfSamplesWritten += numOfSamplesToWrite;
        }
        cout << ".";
    }
    cout << endl << "Completed processing." << endl;

    // Close the file handles and delete the HANCE processor
    fclose (g_inputFileHandle);
    fclose (g_outputFileHandle);
    hanceDeleteProcessor (g_processorHandle);

    return 0;
}
