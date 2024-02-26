# EasySLM

*All code was tested and should work on versions 2019a-2022a of Matlab.

Matlab code for easily using an SLM to create extended patterns or optical tweezers. This code is dependent on OTSLM which can be downloaded from GitHub here https://github.com/ilent2/otslm. Make sure the folder is in your Matlab path and that the Matlab toolboxes OTSLM uses are installed. Additonally, you will should install the required Hardware Support Packages (https://www.mathworks.com/help/imaq/supported-hardware.html) for the camera to work properly through Matlab.

Files: </br>
    - Config.m : Implements a class that simply stores the parameters of an SLM setup. You can add more setups as static class methods. </br>
    - SLM.m : Extends OTSLM ScreenDevice class and acts as a superclass for displaying images to an SLM. </br>
    - CGH.m : Implements Gerchberg-Saxon algorithm for arbitrary phase patterns. </br>
    - DHOT.m : Repackages Dulara's code for creating optical tweezer arrays for compatibility. </br>
    - Patterns.m : Class for containing functions for creating miscellaneous patterns to find phase masks for. </br>
    - RunSLM.mlx : A livescript for finding the phase masks of extended patterns and displaying them to a SLM. </br>
    - RunDHOT.mlx : A livescript for creating optical tweezer arrays and displaying them to a SLM. </br>
    - aberration_correction.m : Functions needed for Dulara's aberration correction routine.
