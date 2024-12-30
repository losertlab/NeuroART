function responses = findStimulatableNeurons(target_cellIDs,dff,stimDFF)
% based on df/f, evaluate which neurons responded to photostimulation

%% Input variables 

% target_cellIDs: nx1, cell ids of the targeted cells during photostimulation (range: 1:N), n = number of cells targeted, N = total number of cells
% dff: NxT, DF/F traces upto current frame. T = total number of frames acquired so far
% stimDFF: Nxt, DF/F traces corresponding to the duration of photostimulation, t = number of frames acquired during photostimulation

%%

nCells = length(target_cellIDs); % number of cells targeted
responses = zeros(nCells,1); % response = 1 if responded(i.e. generated a spike), response = 0 otherwise

for itr = 1:nCells
    cellID = target_cellIDs(itr);
    overallDFF = dff(cellID);
    postStimulationDFF = stimDFF(cellID);

    if max(postStimulationDFF) > (mean(overallDFF) + 2*std(overallDFF)) % criteria for detection of a spike (could be customized based on the experiment)
        responses(itr) = 1;
    end

end


end