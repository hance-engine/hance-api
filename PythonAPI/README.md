# Audio Enhancement in Python with HANCE

**HANCE** is a state-of-the-art audio enhancement company offering models that deliver real-time processing, with latencies as low as 20ms, even on low-power processors. Our models are designed to be ultra-lightweight, with the smallest model being 242KB, while still providing fantastic performance. We also train custom models on request. The HANCE engine is built in C++ and is compatible across various architectures. With our Python wrapper, you can easily integrate HANCE into your Python projects and quickly process files and benchmark our models.

If you're looking for a way to quickly test our models, we recommend either:

- [HANCE Model Player (Mac - AU)](https://github.com/hance-engine/hance-api/tree/hance3-beta?tab=readme-ov-file#:~:text=HANCE%20Model%20Player%20(Mac%20%2D%20AU) 
- [HANCE Model Player (Windows 64 bit - VST3)](https://143687363.fs1.hubspotusercontent-eu1.net/hubfs/143687363/HanceModelPlayer_Win64_2_9_90.exe)


To learn more about HANCE, visit [Hance.ai](https://hance.ai).

## Installation

To install the Python wrapper for HANCE, use `pip`:

    python -m pip install hance

HANCE is compatible with Python 3 and later.

## How to Use

The HANCE Python API is a wrapper around the C++ library. 

For those eager to dive in, the `examples.py` script in our PythonAPI GitHub repository is the perfect starting point. This script features a command-line interface that simplifies the process of experimenting with our models. You can quickly test out various audio enhancement models without writing a single line of code. Here's how to get started:

1. First, clone or download the [examples.py](https://github.com/hance-engine/hance-api/blob/main/PythonAPI/examples.py) file from GitHub to your local machine. 
2. Open your terminal or command prompt and navigate to the directory where you downloaded the file.
3. Execute `python examples.py` to access the command-line interface. Follow the on-screen instructions to select and run audio enhancement models.

## Using the API

To use the API, import it and list the available models:

    import hance
    models = hance.list_models()
    print(models)

## Process a File

To process a file with HANCE, you can use the `process_file` function as follows:

    import hance

    # Initialize HanceEngine and create processor
    hance_engine = hance.HanceEngine()

    # List the available models in the models folder.
    models = hance.list_models()

    # Many HANCE models support multiple outputs, and they can be mixed as preferred.
    # There are models that let you adjust reverb and noise reduction separately,
    # as well as stem separation allowing you, for instance, to output just vocals,
    # or perhaps mix drums and guitar.

    # Create a dummy processor to list the available output busses.
    processor = hance_engine.create_processor(selected_model, 2, 44100)

    # List available output buses
    num_buses = processor.get_number_of_output_buses()
    bus_names = []
    for i in range(num_buses):
        bus_names.append(processor.get_output_bus_name(i))

    # The bus_names list will show you the name of the available output buses.

    hance.process_file(
        model_file_path=selected_model[0],  # Selecting the first model in the list
        input_file_path=input_file_path,
        output_file_path=out_file_path,
        selected_output_bus=0  # Selecting output bus 0, which will be the processed result in most models.
    )

This will apply the enhancement model specified by `models[0]` to the input file located at `input_file_path`, and save the enhanced audio to the output file at `output_file_path`. 

Please note that hance.process_file is using PySoundFile to read and write audio files. While PySoundFile is not a requirement for using HANCE, it is a convenient library for handling audio files in Python.

Check out the `hance_file.py` file to see how you can read and write files block by block for real-time processing.

For more information and examples on using HANCE, see the HANCE documentation.
