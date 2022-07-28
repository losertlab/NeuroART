 % Overdrive_triggering_example_LF.m
%
% Boulder Nonlinear Systems
%
% This program demonstrates the on-board overdrive capabilities of the
% Large-Format SLM board.
%
% Unlike with previous generations of the LF SLM board, overdrive frames
% are now calculated and managed on-board, rather than downloaded
% individually from the PC and triggered individually.
%
% As with the previous multisequence version of the hardware, images can be
% predownloaded to the driver board and then addressed in any order. This
% allows for rapid experimental reconfiguration without needing to
% redownload images.
%
% Expected usage:
% 1. Initialize SDK, power on board, initialize overdrive (includes
% downloading a LUT to the board)
% 2. Download a set of images (up to a max of 2045 images)
% 3. Download an image sequence (up to a max of 32768 image numbers)
% 4. (Optional): Choose a subset of the image sequence to run
% 5. Start triggering overdrive frames
% 6. (Optional): Choose a different subset of the image sequence to run
%
% Required firmware: Apr 16 2018 or later
%
clear all;
% -----------------------------------------------------------------------
%           User-controlled parameters 
% -----------------------------------------------------------------------
image_mode = 'entire_folder';    %from_file: use the filenames specified below.
                                    %entire_folder: use all images in the
                                    %   specified folder
                                    %generate_stripes: generate images that
                                    %   toggle a stripe between known
                                    %   values. End_phase will be a local
                                    %   max.

% --- Options for 'entire_folder' mode ---
imageFolder = 'C:\Program Files\Boulder Nonlinear Systems\examples\images\1536' %\GS_8spot_AB_diag';    %Location of the original image files. Assumed not to end with \
matchstring = '*.bmp';  %Use all images with this filename                                    
                                    
% --- Options for 'generate stripes' mode ---
start_phase = 63;  %bmp values 0-255, where 0 is high voltage and 255 is 2pi
end_phase =127;
stripe_period = 8;  %period of stripe, in pixels
sizex = 1536;        %array size in pixels
sizey = 1536;
bit_depth = 8;
num_images = 6;     %number of images to generate
  
% --- Options for 'from_file' mode ---
image_filenames = {'C:\Program Files\Boulder Nonlinear Systems\examples\images\1536\AstigX.bmp',...
                   'C:\Program Files\Boulder Nonlinear Systems\examples\images\1536\ComaX.bmp',...
                   'C:\Program Files\Boulder Nonlinear Systems\examples\images\1536\moonwalk.bmp',...
                   'C:\Program Files\Boulder Nonlinear Systems\examples\images\1536\Period8_Img128.bmp'};

% --- Choose image download mode ---
write_mode = 'individual';   %choose 'individual' - downloads each image individually. non-triggered
                            %   'sequence' - downloads an image sequence to the board. triggered.                            
% --- Options for 'individual' mode ---
num_loops   = 100;   %Number of times Matlab will loop through the images in 'individual' mode.

% --- Options for 'sequence' mode ---
sequence_mode = 'auto'; %choose 'auto' - will sequence through all images in order
                        %       'custom' - user specifies the order in which to sequence through images
target_sequence_list = [4,4,4,1];  %used for 'custom' mode. list images in any order; repeats are okay

% --- Subsequence options ---
use_subsequence = 0;    %set to 1 to run a subset of the downloaded image sequence (rather than the target sequence list)
subsequence_length = 2; %Length, in images, of subsequence to run
start_index = 2;        %First image of the subsequence will be start_index+1

