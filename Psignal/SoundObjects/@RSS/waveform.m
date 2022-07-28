function [w, ev]=waveform (o,index)
global globalparams home
Duration=get(o,'Duration');
Names = get(o,'Names');
SamplingRate=globalparams.HWparams.fsAO;
PreStimSilence=get(o,'PreStimSilence');
PostStimSilence=get(o,'PostStimSilence');
HWAmpScale = globalparams.HWparams.HWAmpScale;
w=load([home '\Waveforms\RSS\' Names{index}]);
w=w.wave;
%Normalize to +/-1
MAX = max(abs(w));
w = w./MAX;
%Filter w according to speaker calibration
w = IOCalibrationFilter(w);
%Make w +/- HWAmpScale
MAX = max(abs(w));
norm = HWAmpScale/MAX;
w = norm * w;
%10 ms ramp:
w = w(:);
ramp = hanning(round(.02 * SamplingRate*2));
ramp = ramp(1:floor(length(ramp)/2));
w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);
% Now, put it in the silence:
w = [zeros(round(PreStimSilence*SamplingRate),1) ; w(:) ;zeros(round(PostStimSilence*SamplingRate),1)];
% and generate the event structure:
ev = struct('Note',['PreStimSilence , ' Names{index}],...
    'StartTime',0,'StopTime',PreStimSilence,'Trial',[]);
ev(2) = struct('Note',['Stim , ' Names{index}],'StartTime'...
    ,PreStimSilence, 'StopTime', PreStimSilence+Duration(1),'Trial',[]);
ev(3) = struct('Note',['PostStimSilence , ' Names{index}],...
    'StartTime',PreStimSilence+Duration(1), 'StopTime',PreStimSilence+Duration(1)+PostStimSilence,'Trial',[]);
w =  w/max(abs(w));

