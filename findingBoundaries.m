roiBounds = [deg2rad(1:360)' repmat(expectedNeuronRadiusPix,360,1)];
for pp = 1:numNeurons
    ROIxvOut{pp} =  xc(pp) + roiBounds(:,2) .* (cos(roiBounds(:,1))) ;
    ROIyvOut{pp} =  yc(pp) + roiBounds(:,2) .* (sin(roiBounds(:,1))) ;
    roiBW2{pp} = poly2mask( ROIxvOut{pp} , ROIyvOut{pp} , size(RegImg,1) , size(RegImg,2));
end

%DW 11232015 - adjusted for inclusion of neuropil correction
% correct for overlapping ROIs (exclude from both)  
% Remove this part if you need to speed up the initial batch processing for real-time analysis

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