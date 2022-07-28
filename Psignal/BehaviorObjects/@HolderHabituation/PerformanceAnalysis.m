function exptparams = PerformanceAnalysis (o, HW, StimEvents, globalparams, exptparams, TrialIndex, ResponseData)
% Hit: response during Response window after the target. This is a correct response to the target and the water reward will be given.
% Miss: no response during Response window after the target. A timeout will be given.
% HitRate: total number of False Alarm divides by total number of warning trials.
% MissRate: total number of missed trials divides by total number of warning trials.
global StopExperiment;
fs = HW.params.fsAI;
[t,trial,Note,toff,StimIndex] = evtimes(StimEvents,'Stim*');
[Type, StimName, StimNonTarOrTar] = ParseStimEvent (StimEvents(StimIndex));
ResponseWin = [StimEvents(StimIndex).StartTime StimEvents(StimIndex).StopTime];
exptparams.ResponseWin = ResponseWin;
% ResponseData = max(0,diff(ResponseData));
Response = ResponseData(ceil(max(1,fs*ResponseWin(1))):floor(min(length(ResponseData),fs*ResponseWin(2))));
temp = find([Response],1)/fs;
stim=get(exptparams.TrialObject,'PriIndices');
stim=stim(:,1:2); %remove attenuations
TrialStim=get(exptparams.TrialObject,'TrialIndices');
TrialStim=TrialStim(:,1:2); %remove attenuations
blockIndex = rem(TrialIndex-1,size(TrialStim,1))+1;
stimnum=find(sum(stim==repmat(TrialStim(blockIndex,:),size(stim,1),1),2)==size(stim,2));
if ~isempty(temp)
    FirstResponse = temp;
else
    FirstResponse = nan;
end
if isfield(exptparams,'FirstResponse')
    exptparams.FirstResponse(end+1,1:4) = [FirstResponse stim(stimnum(1),[2 1]) ~isempty(find(Response,1))];
else
    exptparams.FirstResponse(1,1:4) = [FirstResponse stim(stimnum(1),[2 1]) ~isempty(find(Response,1))];
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
perf(cnt2).Miss=NaN;
perf(cnt2).Hit=NaN;
perf(cnt2).pHit=NaN;
perf(cnt2).pMiss = NaN;
if strcmpi(StimNonTarOrTar,'Target')
    perf(cnt2).Hit=double(~isempty(find(Response,1)));
    if perf(cnt2).Hit
        perf(cnt2).ThisTrial = 'Hit';
    else
        perf(cnt2).ThisTrial = 'Miss';
    end
    perf(cnt2).Miss = double(~perf(cnt2).Hit);
end
perf(cnt2).ResponseRate = length(find(ResponseData)) / length(ResponseData);
TotalTar = sum(~isnan(cat(1,perf.Hit)));
perf(cnt2).HitRate = sum(cat(1,perf.Hit)==1) /(TotalTar);
perf(cnt2).MissRate = (TotalTar-sum(cat(1,perf.Hit)==1)) /(TotalTar);
%also, calculate block stats
RecentIndex = max(1 , TrialIndex-exptparams.TrialBlock+1):TrialIndex;
tt = cat(1,perf(RecentIndex).Hit);
tt(find(isnan(tt)))=[];
perf(cnt2).RecentHitRate = (sum(tt))/(length(tt));
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
