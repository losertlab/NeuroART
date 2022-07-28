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
tindex0=ones(get(o,'NumberOfTrials'),2);
o=set(o,'PriIndices',tindex0);
o=set(o,'TrialIndices',tindex0);
if nargin==1
    exptparams=o;
else
    exptparams.TrialObject= o;
end