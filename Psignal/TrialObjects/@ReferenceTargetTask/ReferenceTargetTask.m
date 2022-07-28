function o = ReferenceTargetTask(varargin)
switch nargin
    case 0
        o.descriptor = 'ReferenceTargetTask';
        o.PrimaryClass='none';
        o.PrimaryHandle = [];
        o.ProbeClass='none';
        o.ProbeHandle = [];
        o.SamplingRate = [];
        o.OveralldB=65;
        o.NumberOfTrials=[];
        o.RunClass='RTT';
        o.NonTargetAttendB=0;
        o.Probe=[0.1 1];  %probe percent and stimulus number
        o.PriIndices=[];
        o.ProIndices=[];
        o.TrialIndices=[];
        o.Attenuations=[];
        o.NumOfEvPerStim = 3;
        o.UserDefinableFields = {'OveralldB','edit',65,'NonTargetAttendB','edit',0,'Probe','edit',[0.1 1]}
        o = class(o,'ReferenceTargetTask');
        o = ObjUpdate(o);
    case 1
        if isa(varargin{1},'ReferenceTargetTask')
            s = varargin{1};
        else
            error('Wrong argument type');
        end
    otherwise
        error('Wrong number of input arguments');
end
