function o = AuditoryRecognitionTask(varargin)
switch nargin
    case 0
        o.descriptor = 'AuditoryRecognitionTask';
        o.PrimaryClass='none';
        o.PrimaryHandle = [];
        o.ProbeClass='none';
        o.ProbeHandle = [];
        o.SamplingRate = [];
        o.OveralldB=65;
        o.NumberOfTrials=[];
        o.RunClass='ART';
        o.NonTargetAttendB=0;
        o.Probe=[0.1 1];  %probe percent and stimulus number
        o.PriIndices=[];
        o.TrialIndices=[];
        o.Attenuations=[];
        o.Timer=[0 60 120];
        %%o.OptTrig =[delay duration pulsedur voltage TargetNonTargetBoth(0 1 2) probability]
        o.OptTrig=[0 0 0 0 0 0]; 
        o.UserDefinableFields = {'OveralldB','edit',65,'NonTargetAttendB','edit',0,'Probe','edit',...
            [0.1 1],'Timer','edit',[0 60 120],...
            'OptTrig','edit',[0 0 0 0 0 0]};
        o = class(o,'AuditoryRecognitionTask');
        o = ObjUpdate(o);
    case 1
        if isa(varargin{1},'AuditoryRecognitionTask')
            s = varargin{1};
        else
            error('Wrong argument type');
        end
    otherwise
        error('Wrong number of input arguments');
end
