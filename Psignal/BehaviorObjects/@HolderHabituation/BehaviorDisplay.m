function exptparams = BehaviorDisplay (o, HW, StimEvents, globalparams, exptparams, TrialIndex, ResponseData, TrialSound)
%BehaviorDisplay method of holder habituiation behavior
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
subplot(3,4,1:4);
uicontrol('Style', 'pushbutton', 'String', 'Stop',...
    'Position', [10 10 50 25],...
    'Callback', @Exit);
%% Create the title:
titleMes = ['Rig: ' globalparams.Rig '     Animal: ' globalparams.Animal '     Primary: ' ...
    get(exptparams.TrialObject,'PrimaryClass') '     Probe: ' ...
    get(exptparams.TrialObject,'ProbeClass') '     ' ...
    num2str(exptparams.StartTime(1:3),'Date: %1.0f-%1.0f-%1.0f') '     ' ...
    num2str(exptparams.StartTime(4:6),'Start: %1.0f:%1.0f:%1.0f')];
if isfield(exptparams,'StopTime')
    titleMes = [titleMes '     ' num2str(exptparams.StopTime(4:6),'Stop: %1.0f:%1.0f:%1.0f')];
end
if isempty(ResponseData) && isempty(TrialSound)
    %If this is the end, things have been displayed already:
    subplot(3,4,1:4);
    title(titleMes,'interpreter','none','fontsize',8);
    drawnow;
    return;
end
%% Display Recent Hitrate.
c = [0 0 1;  0 1 1];
a=0.000001; b=0.000003;
r = a + (b-a).*rand(TrialIndex,1);
ResponseHR = 100*cat(1,exptparams.Performance(1:end-1).RecentHitRate)-r;
subplot(3,4,1:4)
hold off;
plot(smooth(ResponseHR,exptparams.TrialBlock),'LineWidth',2,'color',c(1,:));
hold on;
r = a + (b-a).*rand(TrialIndex,1);
text(TrialIndex,110,exptparams.Performance(end-1).ThisTrial)
h=legend({'HR'},'Location','NorthWest');
axis ([0 (TrialIndex+1) 0 130]);
text(TrialIndex,110,exptparams.Performance(end-1).ThisTrial)
set(gca,'FontSize',8)
title(titleMes,'FontWeight','bold','interpreter','none','fontsize',8);
LegPos = get(h,'position');
set(h,'fontsize',6);
LegPos(1) = 0.005; % put the legend on the far left of the screen
set(h,'position', LegPos);
xlabel('Trial Number','Fontsize',8);
%% Response latency histograms
%First Response Histogram for target and nontarget
subplot(3,6,7:12);
hold off;
BinSize = 0.1;
FirstResponse=exptparams.FirstResponse;
t=0:BinSize:exptparams.ResponseWin(2);
tar=FirstResponse(FirstResponse(:,2)==1,1);
ProbeClass=get(exptparams.TrialObject,'ProbeClass');
if ~strcmpi(ProbeClass,'none')
    PrimaryHandle = get(exptparams.TrialObject,'PrimaryHandle');
    Discrim = get(PrimaryHandle,'NumFreqRange')>1;
    if strcmpi(ProbeClass,'Silence') && ~Discrim
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
if ~isempty(tar)
    h2=hist(tar,t); %Target
    h2=h2/length(tar); %Normalize by sum, so it becomes the probability of Response
    stairs(t,h2,'color',[1 0 0],'linewidth',2);
    hold on;
    lstr='Hit ';
    aa=axis;
    text(aa(2)-.25,.75.*aa(4),['n=' num2str(size(exptparams.AverageResponse.tar,1))],'color',[1 0 0],'fontsize',8);
else
    h2=hist([],t);
end
ht=title('Response Latency Distibution','fontsize',8);
aa=axis;
set(ht,'Position',[0.5,.75.*aa(4)]);
h=legend(lstr);
LegPos = get(h,'position');
set(h,'fontsize',6);
LegPos(1) = 0.005;
set(h,'position', LegPos);
xlabel('Seconds (re. Stim. Onset)','Fontsize',8);
set(gca,'FontSize',8)
%% Average response traces
subplot(3,4,9:12);
cla;
box on;
tl=conv2(nanmean(exptparams.AverageResponse.tar,1),ones(1,50)/50,'same');
hold on;
set(plot(tl),'color',[1 0 0]);
mx=max([tl]);
if isnan(mx) || mx==0
    mx=1;
end
set(gca,'XTickLabel',get(gca,'Xtick')/fs); %Convert to seconds
set(gca,'xlim',[0 exptparams.LogDuration*fs], 'ylim', [0 mx]*1.2);
xlabel('Time (seconds)','fontsize',8);
for cnt1 = 1:length(StimEvents)
    [Type, StimName, StimNonTarOrTar] = ParseStimEvent (StimEvents(cnt1));
    if strcmpi(Type,'Stim')
        stilen=[StimEvents(cnt1).StartTime StimEvents(cnt1).StopTime];
    end
end
set(plot(stilen([1 1 2 2])*fs,[0 0.6 0.6 0]*max(ylim),'--'),'color','b','LineWidth',2);
ht=title('Average Response','fontsize',8);
aa=axis;
set(ht,'Position',[400,.75.*aa(4)]);
set(addgrid(0,fs*exptparams.ResponseWin,'g','--'),'linewidth',2);
set(gca,'fontsize',8);
drawnow;
function Exit(source,callbackdata)
global StopExperiment;
StopExperiment = 1;
