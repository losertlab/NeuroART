function s = SoundObject(varargin)
% SoundObject Creates a sound object that is the base class of any sound
% used in baphy.
switch nargin
case 0
    s.descriptor = 'none';
    s.Loudness = 0;
    s.PreStimSilence = 0;
    s.PostStimSilence = 0;
    s.Names = {'none'};
    s.MaxIndex = 1;
    s.UserDefinableFields = {'PreStimSilence','edit',0,'PostStimSilence','edit',0};
    s = class(s,'SoundObject');
    s = ObjUpdate (s);
case 1
    % if single argument of class SoundObject, return it
    if isa(varargin{1},'SoundObject')
        s = varargin{1};
    else
        error('Wrong argument type');
    end
case 7
    s.descriptor = varargin{1};
    s.Loudness = varargin{2};
    s.PreStimSilence = varargin{3};
    s.PostStimSilence = varargin{4};
    s.Names = varargin{5};
    s.MaxIndex = varargin{6};
    s.UserDefinableFields = {'PreStimSilence','edit',0,'PostStimSilence','edit',0, varargin{7}{:}};
    s = class(s,'SoundObject');
    s = ObjUpdate (s);
otherwise 
    error('Wrong number of input arguments');
end
