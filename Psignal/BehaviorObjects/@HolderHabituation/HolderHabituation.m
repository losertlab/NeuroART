function o = HolderHabituation (varargin)
switch nargin
    case 0
        % if no input arguments, create a default object
        o.RandomWater    = 0;
        o.PumpDuration = 1;
        o.WaterProb = 1;
        o.IBI = 120;
        o.ITIs = [0:2:16];
        o.descriptor = 'HolderHabituation';
        o.UserDefinableFields = {'RandomWater','checkbox',0,'PumpDuration','edit',1,'WaterProb','edit',1,'IBI','edit',120,'ITIs','edit',[0:2:16]};
        o = class(o,'HolderHabituation');
        o = ObjUpdate(o);
    case 1
        % if single argument of class SoundObject, return it
        if isa(varargin{1},'HolderHabituation')
            o = varargin{1};
        else
            error('Wrong argument type');
        end
    otherwise
        error('Wrong number of input arguments');
end
