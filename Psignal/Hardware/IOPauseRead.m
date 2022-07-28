function Response = IOPauseRead(HW)
global globalparams
fs = globalparams.HWparams.fsAI;
% This function reads the Response signal from the daq card.
Response = HW.DIO.inputSingleScan;
Response = Response(4);
