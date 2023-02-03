function [norm_meanIMG,roiBW2_sp,npBWout_sp,DFF,normF,npSubFluoSmooth,xcRaw,ycRaw,minNpSubFluo,maxAdjF] = computeDFF_new_coder(RegImg,frameRate,pos_lst,r_pixels,winsize,percentBaselineSub,neuropilSubPercent)

% remove calls to exist() to enable mex compilation

% narginchk(8,10)
% if ~exist('winsize','var'); winsize = 300; end % default to 300 frame window
% if ~exist('percentBaselineSub','var'); percentBaselineSub = 50; end % default to 50% baseline subtraction
% if ~exist('neuropilSubPercent','var'); neuropilSubPercent = 70; end % default to 70% neuropil subtraction

% total_start = tic;
%% Dan experimental parameters

% cd(expDir) % expDir is the current working directory
% 01/14/22 removed this section for speed up, also removed
% expDir,exptName,and imTemplate from the list of input arguments

% movID = ['DFF_' exptName];
% datafile = [fullfile(expDir,movID) '.mat'];
% if ~exist(datafile,'file')
%     disp(['Creating data file', datafile]);
%     save(datafile,'movID','-v7.3');cl
% end
% 
% warning('off','stats:pvaluedw:ExactUnavailable')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%DW05062015 - added to make the ROI ring more flexible and dependent on mag
%size rather than fixed

expectedNeuronRadiusPix = r_pixels;
expectedNeuropilRadiusPix = 2*r_pixels; %need to remove outer boundary of neurons
fractionNP = neuropilSubPercent / 100;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

meanIMG = mean(RegImg,3); % Mean image for cell center clicking
norm_meanIMG = (meanIMG - min(meanIMG(:))) / (max(meanIMG(:))-min(meanIMG(:)));

% save(datafile,'meanIMG','norm_meanIMG','imTemplate','-append'); % remove for speed

%% Select neuron centers - FRAUNHOFER -- May have to make xc/yc an input to computeDFF given the Platform layout

xc = pos_lst(:,2);
yc = pos_lst(:,1);

%% PREALLOCATE
% traceExtract_start = tic;

nc = length(xc); % number of cells
dimX = size(meanIMG,1);
dimY = size(meanIMG,2);

% pcimg1 = cell (nc , 16 , 360 );
% pcimg = coder.nullcopy(pcimg1);
% imgCrop = cell ( nc , 32, 32 );
% imgCrop = coder.nullcopy(imgCrop1);
% imgCropNorm1 = cell (nc, 32 , 32 );
% imgCropNorm = coder.nullcopy(imgCropNorm1);
% roiBoundaries = cell ( nc , 360 , 3 );
% smRoiBoundaries = cell ( nc , 360 , 3 );
ROIOut = zeros ( 360 , 2 );
ROIIn =  zeros ( 360 , 2 );
% ROIxvOut = cell ( nc , 360 , 1 );
% ROIyvOut = cell ( nc , 360 , 1 );
% ROIxvIn = cell ( nc , 360 , 1 );
% ROIyvIn = cell ( nc , 360 , 1 );

roiBWout = false(dimX,dimY,nc); % cell ( nc , size(norm_meanIMG,1) , size(norm_meanIMG,2));

roiBW2 = zeros(dimX,dimY, nc);


%neuropil
% npBoundaries = cell ( nc , 360 , 3 );
% smNpBoundaries = cell ( nc , 360 , 3 );
NeuropilOut = zeros ( 360 , 2 );
NeuropilIn =  zeros ( 360 , 2 );

% NPxvOut = cell ( nc , 360 , 1 );
% NPyvOut = cell ( nc , 360 , 1 );
% NPxvIn = cell ( nc , 360 , 1 );
% NPyvIn = cell ( nc , 360 , 1 );

npBWout = false (dimX,dimY,nc);
npBWin = false (dimX,dimY,nc);
npBW2 = zeros(dimX,dimY, nc);