%-------------------------------------------------------------------------
%---        SLM parameters (these usually don't change)                ---
%-------------------------------------------------------------------------
regional_lut_file = 'SLM_LF033_1064nm_2019-09-06_000_LUT.txt'; %Used for Overdrive operation

%--- SDK parameters (these usually don't change) ---
true_frames = 1;
board_number = 1;       %Can be someday used to select individual SLM systems if more than one is connected
                        %Default: 1 = 1st (or only) driver board found

%--- Dummy parameters ---
%(needed so Configure_overdrive is compatible with devices that don't do on-board overdrive; these values don't affect LFSD)
use_GPU        = 1;     %1 = Use GPU for calculating OD frames, if available (not used for LFSD since OD frames are calculated on board)
max_OD_frames = 20;     %Max number of OD frames to reserve in memory (not used for LFSD since OD frames are calculated on board)

% -----------------------------------------------------------------------
%           Initialization
% -----------------------------------------------------------------------
disp('Write mode:')
disp(write_mode);
% --- Create the sequence of input images ---
if strcmp(image_mode,'from_file')
        % --- Load the images ---
    % Added 180-degree rotation to match expected BOSS image orientation
    num_images = length(image_filenames);
    for n = 1:num_images
        images(:,:,n)=fliplr(rot90(imread(image_filenames{n}),3));
    end
    
elseif strcmp(image_mode,'entire_folder')
    image_filenames = dir([imageFolder, filesep, matchstring]); %Filenames of original images
    num_images = length(image_filenames);    % Number of files found
    % --- Load the images ---
    % Added 180-degree rotation to match expected BOSS image orientation
    for n = 1:num_images
        images(:,:,n)=fliplr(rot90(imread([imageFolder, filesep, image_filenames(n).name]),3));
    end
  
elseif strcmp(image_mode,'generate_stripes')
    %background will be generated such that ending phase/stripe contrast is
    %pi
    %Note: need to add a transpose to get vertical stripes for the LF SLM
    ending_contrast = 2^bit_depth/2;
    [start_image, end_image] = generate_start_end_images(start_phase,end_phase,ending_contrast,stripe_period,bit_depth,sizex,sizey);
    for n = 1:num_images
        if mod(n,2)==1
            images(:,:,n)=start_image';
        else
            images(:,:,n)=end_image';
        end
    end
else
    disp('Invalid image_mode choice.')
end



% --- Load DLL ---
%   Overdrive_SDK_C_wrapper.dll, Overdrive_SDK_C_wrapper.h,
%   Overdrive_SDK.dll, CL_ophane.dll, FreeImage.dll, and wdapi1021.dll
%   should all be located in a path accessible by this Matlab program
dll_name = 'Overdrive_SDK_C_wrapper';
dll_header = 'C:\Users\Sauron\Documents\MATLAB\BNS\Overdrive_SDK_C_wrapper.h';
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
    
    % --- Check for Overdrive compatibility (i.e. configure_overdrive worked) ---
    if (overdrive_available == 1)
        disp('Error configuring overdrive. Check LUT file.')
    else
       if strcmp(write_mode,'individual')
          % --- Download images individually ---
          disp('Individual image mode (non-triggered)')
          for n = 1:num_loops
              for m = 1:num_images
                 disp('Image '); disp(m)
                 calllib(dll_name, 'Write_overdrive_image', board_number, images(:,:,m)); 
              end
          end
       elseif strcmp(write_mode,'sequence')
           % --- Set up image sequence ---
            % (The first target is at index 1)
            if strcmp(sequence_mode,'auto')
                target_sequence_list = 1:num_images;
                num_targets = length(target_sequence_list);
            elseif strcmp(sequence_mode,'custom')
                % check to make sure our sequence list was valid
                num_targets = length(target_sequence_list);
                disp([num2str(num_targets), ' indexes in target sequence list'])
                if max(target_sequence_list) > num_images
                    disp(['Warning: target sequence list attempts to address image ',num2str(max(target_sequence_list)), ' but sequence only has ', num2str(num_images), ' images. Will clamp target images to ', num2str(num_images)])
                end
                if length(target_sequence_list) > 32768
                    disp(['Warning: target sequence list is length ', num2str(num2str(length(target_sequence_list))), ' but max allowable length is 32768. Will ignore extra target sequence entries.'])
                end
            end
           
            % --- Download image sequence ----
            disp('Image sequence mode (triggered)')
            % flatten and concatenate the images
            % (assumes all images are the same size)
            image_size = size(images(:,:,1));
            for m=1:num_images
                images_flat(:,m) = reshape(images(:,:,m),image_size(1)*image_size(2),1);
            end
            image_sequence = reshape(images_flat,image_size(1)*image_size(2)*num_images,1);
            
           % Download and display an entire image sequence
           k = 0;
           disp('Downloading images... please wait.');
           download_error = calllib(dll_name, 'Download_image_sequence', board_number, num_images, image_sequence, max_OD_frames);
           disp('Done downloading images.');
           if download_error
               last_error = calllib(dll_name, 'Get_last_error_message');
               disp(['Error during sequence download:',last_error])
           end

           % Load custom target sequence list, if used
           if strcmp(sequence_mode,'auto')
               disp('Sequence mode: auto. Sequencing through image sequence in order.')
%                 calllib(dll_name, 'Disable_triggering', 1);
%                 target_sequence_error = calllib(dll_name, 'Restart_image_sequence', board_number);
%                 if target_sequence_error
%                  last_error = calllib(dll_name, 'Get_last_error_message');
%                  disp(['Error during target sequence loading:',last_error])
%                 end
%                 
           elseif strcmp(sequence_mode,'custom')
               disp('Sequence mode: custom. Using custom order for image sequence')
               target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', board_number, num_targets, target_sequence_list);
               target_sequence_error = target_sequence_error + calllib(dll_name, 'Restart_image_sequence', board_number); %Board will not start listening for triggers until restart is called.
                   
               if target_sequence_error
                last_error = calllib(dll_name, 'Get_last_error_message');
                disp(['Error during target sequence loading:',last_error])
               end
           else
               disp('Error: choose sequence_mode of either auto or custom')
           end
           
                      
           % Enable subsequence, if used
           if strcmp(num2str(use_subsequence),'1')
               disp(['Now using subsequence of start_index ', num2str(start_index), ' and length ',num2str(subsequence_length)])
               if subsequence_length+start_index>num_targets
                   disp(['Subsequencing is attempting to use target index ', num2str(subsequence_length+start_index), ' but target image sequence only contains, ',num2str(num_targets), '. Using full sequence instead.']);
               else
                   sequence_error = calllib(dll_name, 'Select_sequence_subset', board_number, start_index, subsequence_length);
                   sequence_error = sequence_error + calllib(dll_name, 'Restart_image_sequence', board_number); %Board will not start listening for triggers until restart is called.
                   if sequence_error
                    last_error = calllib(dll_name, 'Get_last_error_message');
                    disp(['Error during subsequence selection:',last_error])
                   end
               end
           end
           
           disp('Cycling through entire triggered sequence. Click mouse to change settings, or press any key to stop the sequence.');
           while k==0
                k = waitforbuttonpress();   %Cycle through triggered sequence until user clicks or presses key
                if k==0
                   %Mouse click: display dialog box where user may change
                   %settings. 
                   % To display a single image, use a subsequence with num_images = 1
                   % Then always reload LUT file, regenerate stripes (if used), and redownload sequence
                   prompt = {'Subsequence(1:use subsequence 0:entire target sequence):',...
                            'Subsequence:Start_image:',...
                            'Subsequence:Num_images',...
                            'Generate stripes (1:yes 0:no)',...
                            'Stripes:Start_phase (0-255)',...
                            'Stripes:End_phase (0-255)',...
                            'Change image folder (1:yes 0:keep current)'...
                            'Change target image list (1:yes 0:keep current)'};
                   dlg_title = 'Input';
                   num_lines = 1;
                   defaultans = {num2str(use_subsequence),...
                       num2str(start_index),...
                       num2str(subsequence_length),...
                       num2str(strcmp(image_mode,'generate_stripes')),...
                       num2str(start_phase),...
                       num2str(end_phase),...
                       '0',...
                       '0'};
                   answer = inputdlg(prompt,dlg_title,num_lines,defaultans);
                   if strcmp(answer{7},'1')
                       % user has decided to change the image folder
                       dialog_title = 'Select new image folder';
                       imageFolder = uigetdir(imageFolder,dialog_title);
                        new_image_filenames = dir([imageFolder, filesep, matchstring]); %Filenames of original images
                        new_num_images = length(new_image_filenames);    % Number of files found
                        %note that this new folder will be run in default
                        %order, rather than using a custom target image
                        %sequence. This is to avoid indexing images that no
                        %longer exist.
                        % --- Load the images ---
                        disp(['Image folder changed to: ', imageFolder]);
                        image_filenames = new_image_filenames;
                        num_images = new_num_images;
                        num_targets = num_images; %Since we're using an auto target sequence
                        % Added 180-degree rotation to match expected BOSS image orientation
                        clear images;
                        for n = 1:num_images
                            images(:,:,n)=fliplr(rot90(imread([imageFolder, filesep, image_filenames(n).name]),3));
                        end
                        % flatten and concatenate the images
                        % (assumes all images are the same size)
                        image_size = size(images(:,:,1));
                        clear images_flat;
                        for m=1:num_images
                            images_flat(:,m) = reshape(images(:,:,m),image_size(1)*image_size(2),1);
                        end
                        image_sequence = reshape(images_flat,image_size(1)*image_size(2)*num_images,1);
                       disp(['Number of images read: ', num2str(num_images)]);
                   end
                   if strcmp(answer{4},'1')
                       % user has selected generate stripes mode.
                       disp('Generate stripes mode');
                       image_mode = 'generate_stripes';
                       start_phase = str2num(answer{5});
                       end_phase = str2num(answer{6});
                       % regenerate stripes images, even if they haven't changed, since it's fast.
                       % reads in user's new start_phase and end_phase values, but leaves other generate_stripes options default
                       % num_images is also left default, so existing image
                       % sequence will still work
                          %background will be generated such that ending phase/stripe contrast is pi
                        %Note: added a transpose to get vertical stripes for the LF SLM
                        ending_contrast = 2^bit_depth/2;
                        [start_image, end_image] = generate_start_end_images(start_phase,end_phase,ending_contrast,stripe_period,bit_depth,sizex,sizey);
                        clear images;
                        for n = 1:num_images
                            if mod(n,2)==1
                                images(:,:,n)=start_image';
                            else
                                images(:,:,n)=end_image';
                            end
                        end
                        % flatten and concatenate the images
                        % (assumes all images are the same size)
                        image_size = size(images(:,:,1));
                        for m=1:num_images
                            images_flat(:,m) = reshape(images(:,:,m),image_size(1)*image_size(2),1);
                        end
                        image_sequence = reshape(images_flat,image_size(1)*image_size(2)*num_images,1);
                        disp(['Generate_stripes mode. Start_phase=', num2str(start_phase), ', End_phase=', num2str(end_phase)]);
                        num_targets = num_images; %Since we're using an auto target sequence
                   end %Otherwise, we keep the last image sequence we used.
                   
                   if strcmp(answer{8},'1')
                       % user has decided to change the target sequence
                       dialog_title = 'Select new image folder';
                       input_sequence_list = inputdlg('Enter new target image sequence e.g. [1 2 3 4]', dialog_title);
                       target_sequence_list = str2num(input_sequence_list{1});
                       num_targets = length(target_sequence_list);
                        
                       % warn user of problems with the target sequence
                       disp([num2str(num_targets), ' indexes in target sequence list'])
                       if max(target_sequence_list) > num_images
                            disp(['Warning: target sequence list attempts to address image ',num2str(max(target_sequence_list)), ' but sequence only has ', num2str(num_images), ' images. Will clamp target images to ', num2str(num_images)])
                       end
                       if length(target_sequence_list) > 32768
                            disp(['Warning: target sequence list is length ', num2str(num2str(length(target_sequence_list))), ' but max allowable length is 32768. Will ignore extra target sequence entries.'])
                       end
                
                       disp('Sequence mode: custom. Using custom order for image sequence')
                       target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', board_number, num_targets, target_sequence_list);
                       if target_sequence_error
                        last_error = calllib(dll_name, 'Get_last_error_message');
                        disp(['Error during target sequence loading:',last_error])
                       end
                   end
                   
                   %Always reloads the LUT file (in case the user has made a change to the LUT file in the background)
                   disp('Reloading LUT... please wait.');
                   overdrive_available = calllib(dll_name, 'Load_overdrive_LUT_file', regional_lut_file); %1 = error
                   %Note: uses 'Configure_overdrive' rather than 'Load_overdrive_LUT_file' so that phase wrapping and
                   %target sequence list will be generated automatically.

                   %Redownloads images (rather than check to see if the image sequence has actually changed)
                   % if we're in stripe mode or if we changed the folder.
                   if strcmp(answer{7},'1')||strcmp(answer{4},'1')
                       disp('Re-Downloading images... please wait.');
                       download_error = calllib(dll_name, 'Download_image_sequence', board_number, num_images, image_sequence, max_OD_frames);
                       if download_error
                               last_error = calllib(dll_name, 'Get_last_error_message');
                               disp(['Error during sequence download:',last_error])
                       end
                   end
                   
                   % using subsequence of the target image list
                   if strcmp(answer{1},'1')
                       start_index = str2num(answer{2});
                       subsequence_length = str2num(answer{3});
                       disp(['Now using subsequence of start_index', num2str(start_index), ' and length ',num2str(subsequence_length)])
                       if subsequence_length+start_index>num_targets
                           disp(['Subsequencing is attempting to use target index ', num2str(subsequence_length+start_index), ' but target image sequence only contains, ',num2str(num_targets), '. Using full sequence instead.']);
                       else
                           sequence_error = calllib(dll_name, 'Select_sequence_subset', board_number, start_index, subsequence_length);
                           sequence_error = sequence_error + calllib(dll_name, 'Restart_image_sequence', board_number); %Board will not start listening for triggers until restart is called.
                           if sequence_error
                            last_error = calllib(dll_name, 'Get_last_error_message');
                            disp(['Error during subsequence selection:',last_error])
                           end
                       end
                   end
                   disp('Cycling through triggered sequence. Click mouse to reload LUT file & change parameters, or press any key to stop the sequence.');
             
                else
                   %Keyboard press; stop sequencing
                   calllib(dll_name, 'Stop_image_sequence', board_number);
                   disp('Sequence stopped.')
                end
           end
           disp('Displaying final image. Press any key to turn off SLM and exit.')
           k = waitforbuttonpress();    %Keep displaying the last image until user clicks or key presses again.
       else
           disp('Please select either "individual" or "sequence" for write_mode')
       end
    end
        

    % -------------------------------------------------------------------
    %           Cleanup
    % -------------------------------------------------------------------
    % Turn the SLM power off
    SLM_power_off_error = calllib(dll_name, 'SLM_power', 0);
    if SLM_power_off_error
        disp('Error turning SLM power off');
    else
        disp('Turned SLM power off.');
    end
end

% Always call Delete_SDK before exiting
calllib(dll_name, 'Delete_SDK');
disp(strcat(dll_name, ' exited.'));

fprintf('\n')
unloadlibrary(dll_name);