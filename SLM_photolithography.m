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

slmParams = slmInit(inputParams); % for Boulder Non-linear Systems (BNS) SLM

% USB shutter initialization

warning off;
loadlibrary('PiUsb', 'PiUsb.h');
[stsh, ~] = calllib('PiUsb', 'piConnectShutter', 0, 627); % Connect to stimulation USB shutter, Serial# = 627
            
shutter = 0;
calllib('PiUsb', 'piSetShutterState', shutter, stsh); % To make sure that the stimulation USB shutter is closed at the beginning

%% Calculate Phase Mask: Initialization

%% Other patterns
c= Config.BNS();
sz= c.res;

I = ones(2304,2304);
im = otslm.simple.aperture(sz, sz(1)/400, 'shape', 'circle');
slm= CGH( c, 'incident', I );
disk = slm.cgh(im, 'alpha', 0.5, 'use_gpu', false, 'N', 10);

slm= DHOT1(c);
% slm.add_tweezer([0,0],disk,0);
% pattern = slm.dhot('alpha',1);
% m = pattern;

%%

slmdim = 1536;
mult_factor = 1.5; % high pixel mode
zoom_factor = 3/4; %1/6; % high pixel mode

xoff = 0;
yoff = 0;
vm = 10;
xspm = 172; % x shift for the phase mask
yspm = 22; 
P = [0 0]; 
            
opts=dhot_opts();
opts.vortex_modes= vm; 
opts.niters = 3;
opts.slm_dims = [1536 1536]*mult_factor; % high pixel mode

%% Generate stimulation patterns iteratively 

interval = 100; % gap between two stimulation patterns (in milliseconds)

for VLens = 450:5:700
    opts.lens = -1*VLens;
    [m, ~]=dhot(P, opts, 1);
    m = add_virtualLens(m, opts);      
    ref1 = round(slmdim*(mult_factor - 1)/2);
    ref2 = round(slmdim*(mult_factor + 1)/2);
    msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
            
%     imwrite(double(msk), 'currentMask.tif'); 
%     phaseMask = imread('currentMask.tif'); % uint8   

    phaseMask = im2uint8(msk); % conversion to uint8
                       
    calllib(slmParams.dllName, 'Write_overdrive_image', slmParams.boardNumber, phaseMask); % write to SLM

    calllib('PiUsb', 'piSetShutterState', 1, stsh); % open USB shutter
    pause(0.2); % stim. duration = 300ms (should be a user specified parameter in future)
    calllib('PiUsb', 'piSetShutterState', 0, stsh); % close USB shutter
    pause(interval/1000); % specified by the user in milliseconds (convert to seconds)
end