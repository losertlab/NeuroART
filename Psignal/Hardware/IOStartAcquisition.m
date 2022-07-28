function [ev,HW] = IOStartAcquisition(HW)
% This function starts the data acquisition which can mean different things:
% (1)Without physiology, this means starting the acqusition of behavioral analog
% inputs and digital triggering of analog outputs 2)With physiology, this means
% starting the acquisition of behavioral analog inputs, and from external devices, and
% digital triggering of analog and digital outs.
ev.Note='TRIALSTART';
ev.StartTime=0;
ev.StopTime=0;
ev.Timestamp=datetime;

HW.params.StartClock=clock;
%% WAIT UNTIL INPUT AND OUTPUT BECOME AVAILABLE (if daq is still running)
global globalparams
if HW.AO.IsRunning || HW.AI.IsRunning
    warning('IOStartAcquisition: Device is not ready');
end
while HW.AO.IsRunning || HW.AI.IsRunning
    pause(0.01);
end
global AnalogInputData globalparams lh
AnalogInputData=[];
lh = addlistener(HW.AI,'DataAvailable',@(src, event) IOLog(src, event));
%OSC output sent, whether or not used.
u = udp('localhost', 9000);
fopen(u);
for i = 1:10
    oscsend(u, 'test', 'i', 1234);
end
fclose(u);
HW.AO.stop;
HW.AI.stop;
HW.AO.startBackground;
HW.AI.startBackground;
if ~isempty(HW.params.LineTrigger)
    %% RESET ALL LINES
    HW.DIO.outputSingleScan([HW.params.LineReset]);
    %% TRIGGER ALL LINES
    HW.DIO.outputSingleScan([HW.params.LineTrigger]);
end
