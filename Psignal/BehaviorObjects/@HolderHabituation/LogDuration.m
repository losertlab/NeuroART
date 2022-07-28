function Duration = LogDuration (o, HW, StimEvents, globalparams, exptparams, TrialIndex);
% the log duration is the end of the sound plus response time
Duration = StimEvents(end).StopTime;