function PlayFMSweep(Frange, seconds, dB, device, recordresponse, plotFM)
if nargin < 6
    plotFM=0;
end
clear global
global globalparams HW home
%% Setup pathways, rig and device id's
home=fileparts(which('Psignal'));
load([home '\PsignalConfig.mat'])
globalparams.Rig = rig;
globalparams.disploc = disploc;
globalparams.Device = device;
[HW, globalparams] = ConfigNI (globalparams);
HW.SoftwareAttendB = 80-dB;
removeChannel(HW.AI,1) %First channel is always for Lick--don't need here.
removeChannel(HW.AO,2) %Only need one analog output.
HW.AI.Rate = HW.AO.Rate;
HW.params.fsAI = HW.params.fsAO;
HW.params.NumAOChan = 1;
globalparams.SR=HW.params.fsAO;
IOStopAcquisition(HW);
HW=IOSetAnalogInDuration(HW,seconds);
%% Generate FM sweep
fs=HW.params.fsAO;
t=0:1/fs:seconds-(1/fs);
y = chirp(t,Frange(1),seconds,Frange(2));
IOLoadSound(HW, y);
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
    drawnow
    fprintf('.')
end
IOStopAcquisition(HW);
fprintf('\nDone Playing!\n')
if recordresponse
    %% Collect data
    [AllData AINames] = IOReadAIData(HW);
%     [b a]=butter(6,500/(HW.params.fsAI/2),'high');
%     AllData = filtfilt(b,a,AllData);
    figure
    BinBounds = round(linspace(0,length(AllData),200));
    t=0:1/fs:(length(AllData)/fs)-(1/fs);
    B = diff(Frange)/max(t);
    F = Frange(1)+B.*t;
    for i=1:length(BinBounds)-1
        Vcurrent = rms(AllData(BinBounds(i)+1:BinBounds(i+1)));
        VolFM(i) = VolumeConversion(Vcurrent,'V2dB','BK4944A');
        FVolFM(i) = mean(F(BinBounds(i)+1:BinBounds(i+1)));
    end
    semilogx(FVolFM,VolFM)
    axis tight
    xlabel('Frequency (Hz)')
    ylabel('dB SPL')
end
if plotFM
    figure
    spectrogram(y./max(abs(y)),.5*fs,.25*fs,length(y),fs,'onesided','psd','yaxis');
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
HW.AIch(1).Range = [-5 5];
HW.AIch(1).TerminalConfig = 'SingleEnded';
HW.AOch(1).Range = [-10 10];
%% Assign HW params to globalparams
globalparams.HWparams = HW.params;