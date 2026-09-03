# Raspberry Pi setup for the Rapidly SDK

1. Download and run the Raspberry Pi Imager:
   * Follow the instructions on [Raspberry Pi website](https://www.raspberrypi.com/software/)
   
2. Select "Raspberry Pi OS with desktop and recommended software" (64 bit) and install it on an empty SD card.

3. Select Yes to "Would you like to apply OS customization settings?", then click "Edit settings":
   * Choose user credentials and WiFi settings
   * Write the image to the SD card

4. Login to the Raspberry Pi and:
   * Setup git: [Github website](https://docs.github.com/en/get-started/getting-started-with-git/set-up-git)

5. Fetch the Rapidly SDK:
   * `git clone https://github.com/rapidly-labs/rapidly-sdk`
   * `cd rapidly-sdk`

6. Build the demo program (compiles `ProcessFile`, a small example that uses Rapidly):
   * `sh examples/build-examples.sh`

7. Run the demo on any short `.wav` you have:
   * `./examples/build/Bin/ProcessFile models/speech-denoise-32ms.v1.1.rapidly <path-to-your-input.wav> output.wav`

The processed `output.wav` will appear in the repo root. Without a Rapidly licence key (passed as an optional fourth argument), the output is intentionally watermarked.

And you're good to go.
