function o = SpectrumShift(varargin)
s = SoundObject ('SpectrumShift', 0,0, 0, {''}, 1, {'FreqShift','edit',6,'NumShifts','edit',6,'Duration','edit',0.4,'TarShift','edit',2,'Type','popupmenu','Wheelbarrow|WhiteNoise',...
    'BackgroundNoise','edit',0,'AttenRange','edit',0:10:70});
o.LowFrequency = 1000;
o.HighFrequency = 80000;
o.NumShifts = 1;
o.Duration = 0.4;
o.FreqShift=1000;
o.TarShift=1;
o.Type='Wheelbarrow';
o.BackgroundNoise=-80;  %dB attenuation
o.AttenRange = 0;
o = class(o,'SpectrumShift',s);
o = ObjUpdate (o);