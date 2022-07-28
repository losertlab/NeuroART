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
%Assign targets (flag=1) and nontargets (flag=0)
pri = get(o,'PrimaryHandle');
o=set(o,'PrimaryClass',class(pri));
LowFrequency = ifstr2num(get(pri,'LowFrequency'));
HighFrequency = ifstr2num(get(pri,'HighFrequency'));
TonesPerOctave = ifstr2num(get(pri,'TonesPerOctave'));
Octaves = log2(HighFrequency/LowFrequency);
Freq = LowFrequency*2.^[0:1/TonesPerOctave:Octaves];
Freq=round(Freq(:));
tonenum=length(Freq);
primax=tonenum;
tindex = gettindex(primax, pri, tonenum);
tindex=repmat(tindex,get(o,'TrialsPerFreq'),1);
%Randomize order
ridx = randsample(size(tindex,1),size(tindex,1),'false');
tindex = tindex(ridx,:);
o = set(o,'NumberOfTrials',size(tindex,1));
o=set(o,'TrialIndices',tindex);
o=set(o,'PriIndices',tindex);
if nargin==1
    exptparams=o;
else
    exptparams.TrialObject= o;
end

function tindex=gettindex(primax, pri,tonenum);
tindex=[1:primax]';
tindex=[tindex(:) tindex(:)];
numrange=get(pri,'NumFreqRange');
tar=get(pri,'TarRange');
%Frequency range is divided in to NumFreqRange equally spaced sections, and TarRange is selected for targets.
tindex(:,2)=2-mod(ceil(tindex(:,2)/(tonenum/numrange)),2);
tindex(tindex(:,2)~=tar,2)=0;   %0 for non-target trials
%Attenuations
Atten = get(pri,'AttenRange');
primax = size(tindex,1);
Attenlen=length(Atten);
repTidx = repmat(tindex,Attenlen,1);
[junk I] = sort(repTidx(:,1),1);
tindex = repTidx(I,:);
Atten = repmat(Atten',primax,1);
tindex = [tindex Atten];

