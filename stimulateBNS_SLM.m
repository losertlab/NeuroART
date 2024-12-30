function [phaseMask,msk,I] = stimulateBNS_SLM(targetCells, stimParams)

% targetCells: x, y coordinates of the cells to be stimulated
% (first column -> x, second column -> y)

% Created by Dulara De Zoysa, UMD on 5/13/2022

nTargets = size(targetCells,1); % Total number of cells to be stimulated
slmdim = 1536;
mult_factor = 1.25; % 1.5 for high pixel mode
zoom_factor = 3/4; % 1/6; % high pixel mode

% % Normalize
% targetCells(:,1) = (targetCells(:,1)-(dimX/2))/(dimX/2);
% targetCells(:,2) = (targetCells(:,2)-(dimY/2))/(dimY/2);

xoff = [];
yoff = [];
vm = [];
           
for itr = 1:nTargets
    xoff = [xoff; targetCells(itr,1)]; 
    yoff = [yoff; targetCells(itr,2)];
    vm = [vm, stimParams.VMode]; % vortex modes
end
            
opts=dhot_opts();
opts.vortex_modes= vm; 
opts.niters = stimParams.GS;
opts.lens = -1*stimParams.VLens;
opts.slm_dims = [1536 1536]*mult_factor; % high pixel mode
            
% load the calibration file and transform 2P image coordinates to DHOT coordinates which are sent to the SLM 
P = PMTToDHOT(xoff,yoff); 

tstart = tic;

if stimParams.VMode < 0 % consider negative values as disk shaped patterns, VMode=0 --> points, VMode>0 --> vortices
    c= Config.BNS(); % Calculations are done using EasySLM, based on otslm package
    sz= c.res;
    % I = otslm.simple.gaussian(sz, 750);  % Incident beam (gaussian)
    I = ones(c.res(1),c.res(2)); % uniform incident beam
    im = otslm.simple.aperture(sz, sz(1)/300, 'shape', 'circle'); % disk pattern

    slm= CGH( c, 'incident', I );
    disk = slm.cgh(im, 'alpha', 0.5, 'use_gpu', false, 'N', 10);
    slm= DHOT1(c);
    
    for itr = 1:size(P,1)
        slm.add_tweezer(P(itr,:),disk,0); % add disk pattern on top of each targeted cell
    end

    pattern = slm.dhot('alpha',1);
    m = pattern;

else
    [m, ~]=dhot(P, opts, 0);
end

opts.zernike = [0,0,0,0,0,0,0,0,0,0,0,0,0,0];
[~, IMG]=dhot(P, opts, 1);  % added on 08/18/22 (calculated intensity pattern shows artifacts when zernike coefficients are non-zero)

t = toc(tstart);
fprintf('Time taken for dhot calculations before updating the display is %.2f seconds \n',t);
            
xspm = stimParams.xShift;
yspm = stimParams.yShift;
            
% 05/09/21 Dulara moved this section to the GUI code for speeding up the calculations
m = add_virtualLens(m, opts);
            
ref1 = round(slmdim*(mult_factor - 1)/2);
ref2 = round(slmdim*(mult_factor + 1)/2);
msk = m(ref1 + 1 + xspm: ref2 + xspm, ref1 + 1 + yspm: ref2 + yspm); % crop the mask to the original size of the SLM
            
t = toc(tstart);
fprintf('Time taken until after updating the phase mask display in the GUI is %.2f seconds \n',t);

% imwrite(double(msk), 'currentMask.tif'); 
% phaseMask = imread('currentMask.tif'); % uint8

phaseMask = im2uint8(msk); % conversion to uint8
            
t = toc(tstart); 
fprintf('Total time taken for calculations before sending the phase mask to SLM is %.2f seconds \n',t);
            
            
low = 500; % lower threshold for grayscale display of the intensity
high = prctile(IMG(:),99.9999); % DZ 05/13/21 to have a more generalized upper threshold
if high < low
    low = 0;
end

IM = mat2gray(IMG,double([low,high])); % DZ 04/26/21 (set limits for better visualization)            
                
crop1 = mult_factor*slmdim*(0.5 - zoom_factor/2); % DZ 05/13/21 only show the middle region of the intensity plane for better visualization
crop2 = mult_factor*slmdim*(0.5 + zoom_factor/2);

I = IM(crop1 + 1 : crop2, crop1 + 1 : crop2); % only show 1/6 th of the intensity plane for better visualization
 
%% uncomment if needed for debugging
% imwrite(IM,'fullSizetheoriticalPattern.tif'); % DZ 01/21/22, 
% theoriticalIntensity = IM(1 :mult_factor*slmdim, 1 : mult_factor*slmdim); % double check
% imwrite(I, 'currentSimulation.tif'); 
% imwrite(theoriticalIntensity, 'theoriticalIntensity.tif');
%%
t = toc(tstart);
fprintf('Time taken until after updating the beamlet display in the GUI is %.2f seconds \n',t);

if ~(stimParams.seqMode)
    calllib(stimParams.dll_name, 'Write_overdrive_image', stimParams.board_number, phaseMask); % write to SLM
end
           
% t = toc(tstart);       
% fprintf('Total time taken until after sending the phase mask to SLM is %.2f seconds \n',t);
end