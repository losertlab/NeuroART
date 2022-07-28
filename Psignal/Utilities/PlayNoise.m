function PlayNoise(seconds, device, plotIN)
global globalparams HW home
caller=dbstack;
if strcmp(caller(1).name,'Psignal') && ~exist('home','var')
    clear classes
    clear global
end
%% Setup pathways, rig and device id's
home=fileparts(which('Psignal'));
load([home '\PsignalConfig.mat'])
globalparams.Rig = rig;
globalparams.disploc = disploc;
globalparams.Device = device;
configDAQ;
IOStopAcquisition(HW);
HW=IOSetAnalogInDuration(HW,seconds);
[b a] = butter(3,60000/(HW.params.fsAO/2));
noise = filtfilt(b,a,rand(HW.params.fsAO.*seconds,1));
t=0:1/HW.params.fsAO:seconds-(1/HW.params.fsAO);
AM = repmat([ones(1.*HW.params.fsAO,1); zeros(1*HW.params.fsAO,1)],[seconds/2 1]);
IOLoadSound(HW, 3.*noise.*AM);
fprintf('\nBegin Recording in 3...')
pause(1)
fprintf('2...')
pause(1)
fprintf('1...\n')
pause(1)
fprintf('Recording Now...')
IOStartAcquisition(HW);
IsRunning=1;
while IsRunning
    IsRunning=HW.AO.IsRunning;
    pause(eps);
    fprintf('.')
end
IOStopAcquisition(HW);
fprintf('\nDone Recording!\n')
%% Collect data
[AllData AINames] = IOReadAIData(HW);
if plotIN
    figure
    y=AllData;
    fs = HW.params.fsAI;
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
HW.AIch(1).Range = [-1 1];
HW.AIch(1).TerminalConfig = 'SingleEnded';
HW.AOch(1).Range = [-10 10];
%% Assign HW params to globalparams
globalparams.HWparams = HW.params;