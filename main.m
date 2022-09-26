close all
% clear

addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

% for debugging:
verbose = 0; % 1 for printing command window outputs; 0 for suppressing.

%%% Create struct to parse input variables to RT app 09/01/2022

%% Input parameters for NeuroART

inputParams = inputDialogRT(); % get input parameters from the user
if inputParams.canceled
    return;
end

%% SLM Initialization (for photostimulation when the SLM is available)

inputParams = slmInit(inputParams);

%% Initial Batch Settings

batchSettings = batchSettingsDialog(inputParams); % initial batch settings
if batchSettings.canceled
    return;
end

rfParams = rfDialog(inputParams);

%% Reading experimental parameters and motion correction of the initial batch

[exptTextOutput, exptVars] = evalc('exptVarsInit(inputParams, batchSettings)');
if verbose, disp(exptTextOutput), end
if (exptVars.error)
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

%motion correction should  be independent of most experimental variables.
%We should adapt the inputs for this function to be reused outside of
%neuroart.
[RegIMG,imTemplate] = motionCorrection(IMG,exptVars,inputParams,batchSettings);

% Mean image and normalized mean image for cell center clicking and GUI:
meanIMG = mean(RegIMG,3); 
normMeanIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));
% this used to be redIMG, or norm_redMeanIMG. Is there a use for this that
% is separate for the red channel? 
    
registrationTime = toc(time);
if verbose, fprintf('Registration took %.4f seconds\n',registrationTime - aquisitionTime), end
    
%% Detect centroids of the cells (ROIs)

if inputParams.CFIND == 1 % Manual
    figure; imagesc(normMeanIMG); colormap('gray'); axis('square')
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
        load([path file],'ptsIdx');
        cell_centroids(:,1) = ptsIdx(:,2); %yc
        cell_centroids(:,2) = ptsIdx(:,3); %xc
    end
elseif inputParams.CFIND == 3 % Cite-on
%%     system('activate cite-on & python test.py')
    ups = 2.0; % upscaling factor
    th = 0.5;  % threshold
    [~,cmdOutput] = system(['activate cite-on & python test.py' ' -x ' num2str(exptVars.dimX) ' -y ' num2str(exptVars.dimY) ' -n ' num2str(length(batchSettings.frameBlock)) ' -p ' inputParams.imageFile ' -u ' num2str(ups) ' -t ' num2str(th)]);
    %disp(cmdOutput);
    T = readtable('cell_coordinates.csv');
    
    cell_centroids(:,1) = T.Var2; %yc
    cell_centroids(:,2) = T.Var1; %xc 
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
        [caimanTextOutput,cell_centroids,~,~,~] = evalc('CaImAn_CellFinder(RegIMG,caimanParams)'); % suppressing printed statements
    else
        [cell_centroids,~,~,~] = CaImAn_CellFinder(RegIMG,caimanParams); % CaImAn cell finder
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
%     [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(pwd,inputParams.EXID,RegIMG,exptVars,cell_centroids,imTemplate,exptVars.rPixels,exptVars.dffWindow/2,inputParams.fluorPercentile); 
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(RegIMG,exptVars.frameRate,cell_centroids,exptVars.rPixels,floor(exptVars.dffWindow/2),inputParams.fluorPercentile,neuropilSubPercent);   
else
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_filled(RegIMG,exptVars,cell_centroids(:,2),cell_centroids(:,1),exptVars.rPixels,floor(exptVars.dffWindow/2),inputParams.fluorPercentile); 
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

rDisplay = floor(exptVars.rPixels*0.4); % display smaller size patches


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
RTparams.norm_meanRedIMG = normMeanIMG;
RTparams.tstack = tstack;
RTparams.dll_name = inputParams.dllName;
RTparams.board_number = inputParams.boardNumber;
RTparams.SLM_ON = inputParams.slmOn;

%% Real time analysis

realTimeApp(RTparams);
rmpath(genpath('tools'));
rmpath(genpath('Psignal')); % path to Psignal folder