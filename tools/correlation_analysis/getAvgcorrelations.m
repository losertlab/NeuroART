function [indices,normalizedCorr]  = getAvgcorrelations(params)

% params: a struct with input parameters

% params.DFF: the csv file containing NxT matrix of DF/F or spike counts 
% (N = number of neurons, T = number of time points)
% params.coordinates: csv file containing Nx2 matrix of cell coordinates
% params.start: frame id of the starting time point for analysis
% params.stop: frame id of the ending time point for analysis
% params.avgStepSize: frame block size for averaging (default: 1, no averaging)
% params.dimX: number of pixels in X dimension
% params.dimY: number of pixels in Y dimension
% params.micronsPerPixel: microns per pixel
% params.distStepSize: step size to group cells by distances (in pixels)
% params.type: whether the distance is measured in 'Linear' or 'Log' scale
% params.frameRate: frame rate of acquired data
% params.minNeuronsPerWin: minimum number of neurons required per window
% params.windowSizes: column vector of window sizes considered for analysis
% params.overlapFactor: the amount of shift used to generate overlapping
% params.centerWindow: (0: don't center the window, 1: use only one window
% at the center of the FOV)
% params.correlations: the type of correlation used for analysis
% (instantaneous/Pearson)
% windows for analysis. (given as a fraction of the window size)

% windowSizes are the side lengths (in pixels) of tiles used to divide the FOV to subregions

windowSizes = params.windowSizes;
dimX = params.dimX;
dimY = params.dimY;

DFF = params.DFF;
coordinates = params.coordinates;

pearsonCorr = 0;
if params.correlations == "Pearson"
    pearsonCorr = 1;
end

% DFF_temp = csvread(params.DFF); % params.DFF; % csvread reads data as one column for large datasets
% coordinates = csvread(params.coordinates); % params.coordinates;
% 
% if size(DFF_temp,2)<2
%     T = length(DFF_temp)/size(coordinates,1);
%     DFF = zeros(size(coordinates,1),T);
% 
%     for i = 1:size(coordinates,1)
%         DFF(i,:) = DFF_temp(T*(i-1)+1 : T*i);
%     end
% else
%     DFF = DFF_temp;
% end

n = size(DFF,1); % number of neurons
startF = params.start;
stopF = params.stop;
avgStepSize = params.avgStepSize;
deltaR = params.distStepSize;

if avgStepSize > 1
    stopF = stopF-mod(stopF-startF+1,avgStepSize); % make sure the number of frames is divisible by the averaging factor
    DFF = DFF(:,startF:stopF); % extract only the frames considered for analysis
    DFFr = reshape(DFF,n,avgStepSize,size(DFF,2)/avgStepSize); % reshape before averaging
    DFF = reshape(mean(DFFr,2),n,size(DFF,2)/avgStepSize); % DF/F after averaging
else
    DFF = DFF(:,startF:stopF); % extract only the frames considered for analysis, no averaging
end

if params.type == "Linear"
    ngroups = ceil(max(windowSizes)*sqrt(2)/deltaR);
else
    ngroups = ceil(log(max(windowSizes)*sqrt(2))/deltaR);
end

normalizedCorr = zeros(ngroups,length(windowSizes));

for itr = 1:length(windowSizes)
    winSize = windowSizes(itr);
    shift = winSize*params.overlapFactor;
    
    if params.centerWindow
        xStart = ceil((dimX+1)/2)-floor(winSize/2); % only one window at the center of the FOV
        yStart = ceil((dimY+1)/2)-floor(winSize/2);
    else
        xStart = 1:shift:dimX-winSize+1; % start indices of tiles in X dimension
        yStart = 1:shift:dimY-winSize+1; % start indices of tiles in Y dimension
    end
    
    
    
%     ntiles = length(xStart)*length(yStart);
%     avgCorr = zeros(ngroups,ntiles);
%     counts = zeros(ngroups,ntiles);

    avgCorr = zeros(ngroups,1);
    counts = zeros(ngroups,1);
    
    for xitr = 1:length(xStart)
        for yitr = 1:length(yStart)
            xstart = xStart(xitr);
            xstop = xstart + winSize - 1;
            ystart = yStart(yitr);
            ystop = ystart + winSize - 1;
            cellInds = find(coordinates(:,1)>= xstart & coordinates(:,1)<= xstop & coordinates(:,2)>=ystart & coordinates(:,2)<=ystop);

            if length(cellInds)>=params.minNeuronsPerWin
                A = DFF(cellInds,:);
                N = length(cellInds);
                spatial_avg = mean(A,1); % 1xT vector of spatial average
                A_sub = A - repmat(spatial_avg,N,1);
                if pearsonCorr
                    C = corr(A_sub');
                else
                    C = A_sub*A_sub';
                end
                dist = squareform(pdist(coordinates(cellInds,:)));
%                 tileId = (yitr-1)*sqrt(ntiles) + xitr;

                if params.type == "Linear"
                    for i = 1:N
                        for j = 1:i
                            val = C(i,j);
                            idx = round(dist(i,j)/deltaR)+1;
                            counts(idx) = counts(idx) + 1;
                            avgCorr(idx) = avgCorr(idx) + val;
        %                     avgCorr(idx,tileId) = (avgCorr(idx,tileId)*(counts(idx,tileId)-1) + val)/counts(idx,tileId);  
                        end
                    end
                else
                    for i = 1:N
                        for j = 1:i
                            val = C(i,j);
                            idx = round(log(dist(i,j))/deltaR)+1;
                            counts(idx) = counts(idx) + 1;
                            avgCorr(idx) = avgCorr(idx) + val;  
                        end
                    end
                    
                end
            end
            
        end
    end
    
    temp = avgCorr./counts;
    temp(isnan(temp)) = 0; % omit NaNs
    normalizedCorr(:,itr) = temp/temp(1); % divide by the first element to normalize

end

indices = [0; deltaR*(0.5 + (0:ngroups-1)')]; % add a zero to the front