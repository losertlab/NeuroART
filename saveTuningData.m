function [BFval, BFkHzVals] = saveTuningData(DFFsmooth,exptVars,psignalFile,thorSyncFile,JiSignalInfo,xcRaw,ycRaw,norm_meanIMG,numInitFrames,r,gap,exptId)

outputFile = ['TuningAnalysis_' exptId];

figure('Name','Saving Tuning Data ...','NumberTitle','off','Toolbar','none');
imshow(norm_meanIMG);
iptsetpref('ImshowBorder','tight');
hold on;

theta = linspace(0, 2*pi, 20)'; % Generate circular patches around the centroids of identified neurons
xx = bsxfun(@plus, r .* cos(theta), xcRaw');
yy = bsxfun(@plus, r .* sin(theta), ycRaw');
ptc = patch(xx, yy, zeros(1,length(xcRaw),3));
cc = []; % color code
BFval = zeros(length(xcRaw), length(numInitFrames:gap:length(DFFsmooth)));
itr = 1;

for i = numInitFrames:gap:length(DFFsmooth)
    if isempty(JiSignalInfo)
        tuningInfo = getTuningRealTimeFunction(DFFsmooth(:,1:i),exptVars,psignalFile,thorSyncFile);
    else
        tuningInfo = getTuningRealTimeFunction_Ji(DFFsmooth(:,1:i),exptVars,JiSignalInfo);
    end
    
    if isempty(cc)      
        cc(1,:,:) = jet(numel(tuningInfo.kHzVals));
    end
    
    set(ptc,'CData',cc(:,tuningInfo.BFval,:));
    BFval(:,itr) = tuningInfo.BFval;
    
    im(itr) = getframe(gcf);
    itr = itr + 1;
    
end

BFkHzVals = tuningInfo.kHzVals;
close(gcf);

writerObj = VideoWriter([outputFile '.avi']);
writerObj.FrameRate = 30;
% set the seconds per image
% open the video writer
open(writerObj);
% write the frames to the video
for i=1:length(im)
    % convert the image to a frame
    frame = im(i) ;    
    writeVideo(writerObj, frame);
end
% close the writer object
close(writerObj);


end