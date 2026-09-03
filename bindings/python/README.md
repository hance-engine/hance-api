# Audio separation in Python with the Rapidly SDK

The **Rapidly SDK** is a model inference library built with audio in mind. Pre-trained models for speech noise suppression, de-reverberation and music stem separation are available out of the box, and we can train models for any use case. Get in touch if you have specific requirements.

Our models are trained specifically for real-time usage, achieving low latencies down to 11 milliseconds in speech separation applications. The models are designed to be small and resource-efficient, with model file sizes down to 241 KB for the smallest noise suppression model.

The Rapidly SDK is built in C++ and is compatible across various architectures. With our Python wrapper, you can easily integrate Rapidly into your Python projects and quickly process files and benchmark our models.

To learn more about Rapidly, visit [Rapidly](https://rapidly.io).


## Installation

To install the Python wrapper for Rapidly, use `pip`:

    python -m pip install rapidly

Rapidly requires Python 3.

## How to Use

To use the API, import it and list the available models:

    import rapidly
    models = rapidly.list_models()
    print(models)

To download and update to the latest models, simply call:

    rapidly.update_models()

## Process a File

To process a file with Rapidly, use the `process_file` function:

    import rapidly

    # List the available models in the models folder.
    models = rapidly.list_models()

    # Process a file using the first model in the list.
    rapidly.process_file(
        model_file_path=models[0],
        input_file_path="path/to/input.wav",
        output_file_path="path/to/output.wav",
        selected_output_bus=0  # Bus 0 is the processed result in most models.
    )

Every model has named output buses: the speech models expose the cleaned dialogue plus the removed noise (and reverb), the music model exposes vocals, bass, guitar, drums, piano and residual. You can inspect available buses like this:

    rapidly_engine = rapidly.RapidlyEngine()
    processor = rapidly_engine.create_processor(models[0], 2, 48000)

    num_buses = processor.get_number_of_output_buses()
    for i in range(num_buses):
        print(i, processor.get_output_bus_name(i))

Pass the desired bus index as `selected_output_bus` in `process_file`.

### Models

The models in the **models** folder use clear, descriptive names. For example, `speech-denoise-32ms.v1.1.rapidly` indicates a model designed to _denoise_ speech with a latency of 32 ms. The `micro` size variant (e.g. `speech-denoise-micro-32ms.v1.1.rapidly`) is a compact build of the same model for CPU-constrained scenarios.

All models within a family (for example, the **speech-denoise** family) share similar characteristics. For general denoising, we recommend starting with `speech-denoise-96ms.v1.1.rapidly` to check if it meets your needs, and moving down to shorter latency variants if needed.

If you have specific requirements or challenging audio conditions, we can build customised models optimised for your use case. Feel free to [contact us](https://rapidly.io/contact).

`process_file` uses [PySoundFile](https://pysoundfile.readthedocs.io/) to read and write audio files. WAV, FLAC, OGG, and MAT formats are supported natively. Other formats require [ffmpeg](https://ffmpeg.org/) to be installed.

For more information, see the [Rapidly documentation](https://rapidly.io/docs/).
