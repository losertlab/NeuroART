function o = Shaping(varargin)
global globalparams
switch nargin
    case 0
        o.descriptor = 'Shaping';
        o.PrimaryClass='none';
        o.PrimaryHandle = [];
        o.ProbeClass='none';
        o.ProbeHandle = [];
        o.SamplingRate = 1000;
        o.OveralldB=65;
        o.NumberOfTrials=10;
        o.RunClass='SHP';
        o.PriIndices=[];
        o.TrialIndices=[];
        o.UserDefinableFields = {'OveralldB','edit',65,'NumberOfTrials','edit',0,'SamplingRate','edit',1000}
        o = class(o,'Shaping');
        o = ObjUpdate(o);
    case 1
        if isa(varargin{1},'Shaping')
            s = varargin{1};
        else
            error('Wrong argument type');
        end
    otherwise
        error('Wrong number of input arguments');
end
