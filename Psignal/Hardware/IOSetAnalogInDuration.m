function HW=IOSetAnalogInDuration (HW, Duration);
% This function sets the Sample per Trigger field of analog input to match the Duration. 
% It uses the HW.params.fsAI to calculate the number of samples needed.
% Duration specifies the time duration of data logging and is in Seconds.
HW.AI.DurationInSeconds = Duration(1);
