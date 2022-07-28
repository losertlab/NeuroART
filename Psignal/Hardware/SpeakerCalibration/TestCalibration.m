function TestCalibration
%% This program plays a frequency modulated sweep that has been flattened for the speaker, in order to test the calibration filter. The recorded FM
%% sweep should be at dBSPLRef for the duration of the sweep (i.e. at all calibrated frequencies).
global globalparams HW
R = globalparams.R;
%Make tones
numtones=16;
globalparams.NSteps = round(globalparams.LStim*globalparams.SR);
f=linspace(globalparams.Fband(1),globalparams.Fband(2),numtones);
tonedur=0.2;
t=0:1/globalparams.SR:(tonedur)-(1/globalparams.SR);
fprintf(['\n ====== Synthesizing Tones ====== \n']);
Tones =[];
for i = 1:length(f)
    tone = sin(2*pi*f(i).*t)';
    %Normalize tone to +/-VRef
    MAX = max(abs(tone));
    tone = globalparams.VRef*(tone./MAX);
    ramp = hanning(round(.02 * HW.params.fsAO*2));
    ramp = ramp(1:floor(length(ramp)/2));
    tone(1:length(ramp)) = tone(1:length(ramp)) .* ramp;
    tone(end-length(ramp)+1:end) = tone(end-length(ramp)+1:end).*flipud(ramp);
    %Whiten the tone
    spec = globalparams.R.WhiteningSpec';
    mic = globalparams.microphone;
    toneWhite = IOCalibrationFilter(tone, spec, mic);
    toneWhite(1:length(ramp)) = toneWhite(1:length(ramp)) .* ramp;
    toneWhite(end-length(ramp)+1:end) = toneWhite(end-length(ramp)+1:end) .* flipud(ramp);
    toneWhite = [zeros(length(tone),1); toneWhite; zeros(length(tone),1)];
    Tones=[Tones; toneWhite];
end
%Play tones
HW=IOSetAnalogInDuration(HW,round(length(Tones)/globalparams.SR));
IOStopAcquisition(HW);
IOLoadSound(HW, Tones);
fprintf(['\n ====== Playing Tones ====== \n']);
IOStartAcquisition(HW);
wait(HW.AI,round(length(Tones)/globalparams.SR)+1)
IOStopAcquisition(HW);
fprintf(['\n ====== Done Playing Tones ====== \n']);
%Collect data
[AIData AINames] = IOReadAIData(HW);
figure(globalparams.Fig)
subplot(2,2,3)
AIdB=VolumeConversion(rms(AIData),'V2dB',globalparams.microphone);
t=0:1/globalparams.SR:(length(AIData)/R.Fs)-(1/globalparams.SR);
plot(t,AIData,'k')
xlabel('Time (s)')
ylabel('Volts')
title([{'Calibrated Test Tone Waveform'};{['RMS Level: ' num2str(roundTo(AIdB,1)) ' dB SPL']}],'fontsize',10)
aa=axis;
ylim([-max(aa(3:end)) max(aa(3:end))].*1.5)
grid on
f=linspace(0,R.Fs,length(AIData));
subplot(2,2,4)
AIDataSpec=(2/length(AIData))*(abs(fft(AIData)));
AIDataSpecdB=VolumeConversion(AIDataSpec,'V2dB',globalparams.microphone);
plot(f/1000,AIDataSpecdB,'k')
xlim(globalparams.Fband/1000)
xlabel('Frequency (kHz)')
ylabel('dB SPL')
title('Calibrated Test Tone Spectrum','fontsize',10)
aa=axis;
ylim([0 aa(4)])
grid on
globalparams.R = R;