function [norm_meanIMG,roiBW2,npBWout,DFF,normF,npSubFluoSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_filled(RegImg,exptVars,xc,yc,expectedNeuronRadiusPix,winsize,percentBaselineSub)

narginchk(6,7)
if ~exist('winsize','var'); winsize = 300; end % default to 300 frame window
if ~exist('percentBaselineSub','var'); percentBaselineSub = 50; end % default to 50% baseline subtraction

total_start = tic;
numNeurons = length(xc);
meanIMG = mean(RegImg,3); % Mean image for cell center clicking
norm_meanIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));

%% PREALLOCATE
traceExtract_start = tic;

ROIxvOut = cell ( numNeurons , 360 , 1 );
ROIyvOut = cell ( numNeurons , 360 , 1 );
roiBW2 = cell ( numNeurons , size(RegImg,1) , size(RegImg,2));
npBWout = cell ( numNeurons , size(RegImg,1) , size(RegImg,2));

for pp = 1:length(xc)
    npBWout{pp} = ~ones(size(RegImg,1) , size(RegImg,2));  % matrix of all zeros (logical type)
end
%% PREALLOCATE FLUO AND NPFLUO MATRICES
rawFluo = zeros( size(RegImg,3) , numNeurons );

%% FIND THE BOUNDARIES OF CLICKED NEURONS
roiBounds = [deg2rad(1:360)' repmat(expectedNeuronRadiusPix,360,1)];
for pp = 1:numNeurons
    ROIxvOut{pp} =  xc(pp) + roiBounds(:,2) .* (cos(roiBounds(:,1))) ;
    ROIyvOut{pp} =  yc(pp) + roiBounds(:,2) .* (sin(roiBounds(:,1))) ;
    roiBW2{pp} = poly2mask( ROIxvOut{pp} , ROIyvOut{pp} , size(RegImg,1) , size(RegImg,2));
end

%DW 11232015 - adjusted for inclusion of neuropil correction
% correct for overlapping ROIs (exclude from both)
disp('Adjusting ROI masks for overlap....');
tStartROICorr = tic;
AllMasksTMP =  sum ( cat ( 3 , roiBW2{:} ) , 3 ); % first term of cat (i.e., '3') points to element-wise alignement/stacking of arrays
[oLapRoiY, oLapRoiX] = find( AllMasksTMP > 1 );
for ii = 1:numNeurons
    for yy = 1:length(oLapRoiX)
        roiBW2{ii}(oLapRoiY(yy),oLapRoiX(yy)) = 0;
    end
end

tElapsedROICorr = toc(tStartROICorr);
disp(['    Time elapsed for ROI mask Correction was ', num2str(tElapsedROICorr/60),' minutes']);

%% Extract fluorescence traces for each neuron
% Loop through each neuron to get somatic fluo and neuropil fluo
for nn = 1:numNeurons
    
    [r,c]=find(roiBW2{nn}~=0);
    tmpPixels = NaN(length(r),size(RegImg,3));
    for i = 1:length(r)
        tmpPixels(i,:) = RegImg(r(i),c(i),:);
    end
    rawFluo(:,nn) = nanmean(tmpPixels,1);
    
end

traceExtract_finish = toc(traceExtract_start);
fprintf('Trace extraction took %.1f minutes\n',traceExtract_finish/60)

npSubFluo = rawFluo; % No need for neuropil correction for in-vitro cells

%% apply assymetrical (exponential) filter to traces - smoothing - MODIFIED FROM PLENZ LAB AVALANCHE CODE
tStartDN = tic;
disp('Denoising fluorescence traces with Plenz Lab approach.....');
[nrois , ~] = size(npSubFluo');
T = 1/exptVars.frameRate;

% Normalizing raw F to get rid of negative vals before dF/F calc % 7/2020
minNpSubFluo = min(npSubFluo(:));
adjF = npSubFluo - minNpSubFluo;
maxAdjF = max(adjF(:));
normF = adjF ./ maxAdjF;

%smoothed raw dR/R or dF/F - commented out 9/16, uncommented 6/2020 for slower volume data
if T >= 0.2
    smoothwin = 3;
elseif T >= 0.13 && T <= 0.2
    smoothwin = 5; %7
elseif T <=0.13
    smoothwin = 9; %13
end
symmFLAG = 1; % 1 ->symmetric, 2->asym, T = frame time, Tau -> 1.5

for j = 1 : nrois % NEED FOR IMPROVEMENT - ELIMINATE FOR LOOP?
    
    %%%%% SYMMETRIC (new as of Sept2016)
    tmp = normF(:,j)';
    npSubFluoSmooth(:,j) = smooth2(tmp,symmFLAG,smoothwin,T,1.5); %Maybe smooth after DFF calc?
    
end

normF = normF'; % added by dulara on 05/24/2020

tElapsedDN = toc(tStartDN);
disp(['     Time elapsed for denoising was ', num2str(tElapsedDN/60) , ' minutes']);

percent = percentBaselineSub;
DFF.smooth = slideWinSub(npSubFluoSmooth',winsize,percent);
DFF.raw = slideWinSub(normF,winsize,percent);

%% clean up raw (remove bad ROIs)
clean_start = tic;

DFF.smoothClean = DFF.smooth((min(DFF.smooth')> 2*median(min(DFF.smooth))),:); % DW09232016 changed this to be data-based rather than hardcoded / ZB09302016 changed mean to median
xcRaw = xc;
ycRaw = yc;

% convert to sparse matrices for efficiency during real time analysis
n = size(roiBW2,1);
for nn = 1:n
    roiBW2{nn} = sparse(roiBW2{nn});
    npBWout{nn} = sparse(npBWout{nn});
end

clean_finish = toc(clean_start);
fprintf('Removing bad ROIs took %.1f minutes\n',clean_finish/60)

total_finish = toc(total_start);
fprintf('Script completed successfully in %.1f minutes\n',total_finish/60);

end