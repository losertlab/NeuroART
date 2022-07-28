function exptparams = BehaviorDisplay (o, HW, StimEvents, globalparams, exptparams, TrialIndex, ResponseData, TrialSound)
%BehaviorDisplay method of GoNogo behavior
fs = HW.params.fsAI;
%% See if the results figure exists, otherwise create it:
if ~isfield(exptparams, 'Figure')
    exptparams.Figure = figure('position',[1 1 800 450]);
    movegui(exptparams.Figure,globalparams.disploc);
end
if ~(isempty(ResponseData) && isempty(TrialSound))
    try
        figure(exptparams.Figure)
    catch
        exptparams.Figure = figure('position',[1 1 800 450]);
        movegui(exptparams.Figure,globalparams.disploc);
    end
end
subplot(3,6,1:4);
uicontrol('Style', 'pushbutton', 'String', 'Stop',...
    'Position', [10 10 50 25],...
    'Callback', @Exit);
%% Create the title:
titleMes = ['Rig: ' globalparams.Rig '     Animal: ' globalparams.Animal  '     '...
    num2str(exptparams.StartTime(1:3),'     Date: %1.0f-%1.0f-%1.0f') '     ' ...
    num2str(exptparams.StartTime(4:6),'Start: %1.0f:%1.0f:%1.0f')];
if isfield(exptparams,'StopTime')
    titleMes = [titleMes '     ' num2str(exptparams.StopTime(4:6),'Stop: %1.0f:%1.0f:%1.0f')];
end
if isempty(ResponseData) && isempty(TrialSound)
    %If this is the end, things have been displayed already:
    subplot(3,6,1:4);
    title(titleMes,'interpreter','none','fontsize',8);
    drawnow;
    return;
end

%% Display Recent Hitrate, FalseAlarmRate.
c = [0 0 1;  0 1 1];
a=0.000001; b=0.000003;
r = a + (b-a).*rand(TrialIndex,1);
ResponseHR = 100*cat(1,exptparams.Performance(1:end-1).RecentHitRate)-r;
subplot(3,6,1:4)
hold off;
plot(smooth(ResponseHR,exptparams.TrialBlock),'LineWidth',2,'color',c(1,:));
hold on;
r = a + (b-a).*rand(TrialIndex,1);
ResponseFR = 100*cat(1,exptparams.Performance(1:end-1).RecentFalseAlarmRate)-r;
subplot(3,6,1:4)
plot(smooth(ResponseFR,exptparams.TrialBlock),'LineWidth',2,'color',c(2,:));
text(TrialIndex,115,['[ ' exptparams.Performance(end-1).ThisTrial(1) ' ]'],'Color', 'w', ...
     'BackgroundColor', 'k', ...
     'HorizontalAlignment', 'Center','fontsize',8)
h=legend({'HR','FR'},'Location','NorthWest');
axis ([0 (TrialIndex+1) 0 140]);
set(gca,'FontSize',8)
title(titleMes,'FontWeight','bold','interpreter','none','fontsize',8);
set(h,'fontsize',6);
xlabel('Trial Number','Fontsize',8);
ylabel('Response Rate (%)')
xlim([0 TrialIndex+1])

%% Response latency histograms
%First Response Histogram for target and nontarget
subplot(3,6,7:12);
hold off;
BinSize = 0.1;
t = 0:1/fs:StimEvents(3).StopTime-(1/fs);
FirstResponse=exptparams.FirstResponse;
TarRange=get(exptparams.TrialObject,'PrimaryHandle');
TarRange = get(TarRange,'TarRange');
tar=FirstResponse(ismember(FirstResponse(:,2),TarRange),1);
ProbeClass=get(exptparams.TrialObject,'ProbeClass');
PrimaryHandle = get(exptparams.TrialObject,'PrimaryHandle');
if ~strcmpi(ProbeClass,'none')
    Discrim = get(PrimaryHandle,'NumFreqRange')>1;
    if (strcmpi(ProbeClass,'Silence') && ~Discrim) || strcmpi(ProbeClass,'Ripples')
        nontar=FirstResponse(FirstResponse(:,2)==-1,1);
        probe=[];
    elseif strcmpi(ProbeClass,'Silence') && Discrim || ~strcmp(ProbeClass,'Silence')
        probe=FirstResponse(FirstResponse(:,2)==-1,1);
        nontar=FirstResponse(FirstResponse(:,2)==0,1);
    end
