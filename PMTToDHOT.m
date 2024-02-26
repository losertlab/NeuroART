function P_DHOT = PMTToDHOT(xc,yc)
    
%     calibrationFile = load('PMTToSlm_x7936y9462_3x.mat');
%     cameraToSlm = calibrationFile.cameraToSlm;

%     For the Hamamatsu camera
%     calibrationFile = load('20220812_x7588_y9446_L4_100spacing_vL600_xs25_ys135_tip-0.0075_tilt-0.015_ast2_0.015_CamSLMCalib.mat');
%     calibrationFile = load(['.' filesep 'calibration' filesep '20221215_CamSLMCalib.mat']);

    % For PMTs (While using ScanImage)

%     calibrationFile = load(['.' filesep 'calibration' filesep '20230118_PMTSLMCalib_x7635y9420_2x.mat']); % for 2x zoom factor
%     calibrationFile = load(['.' filesep 'calibration' filesep '20230118_PMTSLMCalib_x7635y9420_3x.mat']); % for 3x zoom factor

    PMT2Cam = load(['.' filesep 'calibration' filesep '20240212_PMTCamCalib.mat']); % PMT to Camera calibration file
    Cam2SLM = load(['.' filesep 'calibration' filesep '20240212_CamSLMCalib.mat']); % Camera to SLM calibration file

    neuronXY = [xc yc];

    %% First transform coordinates of the PMT image to Hamamatsu camera coordinates (skip this for 1P images)

    PMTToCam = PMT2Cam.trafos.cameraToSlm;
    [xCam, yCam]=tforminv(neuronXY,PMTToCam);
    neuronXY = [xCam yCam];

    %%

    CamToSLM = Cam2SLM.trafos.cameraToSlm;
    [xSLM, ySLM]=tforminv(neuronXY,CamToSLM);

%     xDHOT=-2.245*(xSLM-541)/540; % hard coded  by Samira 20191001
%    
%     yDHOT=-2.245* (ySLM-541)/540; 
%     P_DHOT = [xDHOT yDHOT];
%     

%% 20211028

% 30 units in the table --> 122 pixels in theoritical intensity image (2304x2304)
% 1 pixel --> 30/122 units

    xDHOT= 0.2833*(xSLM-1153)/1152; %DZ 10/21
    yDHOT= 0.2833* (ySLM-1153)/1152; 
    P_DHOT = [xDHOT yDHOT];
    
%     xDHOT= -2.245*(xSLM-1153)/1152; %DZ 10/21
%     yDHOT= -2.245* (ySLM-1153)/1152; 
%     P_DHOT = [xDHOT yDHOT];

end
    