function Calibration = IOLoadSpeakerCalibration(varargin)
% Nikolas A. Francis 2018

global home globalparams
if length(varargin)==1 % Arguments provided as a struct
    P = varargin{1};
else
    P = parsePairs(varargin);
end
if ~isfield(P,'Speaker')
    error('A speaker must be specified!');
end
if ~isfield(P,'Microphone')
    error('A microphone must be specified');
end
SpeakerPath = home;
if isdeployed
    FileName = [SpeakerPath,'\SpeakerCalibration_',P.Speaker,'_',P.Microphone,'_',globalparams.Rig,'.mat'];
else
    [FileName FilePath] = uigetfile([home '\*.mat'],'Load Speaker Calibration File');
    FileName = [FilePath '\' FileName];
end
if exist(FileName,'file')
    tmp = load(FileName);
    R = tmp.R;
else
    error(['Calibration File "',FileName,'" does not exist.']);
end
% Return what has been passed (Speaker and Microphone names)
Calibration = P;
%Whitening filter to flatten speaker output
Calibration.WhiteningSpec = R.WhiteningSpec;
% SR is passed in case a different SR is used and downsampling is necessary
Calibration.SR = R.Fs;
Calibration.VRef = R.VRef;
Calibration.cdBSPL = R.dBSPLRef;