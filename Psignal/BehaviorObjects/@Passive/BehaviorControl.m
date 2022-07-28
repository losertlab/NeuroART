function [LickEvents, exptparams, t] = BehaviorControl (o, HW, StimEvents, globalparams, exptparams, TrialIndex);
CurrentTime = IOGetTimeStamp(HW);
t=[];
while CurrentTime < exptparams.LogDuration
    CurrentTime = IOGetTimeStamp(HW);
end
LickEvents = [];