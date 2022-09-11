close all
% clear

addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

%%% Create struct to parse input variables to RT app 09/01/2022

%% Input parameters for NeuroART

inputParams = inputDialogRT(); % get input parameters from the user
if inputParams.canceled
    return;
end

%% SLM Initialization (for photostimulation when the SLM is available)

% Boulder Nonlinear Systems
% Required firmware: Apr 16 2018 or later

if inputParams.SLM == 2 %Boulder Nonlinear Systems
    regional_lut_file = 'SLM_LF033_1064nm_2019-09-06_000_LUT.txt'; %Used for Overdrive operation

    %--- SDK parameters (these usually don't change) ---
    true_frames = 1;
    inputParams.boardNumber = 1;       % Can be used to select individual SLM systems if more than one is connected
                            % Default: 1 = 1st (or only) driver board found

    %--- Dummy parameters ---
    % (needed so Configure_overdrive is compatible with devices that don't do on-board overdrive; these values don't affect LFSD)
    use_GPU        = 1;     %1 = Use GPU for calculating OD frames, if available (not used for LFSD since OD frames are calculated on board)
    max_OD_frames = 20;     %Max number of OD frames to reserve in memory (not used for LFSD since OD frames are calculated on board)

    % --- Load DLL ---
    %   Overdrive_SDK_C_wrapper.dll, Overdrive_SDK_C_wrapper.h,
    %   Overdrive_SDK.dll, CL_ophane.dll, FreeImage.dll, and wdapi1021.dll
    %   should all be located in a path accessible by this Matlab program

    inputParams.dllName = 'Overdrive_SDK_C_wrapper';
    dll_header = 'Overdrive_SDK_C_wrapper.h';
    loadlibrary(inputParams.dllName, dll_header);

    % --- Create the SDK instance ---
    calllib(inputParams.dllName, 'Create_SDK');
    overdrive_available = calllib(inputParams.dllName, 'Configure_overdrive', use_GPU, regional_lut_file, max_OD_frames); %1 = error
    num_boards = calllib(inputParams.dllName, 'Num_devices');

    if num_boards == 0
        disp(strcat(inputParams.dllName, ' was not successfully constructed'));
        disp(calllib(inputParams.dllName, 'Get_last_error_message'));
    else
        disp(strcat(inputParams.dllName, ' was successfully constructed'));
        fprintf('Found %u SLM controller(s)\n', num_boards);

        % Calculate OD frames and write them to the SLM

        % Set the basic SLM parameters
        true_frame_error = calllib(inputParams.dllName, 'Set_true_frames', true_frames);
        if true_frame_error
            disp('Error setting true frames');
        end

        % Turn SLM power on
        % In OverDrive C wrapper, input is only power state, not board number.
        SLM_power_on_error = calllib(inputParams.dllName, 'SLM_power', 1); 
        if SLM_power_on_error
            disp('Error turning SLM power on');
        end

        % --- Check for Overdrive compatibility (i.e. configure_overdrive worked) ---
        if (overdrive_available == 1)
            disp('Error configuring overdrive. Check LUT file.')
        else
            disp('Individual image mode (non-triggered)')
            inputParams.slmOn = true;
        end
    end
end

%% Initial Batch Settings

batchSettings = batchSettingsDialog(inputParams); % initial batch settings
if batchSettings.canceled
    return;
end

thorSyncFile = '';
psignalFile = '';
JiSignalInfo = [];
norm_meanRedIMG = [];

if inputParams.RF == 1 % Analyzing receptive fields
     promptMessage = sprintf('Do you have Ji Signal Files?');
     button = questdlg(promptMessage, 'Tuning Analysis', 'Yes', 'No', 'Yes');
     if strcmpi(button, 'Yes')
         [file,path] = uigetfile('*.mat','Please select the JiSignal file','stim_seq_210402_tones.mat');
         JiSignalFile = [path,file];
         JiSignalInfo = load(JiSignalFile).preset_stim_seq;
     else
         [file,path] = uigetfile('*.h5','Please select the thorSync file','Episode001.h5');
         thorSyncFile = [path,file];
         [file,path] = uigetfile('*.mat','Please select the pSignal file','RND_352r_2020_03_07_Phys_1.mat');
         psignalFile = [path,file];
     end
end

%% Reading experimental parameters and motion correction of the initial batch

XML = danParseXML('Experiment.xml');
exptVars = xmlVersionCheck(XML);
exptVars.frameRate = 10; % for images acquired from micromanager


imagingFreq = exptVars.frameRate;
dffwindow = floor(imagingFreq * inputParams.winSizeSeconds); % 600 or [i-300, i+300]
r_pixels = round(inputParams.R/exptVars.micronsPerPixel);

if (dffwindow > batchSettings.numInitFrames)
    promptMessage = sprintf('Initial batch should have at least %d frames',dffwindow + 1);
    msgbox(promptMessage,'Error','error');  
    return;
end

ist = tic;
fprintf('Image registration started ... \n');

%% Read the frames in iteratively

[IMG, wait, frameid, fh,tstack] = readInitialBatch(inputParams.imageFile,batchSettings.frameBlock,exptVars,batchSettings.GREENCHAN,inputParams.NUMCHAN,inputParams.SCOPE,inputParams.FORMAT);

if wait == 10000
    promptMessage = sprintf('Number of acquired images is insufficient to achieve the specified size of the initial batch');
    msgbox(promptMessage,'Error','error');  
    return;
end

I = (mean(IMG(:,:,1:length(batchSettings.frameBlock)),3)); % Mean image
%I = (stdev(IMG(:,:,1:length(batchSettings.frameBlock)),3)); % Standard deviation image
fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
imTemplate = fft2(fixed);

tstop = toc(ist);
fprintf('Initial aquisition took %.4f seconds\n',tstop);

RegIMG = zeros( exptVars.dimY , exptVars.dimX , length(batchSettings.frameBlock), 'uint16');

if inputParams.RCHAN == 1 % if the red channel is not available
    for j = 1:length(batchSettings.frameBlock)
        % using Fourier transformation of images for registration
%         error = dftregistration_coder_mex(imTemplate,fft2(IMG(:,:,j)),inputParams.dftResolution);

%         [regFrame,~,~] = regseqRT_coderGPU_mex(imTemplate,IMG(:,:,j));  % commented DZ 02/10/22
        [regFrame,~,~] = regseqRT(imTemplate,IMG(:,:,j));

        RegIMG(:,:,j) = gather(regFrame);
%         ty(j) = ty; %error(3);
%         tx(j) = tx; %error(4);
    end
else % Only valid for individual tif files spitted out by Bruker (2 channel data)
        
    RedIMG = nan(exptVars.dimX,exptVars.dimY,length(batchSettings.frameBlock)); %% change to length(fileInd:length(files))   
    redID = 2;
        
    if batchSettings.GREENCHAN == 2
        redID = 1; 
    end

    for i = 1:length(batchSettings.frameBlock)
        fName = fullfile(inputParams.IMGFOLDER, [inputParams.IMG '_Ch' num2str(redID) '_' sprintf('%05d',i) '.ome' inputParams.FORMAT]); % Format of the Bruker tif filenames 
        RedIMG(:,:,i)  = imread(fName);
%             fh1 = fopen(files(batchSettings.START-1+i).name); % read raw image
%             RedImg = reshape(fread(fh1,inf,'uint16=>uint16'),[exptVars.dimX, exptVars.dimY]);
%             RedIMG(:,:,i)  = RedImg';
%             fclose(fh1);
        
    end

    clear files

    cd(pwd)
    
    I = (mean(RedIMG(:,:,1:length(batchSettings.frameBlock)),3)); % Mean image
    fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
    imTemplate_red = fft2(fixed);
        
    for j = 1:length(batchSettings.frameBlock)
        % using Fourier transformation of images for registration
        error = dftregistration(imTemplate_red,fft2(RedIMG(:,:,j)),inputParams.dftResolution);
        ty(j) = error(3);
        tx(j) = error(4);
    end
        
    meanIMG = mean(RedIMG,3); % Mean image for cell center clicking
    norm_meanRedIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));
        
    clear RedIMG RedImg imTemplate_red
