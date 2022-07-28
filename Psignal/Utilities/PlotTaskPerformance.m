function BhvOut = PlotTaskPerformance(P, Data)
%This code is an analysis of behavior run from Psignal using the
%AuditoryRecognitionTask trialobject. It will currently function for both
%detection and discrimination experiments. It does not seperate different
%targets or nontargets, and treats probes as nontargets.

%Inputs
%Data: Psignal data
%P: Analysis parameters

addpath(genpath('C:\psignal\'));

%Load Data
if nargin < 1
    [FileName FilePath] = uigetfile('*.mat','Load Psignal Data');
    Data = LoadPsignalParams([FilePath FileName]);
    bb=strsep(FilePath,'\');
    
    %Generate Psignal Matrices.
    P.expectedFPS = 30;
    P.animal = bb{end-1};
    P.expname = {'Behavior_001'};
    P.path = [];
    P.psignalfiles = {[FilePath FileName]};
    Data.PsignalMatrix=GeneratePsignalMatrix(P);
end

if nargin < 2
    P.pdfbinsize = 0.01;
end

f=randi([56634 325254634]);
figure(f);
set(gcf,'position',[379   311   776   437]);
BhvOut=[];

%Find trial length and response window
pre=Data(1).Primary.PreStimSilence;
dur=Data(1).Primary.Duration;
post=Data(1).Primary.PostStimSilence;
tmax = pre+dur+post;
RW=Data(1).Behavior.ResponseWindow;

%Gather response data per experiment, by animal
Animals=[];
for i = 1:length(Data)
    Animals = [Animals; {Data(i).globalparams.Animal}];
end
Animals = unique(Animals);

%EH: early hit, DH: delayed hit, M: miss, EF: early false alarm, DF: delayed false alarm, CR: correct rejection
EH=[];
DH=[];
M=[];
EF=[];
DF=[];
CR=[];
E=[];
%FR: first response
EH_FR=[];
DH_FR=[];
EF_FR=[];
DF_FR=[];
%Response traces
DH_Tar=[];
EH_Tar=[];
EF_NonTar=[];
DF_NonTar=[];
%Total Response Traces
AllTarTraces = [];
AllNonTarTraces = [];
for c = 1:length(Animals)
    %first response
    EH_FR_Temp=[];
    DH_FR_Temp=[];
    EF_FR_Temp=[];
    DF_FR_Temp=[];
    %performance stats
    E_Temp=[];
    EH_Temp=[];
    DH_Temp=[];
    M_Temp=[];
    DF_Temp=[];
    EF_Temp=[];
    CR_Temp=[];
    %Response traces
    EF_NonTar_Temp=[];
    DF_NonTar_Temp=[];
    EH_Tar_Temp=[];
    DH_Tar_Temp=[];
    Trial_count=[];
    for i = 1:length(Data)
        
        %Total # trials for each experiment
        Trial_count = [Trial_count; Data(i).exptevents(end).Trial] ;
        
        if strcmpi(Animals{c},Data(i).globalparams.Animal)
            
            %Response data
            Tar = Data(i).exptparams.AverageResponse.tar;
            NonTar = Data(i).exptparams.AverageResponse.nontar;
            
            %Find trial types
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'Early'));
            E_idx=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'EarlyHit'));
            EH_idx=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'Hit'));
            DH_idx=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'Miss'));
            M_idx=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'FalseAlarm'));
            DF_idx=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'EarlyFalseAlarm'));
            EF_idx=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'CorrectReject'));
            CR_idx=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            
            %Mistaken EF
            if isempty(NonTar) && ~isempty(EF_idx)
                E_idx = EF_idx;
                EF_idx=[];
            end
            
            %Response latencies: probes treated as nontargets
            FR=Data(i).exptparams.FirstResponse(EH_idx,:);
            EH_FR_Temp = [EH_FR_Temp; FR(FR(:,2)==1 & FR(:,4)==1,1)];
            FR=Data(i).exptparams.FirstResponse(DH_idx,:);
            DH_FR_Temp = [DH_FR_Temp; FR(FR(:,2)==1 & FR(:,4)==1,1)];
            FR=Data(i).exptparams.FirstResponse(DF_idx,:);
            DF_FR_Temp = [DF_FR_Temp; FR((FR(:,2)==0 | FR(:,2)==-1) & FR(:,4)==1,1)];
            FR=Data(i).exptparams.FirstResponse(EF_idx,:);
            EF_FR_Temp = [EF_FR_Temp; FR((FR(:,2)==0 | FR(:,2)==-1)  & FR(:,4)==1,1)];
            
            %# trials for each response type
            E_Temp = [EH_Temp; length(E_idx)];
            EH_Temp = [EH_Temp; length(EH_idx)];
            DH_Temp = [DH_Temp; length(DH_idx)];
            M_Temp = [M_Temp; length(M_idx)];
            EF_Temp = [EF_Temp; length(EF_idx)];
            DF_Temp = [DF_Temp; length(DF_idx)];
            CR_Temp = [CR_Temp; length(CR_idx)];
            
            %Response traces
            AllTarTraces = [AllTarTraces; Tar];
            AllNonTarTraces = [AllNonTarTraces; NonTar];
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'Target'));
            TarTrials=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=find(strcmpi(Data(i).PsignalMatrix.TagNames(:,2),'NonTarget'));
            NonTarTrials=find(sum(squeeze(Data(i).PsignalMatrix.Tags(:,:,idx))));
            idx=[];
            for ii = 1:length(EH_idx)
                idx = [idx; find(TarTrials==EH_idx(ii))];
            end
            EH_Tar_Temp = [EH_Tar_Temp; nanmean(Tar(idx,:),1)];
            idx=[];
            for ii = 1:length(DH_idx)
                idx = [idx; find(TarTrials==DH_idx(ii))];
            end
            DH_Tar_Temp = [DH_Tar_Temp; nanmean(Tar(idx,:),1)];
            Tar = Data(i).exptparams.AverageResponse.tar;
            idx=[];
            for ii = 1:length(EF_idx)
                idx = [idx; find(NonTarTrials==EF_idx(ii))];
            end
            EF_NonTar_Temp = [EF_NonTar_Temp; nanmean(NonTar(idx,:),1)];
            idx=[];
            for ii = 1:length(DF_idx)
                idx = [idx; find(NonTarTrials==DF_idx(ii))];
            end
            DF_NonTar_Temp = [DF_NonTar_Temp; nanmean(NonTar(idx,:),1)];
            
        end
    end
    E=[E; {E_Temp}];
    EH=[EH; {EH_Temp}];
    DH=[DH; {DH_Temp}];
    M=[M; {M_Temp}];
    EF=[EF; {EF_Temp}];
    DF=[DF; {DF_Temp}];
    CR=[CR; {CR_Temp}];
    EH_FR=[EH_FR; {EH_FR_Temp}];
    DH_FR=[DH_FR; {DH_FR_Temp}];
    EF_FR=[EF_FR; {EF_FR_Temp}];
    DF_FR=[DF_FR; {DF_FR_Temp}];
    EH_Tar = [EH_Tar; {EH_Tar_Temp}];
    DH_Tar = [DH_Tar; {DH_Tar_Temp}];
    EF_NonTar = [EF_NonTar; {EF_NonTar_Temp}];
    DF_NonTar = [DF_NonTar; {DF_NonTar_Temp}];
