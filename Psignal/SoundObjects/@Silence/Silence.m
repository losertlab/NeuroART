function o = Silence (o);
switch nargin
case 0
    % if no input arguments, create a default object
    s = SoundObject ('Silence', 0, 0, 0, {'Silence'}, 1, {'Duration','edit',1});
    o.Duration = 1;
    o.NonTarget = 0;
    o.BackgroundNoise=[];
    o = class(o,'Silence',s);
    o = ObjUpdate (o);
case 1
    % if single argument of class SoundObject, return it
    if isa(varargin{1},'Silence')
        o = varargin{1};
    else
        error('Wrong argument type');
    end
case 6
    %%
otherwise
    error('Wrong number of input arguments');
end