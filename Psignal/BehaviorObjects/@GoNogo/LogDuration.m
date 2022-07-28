function Duration = LogDuration (o, HW, StimEvents, globalparams, exptparams, TrialIndex);
% the log duration is the end of the sound. This assumes that the response window is always containted within the
% post-stimulus silence.
Duration = StimEvents(end).StopTime;