function fname = saveLabelledNeurons(cellRanks,selectedCellList,hiddenCells,Fluo,xcRaw,ycRaw,norm_meanIMG,initial_batchsize,r,gap,exptId)

% Input variables
% cellRanks: cell rankings obtained from the minimum spanning tree method (cell ids of the mostly population-correlated cells)
% Fluo: smoothed fluorescence intensities of each neurons obtained at each timestep
% xcRaw: x coordinates of neurons
% ycRaw: y coordinates of neurons
% norm_meanIMG: normalized mean image of the initial batch of frames
% initial_batchsize: number of frames in the initial batch
% r: average radius of a neuron (pixels)
% gap: Frequency of updating the figures (e.g. every 15 frames)

nROIs = length(xcRaw); % number of ROIs

outputFolder = ['Labelled_Neurons_' exptId];
mkdir(outputFolder);

%% Find upper and lower thresholds for grayscale patches

A = max(Fluo'); 
B = sort(A,'ascend');
upperLim = B(ceil(nROIs*0.75)); % 75th percentile (to remove outliers)
A = min(Fluo'); 
B = sort(A,'descend');
lowerLim = B(ceil(nROIs*0.75)); 

figure('Name','Saving Output Images ...','NumberTitle','off','Toolbar','none');
imshow(norm_meanIMG);
iptsetpref('ImshowBorder','tight');
hold on;

theta = linspace(0, 2*pi, 20)'; % Generate circular patches around the centroids of identified neurons
xx = bsxfun(@plus, r .* cos(theta), xcRaw');
yy = bsxfun(@plus, r .* sin(theta), ycRaw');
ptc = patch(xx, yy, (Fluo(:,initial_batchsize)-lowerLim)./upperLim);

% Initialize circles (labels of top 5 population-correlated cells)
first = plot(NaN,NaN,'Color','g','LineWidth',2);
second = plot(NaN,NaN,'Color','m','LineWidth',2);
third = plot(NaN,NaN,'Color','b','LineWidth',2);
fourth = plot(NaN,NaN,'Color','b','LineWidth',2);
fifth = plot(NaN,NaN,'Color','b','LineWidth',2);
text1 = text(NaN,NaN,'\leftarrow 1', 'Color','w');
text2 = text(NaN,NaN,'\leftarrow 2', 'Color','w');
text3 = text(NaN,NaN,'\leftarrow 3', 'Color','w');
text4 = text(NaN,NaN,'\leftarrow 4', 'Color','w');
text5 = text(NaN,NaN,'\leftarrow 5', 'Color','w');

if numel(hiddenCells)>0
    for j = 1:numel(hiddenCells)
        plot(xx(:,hiddenCells(j)),yy(:,hiddenCells(j)),'Color','r','LineWidth',2);
    end
end

numOfSelectedCells = numel(find(selectedCellList));

if numOfSelectedCells>0
    for j = 1:numOfSelectedCells
        plot(xx(:,selectedCellList(j)),yy(:,selectedCellList(j)),'Color','y','LineWidth',2);
    end
end

[dimX,dimY] = size(norm_meanIMG);
text(dimX*0.8,dimY*0.05,'Red: Hidden', 'Color','w');
text(dimX*0.75,dimY*0.10,'Yellow: Selected', 'Color','w');

for i = 1:size(cellRanks,2)
    cellRank = cellRanks(:,i);
    set(ptc,'CData',(Fluo(:,initial_batchsize+gap*i)-lowerLim)./upperLim);
    set(first,'XData',xx(:,cellRank(1)),'YData',yy(:,cellRank(1)));
    set(second,'XData',xx(:,cellRank(2)),'YData',yy(:,cellRank(2)));
    set(third,'XData',xx(:,cellRank(3)),'YData',yy(:,cellRank(3)));
    set(fourth,'XData',xx(:,cellRank(4)),'YData',yy(:,cellRank(4)));
    set(fifth,'XData',xx(:,cellRank(5)),'YData',yy(:,cellRank(5)));
    set(text1,'Position',[xcRaw(cellRank(1)),ycRaw(cellRank(1))]);
    set(text2,'Position',[xcRaw(cellRank(2)),ycRaw(cellRank(2))]);
    set(text3,'Position',[xcRaw(cellRank(3)),ycRaw(cellRank(3))]);
    set(text4,'Position',[xcRaw(cellRank(4)),ycRaw(cellRank(4))]);
    set(text5,'Position',[xcRaw(cellRank(5)),ycRaw(cellRank(5))]);
        
    im = getframe(gcf);
    iptsetpref('ImshowBorder','tight'); % remove all the borders around the image
    fname = ['.' filesep outputFolder filesep 'Frame_' sprintf('%05d',gap*i) '.tif'];
    imwrite(im.cdata, fname);
    
end

close(gcf);

% writerObj = VideoWriter('WindowSize_500.avi');
% writerObj.FrameRate = 30;
% % set the seconds per image
% % open the video writer
% open(writerObj);
% % write the frames to the video
% for i=1:length(im)
%     % convert the image to a frame
%     frame = im(i) ;    
%     writeVideo(writerObj, frame);
% end
% % close the writer object
% close(writerObj);

end