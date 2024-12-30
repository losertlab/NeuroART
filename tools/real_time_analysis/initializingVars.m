        dimX = exptVars.dimX;
        dimY = exptVars.dimY;
        IMG = nan(dimX,dimY,length(batchSettings.frameBlock)); %% change to length(fileInd:length(files))
            
        frameid = 1;
        wait = 0;