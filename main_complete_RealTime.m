close all
% clear

addpath(genpath('utilities'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

%%% Create struct to parse input variables to RT app 09/01/2022

%% Input parameters for NeuroART

dataPath = [pwd, filesep];  % set datapath to current folder

inputDialogRT; % get input parameters from the user
if Cancelled
    return;
end

workingDir = pwd;
symmFLAG = 1; % 2             % 1 ->symmetric, 2->asym, T = frame time, Tau -> 1.5
smoothwin = 9; %3             % window size used to smooth fluorescence intensity (F) traces
winSizeSeconds = 20; %5       % the window size (in seconds) considered to calculate the baseline fluorescence intensity
percent = 50; % percentile considered to calculate the baseline fluorescence intensity
dftResolution = 1;

r = Answer.R; % average radius of ROIs (in microns)

displayWin = Answer.DISPLAY; % Number of frames to be displayed at a time in the DF/F plots
exptId = Answer.EXID; % experiment ID
if Answer.FORMAT == 1
    imgType = '.raw';
else
    imgType = '.tif';
end
ImageFile = [Answer.IMGFOLDER, filesep, Answer.IMG, imgType]; % The file that contains all the images
numChannels = Answer.NUMCHAN; % Number of channels
scope = Answer.SCOPE; % 1) Thorlabs B-Scope 2) Bruker Ultima 2P+ 3) Bruker Ultima 2P 4) BEAMM(ScanImage) 5) MOM-Thors 6) Offline Mode

dll_name = '';
board_number = 1;  
SLM_ON = 0; % DZ 04/22 to check the status of SLM (Default: OFF)

%% SLM Initialization (for photostimulation when the SLM is available)

% Boulder Nonlinear Systems
% Required firmware: Apr 16 2018 or later

