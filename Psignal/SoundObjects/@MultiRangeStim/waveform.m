function [w, ev]=waveform (o,index)
global globalparams HW
Duration=get(o,'Duration');
Names = get(o,'Names');
if length(Names) > 1
    Frequency = ifstr2num(Names{index(1)});
else
    Frequency = ifstr2num(Names{1});
end
LF = get(o,'LowFrequency');
HF = get(o,'HighFrequency');
SamplingRate=globalparams.HWparams.fsAO;
PreStimSilence=get(o,'PreStimSilence');
PostStimSilence=get(o,'PostStimSilence');
Type=deblank(get(o,'Type'));
% batten=get(o,'NoiseAtten');
batten=99;
AMRate = get(o,'AM');
if ~strcmpi(Type,'tone')
    period=1/Frequency(1);
    period=round(period*SamplingRate);
    w=zeros(round(Duration(1)*SamplingRate),1);
end
switch lower(Type)
    case 'tone'
        t=0:1/SamplingRate:Duration(1)-(1/SamplingRate);
        w=sin(2*pi*Frequency(1)*t)';
    case 'irn'
        duration=get(o,'duration');
        period=1/Frequency(1);
        [irn clicktrain AMNoise HarmStack MissFund]=GenPitchStim(o, period, duration);
        w=irn;
    case 'pitchstim'
        duration=get(o,'duration');
        period=1/Frequency(1);
        [irn clicktrain AMNoise HarmStack MissFund]=GenPitchStim(o, period, duration);
        switch index(5)
            case 1
                pitchstim='IRN';
                w=irn;
            case 2
                pitchstim='clicktrain';
                w=clicktrain;
            case 3
                pitchstim='AMNoise';
                w = AMNoise;
            case 4
                pitchstim = 'HarmStack';
                w = HarmStack;
            case 5
                pitchstim = 'MissFund';
                w = MissFund;
        end
    case 'whitenoise'
        w = wgn(SamplingRate.*Duration(1),1,1);
        w=w./max(abs(w));
        w=w-mean(w);
%         [b a] = butter(6,LF/(SamplingRate/2),'high');
%         w = filtfilt(b,a,w);
%         [b a] = butter(6,HF/(SamplingRate/2),'low');
%         w = filtfilt(b,a,w);
end
clear t
%Apply amplitude moudlation, if rate > 0
if AMRate > 0 && ~strcmpi(lower(Type),'click')
    t=0:1/SamplingRate:Duration-(1/SamplingRate);
    AM = .5*(cos(2*pi*AMRate.*t)+1);
    w = w.*AM';
end
if batten < 80  && ~strcmpi(lower(Type), 'whitenoise') %Add background noise
    bnoise = -1 + 2.*rand(SamplingRate.*Duration(1),1);
    bnoise=bnoise-mean(bnoise);
    [b a] = butter(6,LF/(SamplingRate/2),'high');
    bnoise = filtfilt(b,a,bnoise);
    [b a] = butter(6,HF/(SamplingRate/2),'low');
    bnoise = filtfilt(b,a,bnoise);
    %Normalize to +/-1
    MAX = max(abs(bnoise));
    bnoise = bnoise./MAX;
    bnoise=bnoise*10^(-batten/20);  %noise attenuation in dB
    w = w+bnoise;
    %Normalize to +/-1
    MAX = max(abs(w));
    w = w./MAX;
end
clear t
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
w = [zeros(round(PreStimSilence*SamplingRate),1) ; w(:) ;zeros(round(PostStimSilence*SamplingRate),1)];
% and generate the event structure:
ev = struct('Note',['PreStimSilence , ' num2str(Frequency(1))],...
    'StartTime',0,'StopTime',PreStimSilence,'Trial',[]);
ev(2) = struct('Note',['Stim , ' num2str(Frequency(1))],'StartTime'...
    ,PreStimSilence, 'StopTime', PreStimSilence+Duration(1),'Trial',[]);
ev(3) = struct('Note',['PostStimSilence , ' num2str(Frequency(1))],...
    'StartTime',PreStimSilence+Duration(1), 'StopTime',PreStimSilence+Duration(1)+PostStimSilence,'Trial',[]);
function [irn clicktrain AMNoise HarmStack MissFund] = GenPitchStim(o, period, duration)
global globalparams
fs=globalparams.HWparams.fsAO;
shift = ceil(period*fs);
itr=5;
t=0:1/fs:duration+((shift*itr)/fs)-(1/fs);
%IRN
x = wgn(length(t),1,1);
x=x./max(abs(x));
y=[zeros((shift*itr),1); x; zeros((shift*itr),1)];
for i=1:itr
    y1=circshift(y,shift,1);
    y=y+y1;
end
irn=y(2*shift*itr+1:end-(shift*itr));
%Click train
clicktrain = zeros(size(irn));
clicktrain(1:shift:end,1)=1;
%AM Noise
f0=1/period;
t=0:1/fs:duration-(1/fs);
AMtone = sin(2*pi*f0.*t);
x = wgn(length(t),1,1)';
x=x./max(abs(x));
AMNoise = AMtone.*x;
%Harmonic stacks
HarmStack=0;
for i = 1:(fs/2.5)/f0
    HarmStack = HarmStack+sin(2*pi*f0*i.*t);
end
HarmStack = HarmStack./max(abs(HarmStack));
%Missing F0
MissFund=0;
for i = 2:(fs/2.5)/f0
    MissFund = MissFund+sin(2*pi*f0*i.*t);
end
MissFund = MissFund./max(abs(MissFund));
function ev=updateEvs(ev,tag)
for i=1:length(ev)
    ev(i).Note = deblank([ev(i).Note ' ,' tag]);
end