%% PREALLOCATE FLUO AND NPFLUO MATRICES

rawFluo = zeros( size(RegImg,3) , nc );
npFluo = zeros( size(RegImg,3) , nc );
npSubFluoSmooth = zeros( size(RegImg,3) , nc );

%% FIND THE BOUNDARIES OF CLICKED NEURONS

for pp = 1:nc
    xpt=xc(pp);
    ypt=yc(pp);
    imgCrop = imcrop(meanIMG,[xpt-15 ypt-15 31 31]);
    imgCropNorm = (imgCrop - min(imgCrop(:)))  ./ (max(imgCrop(:)) - min(imgCrop(:)));
    tmpNeuron = imgpolarcoord_coder(imgCropNorm);  % this comes from Matlab Central Download
    
%     imgCropNorm{pp} = (imgCrop{pp} - min(imgCrop{pp}(:)))  ./ (max(imgCrop{pp}(:)) - min(imgCrop{pp}(:)));
%     pcimg{pp} = imgpolarcoord_coder(imgCropNorm{pp} );  % this comes from Matlab Central Download
    
    RingPks = zeros(size(tmpNeuron,2),1); %reset vals to zero
    
%     tmpNeuron = pcimg{pp}; %iterate this for each selected neuron
    
    for cc = 1:size(tmpNeuron,2) % for every direction - find the inner part of the ring
        
        pkTmp = find(diff(tmpNeuron(:,cc)) == min(diff(tmpNeuron(:,cc)))); % DW07122015_changed to make this more robust - seems to be working right now - continue testing
        
        if ~isempty(pkTmp)
            if length(pkTmp) > 1 %more than one pixel identified - grab the first one
                if pkTmp(1) < expectedNeuronRadiusPix && pkTmp(1) > 2
                    RingPks(cc) = pkTmp(1);
                else
                    RingPks(cc) = expectedNeuronRadiusPix;
                end
            else
                if pkTmp(1) < expectedNeuronRadiusPix && pkTmp(1) > 2
                    RingPks(cc) = pkTmp;
                else
                    RingPks(cc) = expectedNeuronRadiusPix;
                end
            end
        elseif cc == 1 % if its'the first direction and no peaks are found
            RingPks(cc) = expectedNeuronRadiusPix;  %made this dependent on mag factor DW_02022015
        else
            RingPks(cc) = RingPks(cc-1);
        end
        ROIOut(cc,:) = [ deg2rad(cc)  RingPks(cc) ];
        ROIIn(cc,:) = [ deg2rad(cc)   RingPks(cc)-2 ];
        
        %%%%%%NEED TO INCLUDE NEUROPIL SIGNAL (SVOBODA LAB USES ~20 UM
        %%%%%%FROM CELL CTR EXCLUDING ROIs ALL OTHER NEURONS - NATURE
        %%%%%%2015)
        NeuropilIn(cc,:) = [deg2rad(cc) ROIOut(cc,2)+1];
        NeuropilOut(cc,:) = [deg2rad(cc) expectedNeuropilRadiusPix];
        
        %%%%%%NEED TO INCLUDE NEUROPIL SIGNAL (SVOBODA LAB USES ~20 UM
        %%%%%%FROM CELL CTR EXCLUDING ROIs ALL OTHER NEURONS - NATURE
        %%%%%%2015)
    end
    
%     roiBoundaries{pp} = [ ROIIn(:,1) ROIIn(:,2)  ROIOut(:,2) ]; % [PolarCoords (0-2Pi)    InnerRing     OuterRing]
    smRoiBoundaries = [ ROIIn(:,1) smooth(ROIIn(:,2),10)  smooth(ROIOut(:,2),10) ]; % [PolarCoords (0-2pi)    InnerRing     OuterRing]
    %DW 11232015 - included neuropil variables
