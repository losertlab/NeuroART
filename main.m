close all
clearvars

addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

% For debugging:
verbose = 1; % 1 for printing command window outputs; 0 for suppressing.

%% OASIS-spike deconvolution setup

addpath(fullfile('OASIS_matlab','functions'));
addpath(fullfile('OASIS_matlab', 'packages', 'oasis'));
addpath(fullfile('OASIS_matlab', 'packages', 'oasis_kernel'));
addpath(fullfile('OASIS_matlab', 'packages', 'constrained-foopsi'));
addpath(fullfile('OASIS_matlab', 'packages', 'MCMC'));
addpath(fullfile('OASIS_matlab', 'packages', 'MCMC', 'utilities'));
addpath(fullfile('OASIS_matlab'));

%%

% Check for or initialize config file.
configFileName = 'neuroArtConfig.json';
if ~isfile(configFileName)
    (neuroArtConfigInit(configFileName));
    fprintf("Please update your neuroArtConfig.json \n");
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

slmParams = slmInit(inputParams); % for Boulder Non-linear Systems (BNS) SLM

%% Initial Batch Settings
try
    batchSettings = batchSettingsDialog(inputParams); % initial batch settings
catch exception
    disp([genericErrorMessage, exception.message]);
    return;
end

rfParams = rfDialog(inputParams); % select files for analyzing receptive fields (if available)

%% Reading experimental parameters and motion correction of the initial batch

try
    inputParams.frameRate = 15; % frameRate = 10, for images acquired from micromanager
    [exptTextOutput, exptVars] = evalc('exptVarsInit(inputParams, batchSettings)');
    if verbose, disp(exptTextOutput), end
catch exception
    disp([genericErrorMessage, exception.message]);
    return;
end

%% Read the frames in iteratively

time = tic;
if verbose, fprintf('Image registration started ... \n'),end

% this function is quite large. Should be broken up into smaller pieces.
[IMG, wait, frameid, fh,tstack] = readInitialBatch(inputParams,batchSettings,exptVars);

if wait == 10000
    promptMessage = sprintf('Number of acquired images is insufficient to achieve the specified size of the initial batch');
    msgbox(promptMessage,'Error','error');  
    return;
end

aquisitionTime = toc(time);
if verbose, fprintf('Initial aquisition took %.4f seconds\n',aquisitionTime), end

% motion correction should  be independent of most experimental variables.
% We should adapt the inputs for this function to be reused outside of neuroart.
[regImg,imTemplate,norm_meanRedIMG] = motionCorrection(IMG,exptVars,inputParams,batchSettings);

% Mean image and normalized mean image for cell center clicking and GUI:
meanIMG = mean(double(regImg),3); 
normMeanIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));
% this used to be redIMG, or norm_redMeanIMG. Is there a use for this that is separate for the red channel? 
    
registrationTime = toc(time);
if verbose, fprintf('Registration took %.4f seconds\n',registrationTime - aquisitionTime), end
    
%% Detect centroids of the cells (ROIs)

if inputParams.CFIND == 1 % Manual
    figure; imagesc(normMeanIMG); colormap('gray'); axis('square'); caxis([0 0.75]);
    selectText = ['Click on Neuron Centers...' newline 'Press Enter after all the cells are selected' newline 'Press delete if you want to deselect the last selected cell'];
    if verbose, disp (selectText),end
    [xc, yc] = getpts; %  manually select centers of the neurons
    cell_centroids(:,1) = yc;
    cell_centroids(:,2) = xc;
    clear xc yc
elseif inputParams.CFIND == 4 % From File
    [file,path] = uigetfile; % select the .mat file which contains the variable, "ptsIdx": column 2 --> x, column 3 --> y coordinates
    if file == 0
        fprintf('No file was selected ... \n');
        return;
    else
        load([path file],'cell_centroids');
    end
elseif inputParams.CFIND == 3 % Cite-on
%     system('activate cite-on & python test.py')
    ups = 2.0; % upscaling factor
    th = 0.5;  % threshold
    [~,cmdOutput] = system(['activate cite-on & python citeon.py' ' -x ' num2str(exptVars.dimX) ' -y ' num2str(exptVars.dimY) ' -n ' num2str(length(batchSettings.frameBlock)) ' -p ' inputParams.imageFile ' -u ' num2str(ups) ' -t ' num2str(th)]);
    T = readtable('cell_coordinates.csv');
    clear cell_centroids; % clear previously loaded cell coordinates
    cell_centroids(:,1) = T.Var2; %yc
    cell_centroids(:,2) = T.Var1; %xc 
