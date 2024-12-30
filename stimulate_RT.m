function success = stimulate_RT(nTargets,dll_name,board_number)

    success = 0;
    loadlibrary('PiUsb', 'PiUsb.h');
    [stimShutter, ~] = calllib('PiUsb', 'piConnectShutter', 0, 627); 
         
    calllib('PiUsb', 'piSetShutterState', 1, stimShutter); % open USB shutter 

    ii = horzcat(1,1:nTargets); % add additional 1 to the list (first pattern don't last for more than 5ms)
    for idx = ii                  
        seq = idx*ones(1,3);
%         calllib('PiUsb', 'piSetShutterState', 1, stimShutter); % open USB shutter 
        target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', board_number, nTargets, seq);                   
%         pause(0.05); % stim. duration = 300ms (should be a user specified parameter in future)
%         calllib('PiUsb', 'piSetShutterState', 0, stimShutter); % close USB shutter
%         pause(0.02);
    end         
    calllib('PiUsb', 'piSetShutterState', 0, stimShutter); % close USB shutter
       
    % Timing test for iterating through a sequence of holograms
%     calllib('PiUsb', 'piSetShutterState', 1, stimShutter); % open USB shutter 
%     for idx = 1:10                   
%         seq = [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 3 3 3 3];
%         tst = tic;
%         target_sequence_error = calllib(dll_name, 'Load_target_sequence_list', board_number, length(seq), seq);      
%         target_sequence_error = target_sequence_error + calllib(dll_name, 'Restart_image_sequence', board_number);
%         toc(tst)
%     end     
%     calllib('PiUsb', 'piSetShutterState', 0, stimShutter); % close USB shutter

    success = 1;

end