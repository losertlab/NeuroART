function varargout = IOControlPump (HW,PumpAction,Duration,PumpName)
% This function runs the pump for the period specified in Duration.
% The function creates a Timer object that runs for Duration and then calls the same
% function for 'Stop' action. PumpAction can be 'Start' or 'Stop'. It also returns the
% time stamp from begining of the trial which is gotten fromfrom toc now, but might
% changed in future.
global globalparams
if ~exist('Duration','var')
    Duration = 0;
end
if ~exist('PumpName','var')
    PumpName = 'Solenoid';
end
Duration = round(Duration*1000)/1000;
names=[];
for i = 1:length(HW.DIO.Channels)
    name = HW.DIO.Channels(i).Name;
    direction = HW.DIO.Channels(i).MeasurementType;
    if strcmpi(direction,'OutputOnly')
        names = [names {name}];
    end
end
pumpidx=strcmpi(names,PumpName);
if isempty(pumpidx),
    timestamp = IOGetTimeStamp(HW);
    warning('Pump digital output channel ''',PumpName,''' not defined.');
    return;
end
t=[];
switch lower(PumpAction)
    case 'start';
        ev = struct('Note',['BEHAVIOR,PUMPON,',PumpName],'StartTime',...
            IOGetTimeStamp(HW));
        HW.DIO.outputSingleScan(pumpidx);
        % set the timer ONLY if Duration is greater than 0
        if Duration>0
            t = timer('TimerFcn',@(Handle,Event)IOControlPump(HW,'Stop',[],...
                PumpName),'StartDelay',Duration);
            start(t);
            fprintf('[ %s on for %.2fs ]\n',PumpName,Duration);
        end
        ev.StopTime = ev.StartTime+Duration;
    case {'stop',0};
        HW.DIO.outputSingleScan(zeros(1,length(pumpidx)));
        ev = struct('Note',['BEHAVIOR,PUMPOFF,',PumpName],'StartTime',...
            IOGetTimeStamp(HW),'StopTime',[]);
    otherwise error('PumpAction not implemented!');
end
if nargout > 0
    varargout{1} = ev;
    varargout{2} = t;
else
    varargout = {};
end