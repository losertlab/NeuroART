function [IMG, wait, frameid, fh, tstack,exptVars] = readInitialBatch(inputParams,batchSettings,exptVars)


    % scope: 1) Thorlabs B-Scope 2) Bruker Ultima 2P+ 3) Bruker Ultima 2P 4) BEAMM(ScanImage) 5) Nikon 6) Offline Mode
    tstack = [];
    fh = [];
    
    if (inputParams.SCOPE == 1) || (inputParams.SCOPE == 2) || (inputParams.SCOPE == 3) || (inputParams.SCOPE == 4) ||(inputParams.SCOPE == 6 && inputParams.FORMAT == ".raw")
        frameSize = exptVars.dimX*exptVars.dimY;
        initializingVars;
            
        %% Read the frames in iteratively
        
        readingFrames;

    elseif (inputParams.SCOPE == 2 && inputParams.FORMAT == ".tif") 
        initializingVars;
    
        %% Read the frames in iteratively 
    
        IteratingReadingFrames;
        
    elseif (inputParams.SCOPE == 5 && inputParams.FORMAT == ".tif") || (inputParams.SCOPE == 6 && inputParams.FORMAT == ".tif") % Nikon real-time or offline analysis

        % Initialize variables
        wait = 1; % timeout in seconds
        frameid = 1; % starting frame number

        stackFile = inputParams.imageFile;

        doesFileExist;


        % Infinite loop to keep checking for new frames within the stack
        checkingFrames;
        
    else
        frameid = 1;
        wait = 10000;
        dimX = exptVars.dimX;
        dimY = exptVars.dimY;
        IMG = nan(dimX,dimY,length(batchSettings.frameBlock));
        fh = -1; 
    end
    
%     batchResults = struct('IMG', IMG, 'wait', wait, 'frameId', frameid, 'fh', fh, 'tStack', tstack);
    if(wait == 10000)
        exception = MException("readInitialBatch:invalidParameter", 'Number of acquired images is insufficient to achieve the specified size of the initial batch');
        throw(exception);
    end
end

