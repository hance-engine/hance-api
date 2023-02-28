# Audio Enhancement in Python with HANCE

HANCE is an audio enhancement engine that delivers impressive performance. With its Python wrapper, you can easily integrate HANCE into your Python projects to improve the quality of audio signals.

To learn more about HANCE, visit [Hance.ai](https://hance.ai).

## Installation

To install the Python wrapper for HANCE, use 'pip':

    python -m pip install hance

HANCE is compatible with Python 3 and later.

## How to Use
The HANCE Python API is a wrapper around the C++ library. Here are a few ways you can use it.

To use the API, import it and list the available models:

    import hance
    models = hance.list_models()
    print(models)

## Process a file
To process a file with HANCE, you can use the process_file function as follows:

    import hance
    models = hance.list_models()
    hance.process_file(models[0], input_file_path, output_file_path)

This will apply the enhancement model specified by models[0] to the input file located at input_file_path, and save the enhanced audio to the output file at output_file_path. Please note that in this example, we are using PySoundFile to read and write audio files. While PySoundFile is not a requirement for using HANCE, it is a convenient library for handling audio files in Python. If you wish to use the process_file function as shown here, you will need to install PySoundFile.

## Process a stream
In addition to processing audio files, HANCE can also be used on audio streams in real-time. Here is an example using pyaudio to record the microphone, process it in real time, and output it to headphones.

    import pyaudio
    engine = hance.HanceEngine()
    p = pyaudio.PyAudio()

    FORMAT = pyaudio.paFloat32
    CHANNELS = 1
    RATE = 44100
    CHUNK = 512
    
    
    print("\nRecord audio from a microphone and process it in realtime with HANCE.")
    print("To prevent feedback, make sure you are wearing headphones.")
    print("PyAudio will induce some latency with the roundtrip to the soundcard,\nbut the HANCE engine runs in realtime.")
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
            data = stream_record.read(CHUNK, exception_on_overflow = False)
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
        user_input = input("Enter 'p' to toggle processing on and off or 'x' to exit the thread: ")
        if user_input.lower() == "p":
            # Bypass processing and continue the loop
            if processor_active:
                processor_active = False
                print("The processing is bypassed")
            else:
                processor_active = True
                print("The processing is active")
        elif user_input.lower() == "x":
            # Stop the thread
            stop_thread = True
            break
    
    t.join()
    p.terminate()

For more information and examples on using HANCE, see the HANCE documentation.

