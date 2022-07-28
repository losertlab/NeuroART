function exptparams = CanStart (o, HW, StimEvents, globalparams, exptparams, TrialIndex)
% We wait until the animal does not lick for at least NoResponseTime
global StopExperiment;
if TrialIndex>1
    ITIs = get(exptparams.BehaveObject,'ITIs');
    ITI = ITIs(randi(length(ITIs)));
    disp(['ITI: ' num2str(ITI)])
    pause(ITI)
end
if get(o,'Passive') ||  strcmp(globalparams.Device,'OSIO')
    % in the passive mode, we do not wait for the lick/stoplick to start the trial:
    return;
end
InitWater = get(exptparams.BehaveObject,'InitialWater');
if TrialIndex==1 && InitWater
    ev = IOControlPump (HW,'start');
    response=0;
    disp('Waiting for initial response...');
    while ~response
        response = IOResponseRead (HW);
        if response
            ev = IOControlPump (HW,'stop');
        end
    end
else
    drawnow;
end
disp('Waiting for no response time...');
LastTime = clock;
PuffTime=tic;
earlypuff = get(o,'EarlyPuff');
while etime(clock,LastTime)<get(o,'NoResponseTime') && ~StopExperiment
    ThisResponse = IOResponseRead (HW);
    if ThisResponse
        LastTime = clock;
        if toc(PuffTime) > get(o,'NoResponseTime')*3 && earlypuff
            [ev t] = IOControlDigGate (HW,'start',0.15);
            PuffTime=tic;
        end
    end
end

