import os
import platform
import shutil
from sys import argv, executable

from setuptools import setup

os.chdir(os.path.abspath(os.path.dirname(__file__)))

if not os.path.exists(os.path.join("hance", "bin")):
    # We run from the source folder in the github repo
    # We need to copy the Bin and Models folder to the hance folder

    base_path = os.path.abspath(os.path.dirname(__file__))
    up_one_folder = os.path.split(base_path)[0]
    # up_two_folders = os.path.split(up_one_folder)[0]

    shutil.copytree(os.path.join(up_one_folder, "Bin"), os.path.join(base_path, "hance", "bin"))
    shutil.copytree(
        os.path.join(up_one_folder, "Models"), os.path.join(base_path, "hance", "models")
    )


def read(filename):
    try:
        with open(filename, "rb") as fp:
            data = fp.read().decode("utf-8")
    except UnicodeDecodeError:
        with open(filename, "r") as fp:
            data = fp.read()
    return data


def find_version(file_path):
    version_file = read(file_path)
    import re

    version_match = re.search(
        r"^__version__ = ['\"]([^'\"]*)['\"]",
        version_file,
        re.M,
    )
    if version_match:
        return version_match.group(1)

    raise RuntimeError("Unable to find version string.")


def get_package_data_list():
    files_to_include = []

    dirs_to_ignore = ["__pycache__", "bin"]
    files_to_ignore = []
    # Include all files.
    for root, dirs, files in os.walk("hance"):
        for file in files:
            ignore = False
            for dir in root.split(os.sep):
                if dir in dirs_to_ignore:
                    ignore = True

            if file.startswith("."):
                ignore = True
            if file in files_to_ignore:
                ignore = True

            if not ignore:
                files_to_include.append(os.path.join(root, file))

    system_name = platform.system()
    if system_name == "Windows":
        system_name += str(platform.architecture()[0])

    if "--plat-name=manylinux1_x86_64" in argv:
        system_name = "Linux"
    elif "--plat-name=macosx_10_9_x86_64" in argv:
        system_name = "Darwin"
    elif "--plat-name=macosx_11_0_arm64" in argv or "--plat-name=osx-arm64" in argv:
        system_name = "Darwin"
    elif "--plat-name=win_amd64" in argv:
        system_name = "Windows64bit"
    elif "--plat-name=win32" in argv:
        system_name = "Windows32bit"

    bin_path = "bin"
    files_to_include.append("models/*")

    if system_name == "Windows32bit":
        files_to_include.append(os.path.join(bin_path, "Windows", "HanceEngine.dll"))
    elif system_name == "Windows64bit":
        files_to_include.append(os.path.join(bin_path, "Windows_x64", "HanceEngine.dll"))
    elif system_name == "Darwin":
        files_to_include.append(os.path.join(bin_path, "macOS", "libHanceEngine.dylib"))
    elif system_name == "Linux":
        files_to_include.append(os.path.join(bin_path, "Linux", "libHanceEngine.so"))

    if not files_to_include:
        raise Exception(
            "Your system is currently not supported. The HANCE Engine can be built to run on most systems. Please contact us for a custom build."
        )

    return files_to_include


setup(
    name="hance",
    version=find_version("hance/version.py"),
    python_requires="!=2.*,>=3.0",
    description="Audio Enhancement in Python with HANCE",
    long_description=read(os.path.join(os.getcwd(), "README.md")) + "\n",
    long_description_content_type="text/markdown",
    author="HANCE",
    author_email="mail@hance.ai",
    keywords="hance audio enhancement",
    packages=["hance"],
    package_dir={"hance": "hance"},
    # package_data={'hance': ['models/*', 'bin/linux/*']},
    include_package_data=True,
    package_data={"hance": get_package_data_list()},
    # exclude_package_data={'hance': ['hance/bin/Windows_x64/HanceEngine.dll']},
    install_requires=["setuptools"],
    url="https://hance.ai",
    setup_requires=["setuptools", "wheel"],
    license="Other/Proprietary License",
    options={
        "build_scripts": {
            "executable": executable,
        },
    },
    zip_safe=False,
    # https://pypi.python.org/pypi?%3Aaction=list_classifiers
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "Intended Audience :: Information Technology",
        "Intended Audience :: Other Audience",
        "Intended Audience :: Science/Research",
        "Intended Audience :: System Administrators",
        "Topic :: Software Development",
        "Topic :: Software Development :: Libraries",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: System :: Software Distribution",
        "Topic :: Utilities",
        "Topic :: Multimedia :: Sound/Audio",
        "Topic :: Multimedia :: Sound/Audio :: Analysis",
        "License :: Other/Proprietary License",
        "Operating System :: MacOS",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX :: Linux",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: Implementation :: CPython",
    ],
)
