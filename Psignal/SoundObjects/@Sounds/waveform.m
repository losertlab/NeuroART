function [w, ev]=waveform (o,index)
global globalparams HW
Duration=get(o,'Duration');
Names = get(o,'Names');
SamplingRate=globalparams.HWparams.fsAO;
PreStimSilence=get(o,'PreStimSilence');
PostStimSilence=get(o,'PostStimSilence');
Type=deblank(get(o,'Type'));
NoiseParams=get(o,'BackgroundNoise');
AMRate = get(o,'AM');

if length(index)>1 && index(1) ~= -1
    if length(Names) > 1
        Frequency = ifstr2num(Names{index(1)});
    else
        Frequency = ifstr2num(Names{1});
    end
else
    Frequency=0;
    Type='backgroundnoise';
end
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
    case 'clicktrain'
        duration=get(o,'duration');
        period=1/Frequency(1);
        [irn clicktrain AMNoise HarmStack MissFund]=GenPitchStim(o, period, duration);
        w=clicktrain;
    case 'AMNoise'
        duration=get(o,'duration');
        period=1/Frequency(1);
        [irn clicktrain AMNoise HarmStack MissFund]=GenPitchStim(o, period, duration);
        w=AMNoise;
    case 'HarmStack'
        duration=get(o,'duration');
        period=1/Frequency(1);
        [irn clicktrain AMNoise HarmStack MissFund]=GenPitchStim(o, period, duration);
        w=HarmStack;
    case 'MissFund'
        duration=get(o,'duration');
        period=1/Frequency(1);
        [irn clicktrain AMNoise HarmStack MissFund]=GenPitchStim(o, period, duration);
        w=MissFund;
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
end
clear t
if ~strcmpi('type','backgroundnoise')
    %Apply amplitude moudlation, if rate > 0
    if AMRate > 0 && ~strcmpi(lower(Type),'click')
        t=0:1/SamplingRate:Duration-(1/SamplingRate);
        AM = .5*(cos(2*pi*AMRate.*t)+1);
        w = w.*AM';
    end
end

%Ramp
ramp = hanning(round(.02 * SamplingRate*2));
ramp = ramp(1:floor(length(ramp)/2));

%Background noise
if index == -1
    NDur = diff(NoiseParams(2:end));
    bnoise = -1 + 2.*rand(SamplingRate.*NDur,1);
    bnoise=bnoise-mean(bnoise);
    %10 ms ramp:
    bnoise = bnoise(:);
    bnoise(1:length(ramp)) = bnoise(1:length(ramp)) .* ramp;
    bnoise(end-length(ramp)+1:end) = bnoise(end-length(ramp)+1:end) .* flipud(ramp);
    %Normalize to +/-1
    MAX = max(abs(bnoise));
    bnoise = bnoise./MAX;
    w=bnoise*10^(NoiseParams(1)/20);
end
clear t

%Make w +/- vRef from calibration
MAX = max(abs(w));
if ~isempty(HW.Calibration)
    norm =  HW.Calibration.VRef/MAX;
    w = norm * w;
else
    w = w./MAX;
end

%10 ms ramp:
w = w(:);
ramp = hanning(round(.02 * SamplingRate*2));
ramp = ramp(1:floor(length(ramp)/2));
w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);

%Filter w according to speaker calibration.
if ~isempty(HW.Calibration)
    spec = HW.Calibration.WhiteningSpec';
    mic = HW.Calibration.Microphone;
    w = IOCalibrationFilter(w, spec, mic);
end

%10 ms ramp:
w = w(:);
w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);

% Now, put it in the silence:
if index ~=-1
    w = [zeros(round(PreStimSilence*SamplingRate),1) ; w(:) ;zeros(round(PostStimSilence*SamplingRate),1)];
else
    %Silence
    TrialDur = PreStimSilence+Duration+PostStimSilence;
    PostNoiseSilence = TrialDur-NoiseParams(3);
    w = [zeros(round(NoiseParams(2)*SamplingRate),1) ; w(:) ;zeros(round(PostNoiseSilence*SamplingRate),1)];
end

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
AMtone = (cos(2*pi*f0.*t)+1);
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
