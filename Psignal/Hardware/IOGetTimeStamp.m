function timestamp = IOGetTimeStamp(HW)
% returns timestamp of event relative to start of trial (ie, when HW.AI was started)
timestamp=double(HW.AI.ScansAcquired)/HW.params.fsAI;
drawnow;

