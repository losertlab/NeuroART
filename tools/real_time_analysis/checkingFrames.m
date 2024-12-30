while frameid < nFrames + 1
            try
                % Attempt to read the current frame from the stack
                fr = imread(stackFile, frameid);
                IMG(:,:,frameid) = fr;
                
                % Increment the frame number for the next iteration
                frameid = frameid + 1;
                
                % Pause for the next frame based on the desired frame rate
                % pause(1/frameRate);
        
            catch    
                % If the frame does not exist, wait for 1 second and check again           
                pause(wait);
                        
                % After timeout, try reading the frame again 
                try              
                    img = imread(stackFile, frameid);         
                    continue; % If successful, continue to the next iteration    
                catch              
                    disp('No new frame detected within timeout period. Exiting loop.');  
                    wait = 10000; % indicate that the required number of frames was not captured
                    break; % Exit the loop if no new frame appears after the timeout
                end
            end
        end