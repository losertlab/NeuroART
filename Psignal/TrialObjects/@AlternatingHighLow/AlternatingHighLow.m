function o = AlternatingHighLow(varargin)
switch nargin
    case 0
        o.descriptor = 'AlternatingHighLow';
        o.PrimaryClass='none';
        o.PrimaryHandle = [];
        o.ProbeClass='none';
        o.ProbeHandle = [];
        o.SamplingRate = [];
        o.OveralldB=65;
        o.NumberOfTrials=[];
        o.TrialsPerFreq=10;
        o.RunClass='AHL';
        o.PriIndices=[];
        o.TrialIndices=[];
        o.UserDefinableFields = {'OveralldB','edit',65,'TrialsPerFreq','edit',10}
        o = class(o,'AlternatingHighLow');
        o = ObjUpdate(o);
    case 1
        if isa(varargin{1},'AlternatingHighLow')
            s = varargin{1};
        else
            error('Wrong argument type');
        end
    otherwise
        error('Wrong number of input arguments');
end
