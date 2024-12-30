function xyDMD = CamToDMD(xc,yc)


     %% The DMD coordinates does not work as expected when we use the ROI option. Therefore, a direct FOV to DMD calibration is required.

    FOV2DMD = load(['.' filesep 'calibration' filesep '20241217_FOVDMDCalib.mat']); % 4x4 binning Full FOV (800x800) to DMD coordinates
    
    neuronXY = [xc yc];

    FOVToDMD = FOV2DMD.trafos.cameraToSlm;
    [xCam, yCam]=tforminv(neuronXY,FOVToDMD);
    xyDMD = [xCam yCam];



end