end

% offsets = [ty' tx'];
   

clear IMG I fixed 
    
tstop = toc(ist);
fprintf('Initial aquisition and registration took %.4f seconds\n',tstop);
    
%% Detect centroids of the cells (ROIs)

iStart = tic;
% promptMessage = sprintf('Select the method for cell finding');
% button = questdlg(promptMessage, 'Image Registration Completed', 'Manual', 'CaImAn', 'Cite-on', 'CaImAn'); % can't have more than 3 options
if inputParams.CFIND == 1 % Manual
%     meanIMG = std(double(RegIMG),0,3); % Mean image for cell center clicking
    meanIMG = mean(double(RegIMG),3); % Mean image for cell center clicking
    norm_meanIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));
    figure; imagesc(norm_meanIMG); colormap('gray'); axis('square')
    SelectText = ['Click on Neuron Centers...' newline 'Press Enter after all the cells are selected' newline 'Press delete if you want to deselect the last selected cell'];
    disp ( SelectText );
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
    [a,b] = system(['activate cite-on & python test.py' ' -x ' num2str(exptVars.dimX) ' -y ' num2str(exptVars.dimY) ' -n ' num2str(length(batchSettings.frameBlock)) ' -p ' inputParams.imageFile]);
    disp(a);
    disp(b);
    T = readtable('cell_coordinates.csv');
    
    cell_centroids(:,1) = T.Var2; %yc
    cell_centroids(:,2) = T.Var1; %xc 
