function [ev,HW] = IOStopAcquisition (HW);
% This function stops the data acquisition which can mean different things:
% Without physiology, this means stopping the acqusition of lick (Analog Input) and
% sending the trigger (digital output) for stoping the sound (analog output). With
% physiology, this means stoping the acquisition of lick and spike in control computer
% (Analog Input) and sending the triggers (digital output) for stoping the sound
% (analog output) and physiology computer.
global globalparams lh
ev.Note='TRIALSTOP';
if ~isempty(HW.params.LineTrigger)
    %% RESET ALL LINES
    HW.DIO.outputSingleScan([HW.params.LineTrigger]);
end
% Stop the sound and acqusition of lick signal from AI
delete(lh)
HW.AI.stop;
HW.AO.stop;
ev.StartTime=double(get(HW.AI,'ScansAcquired'))/HW.params.fsAI;
ev.StopTime=ev.StartTime;
