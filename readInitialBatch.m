function [IMG, wait, frameid, fh, tstack] = readInitialBatch(inputParams,batchSettings,exptVars)

%fileName,frameBlock,exptVars,greenChannel,numChannels,scope,imgType)

fileName = inputParams.imageFile;
numChannels = inputParams.NUMCHAN;
scope = inputParams.SCOPE;
imgType = inputParams.FORMAT;

frameBlock = batchSettings.frameBlock;
greenChannel = batchSettings.GREENCHAN;

% scope: 1) Thorlabs B-Scope 2) Bruker Ultima 2P+ 3) Bruker Ultima 2P 4) BEAMM(ScanImage) 5) MOM-Thors 6) Offline Mode
tstack = [];
if (scope == 1) || (scope == 2) || (scope == 3) ||(scope == 6 && imgType == ".raw")
    dimX = exptVars.dimX;
    dimY = exptVars.dimY;
    frameSize = dimX*dimY;
    IMG = nan(dimX,dimY,length(frameBlock)); %% change to length(fileInd:length(files))

    frameid = 1;
    wait = 0;

    %% Read the frames in iteratively

    fh = fopen(fileName);
    imgFrame = reshape(fread(fh, frameSize, 'uint16=>uint16'),[dimX dimY]); % Read the first frame

    if (greenChannel == 1)
        IMG(:,:,frameid) = imgFrame;
        frameid = frameid + 1;
    end

    itr = 1; 
    offset = frameSize*2;  % uint16 --> 2 bytes  

    while frameid <= length(frameBlock) &&  wait < 10000 % maximum wait per frame && itr < numChannels*numImagesToRead 

        fseek(fh,0,'eof');
        numFramesRecorded = floor(ftell(fh)/(frameSize*2));

        if numFramesRecorded > itr % detection of a new frame, assuming the raw file isn't fully preallocated

            if numChannels==1 || mod(itr+1,numChannels) == greenChannel || (mod(itr+1,numChannels) == 0 && greenChannel == numChannels)  % Only read if it belongs to the Green Channel
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
elseif (scope == 2 && imgType == ".tif") 
    dimX = exptVars.dimX;
    dimY = exptVars.dimY;
    IMG = nan(dimX,dimY,length(frameBlock)); %% change to length(fileInd:length(files))

    frameid = 1;
    wait = 0;

    %% Read the frames in iteratively 

    [filepath,name,ext] = fileparts(fileName);
    
    while frameid <= length(frameBlock) &&  wait < 10000 % maximum wait per frame && itr < numChannels*numImagesToRead 
        
        fName = fullfile(filepath, [name '_Ch' num2str(greenChannel) '_' sprintf('%05d',frameid) '.ome' ext]); % Format of the Bruker tif filenames 
        
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
    
elseif (scope == 6 && imgType == ".tif")
    frameid = 1;
    wait = 0;
    dimX = exptVars.dimX;
    dimY = exptVars.dimY;
    frameSize = dimX*dimY;
    
    %% Tiff reading code written by Nick 09/15
    warning('off','all') % Suppress all the tiff warnings
    tstack  = Tiff(fileName);
    [I,J] = size(tstack.read());
    K = min(length(imfinfo(fileName)),length(frameBlock)*numChannels);
    data = zeros(I,J,K);
    data(:,:,1)  = tstack.read();
    for n = 2:K
        tstack.nextDirectory()
        data(:,:,n) = tstack.read();
    end
    warning('on','all')
    IMG = zeros([size(data,1) size(data,2) length(frameBlock)]);
    
    for i=1:size(data,3)
        if numChannels==1 || mod(i,numChannels) == greenChannel ||  ((mod(i,numChannels) == 0) && (numChannels == greenChannel))  % Only read if it belongs to the Green Channel
            frameid = frameid + 1;
            IMG(:,:,frameid)=data(:,:,i);
            if frameid == length(frameBlock)
                break;
            end
        end
    end
    
    fh = fopen(fileName);
    
%     IMG = nan(dimX,dimY,length(frameBlock));
% 
%     info = imfinfo(fileName);
%     fh = fopen(fileName);
%     a = info(1).StripOffsets + 2384;
%     fseek(fh,0,'bof');
%     imgFrame = reshape(fread(fh, frameSize, 'uint16=>uint16'),[dimX dimY]); % Read the first frame
% 
%     if (greenChannel == 1)
%         IMG(:,:,frameid) = imgFrame';
%         frameid = frameid + 1;
%     end
% 
%     itr = 1; 
%     offset = a + frameSize*2;  % uint16 --> 2 bytes  
% 
%     while frameid <= length(frameBlock) &&  wait < 10000 % maximum wait per frame && itr < numChannels*numImagesToRead 
% 
%         fseek(fh,0,'eof');
%         numFramesRecorded = floor((ftell(fh)- info(1).StripOffsets)/(2*frameSize+2384));
% %         numFramesRecorded = itr+1;
% 
%         if numFramesRecorded > itr % detection of a new frame, assuming the raw file isn't fully preallocated
% 
%             if numChannels==1 || mod(itr+1,numChannels) == greenChannel ||  ((mod(itr+1,numChannels) == 0) && (numChannels == greenChannel))  % Only read if it belongs to the Green Channel
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
    IMG = nan(dimX,dimY,length(frameBlock));
    fh = -1; 
end


