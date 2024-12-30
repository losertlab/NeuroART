traceExtract_start = tic;

ROIxvOut = cell ( numNeurons , 360 , 1 );
ROIyvOut = cell ( numNeurons , 360 , 1 );
roiBW2 = cell ( numNeurons , size(RegImg,1) , size(RegImg,2));
npBWout = cell ( numNeurons , size(RegImg,1) , size(RegImg,2));

for pp = 1:length(xc)
    npBWout{pp} = ~ones(size(RegImg,1) , size(RegImg,2));  % matrix of all zeros (logical type)
end