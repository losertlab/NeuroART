[filepath,name,ext] = fileparts(inputParams.imageFile);
        
        while frameid <= length(batchSettings.frameBlock) &&  wait < 10000 % maximum wait per frame && itr < inputParams.NUMCHAN*numImagesToRead 
            
            fName = fullfile(filepath, [name '_Ch' num2str(batchSettings.GREENCHAN) '_' sprintf('%05d',frameid) '.ome' ext]); % Format of the Bruker tif filenames 
            
            if isfile(fName) % detection of a new frame
                IMG(:,:,frameid) = imread(fName); % Read the new frame
                wait = 0;
                frameid = frameid + 1;
            else
                wait = wait + 1;
            end
            pause(0.003);
        end
        
        fh = fopen(fName);