function [TrialSound, events , o] = waveform (o,idx)
global globalparams
o=ObjUpdate(o);
ProIndex = get(o,'ProIndices');
PriIndex = get(o,'PriIndices');
TrialSound=[];
events=[];
SamplingRate=globalparams.HWparams.fsAO;
%Reference
StimHandle = get(o,'ProbeHandle');
RefIdx = ProIndex{idx};
LastStop = 0;
for i = 1:length(RefIdx)
    [ref,event]=waveform(StimHandle,RefIdx(i));
    tag='NonTarget';
    for ii = 1:length(event)
        event(ii).StartTime = event(ii).StartTime + LastStop;
        event(ii).StopTime = event(ii).StopTime + LastStop;
    end
    events=[events updateEvs(event,tag)];
    LastStop = event(end).StopTime;
    TrialSound = [TrialSound; ref];
end
NonTargetAttendB=get(o,'NonTargetAttenDB');
NonTargetAttendB=10^(-NonTargetAttendB/20);
TrialSound=TrialSound*NonTargetAttendB;
%Target
StimHandle = get(o,'PrimaryHandle'); % get Primary object
[tar,event]=waveform(StimHandle,PriIndex(idx,1));
for ii = 1:length(event)
    event(ii).StartTime = event(ii).StartTime + LastStop;
    event(ii).StopTime = event(ii).StopTime + LastStop;
end
f=strsep(event(2).Note,',');
f=f{2};
tag='Target';
events=[events updateEvs(event,tag)];
AttendB=PriIndex(idx,3);
TargetAttendB=10^(-AttendB/20);
tar=tar*TargetAttendB;
TrialSound = [TrialSound; tar];
if ~strcmpi(get(StimHandle,'descriptor'),'Silence')
    fprintf('Target Frequency: %s Hz \n',num2str(f));
end
fprintf('Number References: %s \n',num2str(length(RefIdx)));
function ev=updateEvs(ev,tag)
for i=1:length(ev)
    ev(i).Note = deblank([ev(i).Note ' ,' tag]);
end