end
BhvOut.Trial_count = Trial_count;

%Plot lick rasters
fs=1000;
t=-pre:1/fs:dur+post-(1/fs);
figure;
set(gcf,'position',[379   311   776   437]);
%Target
subplot(1,2,1)
imagesc(t,1:size(AllTarTraces,1),1-AllTarTraces)
colormap(gray)
hold on
aa=axis;
plot([0 0],[aa(3) aa(4)],'k')
plot([.5 .5],[aa(3) aa(4)],'k--')
plot([1 1],[aa(3) aa(4)],'k')
box off
set(gca,'ytick',[1 size(AllTarTraces,1)])
ylabel('Trial')
xlabel('Time (s re. tone onset)')
title('Target Licking')
set(gca,'fontsize',8)
if ~isempty(AllNonTarTraces)
    %NonTarget
    subplot(1,2,2)
    imagesc(t,1:size(AllNonTarTraces,1),1-AllNonTarTraces)
    colormap(gray)
    hold on
    aa=axis;
    plot([0 0],[aa(3) aa(4)],'k')
    plot([.5 .5],[aa(3) aa(4)],'k--')
    plot([1 1],[aa(3) aa(4)],'k')
    box off
    set(gca,'ytick',[1 size(AllNonTarTraces,1)])
    xlabel('Time (s re. tone onset)')
    title('NonTarget Licking')
    set(gca,'fontsize',8)
