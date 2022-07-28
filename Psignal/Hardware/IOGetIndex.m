function Ind = IOGetIndex(Engine,Names)
global globalparams
if ~iscell(Names)
    Names = {Names};
end
if  isempty(strfind(globalparams.Device,'NI'))
    daq = get(Engine,'Line');
    daqNames=[];
    for i=1:length(daq)
        daqNames = [daqNames; {daq(i).LineName}];
    end
else
    daq = get(Engine,'Channels');
    daqNames=[];
    for i=1:length(daq)
        daqNames = [daqNames; {daq(i).Name}];
    end
end
for i = 1:length(Names)
    tmp=find((strncmpi(Names{i},daqNames,length(Names{i}))));
    if isempty(tmp),
        error(['Channel of Name ',Names{i},' is not defined.']);
    else
        Ind(i) = tmp;
    end
end
