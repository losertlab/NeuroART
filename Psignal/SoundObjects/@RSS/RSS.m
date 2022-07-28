function o = RSS(varargin)
s = SoundObject ('RSS', 0,0, 0, {''}, 1, {'Duration','edit',0.4,'AttenRange','edit',0:10:70});
o.Duration = 0.4;
o.AttenRange = 0:10:70;
o = class(o,'RSS',s);
o = ObjUpdate (o);