# NeuroART  

#### Real-Time Analysis of calcium imaging of neuronal population activity for informed closed-loop activities

## System Requirements:
- Anaconda 3;
- Python 3.7 (or newer);
- MATLAB 2019b (or newer);
- MATLAB add-ons:
    - Image Processing Toolbox,
    - Statistics and Machine Learning Toolbox,
    - Bioinformatics Toolbox,
    - Parallel Computing Toolbox,
    - Signal Processing Toolbox,
    - MATLAB support for MinGW-w64 C/C++ Compiler (for Windows)
- Minimum of 10 Gb of space on hard drive;
- (Optional) NVIDIA GPU with CUDA 10.2

(Make sure all the relevant variables are added to the PATH.)
e.g. The following should be listed:
- C:\User\anaconda3\Library\bin\
- C:\User\anaconda3\Scripts\ 
- C:\User\anaconda3\


## Installation 

Clone this repository.

### Modules

[CaImAn](https://caiman.readthedocs.io/en/master/index.html) should be already functional from the code.

#### Installing Cite-on:

Open cmd in the NeuroART folder and run the command:
- `git clone https://gitlab.iit.it/fellin-public/cite-on.git`

Now there should be a folder called cite-on. Rename that folder to citeon. (Python doesn't like files with dashes.)

- `conda env create -f citeonEnv.yml`
- `conda activate citeon`
- `conda install libpython m2w64-toolchain -c msys2`
- `cd citeon`
- `python setup.py build_ext --inplace`
- `pip install .`

## Instructions 



