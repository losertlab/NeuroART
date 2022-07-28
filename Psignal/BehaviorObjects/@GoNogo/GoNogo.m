function o = GoNogo (varargin)
% Go/Nogo task behavior control
% NoResponseTime: the time period for no-lick before a trial start, which is necessary
% for a trial initiation. EarlyWindow: a time period starting from the onset of target
% sequence during which the animal should not lick. ResponseWindow: a time period
% following Early window ,during which the anima should lick. TimeOut: silent period,
% use as a penalty for incorrect response to target sequence. PumpDuration: the
% duration of water presentation
switch nargin
    case 0
        o.InitialWater = 1;
        o.NoResponseTime    = 2;
        o.EarlyWindow       = 0; %Start and stop times re. stimulus onset
        o.ResponseWindow    = [2 4]; %Start and stop times (s)
        o.TimeOut           = 0;
        o.ConditioningProb           = 0.1;
        o.PumpDuration      = [1 0];  %reward (1) and conditioning drop (2)
        o.QuitNoResponse = 1;
        o.Passive = 0;
        o.ITIs = 0;
        o.EarlyPuff = 0;
        o.NonTarPuff = 0;
        o.ITIs = 0;
        o.ConditioningTime= 1;
        o.UserDefinableFields = {'NoResponseTime','edit',0.2, 'EarlyWindow', 'edit',0.2, ...
            'ResponseWindow','edit',1,'TimeOut','edit',4, 'ConditioningProb','edit', 0.25, ...
            'ConditioningTime','edit',1,'PumpDuration','edit',[1 0], 'ITIs','edit',[0:1:4], 'QuitNoResponse','checkbox',0,...
            'InitialWater','checkbox',1,'Passive','checkbox', 0,'EarlyPuff','checkbox', 0,'NonTarPuff','checkbox', 0};
        o = class(o,'GoNogo');
        o = ObjUpdate(o);
    case 1
        if isa(varargin{1},'GoNogo')
            o = varargin{1};
        else
            error('Wrong argument type');
        end
    otherwise
        error('Wrong number of input arguments');
end
