function [Data names] = OptoTrigTest(seconds, device, diffsingle)
fs = 200000;
global globalparams HW
globalparams.device = device;
configDAQ;
HW.params.fsAI=fs;
HW.AI.DurationInSeconds = seconds;
fprintf('\nBegin Recording in 3...')
pause(1)
fprintf('2...')
pause(1)
fprintf('1...\n')
pause(1)
fprintf('Recording Now...')
HW.AI.stop;
Data = HW.AI.startForeground;
HW.AI.stop;
fprintf('\nDone Recording!\n')
%% Collect data
names = [];
names=[];
for i = 1:length(HW.AI.Channels)
    name = HW.AI.Channels(i).Name;
    names{i} = name;
end
%Plot data
spec = Data(:,1)-mean(Data(:,1));
sound = Data(:,2)-mean(Data(:,2));
figure
subplot(2,1,1)
t=0:1/fs:((size(Data,1))./fs)-(1/fs);
plot(t,spec,'k','linewidth',1)
axis tight
set(gca,'fontsize',10)
ylabel('Volts')
title('Spectrometer')
subplot(2,1,2)
plot(t,sound,'k','linewidth',1)
title('Sound')
ylabel('Volts')
axis tight
set(gca,'fontsize',10)
xlabel('Time (s)')
function configDAQ
global globalparams HW diffsingle
HW=[];
HW.params.DAQ = 'ni';
HW.params.fsAI=100000;
DAQID = globalparams.device; % NI BOARD ID WHICH CONTROLS STIMULUS & BEHAVIOR
daq.reset;
%% Analog IO
HW.AI = daq.createSession('ni');
HW.AI.Rate=HW.params.fsAI;
HW.AIch(1)=addAnalogInputChannel(HW.AI, DAQID,  1, 'Voltage');
HW.AIch(2)=addAnalogInputChannel(HW.AI, DAQID,  2, 'Voltage');
HW.AIch(1).Name = 'Spectrometer';
HW.AIch(2).Name = 'Sound';
if strcmpi(diffsingle,'SingleEnded')
    HW.AIch(1).TerminalConfig = 'SingleEnded';
    HW.AIch(2).TerminalConfig = 'SingleEnded';
else
    HW.AIch(1).TerminalConfig = 'Differential';
    HW.AIch(2).TerminalConfig = 'Differential';
end
%% Assign HW params to globalparams
globalparams.HWparams = HW.params;