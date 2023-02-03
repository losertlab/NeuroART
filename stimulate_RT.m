function success = stimulate_RT(nTargets,dll_name,board_number)

    success = 0;
    loadlibrary('PiUsb', 'PiUsb.h');
    [stimShutter, ~] = calllib('PiUsb', 'piConnectShutter', 0, 627); 
    

    calllib('PiUsb', 'piSetShutterState', 1, stimShutter); % open USB shutter                     
    for idx = 1:nTargets                   
        seq = idx*ones(1,15);
        target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', board_number, 3, seq);                   
        pause(0.3);                 
    end              
        
    calllib('PiUsb', 'piSetShutterState', 0, stimShutter); % close USB shutter                  
       

    success = 1;

end