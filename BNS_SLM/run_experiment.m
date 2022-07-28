%clear;

% This is where you set the name of the experiment. After you're
% done running the experiment, a MATLAB structure with experiment
% parameters will be saved to the file experiment_name.mat.
experiment_name = 'untitled';

% Set 'flashMask' to true if you want to flash the hologram, and
% set the parameters to set how long you want it to flash for with
% the other parameters.
flashMask = false;
flashInterval = 2; % seconds
flashDuration = 0.5; % seconds

% These are the dimensions of the SLM.
m = 1080;
n = 1920;

% These variables control with images to read for the different
% steps of calibration. Notes:
% 
% - fakePointPatternImg is a fake image which has the calibration
%   pattern with coordinates normalized to [-1, 1] x [-1, 1]
% 
% - pmtTargetImg can be a RAW file or an image (e.g. a TIFF)
% 
% - the other four files must be images, not RAW
% 
cameraPointPatternImg = 'Spattern.tif';
fakePointPatternImg = 'dots.tif';
cameraNaturalImg = 'rCam.tif';
pmtNaturalImg = 'rPMT.tif';
pmtTargetImg = 'pmt.tif';

% The path to the file which stores calibration results for future
% use.
calibrationFilePath = 'trafos.mat';

% This section tries to either load a saved calibration file, or---if
% there is no file---has the user do the calibration phase (and then
% save the results in a MATLAB file).
if exist(calibrationFilePath, 'file') == 2
    load(calibrationFilePath);
    cameraToSlm = trafos.cameraToSlm;
    cameraToPmt = trafos.cameraToPmt;
else
    [cameraToSlm, cameraToPmt] = calibrate(...
        cameraPointPatternImg, ...
        fakePointPatternImg, ...
        cameraNaturalImg, ...
        pmtNaturalImg);
    verify_transforms(...
        cameraToSlm, ...
        cameraToPmt, ...
        cameraPointPatternImg, ...
        fakePointPatternImg, ...
        pmtNaturalImg);
    trafos = struct;
    trafos.cameraToSlm = cameraToSlm;
    trafos.cameraToPmt = cameraToPmt;
    save(calibrationFilePath, 'trafos');
end

% This function has the user select the location of optical trapssaghay
% from which to generate a hologram. The returned variables are:
% 
% - P: contains optical trap locations in the range [0, 1] x [0, 1]
% - neuronXY: contains optical trap locations in PMT space
% 
[P, neuronXY] = select_optical_trap_points(pmtTargetImg, cameraToSlm, ...
                                           cameraToPmt);

% This function visualizes the selected traps in the camera space by
% displaying them over the picture of the calibration pattern taken by
% the camera.
display_slm_points_in_camera_space(P, cameraToSlm, cameraToPmt, ...
                                   cameraPointPatternImg, ...
                                   fakePointPatternImg);

% Actually calculate the hologram using the method in the "dynamic
% holographic optical trap" (DHOT) paper.
% 
% The trap locations are renormalized so that they lie in the domain
% [-1, 1] x [-1, 1].
% 
mask = dhot(2*[1 - P(:, 1), P(:, 2)] - 1);

% Save experiment data to disk for later.
experiment = struct;
experiment.name = experiment_name;
experiment.P = P;
experiment.neuronXY = neuronXY;
experiment.cameraToSlm = cameraToSlm;
experiment.cameraToPmt = cameraToPmt;
experiment.mask = mask;
save(strcat(experiment_name, '.mat'), 'experiment');

% Display the hologram...
if flashMask
    flash_mask(mask, flashInterval, flashDuration);
else
    display_mask(mask);
end
