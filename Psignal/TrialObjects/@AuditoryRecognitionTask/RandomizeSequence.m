function [exptparams] = RandomizeSequence (o, exptparams, globalparams, repetition, RepOrTrial)
global StopExperiment

%Randomizing trial sequence for TrialObject.
if nargin<5
    RepOrTrial = 1; % default is its a trial call
end

% trial call, return:
if ~RepOrTrial
    return;
end
o=ObjUpdate(o);

%Assign targets (flag=1) and nontargets (flag=0)
pri = get(o,'PrimaryHandle');
o=set(o,'PrimaryClass',class(pri));
LowFrequency = ifstr2num(get(pri,'LowFrequency'));
HighFrequency = ifstr2num(get(pri,'HighFrequency'));
TonesPerOctave = ifstr2num(get(pri,'TonesPerOctave'));
Octaves = log2(HighFrequency/LowFrequency);
Freq = LowFrequency*2.^[0:1/TonesPerOctave:Octaves];
if TonesPerOctave > 0
    Freq = LowFrequency*2.^[0:1/TonesPerOctave:Octaves];
else
    Freq = [LowFrequency HighFrequency];
end
Freq=unique(round(Freq(:)));
tonenum=length(Freq);
primax=tonenum;
tindex = gettindex(primax, pri, tonenum);
switch get(o.PrimaryHandle,'Type')
    case 'PitchStim'
        %Make index x5 for all stim types (IRN, AM, Clicks, Harm, MissFund)
        tindex=repmat(tindex,[5 1]);
    case 'WhiteNoise'
        tindex(:,1) = 1;
end
if size(tindex,1) < exptparams.TrialBlock
    tindex=repmat(tindex,[ceil(exptparams.TrialBlock/size(tindex,1)) 1]);
end
o = set(o,'NumberOfTrials',size(tindex,1));
%Assign probes (flag=-1)
pro = get(o,'ProbeHandle');
plist=[];
if ~isempty(pro)
    if strcmpi(get(o,'ProbeClass'),'Ripples')
        ReferenceMaxIndex = get(pro,'MaxIndex');
        RefNumTemp = ones(1,ReferenceMaxIndex);
        TotalTrials = length(RefNumTemp);
        %pick torcs randomly, without replacment, from possible torcs
        RandIndex = randperm(ReferenceMaxIndex);
        for cnt1=1:length(RefNumTemp)
            RefTrialIndex {cnt1} = RandIndex (1:RefNumTemp(cnt1));
            RandIndex (1:RefNumTemp(cnt1)) = [];
        end
        plist=cell2mat(RefTrialIndex)';
        tindex=repmat(tindex,[ceil(length(plist)/size(tindex,1)) 1]);
    else
        probe=get(o,'Probe');
        primax=get(o,'NumberOfTrials');
        ProbeNum=ceil(primax*probe(1));
        Num=length(probe)-1;
        if ~Num
            warning('***Probe settings incorrect: assming [1/3 1]***')
            probe = [1/3 1];
            ProbeNum=ceil(primax*probe(1));
            Num=length(probe)-1;
            o=set(o,'Probe',probe);
        end
        plist=rem(randperm(ceil(ProbeNum/Num)*Num),Num)+1;
        plist=probe(plist(1:ProbeNum)+1)';
    end
    plist(:,2)=-1;    %-1 indicate a probe
    if get(pro,'NonTarget')
        plist(:,2)=0;    %0 indicate a nontarget
    end
    plist(:,3)=0;    %silence,so no attentuation
    plist(:,4)=get(pro,'Duration');%s
