function PlayRandSounds(device, repeats)
global globalparams HW home
caller=dbstack;
if strcmp(caller(1).name,'Psignal') && ~exist('home','var')
    clear classes
    clear global
end
%% Setup pathways, rig and device id's
home=fileparts(which('Psignal'));
globalparams.Device = device;
configDAQ;
IOStopAcquisition(HW);
seconds=10;
HW=IOSetAnalogInDuration(HW,seconds);
%Sounds include: noise w/ rand envelope and frequency bands, FM sweeps at different rates and frequency bands,
%and tones
for reps = 1:repeats
    Sounds=[];
    %Noise
    envidx=[];
    while sum(envidx) < seconds
        r=.1 + (.75-.1)*rand;
        if sum([envidx; r]) < seconds
            envidx = [envidx; r];
        else
            break
        end
    end
    envidx = cumsum([1/HW.params.fsAO; envidx; seconds-sum(envidx)]);
    for i = 1:length(envidx)-1
        idx = round([envidx(i).*HW.params.fsAO:envidx(i+1).*HW.params.fsAO]);
        f1 = 4000 + (10000-4000)*rand;
        f2 = 100 + (38000-100)*rand;
        [b a] = butter(3,[f1 f1+f2]/(HW.params.fsAO/2));
        w = filtfilt(b,a,rand(length(idx),1));
        %5 ms ramp:
        ramp = hanning(round(.005 * HW.params.fsAO*2));
        ramp = ramp(1:floor(length(ramp)/2));
        w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
        w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);
        Sounds = [Sounds; {w./max(abs(w))}];
    end
    %FM sweeps
    envidx=[];
    while sum(envidx) < seconds
        r=.1 + (.75-.1)*rand;
        if sum([envidx; r]) < seconds
            envidx = [envidx; r];
        else
            break
        end
    end
    envidx = cumsum([1/HW.params.fsAO; envidx; seconds-sum(envidx)]);
    for i = 1:length(envidx)-1
        idx = round([envidx(i).*HW.params.fsAO:envidx(i+1).*HW.params.fsAO]);
        f1 = 4000 + (10000-4000)*rand;
        f2 = 100 + (38000-100)*rand;
        fbase = f1;
        Xges = log2((f1+f2)/f1);
        SR = HW.params.fsAO;
        dt = 1/SR;
        TFM = [0:1/SR:(envidx(i+1)-envidx(i))];
        X =  linspace(0,Xges,length(TFM));
        FFM = fbase*2.^X;
        phaseinc = dt.*FFM;
        phases = cumsum(phaseinc);
        FMORIG = sin(2*pi*phases);
        w = FMORIG(:)./max(abs(FMORIG));
        if rand > .5
            w = flipud(w);
        end
        %5 ms ramp:
        ramp = hanning(round(.005 * HW.params.fsAO*2));
        ramp = ramp(1:floor(length(ramp)/2));
        w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
        w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);
        Sounds = [Sounds; {w./max(abs(w))}];
    end
    %Tones
    envidx=[];
    while sum(envidx) < seconds
        r=.05 + (.5-0.05)*rand;
        if sum([envidx; r]) < seconds
            envidx = [envidx; r];
        else
            break
        end
    end
    envidx = cumsum([1/HW.params.fsAO; envidx; seconds-sum(envidx)]);
    for i = 1:length(envidx)-1
        idx = round([envidx(i).*HW.params.fsAO:envidx(i+1).*HW.params.fsAO]);
        f = 4000 + (48000-4000)*rand;
        SR = HW.params.fsAO;
        t = [0:1/SR:(envidx(i+1)-envidx(i))];
        tone = sin(2*pi*f.*t);
        w = tone./max(abs(tone));
        %5 ms ramp:
        ramp = hanning(round(.005 * HW.params.fsAO*2));
        ramp = ramp(1:floor(length(ramp)/2));
        w(1:length(ramp)) = w(1:length(ramp))' .* ramp;
        w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end)' .* flipud(ramp);
        Sounds = [Sounds; {w./max(abs(w))}];
    end
    %Clicks
    envidx=[];
    while sum(envidx) < seconds
        r=.05 + (.5-0.05)*rand;
        if sum([envidx; r]) < seconds
            envidx = [envidx; r];
        else
            break
        end
    end
    envidx = cumsum([1/HW.params.fsAO; envidx; seconds-sum(envidx)]);
    for i = 1:length(envidx)-1
        idx = round([envidx(i).*HW.params.fsAO:envidx(i+1).*HW.params.fsAO]);
        r = 4 + (48-4)*rand;
        SR = HW.params.fsAO;
        t = 0 : 1/SR : envidx(i+1)-envidx(i);
        d = 0 : 1/r : envidx(i+1)-envidx(i);
        w = pulstran(t,d,@rectpuls,2e-5);
        Sounds = [Sounds; {w./max(abs(w))}];
    end
    %Generate sounds
    PlaySounds=[];
    r = randsample(1:length(Sounds),length(Sounds),'false');
    for i = 1:length(r)
        PlaySounds = [PlaySounds; zeros(randsample([0:.25:2].*SR,1),1); Sounds{r(i)}(:)];
    end
    [b a] = butter(3,60000/(HW.params.fsAO/2));
    PlaySounds = filtfilt(b,a,PlaySounds);
    %Filter w according to speaker calibration
    PlaySounds = PlaySounds./max(abs(PlaySounds));
    IOLoadSound(HW, .9*get(HW.AOch.Range,'Max')*PlaySounds);
    fprintf('\nBegin Playing in 3...')
    pause(1)
    fprintf('2...')
    pause(1)
    fprintf('1...\n')
    pause(1)
    fprintf('Playing Now...')
    IOStartAcquisition(HW);
    IsRunning=1;
    while IsRunning
        IsRunning=HW.AO.IsRunning;
        pause(eps);
        fprintf('.')
    end
    IOStopAcquisition(HW);
    fprintf('\nDone Playing!\n')
end
function configDAQ
global globalparams HW
ShutdownHW([]);
HW=[];
HW.params.NumAOChan=1;
HW.params.DAQ = 'ni';
HW.params.fsAO=200000;
HW.params.fsAI=200000;
DAQID = globalparams.Device; % NI BOARD ID WHICH CONTROLS STIMULUS & BEHAVIOR
daq.reset;
%% Analog IO
HW.AI = daq.createSession('ni');
HW.AI.Rate=HW.params.fsAI;
HW.AO = daq.createSession('ni');
HW.AO.Rate=HW.params.fsAO;
HW.AIch(1)=addAnalogInputChannel(HW.AI, DAQID,  1, 'Voltage');
HW.AOch(1)=addAnalogOutputChannel(HW.AO, DAQID, 0, 'Voltage');
HW.AIch(1).Name = 'Microphone';
HW.AOch(1).Name = 'SoundOut';
HW.params.LineReset = [0 0];
HW.params.LineTrigger = [];
HW.AIch(1).Range = [-1 1];
HW.AIch(1).TerminalConfig = 'SingleEnded';
HW.AOch(1).Range = [-10 10];
%% Assign HW params to globalparams
globalparams.HWparams = HW.params;