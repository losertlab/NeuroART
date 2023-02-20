function success = stimulate_RT(nTargets,dll_name,board_number)

    success = 0;
    loadlibrary('PiUsb', 'PiUsb.h');
    [stimShutter, ~] = calllib('PiUsb', 'piConnectShutter', 0, 627); 
                        
    for idx = 1:nTargets                   
        seq = idx*ones(1,15);
        calllib('PiUsb', 'piSetShutterState', 1, stimShutter); % open USB shutter 
        target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', board_number, 3, seq);                   
        pause(0.3); % stim. duration = 300ms (should be a user specified parameter in future)
        calllib('PiUsb', 'piSetShutterState', 0, stimShutter); % close USB shutter
    end              

    success = 1;

end