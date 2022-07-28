function [BehaviorEvents, exptparams, t] = BehaviorControl(o, HW, StimEvents, globalparams, exptparams, TrialIndex)
% Behavior Object for GoNogo object
% response during the response window after a reference is a false alarm.
% response within response window after a target, a drop of water rewarded.
% A response before the response window and after onset of target is an early response.
[t,trial,Note,toff,StimIndex] = evtimes(StimEvents,'Stim*');
fs = HW.params.fsAI;
StimIndex=StimIndex(end);
[Type, StimName, TarNonTar] = ParseStimEvent (StimEvents(StimIndex(end)));
EarlyWin = get(o,'EarlyWindow');
Earlywin = [(StimEvents(StimIndex).StartTime+EarlyWin(1)) (StimEvents(StimIndex).StartTime+EarlyWin(2))];
ResponseWin = get(o,'ResponseWindow');
ConditioningTime = get(o,'ConditioningTime');
TargetWin = [StimEvents(StimIndex).StartTime StimEvents(StimIndex).StopTime];
CurrentTime = IOGetTimeStamp(HW);
LastResponse = 0;
TimeOutFlag=0;
RespFlag=0;
earlyFlag=0;
condFlag=0;
BehaviorEvents=[];
PumpDuration = get(o,'PumpDuration');
ConditioningProb = get(o,'ConditioningProb');
condrand=rand;
disptar=0;
passive = get(o,'Passive');
earlypuff = get(o,'EarlyPuff');
nontarpuff = get(o,'NonTarPuff');
t=[];
while CurrentTime < exptparams.LogDuration
    %Disp Status
    if CurrentTime >= TargetWin(1) && CurrentTime <= TargetWin(2) && disptar == 0
        disp('SOUND ON');
        disptar=1;
    elseif CurrentTime > TargetWin(2) && disptar == 1
        disp('SOUND OFF');
        disptar=0;
    end
    if ~passive
        %Deliver conditioning water
        if CurrentTime>=ConditioningTime(1) && earlyFlag==0 && RespFlag==0 && ...
                condFlag==0 && ConditioningProb>0 && strcmpi(TarNonTar,'target')
            if condrand >= 1-ConditioningProb
                ev = IOControlPump (HW,'start',PumpDuration(2));
                BehaviorEvents = AddEvent(BehaviorEvents, ev, TrialIndex);
                condFlag=1;
                ThisResponse=0;
            end
        end
        %Detect responses
        if ~condFlag
            ThisResponse = IOResponseRead (HW);
        end
        Response = ThisResponse && ~LastResponse;%only detect first lick in a trial
        LastResponse = ThisResponse;
        if (Response)
            %The session-based DAQ interface has a slower online sampling rate, due to binning of
            %aquired data. Currently, this is set to a 5 ms bin. To deal with licks that occur just
            %at the early/response window borders, after a digital lick it detected above, then the
            %analog signal is called up and analyzed for the exact lick time.
            LickTime=[];
            while isempty(LickTime) && CurrentTime < exptparams.LogDuration
                drawnow;
                [ResponseData ResponseNames] = IOReadAIData(HW);
                if ~isempty(ResponseData)
                    LickTime=(find(round(ResponseData(:,1)),1)/fs)-(1/fs); %-1/fs to account for sample 1 = time 0.
                end
                CurrentTime = IOGetTimeStamp(HW);
            end
            if isempty(LickTime)
                LickTime = nan;
            end
            if earlyFlag==0 && LickTime<=ResponseWin(2) && LickTime>=ResponseWin(1) && (RespFlag==0)
                RespFlag = 1;
            end
            if LickTime>=Earlywin(1) && LickTime<Earlywin(2) && earlyFlag==0
                earlyFlag=1;
            end
        end
        if earlyFlag==1 && ~strcmpi(TarNonTar,'NonTarget')
            %Deliver Air puff if lick during early window
            if earlypuff==1
                if ~strcmp(globalparams.Device,'OSIO')
                    [ev t] = IOControlDigGate (HW,'start',0.15);
                    BehaviorEvents = AddEvent(BehaviorEvents, ev, TrialIndex);
                elseif strcmp(globalparams.Device,'OSIO')
                    disp('Simulated air puff.')
                end
            end
            TimeOutFlag=1;
            RespFlag=2;
            earlyFlag=2;
        end
        if RespFlag==1 && strcmpi(TarNonTar,'NonTarget')
            %Deliver Air puff if lick during non target
            if nontarpuff==1
                if ~strcmp(globalparams.Device,'OSIO')
                    [ev t] = IOControlDigGate (HW,'start',0.15);
                    BehaviorEvents = AddEvent(BehaviorEvents, ev, TrialIndex);
                elseif strcmp(globalparams.Device,'OSIO')
                    disp('Simulated air puff.')
                end
            end
            TimeOutFlag=1;
            RespFlag=2;
        end
        if earlyFlag==1 && strcmpi(TarNonTar,'NonTarget')
            %Deliver Air puff if lick during non target
            if nontarpuff==1
                if ~strcmp(globalparams.Device,'OSIO')
                    [ev t] = IOControlDigGate (HW,'start',0.15);
                    BehaviorEvents = AddEvent(BehaviorEvents, ev, TrialIndex);
                elseif strcmp(globalparams.Device,'OSIO')
                    disp('Simulated air puff.')
                end
            end
            TimeOutFlag=1;
            RespFlag=2;
            earlyFlag=2;
        end
        %Deliver water if response during response window after target
        if RespFlag==1 && strcmpi(TarNonTar,'target')
            if PumpDuration(1) > 0 &&  ~strcmp(globalparams.Device,'OSIO')
                [ev t] = IOControlPump (HW,'start',PumpDuration(1));
                BehaviorEvents = AddEvent(BehaviorEvents, ev, TrialIndex);
            elseif PumpDuration(1) > 0 &&  strcmp(globalparams.Device,'OSIO')
                disp('Simulated pump on for 1 second.')
            end
            RespFlag=2;
        end
    end
    CurrentTime = IOGetTimeStamp(HW);
end
if TimeOutFlag>0 && ~strcmpi(TarNonTar,'probe')
    ThisTime = clock;
    TimeOut = ifstr2num(get(o,'TimeOut'));
    disp(['Time Out Punishment for ' num2str(TimeOut) 's']);
    while etime(clock,ThisTime) < (TimeOut+exptparams.LogDuration-CurrentTime)
        drawnow;
    end
end