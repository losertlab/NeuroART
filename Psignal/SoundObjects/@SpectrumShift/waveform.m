function [w, ev]=waveform (o,index)
global globalparams home
Duration=get(o,'Duration');
Names = get(o,'Names');
if length(Names) > 1
    Frequency = ifstr2num(Names{index});
else
    Frequency = ifstr2num(Names{1});
end
LF = get(o,'LowFrequency');
HF = get(o,'HighFrequency');
SamplingRate=globalparams.HWparams.fsAO;
PreStimSilence=get(o,'PreStimSilence');
PostStimSilence=get(o,'PostStimSilence');
HWAmpScale = globalparams.HWparams.HWAmpScale;
Type=deblank(get(o,'Type'));
batten=get(o,'BackgroundNoise');
switch lower(Type)
    case {'wheelbarrow'}
        %Load sound
        [w fs] = audioread([home '\waveforms\wheelbarrow\wheelbarrow.wav']);
        %Filter
        [b a]=butter(6,[500 6500]./(fs/2));
        w = filtfilt(b,a,w);
        f=linspace(0,fs,length(w));
        %Compute FFT
        W=fft(w);
        %Frequency shift spectrum peak
        fshift = get(o,'FreqShift');
        idxdiff = find(f>fshift,1,'first')-1;
        W2 = circshift(W,round(idxdiff),1);
        w2 = ifft(W2);
        %Filter
        [b a]=butter(6,([500 6500]+fshift)./(fs/2));
        w2 = filtfilt(b,a,w2);
        w2 = real(w2).*2;
        %Interpolate
        [P,Q] = rat(SamplingRate/fs);
        w = resample(w2,P,Q);
    case 'whitenoise'
        w = -1 + 2.*rand(SamplingRate.*Duration(1),1);
        w=w-mean(w);
        [b a] = butter(6,LF/(SamplingRate/2),'high');
        w = filtfilt(b,a,w);
        [b a] = butter(6,HF/(SamplingRate/2),'low');
        w = filtfilt(b,a,w);
end
clear t
if batten > -99  %add background noise
    bnoise=rand(size(w))-0.5;
    rms1=sqrt(mean(w.^2));
    rms2=sqrt(mean(bnoise.^2));
    w=w/rms1; bnoise=bnoise/rms2;  %normalized to same RMS
    bnoise=bnoise*10^(batten/20);  %noise attnuation in dB
    w=w+bnoise;
end
clear t
%Normalize to +/-1
MAX = max(abs(w));
w = w./MAX;
%Filter w according to speaker calibration
w = IOCalibrationFilter(w);
%Make w +/- HWAmpScale
MAX = max(abs(w));
norm = HWAmpScale/MAX;
w = norm * w;
%20 ms ramp:
w = w(:);
ramp = hanning(round(.04 * SamplingRate*2));
ramp = ramp(1:floor(length(ramp)/2));
w(1:length(ramp)) = w(1:length(ramp)) .* ramp;
w(end-length(ramp)+1:end) = w(end-length(ramp)+1:end) .* flipud(ramp);
% Now, put it in the silence:
w = [zeros(round(PreStimSilence*SamplingRate),1) ; w(:) ;zeros(round(PostStimSilence*SamplingRate),1)];
% and generate the event structure:
if any(strcmpi(Type,{'amtone','amtone2','amtone2a','amtone2c'}))
    ev = struct('Note',['PreStimSilence , ' num2str(Frequency)],...
        'StartTime',0,'StopTime',PreStimSilence,'Trial',[]);
    ev(2) = struct('Note',['Stim , ' num2str(Frequency)],'StartTime'...
        ,PreStimSilence, 'StopTime', PreStimSilence+Duration(1),'Trial',[]);
    ev(3) = struct('Note',['PostStimSilence , ' num2str(Frequency)],...
        'StartTime',PreStimSilence+Duration(1), 'StopTime',PreStimSilence+Duration(1)+PostStimSilence,'Trial',[]);
else
    ev = struct('Note',['PreStimSilence , ' num2str(Frequency(1))],...
        'StartTime',0,'StopTime',PreStimSilence,'Trial',[]);
    ev(2) = struct('Note',['Stim , ' num2str(Frequency(1))],'StartTime'...
        ,PreStimSilence, 'StopTime', PreStimSilence+Duration(1),'Trial',[]);
    ev(3) = struct('Note',['PostStimSilence , ' num2str(Frequency(1))],...
        'StartTime',PreStimSilence+Duration(1), 'StopTime',PreStimSilence+Duration(1)+PostStimSilence,'Trial',[]);
end
w =  w/max(abs(w));

