function o = ObjUpdate (o)
global home
stimlist = what([home '\Waveforms\RSS']);
stimlist = stimlist.mat;
stimlisttemp=[];
for i = 1:length(stimlist)
    if ~isempty(strfind(stimlist{i},'RSS'))
        stimlisttemp = [stimlisttemp; stimlist(i)];
    end
end
stimlist = stimlisttemp;
Names=stimlist;
o = set(o,'Names',Names);
o = set(o,'MaxIndex',length(Names));