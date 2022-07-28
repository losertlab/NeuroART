% Makes use of "dftregistration" code package from:
% Manuel Guizar-Sicairos, Samuel T. Thurman, and James R. Fienup, 
% "Efficient subpixel image registration algorithms," Opt. Lett. 33, 
% 156-158 (2008).
%%  -While loop to read, register, and write images in batches -- dont save registered images
%   -Automatic cell extraction using CaImAn
%   -After identifying ROIs, rank the cells in the descending order of average brightness
%%
addpath(genpath('utilities'));
dftResolution = 1;
XML = danParseXML('Experiment.xml');
exptVars = xmlVersionCheck(XML);
exptVars.numImages = 2000; % initial acquisition
Imgseq = 'Red_'; % image sequence identifier
exptId = '062620_2000';
start_Frame = 1;
lower_bpass = 4;
higher_bpass = 13;

getFileroot = pwd;
directory = 'C:\Users\dular\Documents\LosertLab\Images_Red\';

ist = tic;
cd(directory)
files = dir([Imgseq '*.tif']);
frameBlock = start_Frame:exptVars.numImages; % earlier was, fileInd:length(files)
IMG = nan(exptVars.dimX,exptVars.dimY,length(frameBlock)); %% change to length(fileInd:length(files))
    
for i = start_Frame:length(frameBlock)
    IMG(:,:,i) = imread(files(i).name);
end
 
cd(getFileroot);

I = (mean(IMG(:,:,1:length(frameBlock)),3)); % Mean image
fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
imTemplate = fft2(fixed);

for j = frameBlock
    % using Fourier transformation of images for registration
    error = dftregistration(imTemplate,fft2(IMG(:,:,j)),dftResolution);
    ty(j) = error(3);
    tx(j) = error(4);
end

offsets = [ty' tx'];
    
%% Apply offsets
    
pdTmp = round(max(exptVars.dimY,exptVars.dimX)/3); % 1/3 of IMG size for padding
pd = pdTmp + mod(pdTmp,2); % Make padding amount an even number
RegIMG = zeros( exptVars.dimY , exptVars.dimX , length(frameBlock), 'uint16'); % don't need to index RegIMG if saving and stopping
for t = frameBlock
    tmpRegIMG = zeros(exptVars.dimY+pd,exptVars.dimX+pd,'uint16');
    tmpRegIMG( pd/2+ty(t):pd/2+ty(t)+exptVars.dimY-1 , pd/2+tx(t):pd/2+tx(t)+exptVars.dimX-1) = IMG(:,:,t);
    RegIMG(:,:,t) =  tmpRegIMG(pd/2:pd/2+exptVars.dimY-1 , pd/2:pd/2+exptVars.dimX-1);
end

clear IMG
    
tstop = toc(ist);
fprintf('Initial aquisition and registration took %.4f seconds\n',tstop);
    
% Detect centroids of the cells
iStart = tic;
meanRegIMG = mean(RegIMG,3); % Mean of the registered image
normMeanRegIMG = (meanRegIMG - min(meanRegIMG(:)))./range(meanRegIMG(:)); %Normalize
cell_centroids = CellExtractor_noio(normMeanRegIMG,lower_bpass,higher_bpass);
num_of_cells = length(cell_centroids);
tstop = toc(iStart);
    
fprintf('Total number of cells detected: %d \n',num_of_cells);
fprintf('Cell finding took %.4f seconds\n',tstop);

%% Calculate the average brightness of each cell and rank them in the decsending order

cell_diameter = higher_bpass;
r = (cell_diameter + 1)/2;

num_of_frames = exptVars.numImages;

avg_intensities = zeros(num_of_cells,1);


for n = 1:num_of_cells
    x_start = ceil(cell_centroids(n,1) - r); % mask for the current cell
    x_end = floor(cell_centroids(n,1) + r);
    y_start = ceil(cell_centroids(n,2) - r);
    y_end = floor(cell_centroids(n,2) + r);
    total = 0;
    for x = x_start:x_end
        for y = y_start:y_end
            total = total + meanRegIMG(y,x); 
        end
    end
    avg_intensities(n) = total/((x_end-x_start+1)*(y_end-y_start+1)); %total/no: of pixels
end

[~,cellRanking] = sort(avg_intensities,'descend');
sortedCells = cell_centroids(cellRanking,:);

tstop = toc(ist);
fprintf('total execution time is %.4f seconds\n',tstop);