else
    %NonTarget
    subplot(1,2,2)
    box off
    title('No Non-Targets')
    set(gca,'fontsize',8)
    axis off
end

%Plot response latency histogram across experiments
t=0:P.pdfbinsize:tmax;
Tar_FR = cell2mat([EH_FR;DH_FR]);
[h2_Tar,xi] = ksdensity(Tar_FR,t,'function','pdf');
NonTar_FR = cell2mat([EF_FR;DF_FR]);
[h2_NonTar,xi] = ksdensity(NonTar_FR,t,'function','pdf');

figure(f);
%Target
subplot(2,4,1:2)
hold on
if ~isempty(Tar_FR)
    plot(t,h2_Tar./max(abs(h2_Tar)),'color','k','linewidth',1);
    hold on;
    aa=axis;
    [j TauTar] = max(h2_Tar);
end
%Non-target
subplot(2,4,3:4)
hold on
if ~isempty(AllNonTarTraces)
    plot(t,h2_NonTar./max(abs(h2_Tar)),'k','linewidth',1);
    hold on;
    aa=axis;
    [j TauNonTar] = max(h2_NonTar);
else
    title('No Non-Targets')
    set(gca,'fontsize',8)
    axis off
end

%Plot average response functions
DTarLickogram = [nanmean(cell2mat(DH_Tar),1); nanstd(cell2mat(DH_Tar),[],1)];
ETarLickogram = [nanmean(cell2mat(EH_Tar),1); nanstd(cell2mat(EH_Tar),[],1)];
DNonTarLickogram = [nanmean(cell2mat(DF_NonTar),1); nanstd(cell2mat(DF_NonTar),[],1)];
ENonTarLickogram = [nanmean(cell2mat(EF_NonTar),1); nanstd(cell2mat(EF_NonTar),[],1)];
m = max(abs([ETarLickogram(1,:) DTarLickogram(1,:) DNonTarLickogram(1,:) ENonTarLickogram(1,:)]));

%Target
subplot(2,4,1:2)
fs=1000;
hold on
t=0:1/fs:size(DTarLickogram,2)/fs-1/fs;
plot(t,ETarLickogram(1,:)./m,'r','linewidth',1)
plot(t,DTarLickogram(1,:)./m,'b','linewidth',1)
aa=axis;
legend('Latency','Early H','Delayed H','AutoUpdate','off')
ylim([0 roundTo(aa(4),3)])
tlim = [0 tmax];
xlim(tlim)
title([{'TARGETS'};{'Response Histograms'}],'fontsize',8);
plot([pre pre],[0 roundTo(aa(4),2)],'k')
plot([RW(1) RW(1) ],[0 roundTo(aa(4),2)],'k:')
plot([pre pre]+dur,[0 roundTo(aa(4),2)],'k')
xlim(tlim)
ylabel('Probability')
xlabel('Time (seconds)');
%NonTarget
subplot(2,4,3:4)
if ~isempty(AllNonTarTraces)
    hold on
    t=0:1/fs:size(DNonTarLickogram,2)/fs-1/fs;
    plot(t,ENonTarLickogram(1,:)./m,'r','linewidth',1)
    plot(t,DNonTarLickogram(1,:)./m,'b','linewidth',1)
    aa=axis;
    ylim([0 roundTo(aa(4),3)])
    tlim = [0 tmax];
    xlim(tlim)
    title([{'NONTARGETS'};{'Response Histograms'}],'fontsize',8);
    plot([pre pre],[0 roundTo(aa(4),2)],'k')
    plot([RW(1) RW(1) ],[0 roundTo(aa(4),2)],'k:')
    plot([pre pre]+dur,[0 roundTo(aa(4),2)],'k')
    xlim(tlim)
    xlabel('Time (seconds)');
    legend('Latency','Early FA','Delayed FA')
else
    title('No Non-Targets')
    set(gca,'fontsize',8)
    axis off
end