%     npBoundaries{pp} = [ NeuropilIn(:,1) NeuropilIn(:,2) NeuropilOut(:,2) ];
    smNpBoundaries = [ NeuropilIn(:,1) smooth(NeuropilIn(:,2),10) smooth(NeuropilOut(:,2),10) ];
    
    % CREATE MASKS FOR ALL CLICKED ROIS, THEN SHOW THEM -- DW 11232015
    % renamed variable for consistency
    x1 =  xpt + smRoiBoundaries(:,3) .* (cos(smRoiBoundaries(:,1))) ;
    y1 =  ypt + smRoiBoundaries(:,3) .* (sin(smRoiBoundaries(:,1))) ;
    x2 =  xpt + smRoiBoundaries(:,2) .* (cos(smRoiBoundaries(:,1))) ;
    y2 =  ypt + smRoiBoundaries(:,2) .* (sin(smRoiBoundaries(:,1))) ;
    
%     x1 = ROIxvOut{pp}; % added this section to enable mex compilation
%     y1 = ROIyvOut{pp};
%     x2 = ROIxvIn{pp};
%     y2 = ROIyvIn{pp};
    
    if (x1(end) ~= x1(1)) || (y1(end) ~= y1(1)) % moved from poly2mask
        x11 = vertcat(x1,x1(1));
        y11 = vertcat(y1,y1(1));
    else
        x11 = x1;
        y11 = y1; 
    end
    
    if (x2(end) ~= x2(1)) || (y2(end) ~= y2(1)) % moved from poly2mask
        x21 = vertcat(x2,x2(1));
        y21 = vertcat(y2,y2(1));
    else
        x21 = x2;
        y21 = y2;
    end
  
    roiBWout(:,:,pp) = poly2mask( x11 , y11 , size(meanIMG,1) , size(meanIMG,2));
    roiBWin = poly2mask( x21 , y21 , size(meanIMG,1) , size(meanIMG,2));
    roiBW2(:,:,pp) =  roiBWout(:,:,pp) -  roiBWin;
    temp = roiBW2(:,:,pp);
    if sum(temp(:) < 0) > 0 %accounts for inner diameter extending beyond outer diameter
        temp(temp < 0 ) = 0;
        roiBW2(:,:,pp) = temp;
    end
    
    %DW 11232015 - included for neuropil correction
    % CREATE MASKS FOR NEUROPIL CLICKED ROIS, THEN SHOW THEM
    NPxvOut =  xpt + smNpBoundaries(:,3) .* (cos(smNpBoundaries(:,1))) ;
    NPyvOut =  ypt + smNpBoundaries(:,3) .* (sin(smNpBoundaries(:,1))) ;
    NPxvIn =  xpt + smNpBoundaries(:,2) .* (cos(smNpBoundaries(:,1))) ;
    NPyvIn =  ypt + smNpBoundaries(:,2) .* (sin(smNpBoundaries(:,1))) ;
    npBWout(:,:,pp) = poly2mask( NPxvOut , NPyvOut , size(meanIMG,1) , size(meanIMG,2));
    npBWin(:,:,pp) = poly2mask( NPxvIn , NPyvIn , size(meanIMG,1) , size(meanIMG,2));
    npBW2(:,:,pp) =  npBWout(:,:,pp) -  npBWin(:,:,pp);
    tempN = npBW2(:,:,pp);
    if sum(tempN(:) < 0) > 0 %accounts for inner diameter extending beyond outer diameter
        tempN(tempN < 0 ) = 0;
        npBW2(:,:,pp) = tempN;
    end
end

%DW 11232015 - adjusted for inclusion of neuropil correction
% correct for overlapping ROIs (exclude from both)

% disp('Adjusting ROI masks for overlap....');
% tStartROICorr = tic;

% maskStack = false(size(meanIMG,1) , size(meanIMG,2) , nc); % 01/20/22 added for mex compilation
% for itr = 1:nc
%     maskStack(itr) = roiBWout(:,:itr};
% end

