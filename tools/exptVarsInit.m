function exptVars = exptVarsInit(inputParams, batchSettings)
    exptVars = xmlVersionCheck(danParseXML('Experiment.xml'));
    exptVars.frameRate = 10; % for images acquired from micromanager
    exptVars.dffWindow = floor(exptVars.frameRate * inputParams.winSizeSeconds); % 600 or [i-300, i+300]
    exptVars.rPixels = round(inputParams.R/exptVars.micronsPerPixel);
    exptVars.error = false;
    if (exptVars.dffWindow > batchSettings.numInitFrames)
        promptMessage = sprintf('Initial batch should have at least %d frames',exptVars.dffWindow + 1);
        msgbox(promptMessage,'Error','error');
        exptVars.error = true;
    end
end