end
tindex = [tindex;plist];
tindex=tindex(randperm(size(tindex,1)),:);
ok1=0;
ok2=0;
ok3=0;
if ~isempty(find(tindex(:,2)==-1))
    while tindex(1,2)==-1 || ~ok1 || ~ok2 || ~ok3
        tindex=tindex(randperm(size(tindex,1)),:); %First trial is always target
        [tindex,ok1]=no_more_than3(tindex,1,3);
        if ~ok1,
            [tindex,ok1]=no_more_than3(flipud(tindex),0,2); %no more than 2 consecutive non targets
        end
        [tindex,ok2]=no_more_than3(tindex,0,3);
        if ~ok2,
            TarRange=get(pri,'TarRange');
            [tindex,ok2]=no_more_than3(flipud(tindex),TarRange,3); %no more than 3 consecutive targets
        end
        [tindex,ok3]=no_more_than3(tindex,-1,2);
        if ~ok3,
            [tindex,ok3]=no_more_than3(flipud(tindex),-1,2);%no more than 2 consecutive probes
        end
    end
else
    while tindex(1,2)==-1 || ~ok1 || ~ok2
        tindex=tindex(randperm(size(tindex,1)),:); %First trial is always target
        [tindex,ok1]=no_more_than3(tindex,1,3);
        if ~ok1,
            [tindex,ok1]=no_more_than3(flipud(tindex),0,3); %no more than 3 consecutive non targets
        end
        [tindex,ok2]=no_more_than3(tindex,0,3);
        if ~ok2,
            TarRange=get(pri,'TarRange');
            [tindex,ok2]=no_more_than3(flipud(tindex),TarRange,3); %no more than 3 consecutive targets
        end
    end
end
o = set(o,'NumberOfTrials',size(tindex,1));
o=set(o,'TrialIndices',tindex);
o=set(o,'PriIndices',tindex);
if nargin==1
    exptparams=o;
else
    exptparams.TrialObject= o;
end

%%%%%%% Local Functions %%%%%%%
function tindex=gettindex(primax, pri,tonenum)
tindex=[1:primax]';
tindex=[tindex(:) tindex(:)];
switch get(pri,'descriptor')
    case 'Sounds'
        numrange=get(pri,'NumFreqRange');
        tar=get(pri,'TarRange');
end
%Frequency range is divided in to NumFreqRange equally spaced sections, and TarRange is selected for targets.
tindex(:,2)=2-mod(ceil(tindex(:,2)/(tonenum/numrange)),2);
tindex(~ismember(tindex(:,2),tar),2)=0;   %0 for non-target trials
%Attenuations
Atten = get(pri,'AttenRange');
primax = size(tindex,1);
Attenlen=length(Atten);
repTidx = repmat(tindex,Attenlen,1);
[junk I] = sort(repTidx(:,1),1);
tindex = repTidx(I,:);
Atten = repmat(Atten',primax,1);
tindex = [tindex Atten];
%Durations
Duration = get(pri,'Duration');
primax = size(tindex,1);
Durlen=length(Duration);
repTidx = repmat(tindex,Durlen,1);
[junk I] = sort(repTidx(:,1),1);
tindex = repTidx(I,:);
Duration = repmat(Duration',primax,1);
tindex = [tindex Duration];
if strfind(get(pri,'Type'),'PitchStim')
    %PitchStims
    primax = size(tindex,1);
    NumStim=5;
    repTidx = repmat(tindex,NumStim,1);
    [junk I] = sort(repTidx(:,1),1);
    tindex = repTidx(I,:);
    PitchStim = repmat([1:NumStim]',primax,1);
    tindex = [tindex PitchStim];
end
function [tindex,ok]=no_more_than3(tindex,N,lm)
temp=0;
ok=1;
for i=1:size(tindex,1)
    thistrial=tindex(i,:);
    if tindex(i,2)==N
        temp=temp+1;
    else
        temp=0;
    end
    if temp>lm
        n=find(tindex(i+1:end,2)~=N);
        if ~isempty(n)
            tindex(i,:)=tindex(n(1)+i,:);
            tindex(n(1)+i,:)=thistrial;
            temp=0;
        else
            ok=0;
            break;
        end
    end
end




