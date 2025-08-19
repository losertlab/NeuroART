%% Run this section soon after analyzing the movie of initial global stimulation

DFFraw = DFF.raw;
DFFraw = DFFraw(:,101:end); % omit first 100 frames
DFFraw = DFFraw/100; % convert DFF values from percentages to [0,1] range


%% Detect cells that respond to photostimulation

out = detectRespondingCells(DFFraw, 'Fs', 14.3, 'TargetFreq', 0.5, 'FreqTol', 0.05, 'BaselineMethod', 'movmedian', 'BaselineWinSec', 5, 'SubtractOnly', true, 'MinSNRdB', 6, 'MinAC', 0.2, 'DoPlots', false);

%% Select cells for collective learning experiments

% out.isResponder = ones(236,1); % use this if you want to look at all the cells
ui_view_responders(normMeanIMG, DFFraw, [cell_centroids(:,2),cell_centroids(:,1)], out, 14.3);
