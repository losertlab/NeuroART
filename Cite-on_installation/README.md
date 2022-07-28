# CITE-On: Cell Identification and Trace Extraction Online
**CITE-ON** is a new tool for online and offline analysis of functional 2-photon calcium imaging of neurons
### System Requirements
 - Anaconda 4.6 or greather
 - (Optional) NVIDIA GPU with CUDA 10.2 

### Installation
Clone this repository then install all required dependencies by creating a new environment with the provided env.yml 
```
    conda install -f env.yml
```
this will take care of the eventual GPU dependencies needed, then build cython dependencies
```
    python setup.py build_ext --inplace
```
### Usage
Check the provided notebooks to run through some use case scenarios, docstrings will be your friends!
 