%Plot CDFs for response latency
%Target
h1=subplot(2,4,5);
%Per experiment
FR = [cell2mat(EH_FR); cell2mat(DH_FR)];
FR_Hist = histcounts(FR,10);
FR_Hist = FR_Hist./sum(FR_Hist);
bins = linspace(min(FR),max(FR),length(FR_Hist));
plot(bins,cumsum(FR_Hist),'k','linewidth',2)
hold on
%Per animal
FR = [cellfun(@mean,EH_FR); cellfun(@mean,DH_FR)];
FR_Hist = histcounts(FR,10);
FR_Hist = FR_Hist./sum(FR_Hist);
bins = linspace(min(FR),max(FR),length(FR_Hist));
plot(bins,cumsum(FR_Hist),'color',[.67 .67 .67],'linewidth',2)
legend('Exp.','Animal')
axis tight
ylabel('Probability')
xlabel('First Lick (s)')
set(gca,'fontsize',8)
title('Response Latency')
xLims(1,:) = axis(h1);
%NonTarget
h2=subplot(2,4,7);
%Per experiment
FR = [cell2mat(EF_FR); cell2mat(DF_FR)];
if ~isempty(AllNonTarTraces)
    FR_Hist = histcounts(FR,10);
    FR_Hist = FR_Hist./sum(FR_Hist);
    bins = linspace(min(FR),max(FR),length(FR_Hist));
    plot(bins,cumsum(FR_Hist),'k','linewidth',2)
    hold on
    %Per animal
    FR = [cellfun(@mean,EF_FR); cellfun(@mean,DF_FR)];
    FR_Hist = histcounts(FR,10);
    FR_Hist = FR_Hist./sum(FR_Hist);
    bins = linspace(min(FR),max(FR),length(FR_Hist));
    plot(bins,cumsum(FR_Hist),'color',[.67 .67 .67],'linewidth',2)
    legend('Exp.','Animal')
    axis tight
    ylabel('Probability')
    xlabel('First Lick (s)')
    set(gca,'fontsize',8)
    title('Response Latency')
    xLims(2,:) = axis(h2);
    set(h1,'xlim',[min(xLims(:,1)) max(xLims(:,2))])
    set(h2,'xlim',[min(xLims(:,1)) max(xLims(:,2))])
else
    axis off
end

%Plot CDFs for performance accuracy
%Targets
h1=subplot(2,4,6);
%Per experiment
HR_exp = 100*((cell2mat(EH))+(cell2mat(DH)))./((cell2mat(EH))+(cell2mat(DH))+(cell2mat(M)));
HR_expHist = histcounts(HR_exp,10);
HR_expHist = HR_expHist./sum(HR_expHist);
bins = linspace(min(HR_exp),max(HR_exp),length(HR_expHist));
plot(bins,cumsum(HR_expHist),'k','linewidth',2)
hold on
%Per animal
HR_exp = 100*(cellfun(@mean,EH)+cellfun(@mean,DH))./(cellfun(@mean,EH)+cellfun(@mean,DH)+cellfun(@mean,M));
HR_expHist = histcounts(HR_exp,10);
HR_expHist = HR_expHist./sum(HR_expHist);
bins = linspace(min(HR_exp),max(HR_exp),length(HR_expHist));
plot(bins,cumsum(HR_expHist),'color',[.67 .67 .67],'linewidth',2)
plot([50 50],[0 1],'k')
axis tight
xlabel('Hit Rate (%)')
xLims(1,:) = axis(h1);
title('Responce Accuracy')
set(gca,'fontsize',8)

%NonTargets
h2=subplot(2,4,8);
if ~isempty(AllNonTarTraces)
    %Per experiment
    FR_exp = 100*((cell2mat(EF))+(cell2mat(DF)))./((cell2mat(EF))+(cell2mat(DF))+(cell2mat(CR)));
    FR_expHist = histcounts(FR_exp,10);
    FR_expHist = FR_expHist./sum(FR_expHist);
    bins = linspace(min(FR_exp),max(FR_exp),length(FR_expHist));
    plot(bins,cumsum(FR_expHist),'k','linewidth',2)
    hold on
    %Per animal
    FR_exp = 100*(cellfun(@mean,EF)+cellfun(@mean,DF))./(cellfun(@mean,EF)+cellfun(@mean,DF)+cellfun(@mean,CR));
    FR_expHist = histcounts(FR_exp,10);
    FR_expHist = FR_expHist./sum(FR_expHist);
    bins = linspace(min(FR_exp),max(FR_exp),length(FR_expHist));
    plot(bins,cumsum(FR_expHist),'color',[.67 .67 .67],'linewidth',2)
    plot([50 50],[0 1],'k')
    axis tight
    xlabel('False Alarm Rate (%)')
    title('Response Accuracy')
    set(gca,'fontsize',8)
    xLims(2,:) = axis(h2);
    set(h1,'xlim',[min(xLims(:,1)) max(xLims(:,2))])
    set(h2,'xlim',[min(xLims(:,1)) max(xLims(:,2))])