else
    nontar=FirstResponse(FirstResponse(:,2)==0,1);
    probe=[];
end
lstr=[];  %Legend string
if ~isempty(tar) && sum(~isnan(tar)) > 0
    [h2_Tar,xi] = ksdensity(tar,t,'function','pdf','bandwidth',.2);
    plot(100*(h2_Tar./sum(h2_Tar)),'color',[1 0 0],'linewidth',2);
    hold on;
    lstr='Hit ';
    aa=axis;
    text(aa(2)-1000,.75.*aa(4),['n=' num2str(size(exptparams.AverageResponse.tar,1))],'color',[1 0 0],'fontsize',8);
else
    h2=hist([],t);
end
if ~isempty(nontar) && sum(~isnan(nontar)) > 0
    [h2_NonTar,xi] = ksdensity(nontar,t,'function','pdf','bandwidth',.2);
    plot(100*(h2_NonTar./sum(h2_NonTar)),'color',[.1 .5 .1],'linewidth',2);
    hold on;
    lstr=[lstr;'Fa  '];
    aa=axis;
    text(aa(2)-1000,.5.*aa(4),['n=' num2str(size(exptparams.AverageResponse.nontar,1))],'color',[.1 .5 .1],'fontsize',8);
else
    h1=hist([],t);
end
if ~isempty(probe) && sum(~isnan(probe)) > 0
    [h2_Probe,xi] = ksdensity(probe,t,'function','pdf','bandwidth',.2);
    plot(100*(h2_Probe./sum(h2_Probe)),'color',[.1 .5 .5],'linewidth',2);
    hold on;
    lstr=[lstr;'pHit'];
    aa=axis;
    text(aa(2)-1000,.25.*aa(4),['n=' num2str(size(exptparams.AverageResponse.probe,1))],'color',[1 .5 .5],'fontsize',8);
else
    h1=hist([],t);
end
h=legend(lstr,'AutoUpdate','off','location','northwest');
set(h,'fontsize',6);
EarlyWin = get(o,'EarlyWindow')*fs;
for cnt1 = 1:length(StimEvents)
    [Type, StimName, StimNonTarOrTar] = ParseStimEvent (StimEvents(cnt1));
    if strcmpi(Type,'Stim')
        stilen(1) = fs*get(PrimaryHandle,'PreStimSilence');
        stilen(2) = fs*get(PrimaryHandle,'PreStimSilence') + fs*get(PrimaryHandle,'Duration');
        stilen(3) = fs*get(PrimaryHandle,'PreStimSilence') + fs*get(PrimaryHandle,'Duration') + fs*get(PrimaryHandle,'PostStimSilence');
        ResponseWin= get(o,'ResponseWindow')*fs;
        EarlyWin = stilen(1)+[EarlyWin(1) EarlyWin(2)];
    end
end
plot(stilen([1 1 2 2]),[0 0.6 0.6 0]*max(ylim),'--','color','b','LineWidth',2);
if sum(abs(get(o,'EarlyWindow'))) > 0
    set(addgrid(0,EarlyWin,'c','--'),'linewidth',2);
end
set(addgrid(0,ResponseWin,'g','--'),'linewidth',2);
xlim([0 exptparams.LogDuration*fs])
set(gca,'XTickLabel',get(gca,'Xtick')./fs); %Convert to seconds
set(gca,'FontSize',8)
set(gca,'YTickLabel',get(gca,'Ytick').*100); %Convert to seconds
xlabel('Responce Latency (s)','Fontsize',8);
ylabel('Lick Prob. (%)')

