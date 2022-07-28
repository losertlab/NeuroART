function o = Sounds(varargin)
s = SoundObject ('Sounds', 0,0, 0, {''}, 1, {'NumFreqRange','edit',3,'LowFrequency','edit',1000, 'HighFrequency','edit',1000,...
    'TonesPerOctave','edit',6,'Duration','edit',0.4,'TarRange','edit',2,'Type','popupmenu','Tone|PitchStim|WhiteNoise|IRN|ClickTrain|AMNoise|HarmStack|MissFund',...
    'BackgroundNoise','edit',0,'AM','edit',0,'AttenRange','edit',0:10:70,'NonTarget','edit',0});
o.NumFreqRange = 3;
o.LowFrequency = 4000;
o.HighFrequency = 64000;
o.TonesPerOctave = 2;
o.Duration = 0.4;
o.TarRange=2;
o.Type='Tone';
o.BackgroundNoise=[0 0 1];
o.AM=0;
o.AttenRange = 0:10:70;
o.NonTarget = 0;
o = class(o,'Sounds',s);
o = ObjUpdate (o);