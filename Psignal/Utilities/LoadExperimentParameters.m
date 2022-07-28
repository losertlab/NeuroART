function params = LoadExperimentParameters(matfilename)
%This program load the parameters for a given experiment.
load(matfilename)
%Trialobject parameters
params.TrialObject = get(exptparams.TrialObject);
%Primaryobject parameters
params.PrimaryObject = get(get(exptparams.TrialObject,'PrimaryHandle'));
%Probeobject parameters
params.ProbeObject = get(get(exptparams.TrialObject,'ProbeHandle'));
%Behaveobject parameters
params.ProbeObject = get(exptparams.BehaveObject);

