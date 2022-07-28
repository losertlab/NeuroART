function exptparams = CanStart (o, HW, StimEvents, globalparams, exptparams, TrialIndex)
%Start after inter block interval
if exptparams.TotalTrials > 1
    if exptparams.InRepTrials == 1
        disp(['Inter-block Interval: ' num2str(get(o,'IBI')) ' seconds']);
        pause(get(o,'IBI'))
    end
end
% start after random ITI:
if TrialIndex > 1
    ITIs=get(o,'ITIs');
    if isstr(ITIs)
        ITIs=str2num(ITIs);
    end
    if length(ITIs) > 1
        ITI = randsample(ITIs,1);
        fprintf('[ %s %.2fs ]\n','ITI:' ,ITI);
    else
        ITI = ITIs;
    end
    pause(ITI)
end
