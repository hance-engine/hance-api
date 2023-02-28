Welcome to the Hance APIs
=========================
​
HANCE offers top-quality signal-processing techniques developed by machine learning specialists, sound engineers, and audio processing experts. Our technology is designed to provide users with the highest possible audio experience by removing noise, reverb, and other signal impairments in real time. 

**Why use HANCE?**
* Small footprint
* Light on CPU
* No GPU requirements
* Low latency
* Cross-platform
* Easy to integrate


C API
-----
You can find HTML documentation for the C API in the **Documentation** folder. The HANCE C API is designed to be as simple as possible. The **ProcessFile** example (see the **Examples** folder in the API) illustrates how to create a HANCE processor and process audio with it. [CMake 3.0](https://cmake.org/) or later is required to build the example. To build **ProcessFile**, open the terminal / command line prompt and locate the **Examples/ProcessFile** subfolder in the HANCE API. Now, please type ```./Build.sh``` on Mac or Linux, or ```Build.bat``` on Windows. 

Python API
----------
The Python API is included in this repository, but we recommend using **pip**:

```
python -m pip install hance
```
HANCE is compatible with Python 3 and later. You can find further documentation on the [HANCE PyPi page](https://pypi.org/project/hance/).

If you don't want to use **pip**, you will need to run a setup script first. Please navigate to the **PythonAPI** folder in the terminal and type:
```
setup.py
```

Included Models
---------------
The HANCE API is shipped with different models trained at solving specific tasks. You can find these pre-trained models in the **Models** folder. At the time being, these models are available:
* **Speech Denoise** (filename ```speech-denoise.hance```) is trained to remove all kinds of background noise from speech.
* **Speech De-reverberation** (filename ```speech-dereverb.hance```) is trained to remove unwanted reverberation from speech.

Contact HANCE
-------------

The HANCE team is excited to have you join us and looks forward to helping you
get started. Please feel free to [contact us](https://hance.ai/contact) with any questions or concerns about implementation. Our team is available to help as needed.

We thank you for your interest!