function batchResults = readInitialBatch(inputParams, batchSettings, exptVars)

    % scope: 1) Thorlabs B-Scope 2) Bruker Ultima 2P+ 3) Bruker Ultima 2P 4) BEAMM(ScanImage) 5) MOM-Thors 6) Offline Mode
    tstack = [];
    if (inputParams.SCOPE == 1) || (inputParams.SCOPE == 2) || (inputParams.SCOPE == 3) ||(inputParams.SCOPE == 6 && inputParams.FORMAT == ".raw")
        dimX = exptVars.dimX;
        dimY = exptVars.dimY;
        frameSize = dimX*dimY;
        IMG = nan(dimX,dimY,length(batchSettings.frameBlock)); %% change to length(fileInd:length(files))
    
        frameid = 1;
        wait = 0;
    
        %% Read the frames in iteratively
    
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
    elseif (inputParams.SCOPE == 2 && inputParams.FORMAT == ".tif") 
        dimX = exptVars.dimX;
        dimY = exptVars.dimY;
        IMG = nan(dimX,dimY,length(batchSettings.frameBlock)); %% change to length(fileInd:length(files))
    
        frameid = 1;
        wait = 0;
    
        %% Read the frames in iteratively 
    
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
        
    elseif (inputParams.SCOPE == 6 && inputParams.FORMAT == ".tif")
        frameid = 1;
        wait = 0;
        %dimX = exptVars.dimX;
        %dimY = exptVars.dimY;
        %frameSize = dimX*dimY;
        
        %% Tiff reading code written by Nick 09/15
        warning('off','all') % Suppress all the tiff warnings
        tstack  = Tiff(inputParams.imageFile);
        [I,J] = size(tstack.read());
        K = min(length(imfinfo(inputParams.imageFile)),length(batchSettings.frameBlock)*inputParams.NUMCHAN);
        data = zeros(I,J,K);
        data(:,:,1)  = tstack.read();
        for n = 2:K
            tstack.nextDirectory()
            data(:,:,n) = tstack.read();
        end
        warning('on','all')
        IMG = zeros([size(data,1) size(data,2) length(batchSettings.frameBlock)]);
        
        for i=1:size(data,3)
            if inputParams.NUMCHAN==1 || mod(i,inputParams.NUMCHAN) == batchSettings.GREENCHAN ||  ((mod(i,inputParams.NUMCHAN) == 0) && (inputParams.NUMCHAN == batchSettings.GREENCHAN))  % Only read if it belongs to the Green Channel
                frameid = frameid + 1;
                IMG(:,:,frameid)=data(:,:,i);
                if frameid == length(batchSettings.frameBlock)
                    break;
                end
            end
        end
        
        fh = fopen(inputParams.imageFile);
        
    %     IMG = nan(dimX,dimY,length(batchSettings.frameBlock));
    % 
    %     info = imfinfo(inputParams.imageFile);
    %     fh = fopen(inputParams.imageFile);
    %     a = info(1).StripOffsets + 2384;
    %     fseek(fh,0,'bof');
    %     imgFrame = reshape(fread(fh, frameSize, 'uint16=>uint16'),[dimX dimY]); % Read the first frame
    % 
    %     if (batchSettings.GREENCHAN == 1)
    %         IMG(:,:,frameid) = imgFrame';
    %         frameid = frameid + 1;
    %     end
    % 
    %     itr = 1; 
    %     offset = a + frameSize*2;  % uint16 --> 2 bytes  
    % 
    %     while frameid <= length(batchSettings.frameBlock) &&  wait < 10000 % maximum wait per frame && itr < inputParams.NUMCHAN*numImagesToRead 
    % 
    %         fseek(fh,0,'eof');
    %         numFramesRecorded = floor((ftell(fh)- info(1).StripOffsets)/(2*frameSize+2384));
    % %         numFramesRecorded = itr+1;
    % 
    %         if numFramesRecorded > itr % detection of a new frame, assuming the raw file isn't fully preallocated
    % 
    %             if inputParams.NUMCHAN==1 || mod(itr+1,inputParams.NUMCHAN) == batchSettings.GREENCHAN ||  ((mod(itr+1,inputParams.NUMCHAN) == 0) && (inputParams.NUMCHAN == batchSettings.GREENCHAN))  % Only read if it belongs to the Green Channel
    %                 fseek(fh,offset,'bof');
    %                 imgFrame = reshape(fread(fh, frameSize, 'uint16=>uint16'),[dimX dimY]); % Read the (itr+1)th frame
    %                 IMG(:,:,frameid) = imgFrame';
    %                 frameid = frameid + 1;
    %             end
    %             wait = 0;
    %             itr = itr + 1;
    %             offset = info(1).StripOffsets + (2*frameSize+2384)*itr; % uint16 --> 2 bytes
    %         else
    %             wait = wait + 1;
    %         end
    %         pause(0.007);
    %     end
        
    else
        frameid = 1;
        wait = 10000;
        dimX = exptVars.dimX;
        dimY = exptVars.dimY;
        IMG = nan(dimX,dimY,length(batchSettings.frameBlock));
        fh = -1; 
    end
    batchResults = struct('IMG', IMG, 'wait', wait, 'frameId', frameid, 'fh', fh, 'tStack', tstack);
    if(batchResults.wait == 10000)
        exception = MException("readInitialBatch:invalidParameter", 'Number of acquired images is insufficient to achieve the specified size of the initial batch');
        throw(exception);
    end
end

