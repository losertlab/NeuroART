function [exptparams] = RandomizeSequence (o, exptparams, globalparams, repetition, RepOrTrial)
%Randomizing trial sequence for TrialObject.
if nargin<5
    RepOrTrial = 1; % default is its a trial call
end
% trial call, return:
if ~RepOrTrial
    return;
end
o=ObjUpdate(o);
%Probe Sound: Reference
pro = get(o,'ProbeHandle');
%generate exponential distribution of number of probes
RandStim = get(o,'Probe');
w = exppdf(RandStim,mean(RandStim));
NumRef=randsample(RandStim,1000,'true',w)';
RefNumTemp = [];
ReferenceMaxIndex = get(pro,'MaxIndex');
while sum(RefNumTemp) < ReferenceMaxIndex
    if isempty(NumRef)
        RefNumTemp = [RefNumTemp ReferenceMaxIndex-sum(RefNumTemp)];
    elseif sum(RefNumTemp)+NumRef(1) <= ReferenceMaxIndex
        RefNumTemp = [RefNumTemp NumRef(1)];
        NumRef = circshift (NumRef, [-1 0]);
    else
        NumRef(1)=[];
    end
end
TotalTrials = length(RefNumTemp);
%pick torcs randomly, without replacment, from possible torcs
RandIndex = randperm(ReferenceMaxIndex);
for cnt1=1:length(RefNumTemp)
    RefTrialIndex {cnt1} = RandIndex (1:RefNumTemp(cnt1));
    RandIndex (1:RefNumTemp(cnt1)) = [];
end
%Primary Sound: Target
pri = get(o,'PrimaryHandle');
o=set(o,'PrimaryClass',class(pri));
switch get(o,'PrimaryClass')
    case 'MultiRangeStim'
        LowFrequency = ifstr2num(get(pri,'LowFrequency'));
        HighFrequency = ifstr2num(get(pri,'HighFrequency'));
        TonesPerOctave = ifstr2num(get(pri,'TonesPerOctave'));
        Octaves = log2(HighFrequency/LowFrequency);
        Freq = LowFrequency*2.^[0:1/TonesPerOctave:Octaves];
        Freq=round(Freq(:));
        tonenum=length(Freq);
        primax=tonenum;
    case 'Silence'
        primax = 1;
        tonenum=1;
end
tindex = gettindex(primax, pri, tonenum);
if size(tindex,1) < TotalTrials
    tindex=repmat(tindex,[floor(TotalTrials/size(tindex,1)) 1]);
end
while size(tindex,1) < TotalTrials
    tindex = [tindex; tindex(randsample(length(tindex),1),:)];
end
o = set(o,'NumberOfTrials',size(tindex,1));
o=set(o,'TrialIndices',tindex);
o=set(o,'PriIndices',tindex);
o=set(o,'ProIndices',RefTrialIndex);
if nargin==1
    exptparams=o;
else
    exptparams.TrialObject= o;
end
function tindex=gettindex(primax, pri,tonenum)
tindex=[1:primax]';
tindex=[tindex(:) tindex(:)];
switch get(pri,'descriptor')
    case 'MultiRangeStim'
        numrange=get(pri,'NumFreqRange');
        tar=get(pri,'TarRange');
    case 'SpectrumShift'
        numrange=get(pri,'NumShifts');
        tar=get(pri,'TarShift');
    case 'Silence'
        numrange = 1;
        tar=1;
end
%Frequency range is divided in to NumFreqRange equally spaced sections, and TarRange is selected for targets.
tindex(:,2)=2-mod(ceil(tindex(:,2)/(tonenum/numrange)),2);
tindex(tindex(:,2)~=tar,2)=0;   %0 for non-target trials
%Attenuations
if isfield(pri,'AttenRange')
    Atten = get(pri,'AttenRange');
    primax = size(tindex,1);
    Attenlen=length(Atten);
    repTidx = repmat(tindex,Attenlen,1);
    [junk I] = sort(repTidx(:,1),1);
    tindex = repTidx(I,:);
    Atten = repmat(Atten',primax,1);
    tindex = [tindex Atten];
else
    tindex = [tindex repmat(0,[size(tindex,1) 1])];
end
function [tindex,ok]=no_more_than3(tindex,N,lm);
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




