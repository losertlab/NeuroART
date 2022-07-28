function exptparams = PerformanceAnalysis (o, HW, StimEvents, globalparams, exptparams, TrialIndex, ResponseData)
% EarlyResponse: response during the Early window after target. Early response causes the trial to stop immediately and a timeout.
% Hit: response during Response window after the target. This is a correct response to the target and the water reward will be given.
% Miss: no response during Response window after the target. A timeout will be given.
% FalseAlarm: response during corresponding Response window for a NonTarget.Psignal
% HitRate: total number of False Alarm divides by total number of warning trials.
% EarlyRate: total number of Early response divides by total number of warning trials.
% MissRate: total number of missed trials divides by total number of warning trials.
% FalseAlarmRate: total number of False Alarm divides by total number of NonTargets across all warning trials.
global StopExperiment;
fs = HW.params.fsAI;
[t,trial,Note,toff,StimIndex] = evtimes(StimEvents,'Stim*');
ResponseDataDiff = [0; max(0,diff(ResponseData))];
exptparams.Responses.idx = find(ResponseDataDiff);
exptparams.Responses.fs = fs;
[Type, StimName, StimNonTarOrTar] = ParseStimEvent (StimEvents(StimIndex));
EarlyWin = get(o,'EarlyWindow');
ResponseWin = get(o,'ResponseWindow');
if sum(abs(get(o,'EarlyWindow'))) > 0
    EarlyResponses = ResponseDataDiff(fs*(StimEvents(StimIndex).StartTime+EarlyWin(1))+1:fs*(StimEvents(StimIndex).StartTime+EarlyWin(2))-1);
else
    EarlyResponses=[];
end
TarResponse = ResponseDataDiff(single(max(1,fs*ResponseWin(1)):fs*ResponseWin(2)));
exptparams.ResponseWin = ResponseWin;
stim=get(exptparams.TrialObject,'PriIndices');
stim=stim(:,1:2); %remove attenuations
TrialStim=get(exptparams.TrialObject,'TrialIndices');
TrialStim=TrialStim(:,1:2); %remove attenuations
blockIndex = rem(TrialIndex-1,size(TrialStim,1))+1;
stimnum=find(sum(stim==repmat(TrialStim(blockIndex,:),size(stim,1),1),2)==size(stim,2));
if ~isempty(find(ResponseDataDiff,1)/fs)
    FirstResponse = find(ResponseDataDiff,1)/fs;
else
    FirstResponse = nan;
end
if isfield(exptparams,'FirstResponse')
    exptparams.FirstResponse(end+1,1:4) = [FirstResponse stim(stimnum(1),[2 1]) ~isempty(find(TarResponse,1))];
else
    exptparams.FirstResponse(1,1:4) = [FirstResponse stim(stimnum(1),[2 1]) ~isempty(find(TarResponse,1))];
end
if ~isfield(exptparams,'AverageResponse')
    exptparams.AverageResponse.nontar=[];
    exptparams.AverageResponse.tar=[];
    exptparams.AverageResponse.probe=[];
end
switch lower(StimNonTarOrTar)
    case 'nontarget'
        exptparams.AverageResponse.nontar(end+1,1:length(ResponseData))=ResponseData(:)';
    case 'target'
        exptparams.AverageResponse.tar(end+1,1:length(ResponseData))=ResponseData(:)';
    case 'probe'
        exptparams.AverageResponse.probe(end+1,1:length(ResponseData))=ResponseData(:)';
end
if isfield(exptparams, 'Performance')
    perf = exptparams.Performance(1:end-1);
    cnt2 = length(perf) + 1;
else
    cnt2 = 1;
end
perf(cnt2).Trials=1;
perf(cnt2).ThisTrial='??';
perf(cnt2).EarlyTrial   = double(~isempty(find(EarlyResponses,1)));
perf(cnt2).Miss=NaN;
perf(cnt2).Hit=NaN;
perf(cnt2).FalseAlarm=NaN;
perf(cnt2).pHit=NaN;
perf(cnt2).pMiss = NaN;
if perf(cnt2).EarlyTrial
    perf(cnt2).ThisTrial = 'Early';
end
if strcmpi(StimNonTarOrTar,'nontarget')
    perf(cnt2).FalseAlarm   = double(~perf(cnt2).EarlyTrial && ~isempty(find(TarResponse,1)));
    if perf(cnt2).FalseAlarm
        perf(cnt2).ThisTrial = 'FalseAlarm';
    elseif ~perf(cnt2).EarlyTrial
        perf(cnt2).ThisTrial = 'CorrectReject';
    end
elseif strcmpi(StimNonTarOrTar,'Target')
    perf(cnt2).Hit=double(~perf(cnt2).EarlyTrial && ~isempty(find(TarResponse,1)));
    if perf(cnt2).Hit
        perf(cnt2).ThisTrial = 'Hit';
    elseif ~perf(cnt2).EarlyTrial
        perf(cnt2).ThisTrial = 'Miss';
    end
    perf(cnt2).Miss = double(~perf(cnt2).EarlyTrial && ~perf(cnt2).Hit);
    perf(cnt2).EarlyTrial=perf(cnt2).EarlyTrial+2;
