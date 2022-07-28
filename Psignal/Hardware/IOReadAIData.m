function [Data names] = IOReadAIData(HW)
names = [];
Data = [];
if strcmpi(HW.params.DAQ,'ni')
    global AnalogInputData
    Data = AnalogInputData;
    names=[];
    for i = 1:length(HW.AI.Channels)
        name = HW.AI.Channels(i).Name;
        names{i} = name;
    end
elseif strcmpi(HW.params.DAQ,'audio')
    global AnalogInputData
    Data = round(AnalogInputData);
    names{1}='Lick';
end