AllMasksTMP =  sum ( roiBWout , 3 ); % first term of cat (i.e., '3') points to element-wise alignement/stacking of arrays
[oLapRoiY, oLapRoiX] = find( AllMasksTMP > 1 );
for ii = 1:nc
    for yy = 1:length(oLapRoiX)
        roiBWout(oLapRoiY(yy),oLapRoiX(yy),ii) = 0;
        roiBW2(oLapRoiY(yy),oLapRoiX(yy),ii) = 0;
    end
end
% ROImap =  sum ( cat ( 3 , roiBW2{:} ) , 3 ); % first term of cat (i.e., '3') points to element-wise alignement/stacking of arrays

% tElapsedROICorr = toc(tStartROICorr);
% disp(['    Time elapsed for ROI mask Correction was ', num2str(tElapsedROICorr/60),' minutes']);

% correct for neuropil overlap with ROIs and

% disp('Adjusting neuropil masks for overlap....');
% tStartNPilCorr = tic;

for nn = 1:nc
    tmpNp = npBWout(:,:,nn);
    for rr = 1:nc
        tmp = roiBWout(:,:,rr) + tmpNp;
        [oLapNPY,oLapNPX] = find( tmp > 1 );
        for yy = 1:length(oLapNPX)
            npBWout(oLapNPY(yy),oLapNPX(yy),nn) = 0;
            npBWin(oLapNPY(yy),oLapNPX(yy),nn) = 0;
        end
        for yy = 1:length(oLapRoiX)
            npBWout(oLapRoiY(yy),oLapRoiX(yy),nn) = 0;
            npBWin(oLapRoiY(yy),oLapRoiX(yy),nn) = 0;
        end
    end
end

% tElapsedNPilCorr = toc(tStartNPilCorr);
% disp(['    Time elapsed for Neuropil Mask Correction was ', num2str(tElapsedNPilCorr/60),' minutes']);

% traceExtract_finish = toc(traceExtract_start);
% fprintf('Trace extraction took %.1f minutes\n',traceExtract_finish/60)

%% CALCULATE FLUORESCENCE TRACES for ALL STIMS

% calcF_start = tic;

for nn = 1:nc
    [r,c]=find(roiBW2(:,:,nn)~=0);
    
    tmp = NaN(numel(r),size(RegImg,3));
    for i = 1:numel(r)
        tmp(i,:) = RegImg(r(i),c(i),:);
    end
    rawFluo(:,nn) = nanmean(tmp,1);
    
end

% calcF_finish = toc(calcF_start);
% fprintf('Calculating fluorescence traces took %.1f minutes\n',calcF_finish/60)

%DW 11232015 - included Neuropil calculation
% disp('Calculating Fluo traces for neuropil.....');
% tStartfcalcNP = tic;

%DW11132015 - apply to neuropil calculation.....
% if isempty(gcp('nocreate'))
%     parpool(2)
% end
% for nn = 1:length(xc)
%     [rNp,cNp] = find(npBWout{nn} ~= 0 );
%     npFluo(:,nn) = mean(mean(IMG(rNp,cNp,:),1),2);
% end

for nn = 1:nc
    [rNp,cNp] = find(npBWout(:,:,nn) ~= 0 );
    
    tmp = NaN(numel(rNp),size(RegImg,3));
    for i = 1:numel(rNp)
        tmp(i,:) = RegImg(rNp(i),cNp(i),:);
    end
    npFluo(:,nn) = nanmean(tmp,1);
end

% tElapsedFcalcNP = toc(tStartfcalcNP);
% disp(['    Time Elapsed for calculating fluo for neuropil was ',num2str(tElapsedFcalcNP/60),' minutes']);
% 
% clear IMG

