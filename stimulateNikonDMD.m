function [success,cmdOutput] = stimulateNikonDMD(target_cells,target_cells_group2,stimParams)

% target_cells = coordinates of target cells assigned to group 1
% target_cells_group2 = coordinates of target cells assigned to group 2
% coordinates are provided as nx2 arrays (n = number of targets)
% stimParams: a MATLAB struct with the following parameters
% stimParams.seqMode: indicates whether stimulation happens in a sequence
% stimParams.testMode: set this to true whenever testing a trained network (e.g. group1, group2) 
% stimParams.radius: pixel radius of circular spots used for stimulation
% stimParams.pulseDuration: duration of a stimulation pulse (in ms)
% stimParams.interval: interval between stimulation events (in ms)
% stimParams.niter: number of repetitions of the photostimulation paradigm
% stimParams.background: background percentage 0 - 100 (if background = 20, 20% of the pixels will be turned on randomly)
% stimParams.stimGroup: id of the group that needs to be stimulated (testmode only)

success = 0;

% Use the avialable calibration files to convert x, y coordinates to DMD coodinates

target_cells_DMD = CamToDMD(target_cells(:,1),target_cells(:,2));
target_cells_group2_DMD = [];

if size(target_cells_group2,1) > 0 % there are two groups specified, only allow sequential stim if this is the case

    target_cells_group2_DMD = CamToDMD(target_cells_group2(:,1),target_cells_group2(:,2)); % do this conversion only if the second group has at least one cell
    nTargets = 2; % because there are only two groups, stimulate all the cells in group 1 and then all the cells in group 2

elseif stimParams.seqMode % only group 1 is available, stimulate cells sequentially

    nTargets = size(target_cells,1);

else % only group 1 is available, stimulate cells simultaneously

    nTargets = 1;

end

% disp(target_cells_DMD)
% disp(target_cells_group2_DMD)

radius = stimParams.radius;
t = stimParams.pulseDuration;
T = stimParams.interval;
niter = stimParams.niter;
testMode = stimParams.testMode;
background = stimParams.background;
stimGroup = stimParams.stimGroup;

if (T < nTargets*t)
    disp("Interval between photostimulation events should be greater than the total time taken for individual stimulations within the photostimulation event")
    return
end

commandStr = sprintf('python dmdControl.py "%s" "%s" %d %d %d %d %d %d %d %d',jsonencode(target_cells_DMD),jsonencode(target_cells_group2_DMD),nTargets,radius,t,T,niter,testMode,background,stimGroup); % need to use jsonencode to format matrices to string
[status,cmdOutput] = system(commandStr);

if status ~= 0
    fprintf('Error calling dmdControl.py: %s\n',cmdOutput)
else
    fprintf('Success, stimulation completed \n');
    success = 1;
end



end