%% Average response traces
subplot(3,6,13:18);
cla;
box on;
smoothfact=50;
NonTarAvgResp=smooth(nanmean(exptparams.AverageResponse.nontar,1),smoothfact);
TarAvgResp=smooth(nanmean(exptparams.AverageResponse.tar,1),smoothfact);
plot(NonTarAvgResp,'color',[.1 .5 .1],'LineWidth',2);
hold on;
plot(TarAvgResp,'color',[1 0 0],'LineWidth',2);;
mx=max([NonTarAvgResp; TarAvgResp]);
if ~isempty(exptparams.AverageResponse.probe)
    ProbeAvgResp=smooth(nanmean(exptparams.AverageResponse.probe,1),smoothfact);
    pl=max([ProbeAvgResp]);
    plot(ProbeAvgResp,'color',[1 .5 .5],'LineWidth',2);
    mx=max([mx pl]);
end
if sum(isnan(mx))>0 || sum(mx)==0
    mx=1;
end
set(gca,'XTickLabel',get(gca,'Xtick')); %Convert to seconds
plot(stilen([1 1 2 2]),[0 0.6 0.6 0]*max(ylim),'--','color','b','LineWidth',2);
aa=axis;
if sum(abs(get(o,'EarlyWindow'))) > 0
    set(addgrid(0,EarlyWin,'c','--'),'linewidth',2);
end
set(addgrid(0,ResponseWin,'g','--'),'linewidth',2);
set(gca,'fontsize',8);
set(gca,'YTickLabel',get(gca,'Ytick').*100); %Convert to seconds
ylabel('Lick Rate (%)')
xlabel('Time (s)','fontsize',8);
set(gca,'XTickLabel',get(gca,'Xtick')./fs); %Convert to seconds

%% And last, show the hit and false alarm for each position:
subplot(3,6,5:6);
hold off;
%Number of stimuli
stimnum=get(exptparams.TrialObject,'PriIndices');
%Remove probes
stimnum(stimnum(:,2)==-1,:)=[];
stimnum=unique(stimnum(:,1));
%Targets
hh0=hist(FirstResponse(FirstResponse(:,2)>=0,3),stimnum);
hh0(hh0==0)=1;
hh=hist(FirstResponse(FirstResponse(:,2)>=0 & FirstResponse(:,4)==1,3),stimnum);
set(plot(100*(hh(:)./hh0(:)),'o-','color',[.1 .5 .1]),'linewidth',2,'markersize',5,'markerfacecolor','w','markeredgecolor',[.1 .5 .1]);
axis([0 stimnum(end)+1 0 100]);
hold on
%Non-Targets
%Number of stimuli
stimnum=get(exptparams.TrialObject,'PriIndices');
%Remove probes & targets
stimnum(stimnum(:,2)<1,:)=[];
stimnum=unique(stimnum(:,1));
hh=hh(:);
hh0=hh0(:);
set(plot(stimnum,100*(hh(stimnum)./hh0(stimnum)),'ro-'),'linewidth',2,'markersize',5,'markerfacecolor','r','markeredgecolor','r');
xtl=1:stimnum(end);
if ~isempty(FirstResponse(FirstResponse(:,2)==-1,3)) %Add probe performance
    probenum=get(exptparams.TrialObject,'Probe');
    probenum=probenum(2:end);
    hh1=hist(FirstResponse(FirstResponse(:,2)==-1,3),probenum);  %Total probe trial
    hh1(hh1==0)=1;
    hh=hist(FirstResponse(FirstResponse(:,2)==-1 & FirstResponse(:,4)==1,3),probenum);  %Response during respwin
    hold on;
    set(plot([1:length(probenum)]+stimnum(end),100*(hh(:)./hh1(:)),'o-'),'linewidth',2,'markersize',5,'markerfacecolor','w','markeredgecolor','g');
    addgrid(0,stimnum(end)+0.5,'m');
    set(gca,'xlim',[0 stimnum(end)+length(probenum)+1]);
    xtl=[xtl probenum];
    xlabel('          Primary Set      ----------       Probe Set','fontsize',6);
else
    xlabel('Primary Set','fontsize',6);
end
ht=title('Avg. Response Rate','fontsize',8,'FontWeight','normal');
aa=axis;
set(gca,'xtick',1:length(xtl),'xticklabel',xtl);
set(gca,'FontSize',8)
drawnow;
ylim([0 130])
set(gca,'ytick',[])

function Exit(source,callbackdata)
global StopExperiment;
StopExperiment = 1;
