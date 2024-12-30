% Check if the stack file exists
        if ~exist(stackFile, 'file')
            error('Stack file does not exist.');
        end

        fr = imread(stackFile, frameid);
        dims = size(fr);
        exptVars.dimX = dims(1);
        exptVars.dimY = dims(2);
        nFrames = length(batchSettings.frameBlock);

        IMG = zeros([dims(1),dims(2),nFrames],'uint16');