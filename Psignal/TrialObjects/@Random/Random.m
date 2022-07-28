function o = Random(varargin)
switch nargin
    case 0
        o.descriptor = 'Random';
        o.PrimaryClass='none';
        o.PrimaryHandle = [];
        o.ProbeClass='none';
        o.ProbeHandle = [];
        o.SamplingRate = [];
        o.OveralldB=65;
        o.NumberOfTrials=[];
        o.TrialsPerFreq=10;
        o.RunClass='RND';
        o.PriIndices=[];
        o.TrialIndices=[];
        o.UserDefinableFields = {'OveralldB','edit',65,'TrialsPerFreq','edit',10}
        o = class(o,'Random');
        o = ObjUpdate(o);
    case 1
        if isa(varargin{1},'Random')
            s = varargin{1};
        else
            error('Wrong argument type');
        end
    otherwise
        error('Wrong number of input arguments');
end
