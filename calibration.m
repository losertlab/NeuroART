load('DMD_coordinates.mat')
radius = 3;
t = 500;
T = 2000;
niter = 5;
nTargets = 1;
target_cells_group2_DMD = [];
testMode = 0;
background = 0;
stimGroup = 1;

commandStr = sprintf('python dmdControl_calibration.py "%s" "%s" %d %d %d %d %d %d %d %d',jsonencode(target_cells_DMD),jsonencode(target_cells_group2_DMD),nTargets,radius,t,T,niter,testMode,background,stimGroup);
[status,cmdOutput] = system(commandStr);