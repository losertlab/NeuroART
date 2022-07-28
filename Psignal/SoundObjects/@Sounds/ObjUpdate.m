function o = ObjUpdate (o)
LowFrequency = ifstr2num(get(o,'LowFrequency'));
HighFrequency = ifstr2num(get(o,'HighFrequency'));
TonesPerOctave = ifstr2num(get(o,'TonesPerOctave'));
Octaves = log2(HighFrequency/LowFrequency);
if TonesPerOctave > 0
    Freq = LowFrequency*2.^[0:1/TonesPerOctave:Octaves];
else
    Freq = [LowFrequency HighFrequency];
end
Freq=round(Freq(:));
Names=cellstr(num2str(Freq));
o = set(o,'Names',Names);
o = set(o,'MaxIndex',length(Names));