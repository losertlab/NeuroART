
% Debugging SLM driver.

addpath('../');

c= Config.BNS();
sz= c.res;

% I = otslm.simple.gaussian(sz, 750);  % Incident beam (gaussian)
I = ones(2304,2304);
im = otslm.simple.aperture(sz, sz(1)/100, 'shape', 'circle');

slm= CGH( c, 'incident', I );
disk = slm.cgh(im, 'alpha', 0.5, 'use_gpu', false, 'N', 10);

slm= DHOT1(c);
% slm.add_tweezer([0,0],disk,0);
% slm.add_tweezer([0,-0.2;0,0],disk,0);
slm.add_vortex([0,0],-7,0);
pattern = slm.dhot('alpha',1);

m = pattern;

% f=173;
% 
% lens= slm.virtual_lens([0,0],f);
% p = slm.apply_lens(pattern,lens);
% slm.show(p);

%% Initialize SLM

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

%% Run SLM

slmdim = 1536;
mult_factor = 1.5; % high pixel mode

tstart = tic;
            
xspm = 190;
yspm = 22;

opts=dhot_opts();
opts.vortex_modes= -7; 
opts.niters = 3;
opts.lens = -600;
opts.slm_dims = [1536 1536]*mult_factor; % high pixel mode
opts.zernike = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]; % 081622 tip, tilt, ast2 hardcoded for now
            
% 05/09/21 Dulara moved this section to the GUI code for speeding up the calculations
m = add_virtualLens(m, opts);
            
ref1 = round(slmdim*(mult_factor - 1)/2);
ref2 = round(slmdim*(mult_factor + 1)/2);
msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
            
t = toc(tstart);
fprintf('Time taken until after updating the phase mask display in the GUI is %.2f seconds \n',t);

imwrite(double(msk), 'currentMask.tif'); 
phaseMask = imread('currentMask.tif'); % uint8   
            
t = toc(tstart); 
fprintf('Total time taken for calculations before sending the phase mask to SLM is %.2f seconds \n',t);       


calllib(dll_name, 'Write_overdrive_image', board_number, phaseMask); % write to SLM

%% Cleanup

% SLM_power_off_error = calllib(dll_name, 'SLM_power', 0);
% if SLM_power_off_error
%     disp('Error turning SLM power off');
% else
%     disp('Turned SLM power off.');
% end
% 
% 
% calllib(dll_name, 'Delete_SDK'); % Always call Delete_SDK before exiting
% disp(strcat(dll_name, ' exited.'));
%  
% fprintf('\n');
% unloadlibrary(dll_name);
% 
