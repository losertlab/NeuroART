function exptVars = xmlVersionCheck(XML,numImagesOverwrite)

% Check ThorImage version for correct pointers -- could also use a strfind
% but some of the str names changed with newer versions therefore you have
% to check every time a new version comes out anyway...
if strcmp(XML.Children(10).Attributes.Value,'4.0.2020.2171')||strcmp(XML.Children(10).Attributes.Value,'4.0.2019.8191')
    fprintf('Experiment used ThorImage Version %s\n',XML.Children(10).Attributes.Value);
    exptVars.dimX = str2double(XML.Children(26).Attributes(36).Value); % in pixels
    exptVars.dimY = str2double(XML.Children(26).Attributes(37).Value);
    exptVars.numImages = str2double(XML.Children(42).Attributes(7).Value);
    exptVars.micronsPerPixel = str2double(XML.Children(26).Attributes(35).Value);
    exptVars.frameRate = str2double(XML.Children(26).Attributes(23).Value); 
    exptVars.flybackFrames = str2double(XML.Children(42).Attributes(4).Value);
    exptVars.stepSizeUM = str2double(XML.Children(18).Attributes(5).Value);
    exptVars.rStagePos = str2double(XML.Children(64).Attributes(1).Value);
    exptVars.numZSteps = str2double(XML.Children(18).Attributes(6).Value);
    exptVars.totalZplanes = exptVars.numZSteps+exptVars.flybackFrames;
    exptVars.ZstageEnable =  str2double(XML.Children(18).Attributes(1).Value);
    if exptVars.ZstageEnable==0
        exptVars.flybackFrames = 0;
        exptVars.numZSteps = 1;
        exptVars.totalZplanes = 1;
    end
    exptVars.ChannelEnableSet = XML.Children(26).Attributes(13).Value;
    % this integer basically encodes active channels with corresponding bit number
    % 1-A, 2-B, 3-C, 4-D
    switch exptVars.ChannelEnableSet
        case {'1','2','4','8'}
            exptVars.NumChannels = 1;
        case {'3','5','6','9','10','12'}
            exptVars.NumChannels = 2;
        case {'15'}
            exptVars.NumChannels = 4;    
        otherwise
            fprintf('Not sure how many channels used in set number %s,\n but most likely...\n',exptVars.ChannelEnableSet);
            exptVars.NumChannels = 3;
    end
    fprintf('Number of channels used = %d\n',exptVars.NumChannels);
        
elseif strcmp(XML.Children(10).Attributes.Value,'3.1.2017.10021')
    fprintf('Experiment used ThorImage Version %s\n',XML.Children(10).Attributes.Value)
    exptVars.dimX = str2double(XML.Children(26).Attributes(31).Value); % in pixels
    exptVars.dimY = str2double(XML.Children(26).Attributes(32).Value);
    exptVars.numImages = str2double(XML.Children(42).Attributes(5).Value);
    exptVars.micronsPerPixel = str2double(XML.Children(26).Attributes(30).Value);
    exptVars.frameRate = str2double(XML.Children(26).Attributes(19).Value);
    exptVars.rStagePos = str2double(XML.Children(54).Attributes(1).Value);
    exptVars.ZstageEnable = str2double(XML.Children(42).Attributes(13).Value);
    if exptVars.ZstageEnable
        exptVars.flybackFrames = str2double(XML.Children(42).Attributes(4).Value);
        exptVars.stepSizeUM = str2double(XML.Children(18).Attributes(5).Value);
        exptVars.numZSteps = str2double(XML.Children(18).Attributes(6).Value);
        exptVars.totalZplanes = exptVars.numZSteps+exptVars.flybackFrames;
    else
        exptVars.flybackFrames = 0;
        exptVars.numZSteps = 1;
        exptVars.totalZplanes = 1;
    end
    
elseif strcmp(XML.Children(10).Attributes.Value,'3.0.2016.10131')
    fprintf('Experiment used ThorImage Version %s\n',XML.Children(10).Attributes.Value)
    exptVars.dimX = str2double(XML.Children(26).Attributes(23).Value); % in pixels
    exptVars.dimY = str2double(XML.Children(26).Attributes(24).Value);
    exptVars.numImages = str2double(XML.Children(34).Attributes(5).Value); % FIX ***************
    exptVars.micronsPerPixel = str2double(XML.Children(26).Attributes(22).Value);
    exptVars.frameRate = str2double(XML.Children(26).Attributes(11).Value);
    exptVars.flybackFrames = str2double(XML.Children(34).Attributes(4).Value);
    exptVars.stepSizeUM = str2double(XML.Children(18).Attributes(3).Value);
    exptVars.numZSteps = str2double(XML.Children(18).Attributes(4).Value);
    exptVars.rStagePos = str2double(XML.Children(46).Attributes(1).Value);
    exptVars.totalZplanes = exptVars.numZSteps+exptVars.flybackFrames;
    if exptVars.numZSteps==1
        exptVars.totalZplanes = 1;
    end
elseif strcmp(XML.Children(10).Attributes.Value,'2.1.2014.9102') % Not using old ThorImage moving forward
    fprintf('Experiment used ThorImage Version %s\n',XML.Children(10).Attributes.Value)
    exptVars.dimX = str2double(XML.Children(28).Attributes(16).Value); % in pixels
    exptVars.dimY = str2double(XML.Children(28).Attributes(16).Value);
    exptVars.dimXmicrons = str2double(XML.Children(22).Children(2).Children(2).Attributes(2).Value) *1000;
    exptVars.micronsPerPixel = exptVars.dimXmicrons/exptVars.dimX;
    exptVars.frameRate = 30; % FIND THIS XML POINTER
    exptVars.numImages = str2double(XML.Children(36).Attributes(5).Value);
    exptVars.flybackFrames = 0;
    exptVars.numZSteps = 1;
    exptVars.totalZplanes = exptVars.numZSteps+exptVars.flybackFrames;
else
    fprintf('Experiment used unfamiliar ThorImage Version %s, please add to code\n',XML.Children(10).Attributes.Value)
end

if exist('numImagesOverwrite','var') && ~isempty(numImagesOverwrite)
    if exptVars.numImages > numImagesOverwrite
        exptVars.numImages = numImagesOverwrite;
    end    
end

%% Version-invariant parameters
exptVars.numVolumes = round(exptVars.numImages/exptVars.totalZplanes); %not sure if it works this way actually
exptVars.numBytes = 2; % assumes 16-bit for all Thorlabs data
exptVars.segmentSize = (exptVars.dimX * exptVars.dimY * exptVars.numBytes); %size of image in bytes
        
end