else %the probe
    perf(cnt2).pHit=double(~perf(cnt2).EarlyTrial && ~isempty(find(TarResponse,1)));
    if perf(cnt2).pHit
        perf(cnt2).ThisTrial = 'probe Hit';
    elseif ~perf(cnt2).EarlyTrial
        perf(cnt2).ThisTrial = 'probe Miss';
    end
    perf(cnt2).pMiss = double(~perf(cnt2).EarlyTrial && ~perf(cnt2).pHit);
    perf(cnt2).EarlyTrial=perf(cnt2).EarlyTrial+4;
end
perf(cnt2).ResponseRate = length(find(ResponseDataDiff)) / length(ResponseDataDiff);
TotalTar = sum(~isnan(cat(1,perf.Hit)));
TotalNonTar = sum(~isnan(cat(1,perf.FalseAlarm)));
TotalProbe  = sum(~isnan(cat(1,perf.pHit)));
TotalEarlyNonTar = sum(cat(1,perf.EarlyTrial)==1);
TotalEarlyTar = sum(cat(1,perf.EarlyTrial)==3);
TotalEarlyProbe = sum(cat(1,perf.EarlyTrial)==5);
perf(cnt2).EarlyRate = (TotalEarlyTar+TotalEarlyNonTar)/(TotalTar+TotalNonTar);
perf(cnt2).HitRate = sum(cat(1,perf.Hit)==1) /(TotalTar-TotalEarlyTar);
perf(cnt2).MissRate = (TotalTar-sum(cat(1,perf.Hit)==1)-TotalEarlyTar) /(TotalTar-TotalEarlyTar);
perf(cnt2).FalseAlarmRate  = sum(cat(1,perf.FalseAlarm)==1) /(TotalNonTar-TotalEarlyNonTar);
perf(cnt2).CorrectRejectRate= (TotalNonTar-sum(cat(1,perf.FalseAlarm)==1)-TotalEarlyNonTar) /(TotalNonTar-TotalEarlyNonTar);
perf(cnt2).DiscriminationRate = perf(cnt2).HitRate * (1-perf(cnt2).FalseAlarmRate);
perf(cnt2).pHitRate = sum(cat(1,perf.pHit)==1) /(TotalProbe-TotalEarlyProbe);
perf(cnt2).pMissRate = (TotalProbe-sum(cat(1,perf.pHit)==1)-TotalEarlyProbe) /(TotalProbe-TotalEarlyProbe);
%also, calculate block stats
RecentIndex = max(1 , TrialIndex-exptparams.TrialBlock+1):TrialIndex;
tt = cat(1,perf(RecentIndex).FalseAlarm);
tt(find(isnan(tt)))=[];
perf(cnt2).RecentFalseAlarmRate = sum(tt)/(length(tt));
tt = cat(1,perf(RecentIndex).Hit);
tt(find(isnan(tt)))=[];
perf(cnt2).RecentHitRate = (sum(tt))/(length(tt));
perf(cnt2).RecentDiscriminationRate = perf(cnt2).RecentHitRate * (1-perf(cnt2).RecentFalseAlarmRate);
% change all rates to percentage. If its not rate, put the sum and 'out of' at the end
PerfFields = fieldnames(perf);
for cnt1 = 1:length(PerfFields)
    if isinf(perf(cnt2).(PerfFields{cnt1}))
        perf(cnt2).(PerfFields{cnt1}) = 0;
    end
    if ~isempty(strfind(PerfFields{cnt1},'Rate')) % if its a rate, do not divide by number of trials, just make it percentage:
        if isnan(perf(cnt2).(PerfFields{cnt1}))
            perf(cnt2).(PerfFields{cnt1}) = 0;
        end
        perfPer.(PerfFields{cnt1}) = round(perf(cnt2).(PerfFields{cnt1})*100);
    else
        if isnumeric(perf(cnt2).(PerfFields{cnt1}))
            perfPer.(PerfFields{cnt1})(1) = sum(cat(1,perf.(PerfFields{cnt1}))>0);
        else
            perfPer.(PerfFields{cnt1}) = perf(cnt2).(PerfFields{cnt1});
        end
    end
end
exptparams.Performance(cnt2) = perf(cnt2);
exptparams.Performance(cnt2+1) = perfPer;
%Non-response flag
if get(o, 'QuitNoResponse')
    target_tr=find(~isnan(cat(1,perf.Hit)));
    if isfield(exptparams,'Performance') && (length(target_tr)>=5) && isempty(strfind(globalparams.Physiology,'Passive'))
        DidSheRespond = cat(1,exptparams.Performance(target_tr(end-4:end)).Miss);
        if sum(DidSheRespond) == 5
            StopExperiment = 1;
        end
    end
end