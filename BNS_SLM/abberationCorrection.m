tst = tic;

regional_lut_file = 'SLM_LF033_1064nm_2019-09-06_000_LUT.txt'; %Used for Overdrive operation

%--- SDK parameters (these usually don't change) ---
true_frames = 1;
board_number = 1;       %Can be someday used to select individual SLM systems if more than one is connected
                        %Default: 1 = 1st (or only) driver board found

SLM_ON = 0; % DZ 04/22 to check the status of SLM

%--- Dummy parameters ---
%(needed so Configure_overdrive is compatible with devices that don't do on-board overdrive; these values don't affect LFSD)
use_GPU        = 1;     %1 = Use GPU for calculating OD frames, if available (not used for LFSD since OD frames are calculated on board)
max_OD_frames = 20;     %Max number of OD frames to reserve in memory (not used for LFSD since OD frames are calculated on board)

% -----------------------------------------------------------------------
%           Initialization
% -----------------------------------------------------------------------

% --- Load DLL ---
%   Overdrive_SDK_C_wrapper.dll, Overdrive_SDK_C_wrapper.h,
%   Overdrive_SDK.dll, CL_ophane.dll, FreeImage.dll, and wdapi1021.dll
%   should all be located in a path accessible by this Matlab program

dll_name = 'Overdrive_SDK_C_wrapper';
dll_header = 'Overdrive_SDK_C_wrapper.h';
loadlibrary(dll_name, dll_header)

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
    
    %-------------------------------------------------------------------
    %       Calculate OD frames and write them to the SLM
    %-------------------------------------------------------------------
    
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
    
%      tm = zeros(1,500);
    % --- Check for Overdrive compatibility (i.e. configure_overdrive worked) ---
    if (overdrive_available == 1)
        disp('Error configuring overdrive. Check LUT file.')
    else
        disp('Individual image mode (non-triggered)')
        SLM_ON = 1;
    end 
end

opts=dhot_opts();
opts.vortex_modes= 100; 
opts.niters = 3;
opts.lens= 0;  % -1368 
opts.slm_dims = [1536 1536]*1.5;
imgFolder = 'C:\Users\Sauron\Desktop\BK Testing\06-03-21 AO Test - Dulara\';
        
P = [0 0];  
xspm = -1;
yspm = -41;

[m, ~]=dhot(P, opts, 0);
m = add_virtualLens(m, opts);           
ref1 = round(1536*(1.5 - 1)/2);            
ref2 = round(1536*(1.5 + 1)/2);
msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
imwrite(double(msk), 'currentMask.tif');            
phaseMask = imread('currentMask.tif'); % uint8                      
if SLM_ON  % DZ 04/22 write the phase mask only if the SLM connection is established                
    calllib(dll_name, 'Write_overdrive_image', board_number, phaseMask); % write to SLM            
end

pause(0.03);
error = calcError(imgFolder);
error_init = error;
C2_n2 = 0;
e = [];
      
opts.abberationCorrection = 1;
for i = -0.02:0.001:0.02
    opts.C2_n2 = i;
    [m, ~]=dhot(P, opts, 0);
    m = add_virtualLens(m, opts);           
    ref1 = round(1536*(1.5 - 1)/2);            
    ref2 = round(1536*(1.5 + 1)/2);
    msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
    imwrite(double(msk), 'currentMask.tif');            
    phaseMask = imread('currentMask.tif'); % uint8                      
    if SLM_ON  % DZ 04/22 write the phase mask only if the SLM connection is established                
        calllib(dll_name, 'Write_overdrive_image', board_number, phaseMask); % write to SLM            
    end
    pause(0.03);
    err = calcError(imgFolder);
    e = [e err];
    if err<error
        C2_n2 = i;
        error = err;
    end
end

% opts.C2_n2 = 0;
% error = error_init;
% C2_2 = 0;
% 
% toc(tst)
% 
% for i = -0.02:0.001:0.02
%     opts.C2_2 = i;
%     [m, ~]=dhot(P, opts, 0);
%     m = add_virtualLens(m, opts);           
%     ref1 = round(1536*(1.5 - 1)/2);            
%     ref2 = round(1536*(1.5 + 1)/2);
%     msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
%     imwrite(double(msk), 'currentMask.tif');            
%     phaseMask = imread('currentMask.tif'); % uint8                      
%     if SLM_ON  % DZ 04/22 write the phase mask only if the SLM connection is established                
%         calllib(dll_name, 'Write_overdrive_image', board_number, phaseMask); % write to SLM            
%     end
%     pause(0.03);
%     err = calcError(imgFolder);
%     e = [e err];
%     if err<error
%         C2_2 = i;
%         error = err;
%     end
% end
% 
% opts.C2_2 = 0;
% error = error_init;
% C4_n2 = 0;
% 
% for i = -0.02:0.001:0.02
%     opts.C4_n2 = i;
%     [m, ~]=dhot(P, opts, 0);
%     m = add_virtualLens(m, opts);           
%     ref1 = round(1536*(1.5 - 1)/2);            
%     ref2 = round(1536*(1.5 + 1)/2);
%     msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
%     imwrite(double(msk), 'currentMask.tif');            
%     phaseMask = imread('currentMask.tif'); % uint8                      
%     if SLM_ON  % DZ 04/22 write the phase mask only if the SLM connection is established                
%         calllib(dll_name, 'Write_overdrive_image', board_number, phaseMask); % write to SLM            
%     end
%     pause(0.03);
%     err = calcError(imgFolder);
%     e = [e err];
%     if err<error
%         C4_n2 = i;
%         error = err;
%     end
% end
% 
% opts.C4_n2 = 0;
% error = error_init;
% C4_2 = 0;
% 
% for i = -0.02:0.001:0.02
%     opts.C4_2 = i;
%     [m, ~]=dhot(P, opts, 0);
%     m = add_virtualLens(m, opts);           
%     ref1 = round(1536*(1.5 - 1)/2);            
%     ref2 = round(1536*(1.5 + 1)/2);
%     msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
%     imwrite(double(msk), 'currentMask.tif');            
%     phaseMask = imread('currentMask.tif'); % uint8                      
%     if SLM_ON  % DZ 04/22 write the phase mask only if the SLM connection is established                
%         calllib(dll_name, 'Write_overdrive_image', board_number, phaseMask); % write to SLM            
%     end
%     pause(0.03);
%     err = calcError(imgFolder);
%     e = [e err];
%     if err<error
%         C4_2 = i;
%         error = err;
%     end
% end
% 
% opts.C4_2 = 0;

 % -------------------------------------------------------------------
    %           Cleanup
    % -------------------------------------------------------------------
   
%     SLM_power_off_error = calllib(dll_name, 'SLM_power', 0);
%     if SLM_power_off_error
%         disp('Error turning SLM power off');
%     else
%         disp('Turned SLM power off.');
%     end
% 
% 
% calllib(dll_name, 'Delete_SDK'); % Always call Delete_SDK before exiting
% disp(strcat(dll_name, ' exited.'));
%  
% fprintf('\n')
% unloadlibrary(dll_name);