else
    axis off
end

%Gather behavioral Statistics across experiments
%Early Reponses
ER  = 100*((cell2mat(E)))./((cell2mat(E))+((cell2mat(EH))+(cell2mat(DH))+(cell2mat(M))));
BhvOut.EarlyRate.exp.mu = nanmean(ER);
BhvOut.EarlyRate.exp.std = nanstd(ER);
BhvOut.EarlyRate.exp.N = length(ER);
BhvOut.EarlyRate.exp.Data = ER;
%Hits
EHR  = 100*((cell2mat(EH)))./((cell2mat(EH))+(cell2mat(DH))+(cell2mat(M)));
EL  = cell2mat(EH_FR);
BhvOut.EarlyHitRate.exp.mu = nanmean(EHR);
BhvOut.EarlyHitRate.exp.std = nanstd(EHR);
BhvOut.EarlyHitRate.exp.N = length(EHR);
BhvOut.EarlyHitRate.exp.Data = EHR;
BhvOut.EarlyHitLatency.exp.mu = nanmean(EL);
BhvOut.EarlyHitLatency.exp.std = nanstd(EL);
BhvOut.EarlyHitLatency.exp.N = length(EL);
BhvOut.EarlyHitLatency.exp.Data = EL;
%False Alarms
EFR  = 100*((cell2mat(EF)))./((cell2mat(EF))+(cell2mat(EF))+(cell2mat(CR)));
EL  = cell2mat(EF_FR);
BhvOut.EarlyFalseAlarmRate.exp.mu = nanmean(EFR);
BhvOut.EarlyFalseAlarmRate.exp.std = nanstd(EFR);
BhvOut.EarlyFalseAlarmRate.exp.N = length(EFR);
BhvOut.EarlyFalseAlarmRate.exp.Data = EFR;
BhvOut.EarlyFalseAlarmLatency.exp.mu = nanmean(EL);
BhvOut.EarlyFalseAlarmLatency.exp.std = nanstd(EL);
BhvOut.EarlyFalseAlarmLatency.exp.N = length(EL);
BhvOut.EarlyFalseAlarmLatency.exp.Data = EL;
%Delayed responses
%Hits
DHR  = 100*((cell2mat(DH)))./(((cell2mat(EH))+(cell2mat(DH))+(cell2mat(M))));
DL  = cell2mat(DH_FR);
BhvOut.DelayedHitRate.exp.mu = nanmean(DHR);
BhvOut.DelayedHitRate.exp.std = nanstd(DHR);
BhvOut.DelayedHitRate.exp.N = length(DHR);
BhvOut.DelayedHitRate.exp.Data = DHR;
BhvOut.DelayedHitLatency.exp.mu = nanmean(DL);
BhvOut.DelayedHitLatency.exp.std = nanstd(DL);
BhvOut.DelayedHitLatency.exp.N = length(DL);
BhvOut.DelayedHitLatency.exp.Data = DL;
%False Alarms
DFR  = 100*((cell2mat(DF)))./((cell2mat(DF))+(cell2mat(EF))+(cell2mat(CR)));
DL  = cell2mat(DF_FR);
BhvOut.DelayedFalseAlarmRate.exp.mu = nanmean(DFR);
BhvOut.DelayedFalseAlarmRate.exp.std = nanstd(DFR);
BhvOut.DelayedFalseAlarmRate.exp.N = length(DFR);
BhvOut.DelayedFalseAlarmRate.exp.Data = DFR;
BhvOut.DelayedFalseAlarmLatency.exp.mu = nanmean(DL);
BhvOut.DelayedFalseAlarmLatency.exp.std = nanstd(DL);
BhvOut.DelayedFalseAlarmLatency.exp.N = length(DL);
BhvOut.DelayedFalseAlarmLatency.exp.Data = DL;
%All Hits
HR  = 100*((cell2mat(EH))+(cell2mat(DH)))./(((cell2mat(EH))+(cell2mat(DH))+(cell2mat(M))));
HL  = cell2mat([EH_FR; DH_FR]);
BhvOut.HitRate.exp.mu = nanmean(HR);
BhvOut.HitRate.exp.std = nanstd(HR);
BhvOut.HitRate.exp.N = length(HR);
BhvOut.HitRate.exp.Data = HR;
BhvOut.HitLatency.exp.mu = nanmean(HL);
BhvOut.HitLatency.exp.std = nanstd(HL);
BhvOut.HitLatency.exp.N = length(HL);
BhvOut.HitLatency.exp.Data = HL;
%All False Alarms
FR  = 100*((cell2mat(EF))+(cell2mat(DF)))./((cell2mat(EF))+(cell2mat(DF))+(cell2mat(CR)));
FL  = cell2mat([EF_FR; DF_FR]);
BhvOut.FalseAlarmRate.exp.mu = nanmean(FR);
BhvOut.FalseAlarmRate.exp.std = nanstd(FR);
BhvOut.FalseAlarmRate.exp.N = length(FR);
BhvOut.FalseAlarmRate.exp.Data = FR;
BhvOut.FalseAlarmLatency.exp.mu = nanmean(FL);
BhvOut.FalseAlarmLatency.exp.std = nanstd(FL);
BhvOut.FalseAlarmLatency.exp.N = length(FL);
BhvOut.FalseAlarmLatency.exp.Data = FL;
%Miss rate
MR  = 100*((cell2mat(M)))./(((cell2mat(EH))+(cell2mat(DH))+(cell2mat(M))));
BhvOut.MissRate.exp.mu = nanmean(MR);
BhvOut.MissRate.exp.std = nanstd(MR);
BhvOut.MissRate.exp.N = length(MR);
BhvOut.MissRate.exp.Data = MR;
%Correct rejection rate
CrR  = 100*((cell2mat(CR)))./((cell2mat(EF))+(cell2mat(DF))+(cell2mat(CR)));
BhvOut.CorrectRejectionRate.exp.mu = nanmean(CrR);
BhvOut.CorrectRejectionRate.exp.std = nanstd(CrR);
BhvOut.CorrectRejectionRate.exp.N = length(CrR);
BhvOut.CorrectRejectionRate.exp.Data = CrR;

