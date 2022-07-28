function [StimEvents TrialSound] = TTLOutput(globalparams, exptparams, TrialSound,StimEvents)
%This function generates the triggers used to control external hardware

%Initialize variables
global HW
%Trigger rates
PhysHz = str2num(globalparams.PhysHz);
%Trial duration
TrialDur = StimEvents(end).StopTime - StimEvents(1).StartTime;
%Default pulse duration is 10ms
PulseDur = globalparams.HWparams.PhysTrigDur;
%Default delay is 0ms
Delay=0;
%Make sure sampling rates are consistent
TOfs=get(exptparams.TrialObject,'SamplingRate');
HWfs = globalparams.HWparams.fsAO;
if ~isempty(TOfs)
    if TOfs~=HWfs
        fs = TOfs;
    else
        fs = HWfs;
    end
end
%voltage scaling factor for trigger
HWAmpScale = HW.params.HWAmpScale(1);

%Length of PhysHz determines number of triggers being used. Currently, if
%only one trigger is required, it is sent on both the analog and counter
%channel. If 2 triggers are required, then only PhysHz(2) corresponds to
%the analog channe.
if length(PhysHz) > 0
    %Pulse train
    if PhysHz(1) > 0
        %Anlaog trigger
        fs = globalparams.HWparams.fsAO;
        t = 0:1/fs:TrialDur-(1/fs);
        d = PulseDur/2:1/PhysHz:TrialDur;
        triggers = pulstran(t,d,'rectpuls',PulseDur);
        triggers = triggers(1:length(TrialSound));
        TrialSound(:,2) = [zeros(fs.*Delay,1); HWAmpScale.*triggers(1:end-(fs*Delay))];
        if length(HW.AO.Channels)==3
            %Digital trigger
            HW.AO.Channels(3).InitialDelay=Delay;
            HW.AO.Channels(3).DutyCycle = PulseDur*PhysHz(1);
            HW.AO.Channels(3).Frequency = PhysHz(1);
        end
        %Gate
    elseif PhysHz(1)==0
        %Anlaog trigger
        fs = globalparams.HWparams.fsAO;
        triggers = [0 ones(1,floor(fs.*(TrialDur-(2/fs)))) 0]';
        TrialSound(:,2) = [zeros(fs.*Delay,1); HWAmpScale.*triggers(1:end-(fs*Delay))];
        if length(HW.AO.Channels)==3
            %Digital trigger
            HW.AO.Channels(3).InitialDelay=Delay;
            HW.AO.Channels(3).DutyCycle = 0.5;
            HW.AO.Channels(3).Frequency = 1/(TrialDur*2);
        end
    end
    if length(PhysHz) > 1
        %Setup for optional trigger on analog line
        NoOptTrig=1;
        OptTrig=0;
        if isfield(get(exptparams.TrialObject),'OptTrig')
            OptTrig = get(exptparams.TrialObject,'OptTrig');
        end
        if sum(OptTrig)>0
            delay = OptTrig(1);
            duration = OptTrig(2);
            pulsedur = OptTrig(3)./1000;
            pulsevolts = OptTrig(4);
            TargetNonTarget = OptTrig(5);
            prob = OptTrig(6);
            %Pulse train
            if PhysHz(2) > 0
                fs = globalparams.HWparams.fsAO;
                t = 0:1/fs:duration-(1/fs);
                d = pulsedur/2:1/PhysHz(2):duration;
                triggers = pulstran(t,d,'rectpuls',pulsedur);
                %Gate
            elseif PhysHz(2)==0
                fs = globalparams.HWparams.fsAO;
                triggers = [0 ones(1,floor(fs.*(duration-(2/fs)))) 0];
            end
            trig = [zeros(fs.*delay,1); pulsevolts.*triggers'; zeros(size(TrialSound,1)-(length(triggers)+fs.*delay),1)];
            bb=strsep(StimEvents(2).Note,',');
            StimType = bb{3};
            if globalparams.HWparams.NumAOChan > 2
                TrialSound = [TrialSound trig];
            else
                TrialSound(:,2) = trig;
            end
            NoOptTrig=0;
            if prob > (1-rand) %probablity of randomly selecting a number (0:1) NOT > prob, ie. not sending trig
                if TargetNonTarget == 0
                    if ~isempty(strfind(StimType,'NonTarget'))
                        TrialSound(:,end)=0*TrialSound(:,end);
                        NoOptTrig=1;
                    end
                elseif TargetNonTarget == 1
                    if isempty(strfind(StimType,'NonTarget'))
                        TrialSound(:,end)=0*TrialSound(:,end);
                        NoOptTrig=1;
                    end
                end
            else
                TrialSound(:,end)=0*TrialSound(:,end);
                NoOptTrig=1;
            end
        end
        if NoOptTrig == 0
            if length(PhysHz) > 1
                disp('***OVERWRITING ANALOG FRAME TRIGGER WITH OptTrig***')
            end
            disp('OptTrig Trial')
            StimEvents = updateEvs(StimEvents, 'OptTrigOn');
        end
    end
end
function ev=updateEvs(ev,tag)
for i=1:length(ev)
    ev(i).Note = deblank([ev(i).Note ' ,' tag]);
end







