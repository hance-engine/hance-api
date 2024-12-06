import os
import subprocess
import numpy as np
import hance

try:
    import soundfile as sf
except ImportError:
    print("Soundfile is not installed. Please install it to process files using the command:")
    print("pip install soundfile")
    exit()

def process_file(
    model_file_path: str, input_file_path: str, output_file_path: str, license_string: str = "", selected_output_bus: int = 0
):
    """
    Processes a full wave file using the preloaded model
    """
    hance_engine = hance.HanceEngine()
    if license_string:
        if not hance_engine.add_license(license_string):
            raise Exception("License key not accepted")

    created_temp_file = False
    try:
        in_file_info = sf.SoundFile(input_file_path)
    except sf.LibsndfileError:
        # Try converting using ffmpeg
        if not shutil.which("ffmpeg"):
            raise Exception(
                "Could not open input file. Supported formats are WAV, FLAC, OGG, and MAT. Please install ffmpeg to support more formats."
            )
        print("Converting input file to WAV using ffmpeg...")
        temp_input = "temp_input.wav"
        subprocess.run(["ffmpeg", "-i", input_file_path, temp_input], check=True)
        input_file_path = temp_input
        in_file_info = sf.SoundFile(input_file_path)
        created_temp_file = True

    processor = hance_engine.create_processor(
        model_file_path, in_file_info.channels, in_file_info.samplerate
    )

    for i in range(processor.get_number_of_output_buses()):
        processor.set_output_bus_sensitivity(i, 0.0)
        if i == selected_output_bus:
            processor.set_output_bus_volume(i, 1.0)
        else:
            processor.set_output_bus_volume(i, 0.0)
        

    output_file = sf.SoundFile(
        output_file_path,
        mode="w",
        samplerate=in_file_info.samplerate,
        channels=in_file_info.channels,
        subtype=in_file_info.subtype,
        endian=in_file_info.endian,
        format=in_file_info.format,
    )

    block_size = 65536
    num_of_samples_written = 0
    num_of_samples = len(in_file_info) // in_file_info.channels

    for audio_block in sf.blocks(
        input_file_path, dtype="float32", blocksize=block_size, always_2d=True
    ):
        audio_out = processor.process(audio_block)
        if audio_out.size != 0:
            output_file.write(audio_out)
            num_of_samples_written += audio_out.shape[0]

    # Process remaining samples by feeding silence to the processor
    silent_audio = np.zeros([block_size, in_file_info.channels], dtype="float32")
    while num_of_samples_written < num_of_samples:
        remaining_samples = num_of_samples - num_of_samples_written
        samples_to_process = min(block_size, remaining_samples)
        audio_out = processor.process(silent_audio[:samples_to_process])
        if audio_out.size != 0:
            output_file.write(audio_out)
            num_of_samples_written += audio_out.shape[0]

    output_file.close()
    if created_temp_file:
        os.remove(input_file_path)
