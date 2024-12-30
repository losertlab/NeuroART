function [norm_meanIMG,roiBW2,npBWout,DFF,normF,npSubFluoSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_filled(RegImg,exptVars,xc,yc,expectedNeuronRadiusPix,winsize,percentBaselineSub)

narginchk(6,7)
if ~exist('winsize','var'); winsize = 300; end % default to 300 frame window
if ~exist('percentBaselineSub','var'); percentBaselineSub = 50; end % default to 50% baseline subtraction

total_start = tic;
numNeurons = length(xc);
meanIMG = mean(RegImg,3); % Mean image for cell center clicking
norm_meanIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));

%% PREALLOCATE
processdata;
%% PREALLOCATE FLUO AND NPFLUO MATRICES
rawFluo = zeros( size(RegImg,3) , numNeurons );

%% FIND THE BOUNDARIES OF CLICKED NEURONS
findingBoundaries;

%% Extract fluorescence traces for each neuron
extractFluorescence;

%% apply assymetrical (exponential) filter to traces - smoothing - MODIFIED FROM PLENZ LAB AVALANCHE CODE
applyingFilter;

%% clean up raw (remove bad ROIs)
cleanUpROI;

end