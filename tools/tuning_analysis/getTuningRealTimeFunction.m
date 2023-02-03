function tuningInfo = getTuningRealTimeFunction(dff,exptVars,psignalFile,thorSyncFile)
% dff should be ROIs x Frames
% psignalFile and thorsyncfile should include filepath
%% General stimulus info
% t1 = tic;

stimInfo =  WF_getPsignalInfo_1(psignalFile); % Parse Psignal file
nstims = [numel(stimInfo.uFreqs) numel(stimInfo.uLevels)];
kHzVals = str2double(stimInfo.uFreqs)/1000; % may be useful for displaying in RT window
% repOrdIdx = stimInfo.Trialindicies(:,1); % repOrdIdx is the order that stimuli were played trial-to-trial
dbInds = (stimInfo.Levels-min(stimInfo.Levels)) / 10;
% freqInds = round(2*log2(stimInfo.Freqs/min(stimInfo.Freqs)))+1;
% repOrdIdx = freqInds + (dbInds*nstims(1)); % had to modify for psignal
repOrdIdx = stimInfo.Trialindicies(:,1) + (dbInds*nstims(1));
numTotalTrials = length(stimInfo.Trialindicies);
numStims = numel(stimInfo.uFreqs)*numel(stimInfo.uLevels);
numTrials = numTotalTrials/numStims;

% toc(t1)
% t1 = tic;

%% Experimental timing stuff
% Read thorsync
% thorSyncFile = '\\vault3\data\etc\etc\etc.h5';
Frame_Out = h5read(thorSyncFile,'/DI/Frame Out');
PsignalGate = h5read(thorSyncFile,'/AI/PsignalGate');

% toc(t1)
% t1 = tic;

% Find which frame each trial begins
PsignalGate = PsignalGate/max(PsignalGate);
Frame_Out = Frame_Out/max(Frame_Out);

% toc(t1)
% t1 = tic;

th = 0.95; % the threshold to detect pulses
ai2_crossing = find(PsignalGate(1:end-1)<th & PsignalGate(2:end)>=th);
Frame_Out_crossing = find(Frame_Out(1:end-1)<th & Frame_Out(2:end)>=th);

% toc(t1)
% t1 = tic;

% for each ai2 crossing, find the closest Frame_Out
trialStartVals = zeros(length(ai2_crossing),1);

for j = 1:length(ai2_crossing)
    dist = (Frame_Out_crossing - ai2_crossing(j)).^2;
    [~,trialStartVals(j)] = min(dist);
end

% toc(t1)
% t1 = tic;

% Assign timing vars
analysisWinSeconds = 1; % in Seconds
analysisWinFrames = round(analysisWinSeconds*exptVars.frameRate);
stimFrame = trialStartVals + round(stimInfo.PreStimSilence*exptVars.frameRate);

numCells = size(dff,1);

%% Parse each trial's DFF and compute FRAs
[baseDFF,stimDFF] = deal(NaN(numStims,numTrials,numCells,analysisWinFrames));
flatFRA = NaN(numCells,numStims);
% flatTrialFRA = NaN(numCells,numStims,numTrials);
for trial = 1:length(repOrdIdx)
    
    if (stimFrame(trial) + analysisWinFrames - 1) > size(dff,2)
%         stim = repOrdIdx(trial);
%         for c = 1:numCells
%             flatFRA(c,stim) = 0; % cells x unique stimuli (vector)
%             flatTrialFRA(c,stim,:) = 0; % cells x unique stim x trial mean
%         end
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
%         flatTrialFRA(c,stim,:) = trialMean_stimDFF; % cells x unique stim x trial mean
    end
    
end

a = reshape(flatFRA,size(flatFRA,1),nstims(1),nstims(2));
FRA = permute(a,[1 3 2]);

% toc(t1)
% t1 = tic;
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

% toc(t1)
% t1 = tic;

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