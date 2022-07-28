function o = ObjUpdate (o)
global home
% This function loads the file name and index numbers for specified torc
% rate and Higherst Frequency:
Rates = get(o,'Rates');
Rates= strtrim(Rates);
switch Rates
    case '24',
        RateName = '424';
end
FileNames = ['TORC_' RateName '_*.wav'];
% load the ripple names into the name field:
object_specTEMP = what('TORC');
for i = 1:length(object_specTEMP)
    if ~isempty(strfind(object_specTEMP(i).path,'Waveforms'))
        object_spec=object_specTEMP(i);
    end
end
soundpath = [home filesep 'Waveforms\TORC\'];
RippleFiles = dir([soundpath filesep FileNames]);
[temp, fs] = audioread([soundpath filesep RippleFiles(1).name]);
for cnt1 = 1:length(RippleFiles)
    files{cnt1} = RippleFiles(cnt1).name(1:end-4);
    % Now load the parameters from the file and update the properties:
    % For tarcs, it has a standard form:
    tempPar = caseread([soundpath filesep files{cnt1} '.txt']);
    % read the sampling frequency:
    Params(cnt1).SamplingFrequency = getvalue(tempPar(1,:));
    Params(cnt1).RipplePeak = getvalue(tempPar(2,:));
    Params(cnt1).LowestFrequency = getvalue(tempPar(3,:));
    Params(cnt1).HighestFrequency = getvalue(tempPar(4,:));
    Params(cnt1).NumberOfComponents = getvalue(tempPar(5,:));
    Params(cnt1).HarmonicallySpaced = getvalue(tempPar(6,:));
    Params(cnt1).HarmonicSpacing = getvalue(tempPar(7,:));
    Params(cnt1).SpectralPowerDecay = getvalue(tempPar(8,:));
    Params(cnt1).ComponentRandomPhase = getvalue(tempPar(9,:));;
    Params(cnt1).TimeDuration = getvalue(tempPar(10,:));
    Params(cnt1).RippleAmplitude = getvalue(tempPar(11,:));
    Params(cnt1).Scales = getvalue(tempPar(12,:));
    Params(cnt1).Phase =  getvalue(tempPar(13,:));
    Params(cnt1).Rates = getvalue(tempPar(14,:));
end
o = set(o,'Params',Params);
o = set(o,'Names',files);
o = set(o,'MaxIndex', length(files));
function v = getvalue (text);
% this function returns the numeric value after '=' in text:
tempStart = findstr(text,'=');
IsParan = findstr(text,'(');
tempEnd = findstr(text(1,tempStart:end),' ');
if isempty(IsParan)
    tempEnd = tempEnd(2);
else
    tempStart = IsParan;
    tempEnd = findstr(text(tempStart:end),')')-1;
end
v = ifstr2num(text(1+tempStart:tempStart+tempEnd-1));
