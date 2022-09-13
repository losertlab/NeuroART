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

Open cmd in the NeuroART folder and run the following commands:
- `git clone https://gitlab.iit.it/fellin-public/cite-on.git`
- `ren cite-on citeon` (Python doesn't like files with dashes.)
- `conda env create -f citeonEnv.yml`
- `conda activate citeon`
- `conda install libpython m2w64-toolchain -c msys2`
- `cd citeon`
- `python setup.py build_ext --inplace`
- `pip install .`

## Instructions 

1. Open the NeuroART folder in MATLAB;
2. Run the main script "main_complete_RealTime.m";
3. The first input dialog will appear on the screen as shown below.

<img src="images/dialog1"  width="100" height="200">

- *Average neuron radius*: in microns
- *DF/F display width*: The number of frames of which you would like to observe the ΔF/F traces at a given time. (i.e., The length of the ΔF/F traces to be displayed in the app window). Please make sure that this number does not exceed the number of frames you are planning to collect as the initial batch for identifying cell locations.
- *Experiment ID*: Output variables will be saved as a .mat file under this name.
- *Image folder*: The folder where the image frames will be saved in real-time.
- *Image file name*: The name of the image file (This can be either a RAW file or a TIF stack). It is not necessary to have this file while providing input parameters. The app will wait until this file appears in the folder specified as the “Image folder”.
- *Image format*: RAW/TIF
- *Imaging system*: The name of the imaging system. If all the images are already available and you
only need to analyze data, please select “Offline Mode”.
- *Number of channels*: For single channel data, select 1. Otherwise, select the number of channels available in the acquired images.
- *Analyzing Receptive Fields?* Select “Yes” if you are planning to perform tuning analysis.
- *Red Channel Available?* Select “Yes” if you are using red channel (static channel) for cell detection 
- *Cell Finding Method*: Four options available for cell finding: Manual, CaImAn, Cite-On, and from file (previously saved coordinates)
- *ROI Type*: By default, this app performs calcium image analysis on donut shaped (not filled) regions of interest (ROIs) around each of the identified cells (neurons). If your cells have filled ROIs, select “Filled”
- *Network Analysis Method*: During the real-time analysis loop, the app will rank the cells according to their degree in the functional network, which is derived based on the correlation between each pair of ΔF/F traces. Currently, we derive this network based on two algorithms, minimum spanning tree method (“MST”) and the correlation threshold method (“Correlations”). Select the method you prefer.
- *Correlation Analysis Window*: By default, correlations are calculated for a cumulative window (consider all the frames acquired until the latest frame). However, if you want to only consider short term correlations between the ΔF/F traces, select “Fixed”. Otherwise, select “Cumulative” to continue the analysis on a cumulative window.
- *Correlation Window Size*: If you selected “Fixed” above, please specify the preferred window size
(in seconds).

After inputting all the parameters, click “Continue”.

4. Next, specify the initial batch of frames (IDs of starting and ending frames) for cell identification. Make sure that the *length of the initial batch* is greater than or equal to the *DF/F display width*. Also, provide the *maximum number of frames* that will be collected during the experiment. This will be used as an upper bound for the total number of frames. Therefore, just provide an approximate number for this field.

IMAGE HERE

- *Figure updating frequency*: How frequently the figures/plots need to be updated in the GUI. (e.g., every 30 frames)
- *Green channel ID*: For single channel data this field will be set to 1. For multi-channel data, specify which channel is the green channel. (e.g., Consider a two-channel dataset where the green channel is the second one. Then, green channel ID = 2)

5. If you selected “Yes” for analyzing receptive fields, you will be prompted to locate the “thorSync”
file and the “pSignal” file as shown below. 

IMAGE HERE

6. Make sure that the image acquisition has started. Wait until the app recognizes the image files and the initial batch of frames. If you opted for using the red (static) channel for cell identification, you will be prompted to provide the directory path and the file identifier for red channel data.

7. If you selected the manual cell finding method on step 3, here you will manually select the cells which are visible in the mean image. Press the “Enter” key once
cell clicking is complete. Press the “delete” key if you want to undo the previous click.

IMAGE

8. Wait until all the frames in the initial batch are motion corrected, denoised and ΔF/F traces are calculated for all the identified cells. After completion of these steps, you will be prompted to the dialog box shown below. If you want to continue analyzing the next incoming frames in real-time, click “Yes”. This is the beginning of the real-time loop of the NeuroART pipeline.

9. After completing all the steps, the following window will appear. Select, “Start Real Time Analysis”
to continue with the real-time analysis!

10. The figure below indicates different features of the NeuroART GUI, which can be useful during the real-time analysis.

Use the slider at the bottom left corner to adjust the correlation analysis window size while the images are being acquired.
Tuning analysis module can be turned on/off using the checkbox in the GUI.

11. After the completion of an experiment, all the frames can be saved with labels in the TIF file format. (This is optional)

12. After closing the app window, all the output variables can be found in a .mat file under the name
provided as “Experiment ID”.