npSubFluo = rawFluo - (fractionNP * npFluo);
% fluoAllCorr = fluoAllRaw - (percNP * NPfluoAll);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% PUT THE DE-NOISING, PARSING AND DFF CALCULATIONS HERE - %% DW05192016 added for trialBased analyses

%% apply assymetrical (exponential) filter to traces - smoothing - MODIFIED FROM PLENZ LAB AVALANCHE CODE
% tStartDN = tic;
% disp('Denoising Ca2+ traces with Plenz Lab approach.....');

[nrois , ~] = size(npSubFluo');
T = 1/frameRate;

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
else
    smoothwin = 9; %13
end
symmFLAG = 1; % 1 ->symmetric, 2->asym, T = frame time, Tau -> 1.5

for j = 1 : nrois % NEED FOR IMPROVEMENT - ELIMINATE FOR LOOP?
    
    %%%%% SYMMETRIC (new as of Sept2016)
    tmp = normF(:,j)';
    npSubFluoSmooth(:,j) = smooth2(tmp,symmFLAG,smoothwin,T,1.5); %Maybe smooth after DFF calc?
    
end

normF = normF'; % added by dulara on 05/24/2020

% tElapsedDN = toc(tStartDN);
% disp(['     Time elapsed for denoising was ', num2str(tElapsedDN/60) , ' minutes']);

% % fluoAllSmooth dimensions are (frames,roi)
% F0 = median(fluoAllSmooth,1); % Median fluo for each roi
% tmpDF = bsxfun(@minus,fluoAllSmooth,F0);
% tmpDFF = bsxfun(@rdivide,tmpDF,F0);
% DFF.smooth = 100*tmpDFF';

% performing moving average baseline subtraction for DFF
% winsize = round(exptVars.frameRate * winSizeSeconds);

percent = percentBaselineSub;
DFF.smooth = slideWinSub(npSubFluoSmooth',winsize,percent);
DFF.raw = slideWinSub(normF,winsize,percent);

% convert to sparse matrices for efficiency during real time analysis

roiBW2_sp = cell ( nc , 1); % added to allow mex compilation
npBWout_sp = cell ( nc , 1);
for nn = 1:nc
    roiBW2_sp{nn} = sparse(roiBW2(:,:,nn));
    npBWout_sp{nn} = sparse(npBWout(:,:,nn));
end

%% clean up raw (remove bad ROIs) --> romove for speed up

% clean_start = tic;

% Streamlined the "cleaning" stage 6/2020
% 7/2020 update: might not be a necessary step anymore with normalization changes made above, saving regardless
% [DFF.smoothClean,keptRois.smoothClean] = cleanTrace(DFF.smooth);
% [DFF.rawClean,keptRois.rawClean] = cleanTrace(DFF.raw);

% DFF.smoothClean = DFF.smooth((min(DFF.smooth')> 2*median(min(DFF.smooth))),:); % DW09232016 changed this to be data-based rather than hardcoded / ZB09302016 changed mean to median
% % normFClean = normF((min(DFF.smooth')> 2*median(min(DFF.smooth))),:); % added on 05/24/2020
xcRaw = xc;
ycRaw = yc;
% 
% usedInds = find((min(DFF.smooth')> 2*median(min(DFF.smooth))));
% xc = xc(usedInds);
% yc = yc(usedInds);

% dffParams.winsize = winsize;
% dffParams.percentBaselineSub = percentBaselineSub;
% dffParams.neuropilSubPercent = neuropilSubPercent;

% save(datafile, 'ROImap','npBWout','roiBW2','xc','yc','minNpSubFluo','maxAdjF','-append');
% save(datafile, 'npSubFluo', 'npSubFluoSmooth','DFF','dffParams','xcRaw','ycRaw', '-append');
% 
% clean_finish = toc(clean_start);
% fprintf('Removing bad ROIs took %.1f minutes\n',clean_finish/60)
% 
% total_finish = toc(total_start);
% fprintf('Script completed successfully in %.1f minutes\n',total_finish/60);


end