if Answer.SLM == 2 %Boulder Nonlinear Systems
    regional_lut_file = 'SLM_LF033_1064nm_2019-09-06_000_LUT.txt'; %Used for Overdrive operation

    %--- SDK parameters (these usually don't change) ---
    true_frames = 1;
    board_number = 1;       % Can be used to select individual SLM systems if more than one is connected
                            % Default: 1 = 1st (or only) driver board found

    %--- Dummy parameters ---
    % (needed so Configure_overdrive is compatible with devices that don't do on-board overdrive; these values don't affect LFSD)
    use_GPU        = 1;     %1 = Use GPU for calculating OD frames, if available (not used for LFSD since OD frames are calculated on board)
    max_OD_frames = 20;     %Max number of OD frames to reserve in memory (not used for LFSD since OD frames are calculated on board)

    % --- Load DLL ---
    %   Overdrive_SDK_C_wrapper.dll, Overdrive_SDK_C_wrapper.h,
    %   Overdrive_SDK.dll, CL_ophane.dll, FreeImage.dll, and wdapi1021.dll
    %   should all be located in a path accessible by this Matlab program

    dll_name = 'Overdrive_SDK_C_wrapper';
    dll_header = 'Overdrive_SDK_C_wrapper.h';
    loadlibrary(dll_name, dll_header);

    % --- Create the SDK instance ---
    calllib(dll_name, 'Create_SDK');
    overdrive_available = calllib(dll_name, 'Configure_overdrive', use_GPU, regional_lut_file, max_OD_frames); %1 = error
    num_boards = calllib(dll_name, 'Num_devices');

    if num_boards == 0
        disp(strcat(dll_name, ' was not successfully constructed'));
        disp(calllib(dll_name, 'Get_last_error_message'));
    else
        disp(strcat(dll_name, ' was successfully constructed'));
        fprintf('Found %u SLM controller(s)\n', num_boards);

        % Calculate OD frames and write them to the SLM

        % Set the basic SLM parameters
        true_frame_error = calllib(dll_name, 'Set_true_frames', true_frames);
        if true_frame_error
            disp('Error setting true frames');
        end

        % Turn SLM power on
        % In OverDrive C wrapper, input is only power state, not board number.
        SLM_power_on_error = calllib(dll_name, 'SLM_power', 1); 
        if SLM_power_on_error
            disp('Error turning SLM power on');
        end

        % --- Check for Overdrive compatibility (i.e. configure_overdrive worked) ---
        if (overdrive_available == 1)
            disp('Error configuring overdrive. Check LUT file.')
        else
            disp('Individual image mode (non-triggered)')
            SLM_ON = 1;
        end
    end
end

%% Initial Batch Settings

def_initialBatchSize = 601; % default values for the input dialog
def_BatchSize = 1000;

inputDialog1; % initial batch settings
if Cancelled1
    return;
end

start_Frame = Answer1.START; % starting frame of the initial batch
last_init_Frame = Answer1.END; % ending frame of the initial batch
numInitFrames = last_init_Frame - start_Frame + 1; % Number of frames in the initial batch (used to detect ROIs)
batch_size = Answer1.BSIZE; % Maximum number of frames you expect to acquire after the initial batch of frames (set a tentative upper bound)
gap = Answer1.GAP; % Frequency of updating the figures (e.g. every 15 frames)
greenChannel = Answer1.GREENCHAN; % Make this automatically detected
numImagesToRead = numInitFrames + batch_size;
%moved from below by AVS
frameBlock = start_Frame:last_init_Frame; % earlier was, fileInd:length(files)

thorSyncFile = '';
psignalFile = '';
JiSignalInfo = [];
norm_meanRedIMG = [];

if Answer.RF == 1 % Analyzing receptive fields
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
dffwindow = floor(imagingFreq * winSizeSeconds); % 600 or [i-300, i+300]
r_pixels = round(r/exptVars.micronsPerPixel);

if (dffwindow > numInitFrames)
    promptMessage = sprintf('Initial batch should have at least %d frames',dffwindow + 1);
    msgbox(promptMessage,'Error','error');  
    return;
end

ist = tic;
fprintf('Image registration started ... \n');

frameBlock = start_Frame:last_init_Frame; % earlier was, fileInd:length(files)

%% Read the frames in iteratively

[IMG, wait, frameid, fh,tstack] = readInitialBatch(ImageFile,frameBlock,exptVars,greenChannel,numChannels,scope,imgType);

if wait == 10000
    promptMessage = sprintf('Number of acquired images is insufficient to achieve the specified size of the initial batch');
    msgbox(promptMessage,'Error','error');  
    return;
end

I = (mean(IMG(:,:,1:length(frameBlock)),3)); % Mean image
%I = (stdev(IMG(:,:,1:length(frameBlock)),3)); % Standard deviation image
fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
imTemplate = fft2(fixed);

tstop = toc(ist);
fprintf('Initial aquisition took %.4f seconds\n',tstop);

RegIMG = zeros( exptVars.dimY , exptVars.dimX , length(frameBlock), 'uint16');

if Answer.RCHAN == 1 % if the red channel is not available
    for j = 1:length(frameBlock)
        % using Fourier transformation of images for registration
%         error = dftregistration_coder_mex(imTemplate,fft2(IMG(:,:,j)),dftResolution);

%         [regFrame,~,~] = regseqRT_coderGPU_mex(imTemplate,IMG(:,:,j));  % commented DZ 02/10/22
        [regFrame,~,~] = regseqRT(imTemplate,IMG(:,:,j));

        RegIMG(:,:,j) = gather(regFrame);
%         ty(j) = ty; %error(3);
%         tx(j) = tx; %error(4);
    end
else % Only valid for individual tif files spitted out by Bruker (2 channel data)
        
    RedIMG = nan(exptVars.dimX,exptVars.dimY,length(frameBlock)); %% change to length(fileInd:length(files))   
    redID = 2;
        
    if Answer1.GREENCHAN == 2
        redID = 1; 
    end

    for i = 1:length(frameBlock)
        fName = fullfile(Answer.IMGFOLDER, [Answer.IMG '_Ch' num2str(redID) '_' sprintf('%05d',i) '.ome' imgType]); % Format of the Bruker tif filenames 
        RedIMG(:,:,i)  = imread(fName);
%             fh1 = fopen(files(start_Frame-1+i).name); % read raw image
%             RedImg = reshape(fread(fh1,inf,'uint16=>uint16'),[exptVars.dimX, exptVars.dimY]);
%             RedIMG(:,:,i)  = RedImg';
%             fclose(fh1);
        
    end

    clear files

    cd(workingDir)
    
    I = (mean(RedIMG(:,:,1:length(frameBlock)),3)); % Mean image
    fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
    imTemplate_red = fft2(fixed);
        
    for j = 1:length(frameBlock)
        % using Fourier transformation of images for registration
        error = dftregistration(imTemplate_red,fft2(RedIMG(:,:,j)),dftResolution);
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
if Answer.CFIND == 1 % Manual
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
elseif Answer.CFIND == 4 % From File
    [file,path] = uigetfile; % select the .mat file which contains the variable, "ptsIdx": column 2 --> x, column 3 --> y coordinates
    if file == 0
        fprintf('No file was selected ... \n');
        return;
    else
        load([path file],'ptsIdx');
        cell_centroids(:,1) = ptsIdx(:,2); %yc
        cell_centroids(:,2) = ptsIdx(:,3); %xc
    end
elseif Answer.CFIND == 3 % Cite-on
%%     system('activate cite-on & python test.py')
    [~,~] = system(['activate cite-on & python test.py' ' -x ' num2str(exptVars.dimX) ' -y ' num2str(exptVars.dimY) ' -n ' num2str(length(frameBlock)) ' -p ' ImageFile]);
    T = readtable('cell_coordinates.csv');
    clear cell_centroids;
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
if Answer.ROI == 1 % no filled ROIs
%     [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(pwd,exptId,RegIMG,exptVars,cell_centroids,imTemplate,r_pixels,dffwindow/2,percent); 
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(RegIMG,exptVars.frameRate,cell_centroids,r_pixels,floor(dffwindow/2),percent,neuropilSubPercent);   
else
    [norm_meanIMG,roiBW2,npBWout,DFF,~,fluoAllSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_filled(RegIMG,exptVars,cell_centroids(:,2),cell_centroids(:,1),r_pixels,floor(dffwindow/2),percent); 
end

clear RegIMG

tstop = toc(ist);
fprintf('total execution time for cell finding is %.4f seconds\n',tstop);

%% Real Time Visualization

mst = 1;
mstWin = 0; % This default value corresponds to cumulative analysis
if Answer.NET == 2 % Correlations method
    mst = 0;
end

if Answer.CWIN == 2 % Using a shorter (fixed) window size
    shortWindow = 1;
    mstWin = floor((Answer.CWINF)*imagingFreq); % max. allowed size = currently available number of frames
end
    
clear Answer Cancelled Answer1 Cancelled1

%% Check whether enough number of frames are available

numFrames = frameid - 1; % number of frames read upto now
if (displayWin > numFrames)
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
RTparams.symmFLAG = symmFLAG;
RTparams.smoothwin = smoothwin;
RTparams.r = r_display;
RTparams.dffwindow = dffwindow;
RTparams.percent = percent;
RTparams.last_init_frame = last_init_Frame;
RTparams.displayWin = displayWin;
RTparams.batch_size = batch_size;
RTparams.gap = gap;
RTparams.exptId = exptId;
RTparams.exptVars = exptVars;
RTparams.minNpSubFluo = minNpSubFluo;
RTparams.maxAdjF = maxAdjF;
RTparams.numInitFrames = numInitFrames;
RTparams.mst = mst;
RTparams.mstWin = mstWin;
RTparams.numAvailFrames = numFrames;
RTparams.imagingFreq = imagingFreq;
RTparams.thorSyncFile = thorSyncFile;
RTparams.psignalFile = psignalFile;
RTparams.rawFileHandle = fh;
RTparams.greenChannel = greenChannel;
RTparams.numChannels = numChannels;
RTparams.scope = scope;
RTparams.imgType = imgType;
RTparams.ImageFile = ImageFile;
RTparams.JiSignalInfo = JiSignalInfo;
RTparams.norm_meanRedIMG = norm_meanRedIMG;
RTparams.tstack = tstack;
RTparams.dll_name = dll_name;
RTparams.board_number = board_number;
RTparams.SLM_ON = SLM_ON;

%% Real time analysis

realTimeApp(RTparams);
rmpath(genpath('utilities'));
rmpath(genpath('Psignal')); % path to Psignal folder