%Stats
[h p]=ttest(HR,MR);
BhvOut.HitMiss.exp.ttest = [h p];
[h p]=ttest(EHR,DHR);
BhvOut.RewPun.exp.ttest = [h p];

%Gather behavioral Statistics across animals
%Early Reponses
ER  = 100*((cellfun(@nanmean,E)))./((cellfun(@nanmean,E))+(cellfun(@nanmean,EH))+(cellfun(@nanmean,DH))+(cellfun(@nanmean,M)));
BhvOut.EarlyRate.animals.mu = nanmean(ER);
BhvOut.EarlyRate.animals.std = nanstd(ER);
BhvOut.EarlyRate.animals.N = length(ER);
BhvOut.EarlyRate.animals.Data = ER;
%Hits
EHR  = 100*((cellfun(@nanmean,EH)))./((cellfun(@nanmean,EH))+(cellfun(@nanmean,DH))+(cellfun(@nanmean,M)));
EL  = cellfun(@nanmean,EH_FR);
BhvOut.EarlyHitRate.animals.mu = nanmean(EHR);
BhvOut.EarlyHitRate.animals.std = nanstd(EHR);
BhvOut.EarlyHitRate.animals.N = length(EHR);
BhvOut.EarlyHitRate.animals.Data = EHR;
BhvOut.EarlyHitLatency.animals.mu = nanmean(EL);
BhvOut.EarlyHitLatency.animals.std = nanstd(EL);
BhvOut.EarlyHitLatency.animals.N = length(EL);
BhvOut.EarlyHitLatency.animals.Data = EL;
%False Alarms
EFR  = 100*((cellfun(@nanmean,EF)))./((cellfun(@nanmean,EF))+(cellfun(@nanmean,DF))+(cellfun(@nanmean,CR)));
EL  = cellfun(@nanmean,EF_FR);
BhvOut.EarlyFalseAlarmRate.animals.mu = nanmean(EFR);
BhvOut.EarlyFalseAlarmRate.animals.std = nanstd(EFR);
BhvOut.EarlyFalseAlarmRate.animals.N = length(EFR);
BhvOut.EarlyFalseAlarmRate.animals.Data = EFR;
BhvOut.EarlyFalseAlarmLatency.animals.mu = nanmean(EL);
BhvOut.EarlyFalseAlarmLatency.animals.std = nanstd(EL);
BhvOut.EarlyFalseAlarmLatency.animals.N = length(EL);
BhvOut.EarlyFalseAlarmLatency.animals.Data = EL;
%Delayed responses
%Hits
DHR  = 100*((cellfun(@nanmean,DH)))./((cellfun(@nanmean,EH))+(cellfun(@nanmean,DH))+(cellfun(@nanmean,M)));
DL  = cellfun(@nanmean,DH_FR);
BhvOut.DelayedHitRate.animals.mu = nanmean(DHR);
BhvOut.DelayedHitRate.animals.std = nanstd(DHR);
BhvOut.DelayedHitRate.animals.N = length(DHR);
BhvOut.DelayedHitRate.animals.Data = DHR;
BhvOut.DelayedHitLatency.animals.mu = nanmean(DL);
BhvOut.DelayedHitLatency.animals.std = nanstd(DL);
BhvOut.DelayedHitLatency.animals.N = length(DL);
BhvOut.DelayedHitLatency.animals.Data = DL;
%False alarms
DFR  = 100*((cellfun(@nanmean,DF)))./((cellfun(@nanmean,DF))+(cellfun(@nanmean,DF))+(cellfun(@nanmean,CR)));
DL  = cellfun(@nanmean,DF_FR);
BhvOut.DelayedFalseAlarmRate.animals.mu = nanmean(DFR);
BhvOut.DelayedFalseAlarmRate.animals.std = nanstd(DFR);
BhvOut.DelayedFalseAlarmRate.animals.N = length(DFR);
BhvOut.DelayedFalseAlarmRate.animals.Data = DFR;
BhvOut.DelayedFalseAlarmLatency.animals.mu = nanmean(DL);
BhvOut.DelayedFalseAlarmLatency.animals.std = nanstd(DL);
BhvOut.DelayedFalseAlarmLatency.animals.N = length(DL);
BhvOut.DelayedFalseAlarmLatency.animals.Data = DL;
%All Hits
HR  = 100*((cellfun(@nanmean,EF))+(cellfun(@nanmean,DH)))./((cellfun(@nanmean,EH))+(cellfun(@nanmean,DH))+(cellfun(@nanmean,M)));
HL  = cellfun(@nanmean,[EH_FR; DH_FR]);
BhvOut.HitRate.animals.mu = nanmean(HR);
BhvOut.HitRate.animals.std = nanstd(HR);
BhvOut.HitRate.animals.N = length(HR);
BhvOut.HitRate.animals.Data = HR;
BhvOut.HitLatency.animals.mu = nanmean(HL);
BhvOut.HitLatency.animals.std = nanstd(HL);
BhvOut.HitLatency.animals.N = length(HL);
BhvOut.HitLatency.animals.Data = HL;
%All False Alarms
FR  = 100*((cellfun(@nanmean,EF))+(cellfun(@nanmean,DF)))./((cellfun(@nanmean,EF))+(cellfun(@nanmean,DF))+(cellfun(@nanmean,CR)));
FL  = cellfun(@nanmean,[EF_FR; DF_FR]);
BhvOut.FalseAlarmRate.animals.mu = nanmean(FR);
BhvOut.FalseAlarmRate.animals.std = nanstd(FR);
BhvOut.FalseAlarmRate.animals.N = length(FR);
BhvOut.FalseAlarmRate.animals.Data = FR;
BhvOut.FalseAlarmLatency.animals.mu = nanmean(FL);
BhvOut.FalseAlarmLatency.animals.std = nanstd(FL);
BhvOut.FalseAlarmLatency.animals.N = length(FL);
BhvOut.FalseAlarmLatency.animals.Data = FL;
%Miss rate
MR  = 100*((cellfun(@nanmean,M)))./((cellfun(@nanmean,EH))+(cellfun(@nanmean,DH))+(cellfun(@nanmean,M)));
BhvOut.MissRate.animals.mu = nanmean(MR);
BhvOut.MissRate.animals.std = nanstd(MR);
BhvOut.MissRate.animals.N = length(MR);
BhvOut.MissRate.animals.Data = MR;
%Correct rejection rate
CrR  = 100*((cellfun(@nanmean,CR)))./((cellfun(@nanmean,EF))+(cellfun(@nanmean,DF))+(cellfun(@nanmean,CR)));
BhvOut.CorrectRejectionRate.animals.mu = nanmean(CrR);
BhvOut.CorrectRejectionRate.animals.std = nanstd(CrR);
BhvOut.CorrectRejectionRate.animals.N = length(CrR);
BhvOut.CorrectRejectionRate.animals.Data = CrR;

