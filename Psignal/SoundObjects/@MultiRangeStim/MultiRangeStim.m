function o = MultiRangeStim(varargin)
s = SoundObject ('MultiRangeStim', 0,0, 0, {''}, 1, {'NumFreqRange','edit',3,'LowFrequency','edit',1000, 'HighFrequency','edit',1000,...
    'TonesPerOctave','edit',6,'Duration','edit',0.4,'TarRange','edit',2,'Type','popupmenu','Tone|PitchStim|WhiteNoise|IRN',...
    'BackgroundNoise','edit',0,'AM','edit',0,'AttenRange','edit',0:10:70,'NonTarget','edit',0});
o.NumFreqRange = 3;
o.LowFrequency = 4000;
o.HighFrequency = 64000;
o.TonesPerOctave = 2;
o.Duration = 0.4;
o.TarRange=2;
o.Type='Tone';
o.BackgroundNoise=-80;  %dB attenuation
o.AM=0;
o.AttenRange = 0:10:70;
o.NonTarget = 0;
o = class(o,'MultiRangeStim',s);
o = ObjUpdate (o);