function exptVars = exptVarsInit(inputParams, batchSettings)
    exptVars = xmlVersionCheck(danParseXML('Experiment.xml'));
    exptVars.frameRate = inputParams.frameRate;
    exptVars.dffWindow = floor(exptVars.frameRate * inputParams.winSizeSeconds); % 600 or [i-300, i+300]
    exptVars.rPixels = round(inputParams.R/exptVars.micronsPerPixel);
    exptVars.error = false;
%     if (exptVars.dffWindow > batchSettings.numInitFrames)
%         promptMessage = sprintf('Initial batch should have at least %d frames',exptVars.dffWindow + 1);
%         msgbox(promptMessage,'Error','error');
%         exptVars.error = true;
%     end

    if exptVars.dffWindow > batchSettings.numInitFrames
        exception = MException("exptVarsInit:frameSizeError", 'Initial batch should have at least %d frames', exptVars.dffWindow + 1);
        throw(exception);
    end
end