%Stats
[h p]=ttest(HR,MR);
BhvOut.HitMiss.animals.ttest = [h p];
[h p]=ttest(EHR,DHR);
BhvOut.RewPun.animals.ttest = [h p];

%Plot performance rates
figure
%Hit Rate
bar(1,BhvOut.HitRate.exp.mu,1,'facecolor','w');
hold on
bar(2,BhvOut.HitRate.animals.mu,1,'facecolor','k');
legend('Exps.','Animals','AutoUpdate','off')
errorbar(1,BhvOut.HitRate.exp.mu,2*BhvOut.HitRate.exp.std./sqrt(BhvOut.HitRate.exp.N),'k')
errorbar(2,BhvOut.HitRate.animals.mu,2*BhvOut.HitRate.animals.std./sqrt(BhvOut.HitRate.animals.N),'k')
%Rewarded hit rate
bar(3.5,BhvOut.DelayedHitRate.exp.mu,1,'facecolor','w');
errorbar(3.5,BhvOut.DelayedHitRate.exp.mu,2*BhvOut.DelayedHitRate.exp.std./sqrt(BhvOut.DelayedHitRate.exp.N),'k')
bar(4.5,BhvOut.DelayedHitRate.animals.mu,1,'facecolor','k');
errorbar(4.5,BhvOut.DelayedHitRate.animals.mu,2*BhvOut.DelayedHitRate.animals.std./sqrt(BhvOut.DelayedHitRate.animals.N),'k')
%Punished hit rate
bar(6,BhvOut.EarlyHitRate.exp.mu,1,'facecolor','w');
errorbar(6,BhvOut.EarlyHitRate.exp.mu,2*BhvOut.EarlyHitRate.exp.std./sqrt(BhvOut.EarlyHitRate.exp.N),'k')
bar(7,BhvOut.EarlyHitRate.animals.mu,1,'facecolor','k');
errorbar(7,BhvOut.EarlyHitRate.animals.mu,2*BhvOut.EarlyHitRate.animals.std./sqrt(BhvOut.EarlyHitRate.animals.N),'k')
%Miss rate
bar(8.5,BhvOut.MissRate.exp.mu,1,'facecolor','w');
errorbar(8.5,BhvOut.MissRate.exp.mu,2*BhvOut.MissRate.exp.std./sqrt(BhvOut.MissRate.exp.N),'k')
bar(9.5,BhvOut.MissRate.animals.mu,1,'facecolor','k');
errorbar(9.5,BhvOut.MissRate.animals.mu,2*BhvOut.MissRate.animals.std./sqrt(BhvOut.MissRate.animals.N),'k')
%Early rate
bar(11,BhvOut.EarlyRate.exp.mu,1,'facecolor','w');
errorbar(11,BhvOut.EarlyRate.exp.mu,2*BhvOut.EarlyRate.exp.std./sqrt(BhvOut.EarlyRate.exp.N),'k')
bar(12,BhvOut.EarlyRate.animals.mu,1,'facecolor','k');
errorbar(12,BhvOut.EarlyRate.animals.mu,2*BhvOut.EarlyRate.animals.std./sqrt(BhvOut.EarlyRate.animals.N),'k')
xlim([0 13])
ylabel('Performance Rate (%)')
set(gca,'xtick',[1 3.5 6 9.5 11],'xticklabel',{'Hit','Pun. Hit', 'Rew. Hit','Miss','Early'})
box off
set(gca,'fontsize',8)