else
    [cell_centroids,~,~,~] = CaImAn_CellFinder(RegIMG); % CaImAn cell finder
end
%%
close(gcf);

numOfCells = length(cell_centroids);
tstop = toc(iStart);
    
fprintf('Total number of cells detected: %d \n',numOfCells);
fprintf('Cell finding took %.4f seconds\n',tstop);

delete(gcp('nocreate'));
    
%% Compute DF/F

neuropilSubPercent = 70; % use this default value for now
if inputParams.ROI == 1 % no filled ROIs
%     [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(pwd,inputParams.EXID,RegIMG,exptVars,cell_centroids,imTemplate,r_pixels,dffwindow/2,inputParams.fluorPercentile); 
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(RegIMG,exptVars.frameRate,cell_centroids,r_pixels,floor(dffwindow/2),inputParams.fluorPercentile,neuropilSubPercent);   
else
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_filled(RegIMG,exptVars,cell_centroids(:,2),cell_centroids(:,1),r_pixels,floor(dffwindow/2),inputParams.fluorPercentile); 
end

clear RegIMG

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
    mstWin = floor((inputParams.CWINF)*imagingFreq); % max. allowed size = currently available number of frames
end
    
clear inputParams Cancelled batchSettings Cancelled1

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

r_display = floor(r_pixels*0.4); % display smaller size patches


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
RTparams.dffwindow = dffwindow;
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
RTparams.imagingFreq = imagingFreq;
RTparams.thorSyncFile = thorSyncFile;
RTparams.psignalFile = psignalFile;
RTparams.rawFileHandle = fh;
RTparams.greenChannel = batchSettings.GREENCHAN;
RTparams.numChannels = inputParams.NUMCHAN;
RTparams.scope = inputParams.inputParams.SCOPE;
RTparams.imgType = inputParams.FORMAT;
RTparams.ImageFile = inputParams.imageFile;
RTparams.JiSignalInfo = JiSignalInfo;
RTparams.norm_meanRedIMG = norm_meanRedIMG;
RTparams.tstack = tstack;
RTparams.dll_name = inputParams.dllName;
RTparams.board_number = inputParams.boardNumber;
RTparams.SLM_ON = inputParams.slmOn;

%% Real time analysis

realTimeApp(RTparams);
rmpath(genpath('utilities'));
rmpath(genpath('Psignal')); % path to Psignal folder