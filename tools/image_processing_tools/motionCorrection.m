function [regImg,imTemplate,norm_meanRedIMG] = motionCorrection(IMG,exptVars,inputParams,batchSettings)
%UNTITLED Motion correction for neuroart

I = (mean(IMG(:,:,1:length(batchSettings.frameBlock)),3)); % Mean image
%I = (stdev(IMG(:,:,1:length(batchSettings.frameBlock)),3)); % Standard deviation image

fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
imTemplate = fft2(fixed);


regImg = zeros(exptVars.dimX, exptVars.dimY, length(batchSettings.frameBlock), 'uint16');

if inputParams.RCHAN == 2 % if the red channel is not available

    for j = 1:length(batchSettings.frameBlock)
        % using Fourier transformation of images for registration
        [regFrame,~,~] = regseqRT(imTemplate,IMG(:,:,j));

        regImg(:,:,j) = gather(regFrame);
    end

    norm_meanRedIMG = []; % because in this case the red (opsin expression) channel is not available

else  % Only valid for individual tif files spitted out by Bruker (2 channel data)
        
    RedIMG = nan(exptVars.dimX,exptVars.dimY,length(batchSettings.frameBlock)); %% change to length(fileInd:length(files))   
    redID = 2;
        
    if batchSettings.GREENCHAN == 2
        redID = 1; 
    end

    for i = 1:length(batchSettings.frameBlock)
        fName = fullfile(inputParams.IMGFOLDER, [inputParams.IMG '_Ch' num2str(redID) '_' sprintf('%05d',i) '.ome' inputParams.FORMAT]); % Format of the Bruker tif filenames 
        RedIMG(:,:,i)  = imread(fName); 
    end

    cd(pwd)
    
    I = (mean(RedIMG(:,:,1:length(batchSettings.frameBlock)),3)); % Mean image
    fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
    imTemplate_red = fft2(fixed);
        
    ty = zeros(length(batchSettings.frameBlock),1);
    tx = zeros(length(batchSettings.frameBlock),1);
    
    for j = 1:length(batchSettings.frameBlock)
        % using Fourier transformation of images for registration
        error = dftregistration(imTemplate_red,fft2(RedIMG(:,:,j)),inputParams.dftResolution);
        ty(j) = error(3);
        tx(j) = error(4);
    end

    meanIMG = mean(RedIMG,3); % Mean image for cell center clicking
    norm_meanRedIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));
      
end

end

