function [w,event]=waveform (o,index,IsRef)
% function w=waveform(o, index);
% This is a generic waveform generator function for objects inherit from
% SoundObject class. It simply reads the Names field and load the one
% indicated by index. It assumes the files are in 'Sounds' subfolder in the
% object's folder.
global globalparams home HW
maxIndex = get(o,'MaxIndex');
if index > maxIndex
    error (sprintf('Maximum possible index is %d',maxIndex));
end
event=[];
HWAmpScale = globalparams.HWparams.HWAmpScale;
SamplingRate=globalparams.HWparams.fsAO;
PreStimSilence = ifstr2num(get(o,'PreStimSilence'));
PostStimSilence = ifstr2num(get(o,'PostStimSilence'));
object_spec = what(class(o));
% if object has a field specifying sound path, use this. Otherwise,
% default is <objectpath>/Sounds/
allfields=get(o);
if isfield(allfields,'SoundPath'),
   soundpath=get(o,'SoundPath');
else
   soundpath = [home filesep 'Waveforms'];
end
files = get(o,'Names');
sampindex = strfind(files{index},'sample#');
if ~isempty(sampindex)
    files{index}(sampindex+7) = num2str(ceil(rand(1)*5));
end
bb = strsep(files{index},'_');
[w,fs] = audioread([soundpath filesep bb{1} filesep files{index} '.wav']);
% Check the sampling rate:
if fs~=SamplingRate
    w = resample(w, SamplingRate, fs);
end
% If the object has Duration parameter, cut the sound to match it, if
% possible:
if isfield(get(o),'Duration')
    Duration = ifstr2num(get(o,'Duration'));
    totalSamples = floor(Duration * SamplingRate);
    w = w(1:min(length(w),totalSamples));
else
    Duration = length(w) / SamplingRate;
end
%Make w +/- vRef from calibration
MAX = max(abs(w));
norm =  HW.Calibration.VRef/MAX;
w = norm * w;
%10 ms ramp:
w = w(:);
ramp = hanning(round(.02 * SamplingRate*2));
ramp = ramp(1:floor(length(ramp)/2));
w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);
%Filter w according to speaker calibration.
spec = HW.Calibration.WhiteningSpec';
mic = HW.Calibration.Microphone;
w = IOCalibrationFilter(w, spec, mic);
%10 ms ramp:
w = w(:);
w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);
% Now, put it in the silence:
w = [zeros(ceil(PreStimSilence*SamplingRate),1) ; w(:) ;zeros(ceil(PostStimSilence*SamplingRate),1)];
% and generate the event structure:
event = struct('Note',['PreStimSilence , ' files{index}],...
    'StartTime',0,'StopTime',PreStimSilence,'Trial',[]);
event(2) = struct('Note',['Stim , ' files{index}],'StartTime'...
    ,PreStimSilence, 'StopTime', PreStimSilence+Duration, 'Trial',[]);
event(3) = struct('Note',['PostStimSilence , ' files{index}],...
    'StartTime',PreStimSilence+Duration, 'StopTime',PreStimSilence+Duration+PostStimSilence,'Trial',[]);