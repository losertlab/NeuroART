function inputParamsNew = slmInit(inputParams)
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
    inputParamsNew = inputParams;
end