elseif inputConfig.python.use_python && inputParams.CFIND == 2
    cell_centroids = locateCentroids(inputConfig.python.complex_dynamics_path, inputConfig.python.env);
else
    caimanParams.tau = exptVars.rPixels;    % number of components to be found *
    caimanParams.K = 185;                   % number of components to be found *
    caimanParams.decayTime = 50;            % typical transient time  *
    caimanParams.pOrder = 2;                % order of AR dynamics    *
    caimanParams.nBackground = 2;           % number of background components    *
    caimanParams.minSnr = 2;                % minimum SNR threshold
    caimanParams.mergeThr = 0.8;            % merging threshold       *
    caimanParams.spaceThr = 0.3;            % space correlation threshold (dataset3 --> 0.3)
    caimanParams.cnnThr = 0.2;              % threshold for CNN classifier 
    caimanParams.frameRate = exptVars.frameRate;
    
    if ~verbose
        [caimanTextOutput,cell_centroids,~,~,~] = evalc('CaImAn_CellFinder(regImg,caimanParams)'); % suppressing printed statements
    else
        [cell_centroids,~,~,~] = CaImAn_CellFinder(regImg,caimanParams); % CaImAn cell finder
    end
    
end
%%
close(gcf);

numCells = length(cell_centroids);
segmentationTime = toc(time);
    
if verbose
    fprintf('Total number of cells detected: %d. \n',numCells);
    fprintf('Cell finding took %.4f seconds.\n',segmentationTime - registrationTime);
end

delete(gcp('nocreate'));
    
%% Compute DF/F

neuropilSubPercent = 70; % use this default value for now

% shorten parameter list (create struct dffInput, dffOutput...)
% need work on the computeDFF functions. There is a lot to be done here.

if inputParams.ROI == 1 % no filled ROIs
%     [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(pwd,inputParams.EXID,regImg,exptVars,cell_centroids,imTemplate,exptVars.rPixels,exptVars.dffWindow/2,inputParams.fluorPercentile); 
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(regImg,exptVars.frameRate,cell_centroids,exptVars.rPixels,floor(exptVars.dffWindow/2),inputParams.fluorPercentile,neuropilSubPercent);   
else
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_filled(regImg,exptVars,cell_centroids(:,2),cell_centroids(:,1),exptVars.rPixels,floor(exptVars.dffWindow/2),inputParams.fluorPercentile); 
end

computedffTime = toc(time);
if verbose
    fprintf('dF/F computation took %.4f seconds. \n',computedffTime-segmentationTime);
    fprintf('Total execution time for cell finding is %.4f seconds.\n',computedffTime);
end

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

numFrames = frameid - 1; % number of frames read upto now
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

rDisplay = floor(exptVars.rPixels*0.4); % display smaller sized patches


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
RTparams.filled = (inputParams.ROI == 2); % true if the image consists of filled ROIs
RTparams.r = rDisplay;
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
RTparams.rawFileHandle = fh;
RTparams.greenChannel = batchSettings.GREENCHAN;
RTparams.numChannels = inputParams.NUMCHAN;
RTparams.scope = inputParams.SCOPE;
RTparams.imgType = inputParams.FORMAT;
RTparams.ImageFile = inputParams.imageFile;
RTparams.JiSignalInfo = rfParams.JiSignalInfo;
RTparams.norm_meanRedIMG = norm_meanRedIMG;
RTparams.tstack = tstack;
RTparams.dll_name = slmParams.dllName;
RTparams.board_number = slmParams.boardNumber;
RTparams.SLM_ON = slmParams.slmOn;

if inputParams.SLM == 2
    RTparams.SLM = "BNS";
elseif inputParams.SLM == 3
    RTparams.SLM = "PrairieLink";
else
    RTparams.SLM = "None";
end

%% Real time analysis

realTimeApp(RTparams);
rmpath(genpath('tools'));
rmpath(genpath('Psignal')); % path to Psignal folder