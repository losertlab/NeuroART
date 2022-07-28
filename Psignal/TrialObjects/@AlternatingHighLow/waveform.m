function [TrialSound, events , o] = waveform (o,idx)
o=ObjUpdate(o);
TrialIndex = get(o,'TrialIndices');
StimHandle = get(o,'PrimaryHandle'); % get Primary object
[TrialSound,events]=waveform(StimHandle,TrialIndex(idx,1));
%Assign behavioral meaning
tag='Target';
fprintf('Trial Type: %s \n',tag);
events=updateEvs(events,tag);
f=strsep(events(2).Note,',');
f=f{2};
fprintf('Tone Frequency: %s Hz \n',num2str(f));
%Attenuations
AttendB=TrialIndex(idx,3);
TargetAttendB=10^(-AttendB/20);
TrialSound=TrialSound*TargetAttendB;
tag=num2str(AttendB);
events=updateEvs(events,tag);
function ev=updateEvs(ev,tag)
for i=1:length(ev)
    ev(i).Note = deblank([ev(i).Note ' ,' tag]);
end
