function exptparams = CanStart (o, HW, StimEvents, globalparams, exptparams, TrialIndex)
% start immediately or after ITI.
if TrialIndex>1
    ITIs = get(o,'ITI');
    ITI = ITIs(randi(length(ITIs)));
    disp(['ITI: ' num2str(ITI)])
    pause(ITI)
end