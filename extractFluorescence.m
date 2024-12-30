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