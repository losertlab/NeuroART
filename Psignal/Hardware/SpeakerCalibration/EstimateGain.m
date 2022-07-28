function EstimateGain
%% Ajdust amplifier gain on the playback of a noise. The noise is filtered by the inverse speaker IR, and played at +/- VRef. The user adjusts
%% the amplifier gain so that the recorded rms level of the noise is ~dBSPLRef. Thus, any sound will be played at a given dB SPL by:
%% (1) scaling the sound to +/- 1, (2) filtering the sound by the inverse IR, (3) scaling the sound to +/- VRef, (4) scaling down the sound by the given amound of decibels.
%% The first two steps should guaruntee that a tone or complex sound are at the max dB SPL re. VRef.
global  globalparams HW Noise
R = globalparams.R;
figure(get(globalparams.Fig,'Number'));
g=subplot(2,1,2);
Stopbutton = uicontrol('style','Togglebutton','String','Stop','Value',0,...
    'Units','normalized','Pos',[.01,.01,.1,.06]);
xlabel('AI Sample')
ylabel('dB SPL')
aa=axis;
text([aa(1) aa(1)]/2,[aa(4) aa(4)]/2,'Buffering Response','fontsize',24)
CalbNoise = Noise;
[b a] = butter(6,globalparams.Fband./(globalparams.SR/2));
CalbNoise = filtfilt(b,a,CalbNoise);
norm = globalparams.VRef/max(abs(CalbNoise));
CalbNoise = norm * CalbNoise;
%Whiten the noise for gain calibration
CalbNoiseSpec=fft(CalbNoise);
CalbNoiseSpecPhase=angle(CalbNoiseSpec);
CalbNoiseSpecdB=VolumeConversion(abs(CalbNoiseSpec),'V2dB',globalparams.microphone);
WhiteningSpec=globalparams.R.WhiteningSpec';
WhiteningSpecdB=VolumeConversion(WhiteningSpec,'V2dB',globalparams.microphone);
CalbNoiseSpecdBWhite=CalbNoiseSpecdB + WhiteningSpecdB;
CalbNoiseSpecWhite=VolumeConversion(CalbNoiseSpecdBWhite,'dB2V',globalparams.microphone);
CalbNoiseSpecWhite=CalbNoiseSpecWhite.*exp(j.*CalbNoiseSpecPhase);
CalbNoiseWhite=real(ifft(CalbNoiseSpecWhite));ramp = hanning(round(.2 * HW.params.fsAO*2));
ramp=ramp(1:floor(length(ramp)/2));
CalbNoiseWhite(1:length(ramp))=CalbNoiseWhite(1:length(ramp)) .* ramp;
CalbNoiseWhite(end-length(ramp)+1:end)=CalbNoiseWhite(end-length(ramp)+1:end) .* flipud(ramp);
%Play noise and adjust amplifier gain
HW=IOSetAnalogInDuration(HW,length(CalbNoiseWhite)/globalparams.SR);
IOStopAcquisition(HW);
IOLoadSound(HW,CalbNoiseWhite);
IOStartAcquisition(HW);
Volumes = zeros(10000,1);
i=0;
AIdata=[];
IsRunning=1;
while ~get(Stopbutton,'Value') && IsRunning
    IsRunning = HW.AO.IsRunning;
    i=i+1;
    drawnow
    %Collect data
    [Data names] = IOReadAIData(HW);
    AIdata = Data;
    if length(AIdata) > globalparams.SR*2
        cla(gca);
        hold on;
        Vcurrent = rms(AIdata(end-(globalparams.SR*2):end));
        Volumes(i) = VolumeConversion(Vcurrent,'V2dB',globalparams.microphone);
        Ind = [max(1,i-10):i];
        plot([Ind(1)-.5,Ind(end)+.5],[globalparams.dBSPLRef,globalparams.dBSPLRef],'r');
        plot(Ind,Volumes(Ind),'.-b'); grid on;
        axis([Ind(1)-0.5,Ind(end)+0.5,...
            min([globalparams.dBSPLRef-1,floor(min(Volumes(Ind)))]),...
            max([globalparams.dBSPLRef+1,ceil(max(Volumes(Ind)))])]);
        xlabel('AI Sample')
        ylabel('dB SPL')
    end
end
if ~get(Stopbutton,'Value')
    disp('WARNING: Calibration response not acquired...start over')
    delete(Stopbutton)
    delete(g)
    return
end
delete(Stopbutton)
delete(g)
IOStopAcquisition(HW);
R.cdBSPL = VolumeConversion(Vcurrent,'V2dB',globalparams.microphone);
fprintf(['\n=> Level: ',num2str(R.cdBSPL),'\n']);
R.Fs=globalparams.SR;
globalparams.R = R;