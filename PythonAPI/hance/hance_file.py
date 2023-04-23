import hance
import numpy as np
import subprocess
import os

try:
    import soundfile as sf
except ImportError:
    print("Soundfile is not installed. Please install it to process files using the command:")
    print("pip install Soundfile")
    print("The HANCE engine can be used without Soundfile, but processing files requires it.")
    #exit()


def process_file(model_file_path : str, input_file_path : str, output_file_path : str, license_string : str = ""):
    """
    Processes a full wave file using the preloaded model
    """
    
    hance_engine = hance.HanceEngine()
    if license_string != "":
        if not hance_engine.add_license(license_string):
            raise Exception("License key not accepted")

    created_temp_file = False
    could_open = True
    try:
        in_file_info = sf.SoundFile(input_file_path)
    except sf.LibsndfileError as e:
        could_open = False
    
    if not could_open:
        has_ffmpeg = False
        try:
            subprocess.check_output(['ffmpeg', '-version'])
            has_ffmpeg = True
        except OSError as e:
            raise Exception("Could not open input file. Supported formats are WAV, FLAC, OGG and MAT. Please install ffmpeg to support more formats.")
        
        if has_ffmpeg:
            print("File format is not supported natively. Converting to WAV using ffmpeg. (This may take a while...)")
            subprocess.call(['ffmpeg', '-i', input_file_path, 'temp.wav'])
            input_file_path = 'temp.wav'
            in_file_info = sf.SoundFile(input_file_path)
            created_temp_file = True
    
    processor = hance_engine.create_processor(model_file_path, in_file_info.channels, in_file_info.samplerate)
    
    output_file = sf.SoundFile(output_file_path, mode='w', samplerate=in_file_info.samplerate, channels=in_file_info.channels,
                                subtype=in_file_info.subtype, endian=in_file_info.endian, format=in_file_info.format)

    num_of_samples_written = 0
    block_size = 65536
    for audio_block in sf.blocks(input_file_path, dtype='float32', blocksize=block_size, always_2d=True):
        audio_out = processor.process(audio_block)
        if audio_out.size != 0:
            output_file.write(audio_out)
        num_of_samples_written += audio_out.size

    """
    # Retrieve tail audio by feeding silence into the processor
    silent_audio = np.zeros([block_size, in_file_info.channels])
    while num_of_samples_written < in_file_info.frames:
        audio_out = processor.process(silent_audio)
        if len (audio_out) > in_file_info.frames - num_of_samples_written:
            audio_out = audio_out[0:in_file_info.frames - num_of_samples_written,:]
            output_file.write(audio_out)
        num_of_samples_written += len(audio_out)
    """

    #Rewritten to support older versions of pysoundfile,
    #which doesn't have the in_file_info.frames attribute.
    silent_audio = np.zeros([block_size, in_file_info.channels])
    num_of_samples = len(in_file_info) // in_file_info.channels
    num_of_samples_written = 0

    while num_of_samples_written < num_of_samples:
        remaining_samples = num_of_samples - num_of_samples_written
        samples_to_process = min(block_size, remaining_samples)
        audio_out = processor.process(silent_audio[:samples_to_process])
        
        output_file.write(audio_out)
        num_of_samples_written += samples_to_process

    if created_temp_file:
        os.remove('temp.wav')