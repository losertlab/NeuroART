function [BehaviorEvents, exptparams, t] = BehaviorControl(o, HW, StimEvents, globalparams, exptparams, TrialIndex)
CurrentTime = IOGetTimeStamp(HW);
t=[];
water=0;
PumpDuration = get(o,'PumpDuration');
WaterProb = get(o,'WaterProb');
RandomWater = get(o,'RandomWater');
BehaviorEvents=[];
while CurrentTime < exptparams.LogDuration
    if RandomWater && rand >= 1-WaterProb && ~water
        ev=IOControlPump (HW,'start',PumpDuration(1));
        BehaviorEvents = AddEvent(BehaviorEvents, ev, TrialIndex);
        water=1;
    end
    CurrentTime = IOGetTimeStamp(HW);
end
