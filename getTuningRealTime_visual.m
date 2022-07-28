function tuningInfo = getTuningRealTime_visual(dff,exptVars,trial_list)
% dff should be ROIs x Frames
% psignalFile and thorsyncfile should include filepath

% trial_list --> csv file containing the starting frame ids for each trial (as a row vector)

%% General stimulus info
% t1 = tic;

nstims = [8,1];
kHzVals = [0, 45, 90, 135, 180, 225, 270, 315];
A = readmatrix(trial_list); % returns 1x160 vector
repOrdIdx = (A'/45) + 1;  % index the orientations (1:8)    

numTotalTrials = length(A);
numStims = nstims(1)*nstims(2);

numTrials = numTotalTrials/numStims;

framesPerTrial = 240;
trialStartVals = (0:numTotalTrials-1)'*framesPerTrial + 1;
PreStimFrames = 60; % first 60 frames of each trial --> no stimulus
stimFrame = trialStartVals + PreStimFrames;

analysisWinSeconds = 2; % in Seconds 
analysisWinFrames = round(analysisWinSeconds*exptVars.frameRate); % 60 frames

numCells = size(dff,1);

%% Parse each trial's DFF and compute FRAs

stimDFF = NaN(numStims,numTrials,numCells,analysisWinFrames);
flatFRA = NaN(numCells,numStims);

for trial = 1:length(repOrdIdx)
    
    if (stimFrame(trial) + analysisWinFrames - 1) > size(dff,2)
        continue;
    end
    
    stim = repOrdIdx(trial);
    
    stimTrials = find(repOrdIdx==stim); % find all trials corresponding to this stim
    trialInd = find(stimTrials==trial); % find which repetition # this trial is
    
    % Frame windows for parts of this trial:
    
    stimWindow = (stimFrame(trial)) : (stimFrame(trial) + analysisWinFrames - 1);

    stimDFF(stim,trialInd,:,:) = dff(:,stimWindow);
    
    for c = 1:numCells
        trialMean_stimDFF = mean(stimDFF(stim,:,c,:),4)';
        flatFRA(c,stim) = mean(trialMean_stimDFF,'omitnan'); % cells x unique stimuli (vector)
    end
    
end

a = reshape(flatFRA,size(flatFRA,1),nstims(1),nstims(2));
FRA = permute(a,[1 3 2]);

%% Find best orientation (BF)

BFval = NaN(numCells,1);
for c = 1:numCells
    % Calculate best frequency (BF)
    [~ , tmpBF ] = max( flatFRA(c,:) );
    BFval(c) = mod(tmpBF,nstims(1));  % Preallocate
    if mod(tmpBF,nstims(1))==0
        BFval(c) = nstims(1);
    end
end

% Sort the kHzVals and adjust BFval accordingly

[sorted_kHz,I] = sort(kHzVals);

for c = 1:numCells
    BFval(c) = find(I == BFval(c));
end

%% Organize the outputs
tuningInfo.BFval = BFval;
tuningInfo.fraVals = FRA;
tuningInfo.flatFRA = flatFRA;
tuningInfo.kHzVals = sorted_kHz;
tuningInfo.trialStartVals = trialStartVals;

end