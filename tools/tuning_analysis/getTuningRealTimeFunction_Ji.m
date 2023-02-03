function tuningInfo = getTuningRealTimeFunction_Ji(dff,exptVars,preset_stim_seq)
% dff should be ROIs x Frames
% preset_stim_seq is a struct which contains information about the experiment
%% General stimulus info

kHzVals = unique(preset_stim_seq.freqSeq); % frequency values in kHz
uLevels = unique(preset_stim_seq.attens); % sound pressure levels
nstims = [numel(kHzVals) numel(uLevels)];

dbInds = (preset_stim_seq.attens) / 10;

repOrdIdx = preset_stim_seq.stimSeq + (dbInds*nstims(1));
numTotalTrials = length(preset_stim_seq.stimSeq);
numStims = numel(kHzVals)*numel(uLevels);
numTrials = numTotalTrials/numStims;

%% Experimental timing stuff

% Find which frame each trial begins
trialStartVals = preset_stim_seq.stimOnFrame;

% Assign timing vars
analysisWinSeconds = 1; % in Seconds
analysisWinFrames = round(analysisWinSeconds*exptVars.frameRate);

% stimFrame = trialStartVals + round(stimInfo.PreStimSilence*exptVars.frameRate);
stimFrame = trialStartVals; % Not sure how this applies to Ji signal

numCells = size(dff,1);

%% Parse each trial's DFF and compute FRAs
[baseDFF,stimDFF] = deal(NaN(numStims,numTrials,numCells,analysisWinFrames));
flatFRA = NaN(numCells,numStims);
% flatTrialFRA = NaN(numCells,numStims,numTrials);
for trial = 1:length(repOrdIdx)
    
    if (stimFrame(trial) + analysisWinFrames - 1) > size(dff,2)
        continue;
    end
    
    stim = repOrdIdx(trial);
    
    stimTrials = find(repOrdIdx==stim); % find all trials corresponding to this stim
    trialInd = find(stimTrials==trial); % find which repetition # this trial is
    
    % Frame windows for parts of this trial:
    
%     baseWindow = (stimFrame(trial) - analysisWinFrames) : (stimFrame(trial) - 1);
    stimWindow = (stimFrame(trial))                         : (stimFrame(trial) + analysisWinFrames - 1);

%     baseDFF(stim,trialInd,:,:) = dff(:,baseWindow);
    stimDFF(stim,trialInd,:,:) = dff(:,stimWindow);
    
    for c = 1:numCells
        trialMean_stimDFF = mean(stimDFF(stim,:,c,:),4)';
        flatFRA(c,stim) = mean(trialMean_stimDFF,'omitnan'); % cells x unique stimuli (vector)
    end
    
end

a = reshape(flatFRA,size(flatFRA,1),nstims(1),nstims(2));
FRA = permute(a,[1 3 2]);

%% Find best frequency (BF)
[BFval,BL,BFresp] = deal(NaN(numCells,1));
for c = 1:numCells
    % Calculate best frequency (BF)
    [tmpResp , tmpBF ] = max( flatFRA(c,:) );
    BFval(c) = mod(tmpBF,nstims(1));  % Preallocate
    if mod(tmpBF,nstims(1))==0
        BFval(c) = nstims(1);
    end
%     BL(c) = ceil(tmpBF/nstims(1));
%     BFresp(c) = tmpResp;
    
end

% Sort the kHzVals and adjust BFval accordingly

[sorted_kHz,I] = sort(kHzVals);

for c = 1:numCells
    BFval(c) = find(I == BFval(c));
end

%% Organize the outputs
tuningInfo.BFval = BFval;
% tuningInfo.BFresp = BFresp;
% tuningInfo.BL = BL;
tuningInfo.fraVals = FRA;
tuningInfo.flatFRA = flatFRA;
% tuningInfo.flatTrialFRA = flatTrialFRA;
tuningInfo.kHzVals = sorted_kHz;
tuningInfo.trialStartVals = trialStartVals;

end