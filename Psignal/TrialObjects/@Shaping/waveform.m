function [TrialSound, events , o] = waveform (o,idx)
global globalparams StopExperiment
o=ObjUpdate(o);
HWAmpScale = globalparams.HWparams.HWAmpScale;
TrialIndex = get(o,'TrialIndices');
StimHandle = get(o,'PrimaryHandle'); % get Primary object
% if ~strcmpi(get(StimHandle,'descriptor'),'Silence')
%     warning('Shaping Primary must be Silence');
%     StopExperiment=1;
%     return
% end
[TrialSound,events]=waveform(StimHandle,TrialIndex(idx,1));
%Assign behavioral meaning
tagSTR='Target';
events=updateEvs(events,tagSTR);

function ev=updateEvs(ev,tagSTR)
for i=1:length(ev)
    ev(i).Note = deblank([ev(i).Note ' ,' tagSTR]);
end
