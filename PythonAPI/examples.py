"""
Examples on using the Hance engine.
"""

try:
    import numpy as np
except ImportError:
    print("Numpy is not installed. Please install it to run this example using the command:")
    print("pip install numpy")
    print("The HANCE engine can be used without numpy, but this example requires it.")
    exit()

try:
    import soundfile as sf
except ImportError:
    print("Soundfile is not installed. Please install it to run this example using the command:")
    print("pip install Soundfile")
    print("The HANCE engine can be used without Soundfile, but this example requires it.")
    exit()

import os
import threading

import hance


def process_file(input_file_path, output_file_path):
    # Process a full wav file

    models = ["speech-denoise.hance", "speech-dereverb.hance"]
    print("Available models:")
    for i, model in enumerate(models):
        print(f"{i+1}. {model}")

    model_num = int(input("Select a model to use by entering its number: ")) - 1

    print("Using model: ", models[model_num])
    hance.process_file(models[model_num], input_file_path, output_file_path)
    print("File successfully processed")
    print("Output file: ", output_file_path)


def process_microphone():
    """
    Captures the input of your microphone, process it in realtime
    and ouputs it to an output device.
    """
    import pyaudio

    engine = hance.HanceEngine()
    p = pyaudio.PyAudio()

    FORMAT = pyaudio.paFloat32
    CHANNELS = 1
    RATE = 44100
    CHUNK = 512

    print("\nRecord audio from a microphone and process it in realtime with HANCE.")
    print(
        "PyAudio will induce some latency with the roundtrip to the soundcard,\nbut the HANCE engine runs in realtime."
    )
    print("")

    # Get a list of available input devices
    input_devices = []
    for i in range(p.get_device_count()):
        device_info = p.get_device_info_by_index(i)
        if device_info["maxInputChannels"] > 0:
            input_devices.append(device_info)

    # Print the list of available input devices and ask the user to select one
    print("Available input devices:")
    for i, device in enumerate(input_devices):
        print(f"{i}: {device['name']}")
    input_device_index = int(input("\nSelect an input device by entering its number: "))
    input_device_info = input_devices[input_device_index]

    # Get a list of available output devices
    output_devices = []
    for i in range(p.get_device_count()):
        device_info = p.get_device_info_by_index(i)
        if device_info["maxOutputChannels"] > 0:
            output_devices.append(device_info)

    # Print the list of available output devices and ask the user to select one
    print("\nAvailable output devices:")
    print("To prevent feedback, select a headphones output.")
    print()

    for i, device in enumerate(output_devices):
        print(f"{i}: {device['name']}")
    output_device_index = int(input("\nSelect an output device by entering its number: "))
    output_device_info = output_devices[output_device_index]

    models = hance.list_models()

    print()
    print("Available models:")
    for i, model in enumerate(models):
        print(f"{i+1}. {model}")

    print()
    model_num = int(input("Select a model to use by entering its number: ")) - 1
    selected_model_path = models[model_num]

    processor = engine.create_processor(selected_model_path, CHANNELS, RATE)

    stop_thread = False
    processor_active = True

    def record_and_playback_thread():
        stream_record = p.open(
            format=FORMAT,
            channels=CHANNELS,
            rate=RATE,
            input=True,
            input_device_index=input_device_info["index"],
            frames_per_buffer=CHUNK,
        )

        stream_play = p.open(
            format=pyaudio.paFloat32,
            channels=1,
            rate=RATE,
            frames_per_buffer=CHUNK,
            output=True,
            output_device_index=output_device_info["index"],
        )
        while not stop_thread:
            data = stream_record.read(CHUNK, exception_on_overflow=False)
            audio_buffer = np.frombuffer(data, dtype=np.float32)
            if processor_active:
                audio_buffer = processor.process(audio_buffer)
            stream_play.write(audio_buffer.astype(np.float32).tobytes())

        # stop Recording
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

    t.join()
    p.terminate()


def separate_stems(input_file_path):
    """
    Separates the stems from an input audio file using selected models with the StemSeparator class.
    """
    print("Stem separation using Hance engine with StemSeparator class.")
    models = [
        "vocals_separation.hance",
        "drums_separation.hance",
        "piano_separation.hance",
        "bass_separation.hance",
    ]
    print("Available models for separation:")
    for i, model in enumerate(models):
        print(f"{i+1}. {model}")

    selected_models = input(
        "Select models to use by entering their numbers separated by commas (e.g., 1,3): "
    )
    selected_models_indices = [int(index) - 1 for index in selected_models.split(",")]

    model_paths = []
    for index in selected_models_indices:
        model_paths.append(models[index])

    # Load the input file
    input_audio, sr = sf.read(input_file_path, dtype="float32")
    if input_audio.ndim == 1:  # Mono to Stereo if needed
        input_audio = np.tile(input_audio[:, np.newaxis], (1, 2))

    # Assuming sample rate and number of channels (most common settings for audio files)
    sample_rate = sr
    num_of_channels = input_audio.ndim

    engine = hance.HanceEngine()
    stem_separator = engine.StemSeparator(
        engine.hance_engine, model_paths, num_of_channels, sample_rate
    )

    # Process the input file
    separated_stems = stem_separator.process(input_audio)

    # Save the separated stems
    path, fn = os.path.split(input_file_path)
    for i in range(len(model_paths)):
        stem_name = model_paths[i].split("_")[0]
        output_file_path = os.path.join(path, f"{fn.split('.')[0]}_{stem_name}_separated.wav")
        # Assuming the model order matches the stem_names order, adjust if necessary
        sf.write(
            output_file_path,
            separated_stems[:, i * num_of_channels : (i + 1) * num_of_channels],
            sr,
        )
        print(f"Stem {stem_name} saved to {output_file_path}")

    print("Stem separation completed.")


if __name__ == "__main__":
    print("1. Remove noise or reverb from a speech recording")
    print("2. Separate stems in a music file")
    print("3. Process your microphone in realtime")
    reply = input("Select an option: ")
    if reply == "1":
        input_file_path = input("Input file: ").strip()
        path, fn = os.path.split(input_file_path)
        out_file_path = path + "/" + fn.split(".")[0] + "_processed.wav"
        if os.path.exists(out_file_path):
            reply = input("Output file already exists, would you like to overwrite it? (y/n) ")
            if reply != "y":
                exit()

        process_file(input_file_path.strip(), out_file_path)
    elif reply == "2":
        input_file_path = input("Input file for stem separation: ").strip()
        separate_stems(input_file_path)
    elif reply == "3":
        process_microphone()
