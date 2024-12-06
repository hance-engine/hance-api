"""
Examples on using the Hance engine.
"""

import os
import hance

# Check for required modules and prompt to install if missing
try:
    import numpy as np
except ImportError:
    print("NumPy is not installed. Please install it using the command:")
    print("pip install numpy")
    exit()

try:
    import soundfile as sf
except ImportError:
    print("SoundFile is not installed. Please install it using the command:")
    print("pip install soundfile")
    exit()

def main():
    print("Remove noise from voice recordings or separate stems in mixes using Hance.")
    # Get input file from user
    input_file_path = input("Enter the path to your input audio file: ").strip()
    
    # Remove leading and trailing quotes if present
    if input_file_path.startswith('"') and input_file_path.endswith('"'):
        input_file_path = input_file_path[1:-1]
    elif input_file_path.startswith("'") and input_file_path.endswith("'"):
        input_file_path = input_file_path[1:-1]

    # Replace escaped spaces with regular spaces
    input_file_path = input_file_path.replace("\\ ", " ")

    if not os.path.exists(input_file_path):
        print("Input file does not exist.")
        return

    # Construct output file path
    path, fn = os.path.split(input_file_path)
    out_file_path = os.path.join(path, f"{os.path.splitext(fn)[0]}_processed.wav")

    # Check if output file already exists
    if os.path.exists(out_file_path):
        overwrite = input("Output file already exists. Would you like to overwrite it? (y/n): ").strip()
        if overwrite.lower() != "y":
            print("Operation cancelled.")
            return

    # List available models
    models = hance.list_models()
    print("\nAvailable models:")
    for i, model in enumerate(models):
        print(f"{i+1}. {model}")

    # Allow user to select model by number or provide a path
    model_selection = input("\nSelect a model by entering its number or provide the full path to a model file: ").strip()

    # Determine if the input is a number (selecting from list) or a file path
    if model_selection.isdigit():
        model_num = int(model_selection) - 1
        if 0 <= model_num < len(models):
            selected_model = models[model_num]
        else:
            print("Invalid model number.")
            return
    else:
        selected_model = model_selection
        if not os.path.exists(selected_model):
            print("Model file does not exist.")
            return

    print("Using model:", selected_model)

    # Initialize HanceEngine and create processor
    hance_engine = hance.HanceEngine()
    # Here we might need to know the number of channels and sample rate
    # Let's assume stereo and 44100 Hz
    processor = hance_engine.create_processor(selected_model, 2, 44100)

    # List available output buses
    num_buses = processor.get_number_of_output_buses()
    bus_names = []
    for i in range(num_buses):
        bus_names.append(processor.get_output_bus_name(i))

    # Determine output bus
    if num_buses == 2 and 'Processed' in bus_names:
        bus_num = bus_names.index("Processed")
    else:
        print("\nAvailable output buses:")
        for i, name in enumerate(bus_names):
            print(f"{i+1}: {name}")
        bus_selection = input("\nSelect an output stem or channel by entering its number: ").strip()
        if bus_selection.isdigit():
            bus_num = int(bus_selection) - 1
            if not (0 <= bus_num < num_buses):
                print("Invalid bus number.")
                return
        else:
            print("Invalid input.")
            return

    # Process the file
    hance.process_file(
        model_file_path=selected_model,
        input_file_path=input_file_path,
        output_file_path=out_file_path,
        selected_output_bus=bus_num
    )

    print("\nFile successfully processed.")
    print("Output file:", out_file_path)

if __name__ == "__main__":
    main()
