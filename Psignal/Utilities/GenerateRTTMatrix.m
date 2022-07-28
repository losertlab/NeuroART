
addpath(genpath('C:\Users\nfu\Dropbox\Psignal\Psignal'))


PsignalFile = 'sheila\RTT_sheila_2016_04_13_Phys_2';
PsignalPath = 'C:\Users\nfu\Google Drive\PsignalData\';
savepath='V:\Data\2Photon\4_14_16_TORCs\sheila_TORC_002\PsignalMatrix';


% PsignalFile = 'ji980\RTT_ji980_2016_04_13_Phys_3';
% PsignalPath = 'C:\Users\nfu\Google Drive\PsignalData\';
% savepath='V:\Data\2Photon\4_14_16_TORCs\ji980_TORC_001\PsignalMatrix';


load([PsignalPath PsignalFile]);
fps = 30; %Assumed frames per second
TotalTrials = exptevents(end).Trial;
Probe =get(get(exptparams.TrialObject,'ProbeHandle'));
Primary =get(get(exptparams.TrialObject,'PrimaryHandle'));
TrialObject =get(exptparams.TrialObject);
BehaviorObject = get(exptparams.BehaveObject);
TorcOnset = Probe.PreStimSilence*fps;
TorcOffset = (Probe.PreStimSilence+Probe.Duration)*fps;
TorcTrialDur = (Probe.PreStimSilence+Probe.Duration+Probe.PostStimSilence)*fps;
SilentOnset = Primary.PreStimSilence*fps;
SilentOffset = (Primary.PreStimSilence+Primary.Duration)*fps;
SilentTrialDur = (Primary.PreStimSilence+Primary.Duration+Primary.PostStimSilence)*fps;
OveralldB = TrialObject.OveralldB;
%Task-related features
Data=[];
Data.TagNames{1,1} = 'TorcOnset';
Data.TagNames{2,1} = 'TorcOffset';
Data.TagNames{3,1} = 'TorcIndex';
Data.TagNames{4,1} = 'TorcLevel';
% Data.TagNames{5,1} = 'ITI';
%Preallocation of P
[t,trial,Note,toff,StimIndex] = evtimes(exptevents,'Stim*');
%Stimulus
IDX=1;
Data.Tags=[];
for i = 1:length(StimIndex)
    bb = strsep(exptevents(StimIndex(i)).Note,',');
    if isempty(strfind(Note{i},'Silence'))
        Data.Tags(IDX:IDX+TorcTrialDur-1,:)=0;
        idx = find(strcmpi(Data.TagNames,'TorcOnset'));
        Data.Tags(IDX:IDX+TorcOnset-1,idx) = 1;
        idx = find(strcmpi(Data.TagNames,'TorcOffset'));
        Data.Tags(IDX:IDX+TorcOffset-1,idx) = 1;
        idx = find(strcmpi(Data.TagNames,'TorcIndex'));
        bb = strsep(bb{2},'_');
        Data.Tags(IDX:IDX+TorcOnset-1,idx) = bb{3};
        idx = find(strcmpi(Data.TagNames,'TorcLevel'));
        Data.Tags(IDX:IDX+TorcOnset-1,idx) = OveralldB;
        IDX = size(Data.Tags,1)+1;
    elseif strfind(Note{i},'Silence')
                Data.Tags(IDX:IDX+SilentTrialDur-1,:)=0;
        IDX = size(Data.Tags,1)+1;
    end
end
save(savepath,'Data')