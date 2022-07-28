function AllData = MicRecord(seconds, device, plotIN, input, fs, AttendB, outdevice, diffsingle)
if nargin < 4
    input =[];
    fs=[];
    AttendB = 0;
    outdevice=[];
elseif nargin < 5 && ~isempty(input)
    fs = 200000;
    AttendB = 0;
    outdevice=[];
end
if length(input(:)) > seconds*fs
    input = input(1:seconds*fs);
end
global globalparams HW home diffsingle
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
if ~isempty(outdevice)
    globalparams.outdevice = outdevice;
end
configDAQ;
if ~isempty(input)
    HW.params.fsAO=fs;
    HW.params.fsAI=fs;
    HW.AO.Rate =   HW.params.fsAO;
end
IOStopAcquisition(HW);
HW=IOSetAnalogInDuration(HW,seconds);
if isempty(input)
    IOLoadSound(HW, zeros(HW.params.fsAO.*seconds,1));
else
    input=input./max(abs(input));
    AttendB=10^(-AttendB/20);
    input=input*AttendB;
    IOLoadSound(HW, input);
end
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
global globalparams HW diffsingle
ShutdownHW([]);
HW=[];
HW.params.NumAOChan=1;
HW.params.DAQ = 'ni';
HW.params.fsAO=100000;
HW.params.fsAI=100000;
DAQID = globalparams.Device; % NI BOARD ID WHICH CONTROLS STIMULUS & BEHAVIOR
daq.reset;
%% Analog IO
HW.AI = daq.createSession('ni');
HW.AI.Rate=HW.params.fsAI;
HW.AO = daq.createSession('ni');
HW.AO.Rate=HW.params.fsAO;
HW.AIch(1)=addAnalogInputChannel(HW.AI, DAQID,  1, 'Voltage');
if ~isfield(globalparams, 'outdevice')
    HW.AOch(1)=addAnalogOutputChannel(HW.AO, DAQID, 0, 'Voltage');
else
    HW.AOch(1)=addAnalogOutputChannel(HW.AO, globalparams.outdevice, 0, 'Voltage');
end
HW.AIch(1).Name = 'Microphone';
HW.AOch(1).Name = 'SoundOut';
HW.params.LineReset = [0 0];
HW.params.LineTrigger = [];
HW.AIch(1).Range = [-1 1];
if strcmpi(diffsingle,'SingleEnded')
    HW.AIch(1).TerminalConfig = 'SingleEnded';
else
    HW.AIch(1).TerminalConfig = 'Differential';
end
HW.AOch(1).Range = [-10 10];
%% Assign HW params to globalparams
globalparams.HWparams = HW.params;