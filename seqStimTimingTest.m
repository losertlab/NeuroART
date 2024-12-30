close all
clearvars

addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

inputParams = struct();
inputParams.SLM = 2;  % BNS SLM
inputParams.dllName = '';
inputParams.boardNumber = 1;
inputParams.slmOn = false;
max_OD_frames = 20;

slmParams = slmInit(inputParams); % for Boulder Non-linear Systems (BNS) SLM

% USB shutter initialization

% warning off;
loadlibrary('PiUsb', 'PiUsb.h');
[stsh, ~] = calllib('PiUsb', 'piConnectShutter', 0, 627); % Connect to stimulation USB shutter, Serial# = 627
            
shutter = 0;
calllib('PiUsb', 'piSetShutterState', shutter, stsh); % To make sure that the stimulation USB shutter is closed at the beginning

%% Calculate Phase Mask: Initialization

slmdim = 1536;
mult_factor = 1.5; % high pixel mode
zoom_factor = 3/4; %1/6; % high pixel mode

xoff = 0;
yoff = 0;
vm = 5;
xspm = 172; % x shift for the phase mask
yspm = 22; 
            
opts=dhot_opts();
opts.vortex_modes= vm; 
opts.niters = 3;
opts.slm_dims = [1536 1536]*mult_factor; % high pixel mode
opts.lens = -600;

%% Generate sequential stimulation patterns

num_images = 2;
% nTargets = 3;
%     
[m1, ~]=dhot([0 0.12], opts, 1);
[m2, ~]=dhot([0 0.04], opts, 1);
[m3, ~]=dhot([0 -0.04], opts, 1);

m1 = add_virtualLens(m1, opts);  
m2 = add_virtualLens(m2, opts);
m3 = add_virtualLens(m3, opts);

ref1 = round(slmdim*(mult_factor - 1)/2);
ref2 = round(slmdim*(mult_factor + 1)/2);

msk1 = m1(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
msk2 = m2(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
msk3 = m3(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM

phaseMask1 = im2uint8(msk1); % conversion to uint8
phaseMask2 = im2uint8(msk2); % conversion to uint8
phaseMask3 = im2uint8(msk3); % conversion to uint8   

% imwrite(double(msk1), 'currentMask1.tif'); 
% imwrite(double(msk2), 'currentMask2.tif'); 
% imwrite(double(msk3), 'currentMask3.tif'); 

image_size = size(phaseMask1);

images_flat(:,1) = reshape(phaseMask1,image_size(1)*image_size(2),1);
images_flat(:,2) = reshape(phaseMask2,image_size(1)*image_size(2),1);                 
% images_flat(:,3) = reshape(phaseMask3,image_size(1)*image_size(2),1);
% image_sequence = reshape(images_flat,image_size(1)*image_size(2)*3,1); % stimulate 3 cells in sequence
image_sequence = reshape(images_flat,image_size(1)*image_size(2)*2,1); % stimulate 2 cells in sequence

% Timing test for iterating through a sequence of holograms
    
% for idx = 1:nTargets                   
%     seq = idx*ones(1,15);
%     tst = tic;
%     target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', board_number, nTargets, seq);       
%     toc(tst)
% end     

% imageFolder = 'E:\Desktop_05112022\dulara dump\LensCalibration_DZ102421\PhaseMask_xs22_ys172_L5_vL600'; %\GS_8spot_AB_diag';    %Location of the original image files. Assumed not to end with \
% matchstring = '*.tif';  %Use all images with this filename  
%   
% image_filenames = dir([imageFolder, filesep, matchstring]); %Filenames of original images
% num_images = length(image_filenames);    % Number of files found
% 
% for n = 1:num_images
%     images(:,:,n)=fliplr(rot90(imread([imageFolder, filesep, image_filenames(n).name]),3));
% end
% 
% image_size = size(images(:,:,1));
% for m=1:num_images
%     images_flat(:,m) = reshape(images(:,:,m),image_size(1)*image_size(2),1);
% end
%         
% image_sequence = reshape(images_flat,image_size(1)*image_size(2)*num_images,1);
            
% Download and display an entire image sequence

download_error = calllib(slmParams.dllName, 'Download_image_sequence', slmParams.boardNumber, num_images, image_sequence, max_OD_frames);
if download_error
last_error = calllib(slmParams.dllName, 'Get_last_error_message');
    disp(['Error during sequence download:',last_error])
end

% Load custom target sequence list, if used
% target_sequence_list = [2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2];

target_sequence_list = [1,1,1];
target_sequence_list1 = [2,2,2];
num_targets = length(target_sequence_list);

calllib('PiUsb', 'piSetShutterState', 1, stsh); % open USB shutter
target_sequence_error = calllib(slmParams.dllName, 'Load_target_sequence_list', slmParams.boardNumber, num_targets, target_sequence_list);
% pause(0.01); % stim. duration = 300ms (should be a user specified parameter in future)


target_sequence_error = calllib(slmParams.dllName, 'Load_target_sequence_list', slmParams.boardNumber, num_targets, target_sequence_list1);
% pause(0.01); % stim. duration = 300ms (should be a user specified parameter in future)


target_sequence_error = calllib(slmParams.dllName, 'Load_target_sequence_list', slmParams.boardNumber, num_targets, target_sequence_list);
% pause(0.01); % stim. duration = 300ms (should be a user specifie


target_sequence_error = calllib(slmParams.dllName, 'Load_target_sequence_list', slmParams.boardNumber, num_targets, target_sequence_list1);
% pause(0.01); 

calllib('PiUsb', 'piSetShutterState', 0, stsh); % close USB shutter


% target_sequence_error = target_sequence_error + calllib(slmParams.dllName, 'Restart_image_sequence', slmParams.boardNumber); %Board will not start listening for triggers until restart is called.

% max_OD_frames = 20;
% download_error = calllib(dll_name, 'Download_image_sequence', inputParams.boardNumber, 3, image_sequence, max_OD_frames);
% 
% % complete_download_error = calllib(slmParams.dllName, 'Completed_download_image_sequence', slmParams.boardNumber);
% k = 0;
% 
% % calllib('PiUsb', 'piSetShutterState', 1, stsh); % open USB shutter 
% seq = [3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2];  %used for 'custom' mode. list images in any order; repeats are okay;
% target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', inputParams.boardNumber, length(seq), seq);
% target_sequence_error = target_sequence_error + calllib(dll_name, 'Restart_image_sequence', inputParams.boardNumber); %Board will not start listening for triggers until restart is called.
%            

