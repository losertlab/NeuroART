fh = fopen(inputParams.imageFile);
        imgFrame = reshape(fread(fh, frameSize, 'uint16=>uint16'),[dimX dimY]); % Read the first frame
            
        if (batchSettings.GREENCHAN == 1)
            IMG(:,:,frameid) = imgFrame;
            frameid = frameid + 1;
        end
        
        itr = 1; 
        offset = frameSize*2;  % uint16 --> 2 bytes  
        
        while frameid <= length(batchSettings.frameBlock) &&  wait < 10000 % maximum wait per frame && itr < inputParams.NUMCHAN*numImagesToRead 
        
            fseek(fh,0,'eof');
            numFramesRecorded = floor(ftell(fh)/(frameSize*2));
            
            if numFramesRecorded > itr % detection of a new frame, assuming the raw file isn't fully preallocated
        
                    if inputParams.NUMCHAN==1 || mod(itr+1,inputParams.NUMCHAN) == batchSettings.GREENCHAN || (mod(itr+1,inputParams.NUMCHAN) == 0 && batchSettings.GREENCHAN == inputParams.NUMCHAN)  % Only read if it belongs to the Green Channel
                        fseek(fh,offset,'bof');
                        imgFrame = reshape(fread(fh, frameSize, 'uint16=>uint16'),[dimX dimY]); % Read the (itr+1)th frame
                        IMG(:,:,frameid) = imgFrame;
                        frameid = frameid + 1;
                    end
                    wait = 0;
                    itr = itr + 1;
                    offset = 2*frameSize*itr; % uint16 --> 2 bytes
            else
                wait = wait + 1;
            end
            pause(0.003);
        end