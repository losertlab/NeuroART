function PsignalParams = LoadPsignalParams(dir)
warning('off','MATLAB:unknownElementsNowStruc')
warning('off','MATLAB:elementsNowStruc')

if nargin < 1
    username=getenv('USERNAME');
    dir = ['C:\Users\' username '\Google Drive\PsignalData\'];
end
username=getenv('USERNAME');
addpath(genpath(['C:\Users\' username '\Dropbox\Psignal']));
if isempty(strfind(dir,'.mat'))
    %Ask for data
    [FileName FilePath] = uigetfile([dir '*.mat'],'Load Psignal Data');
    %Display params
    load([FilePath '\' FileName]);
else
    load(dir);
end
TrialObject = exptparams.TrialObject;
if isobject(TrialObject)
    Primary=get(TrialObject,'PrimaryHandle');
    if isobject(Primary)
        Primary = get(Primary);
    end
    if isfield(Primary,'SoundObject')
        if isobject(Primary.SoundObject)
            SoundObject = get(Primary.SoundObject);
            for fn = fieldnames(SoundObject)'
                Primary.(fn{1}) = SoundObject.(fn{1});
            end
        else
            for fn = fieldnames(Primary.SoundObject)'
                Primary.(fn{1}) = Primary.SoundObject.(fn{1});
            end
        end
        Primary = rmfield(Primary,'SoundObject');
        %         Probe=TrialObject.ProbeHandle;
        %         if isobject(Probe)
        %             Probe = get(Probe);
        %         end
    end
    
    TrialObject = get(TrialObject);
else
    Primary=TrialObject.PrimaryHandle;
    if isobject(Primary)
        Primary = get(Primary);
    end
    if isfield(Primary,'SoundObject')
        if isobject(Primary.SoundObject)
            SoundObject = get(Primary.SoundObject);
            for fn = fieldnames(SoundObject)'
                Primary.(fn{1}) = SoundObject.(fn{1});
            end
        else
            for fn = fieldnames(Primary.SoundObject)'
                Primary.(fn{1}) = Primary.SoundObject.(fn{1});
            end
        end
        Primary = rmfield(Primary,'SoundObject');
        %         Probe=TrialObject.ProbeHandle;
        %         if isobject(Probe)
        %             Probe = get(Probe);
        %         end
    end
end
Behavior = exptparams.BehaveObject;
if isobject(Behavior)
    Behavior = get(Behavior);
end
PsignalParams.TrialObject = TrialObject;
PsignalParams.Behavior = Behavior;
PsignalParams.Primary = Primary;
PsignalParams.Probe = [];
PsignalParams.exptevents = exptevents;
PsignalParams.globalparams = globalparams;
PsignalParams.exptparams = exptparams;


