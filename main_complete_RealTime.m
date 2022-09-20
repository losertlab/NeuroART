close all
clearvars

addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

% Check for or initialize config file.
configFileName = 'neuroArtConfig.json';
if ~isfile(configFileName)
    (neuroArtConfigInit(configFileName));
    fprintf("Please update your neuroArtConfig.json");
    return;
end
inputConfig = extractJSON(configFileName);

% Initialize python environment.
pythonInit();

genericErrorMessage = 'Error: ';

%%% Create struct to parse input variables to RT app 09/01/2022

%% Input parameters for NeuroART

try
    inputParams = inputDialogRT(inputConfig); % get input parameters from the user
catch exception
    disp([genericErrorMessage, exception.message]);
    return;
end

%% SLM Initialization (for photostimulation when the SLM is available)

slmParams = slmInit(inputParams);

%% Initial Batch Settings
try
    batchSettings = batchSettingsDialog(inputParams); % initial batch settings
catch exception
    disp([genericErrorMessage, exception.message]);
    return;
end

rfParams = rfDialog(inputParams);

%% Reading experimental parameters and motion correction of the initial batch
try
    exptVars = exptVarsInit(inputParams, batchSettings);
catch exception
    disp([genericErrorMessage, exception.message]);
    return;
end

%% Read the frames in iteratively

ist = tic;
fprintf('Image registration started ... \n');

try
    batchResults = readInitialBatch(inputParams, batchSettings, exptVars);
catch exception
    msgbox(exception.message,'Error','error');
    disp([genericErrorMessage, exception.message]);
    return;
end

[imTemplate, regImg, normMeanRedImg] = registerImages(inputParams, batchSettings,batchResults, exptVars, ist);
    
tstop = toc(ist);
fprintf('Initial acquisition and registration took %.4f seconds\n',tstop);
    
%% Detect centroids of the cells (ROIs)

iStart = tic;
% promptMessage = sprintf('Select the method for cell finding');
% button = questdlg(promptMessage, 'Image Registration Completed', 'Manual', 'CaImAn', 'Cite-on', 'CaImAn'); % can't have more than 3 options
cell_centroids = detectCellCentroids(batchSettings, exptVars, inputParams, regImg);
%%
close(gcf);

fprintf('Total number of cells detected: %d \n',length(cell_centroids));
fprintf('Cell finding took %.4f seconds\n',toc(iStart));

delete(gcp('nocreate'));
    
%% Compute DF/F

neuropilSubPercent = 70; % use this default value for now
if inputParams.ROI == 1 % no filled ROIs
%     [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(pwd,inputParams.EXID,regImg,exptVars,cell_centroids,imTemplate,exptVars.rPixels,exptVars.dffWindow/2,inputParams.fluorPercentile); 
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(regImg,exptVars.frameRate,cell_centroids,exptVars.rPixels,floor(exptVars.dffWindow/2),inputParams.fluorPercentile,neuropilSubPercent);   
else
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_filled(regImg,exptVars,cell_centroids(:,2),cell_centroids(:,1),exptVars.rPixels,floor(exptVars.dffWindow/2),inputParams.fluorPercentile); 
end

%clear regImg

tstop = toc(ist);
fprintf('total execution time for cell finding is %.4f seconds\n',tstop);

%% Real Time Visualization

mst = 1;
mstWin = 0; % This default value corresponds to cumulative analysis
if inputParams.NET == 2 % Correlations method
    mst = 0;
end

if inputParams.CWIN == 2 % Using a shorter (fixed) window size
    shortWindow = 1;
    mstWin = floor((inputParams.CWINF)*exptVars.frameRate); % max. allowed size = currently available number of frames
end
    

%% Check whether enough number of frames are available

numFrames = batchResults.frameId - 1; % number of frames read upto now
if (inputParams.DISPLAY > numFrames)
    promptMessage = sprintf('DF/F display window is larger than the available number of frames');
    msgbox(promptMessage,'Error','error');  
	return;
end

if (mstWin > numFrames)
	promptMessage = sprintf('Window size for correlation analysis exceeds the available number of frames');
	msgbox(promptMessage,'Error','error');  
	return;
end

r_display = floor(exptVars.rPixels*0.4); % display smaller size patches


%% Create struct of input parameters for real time analysis

RTparams = struct;
RTparams.imTemplate = imTemplate;
RTparams.DFF = DFF;
RTparams.npBWout = npBWout;
RTparams.rawFluo = fluoAllSmooth';
RTparams.roiBW2 = roiBW2;
RTparams.xcRaw = xcRaw;
RTparams.ycRaw = ycRaw;
RTparams.norm_meanIMG = norm_meanIMG;
RTparams.symmFLAG = inputParams.symmFlag;
RTparams.smoothwin = inputParams.smoothWin;
RTparams.r = r_display;
RTparams.dffwindow = exptVars.dffWindow;
RTparams.percent = inputParams.fluorPercentile;
RTparams.last_init_frame = batchSettings.END;
RTparams.displayWin = inputParams.DISPLAY;
RTparams.batch_size = batchSettings.BSIZE;
RTparams.gap = batchSettings.GAP;
RTparams.exptId = inputParams.EXID;
RTparams.exptVars = exptVars;
RTparams.minNpSubFluo = minNpSubFluo;
RTparams.maxAdjF = maxAdjF;
RTparams.numInitFrames = batchSettings.numInitFrames;
RTparams.mst = mst;
RTparams.mstWin = mstWin;
RTparams.numAvailFrames = numFrames;
RTparams.imagingFreq = exptVars.frameRate;
RTparams.thorSyncFile = rfParams.thorSyncFile;
RTparams.psignalFile = rfParams.psignalFile;
RTparams.rawFileHandle = batchResults.fh;
RTparams.greenChannel = batchSettings.GREENCHAN;
RTparams.numChannels = inputParams.NUMCHAN;
RTparams.scope = inputParams.SCOPE;
RTparams.imgType = inputParams.FORMAT;
RTparams.ImageFile = inputParams.imageFile;
RTparams.JiSignalInfo = rfParams.JiSignalInfo;
RTparams.norm_meanRedIMG = normMeanRedImg;
RTparams.tstack = batchResults.tStack;
RTparams.dll_name = slmParams.dllName;
RTparams.board_number = slmParams.boardNumber;
RTparams.SLM_ON = slmParams.slmOn;

%% Real time analysis

realTimeApp(RTparams);
rmpath(genpath('utilities'));
rmpath(genpath('Psignal')); % path to Psignal folder