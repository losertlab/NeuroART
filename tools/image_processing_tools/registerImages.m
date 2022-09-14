function [imTemplate, regImg, normMeanRedImg] = registerImages(inputParams, batchSettings,batchResults, exptVars, ist)
    I = (mean(batchResults.IMG(:,:,1:length(batchSettings.frameBlock)),3)); % Mean image
    %I = (stdev(batchResults.IMG(:,:,1:length(batchSettings.frameBlock)),3)); % Standard deviation image
    fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
    imTemplate = fft2(fixed);
    
    tstop = toc(ist);
    fprintf('Initial aquisition took %.4f seconds\n',tstop);
    
    regImg = zeros( exptVars.dimY , exptVars.dimX , length(batchSettings.frameBlock), 'uint16');
    
    normMeanRedImg = [];
    if inputParams.RCHAN == 1 % if the red channel is not available
        for j = 1:length(batchSettings.frameBlock)
            % using Fourier transformation of images for registration
    %         error = dftregistration_coder_mex(imTemplate,fft2(batchResults.IMG(:,:,j)),inputParams.dftResolution);
    
    %         [regFrame,~,~] = regseqRT_coderGPU_mex(imTemplate,batchResults.IMG(:,:,j));  % commented DZ 02/10/22
            [regFrame,~,~] = regseqRT(imTemplate,batchResults.IMG(:,:,j));
    
            regImg(:,:,j) = gather(regFrame);
    %         ty(j) = ty; %error(3);
    %         tx(j) = tx; %error(4);
        end
    else % Only valid for individual tif files spitted out by Bruker (2 channel data)
            
        RedIMG = nan(exptVars.dimX,exptVars.dimY,length(batchSettings.frameBlock)); %% change to length(fileInd:length(files))   
        redID = 2;
            
        if batchSettings.GREENCHAN == 2
            redID = 1; 
        end
    
        for i = 1:length(batchSettings.frameBlock)
            fName = fullfile(inputParams.IMGFOLDER, [inputParams.IMG '_Ch' num2str(redID) '_' sprintf('%05d',i) '.ome' inputParams.FORMAT]); % Format of the Bruker tif filenames 
            RedIMG(:,:,i)  = imread(fName);
    %             fh1 = fopen(files(batchSettings.START-1+i).name); % read raw image
    %             RedImg = reshape(fread(fh1,inf,'uint16=>uint16'),[exptVars.dimX, exptVars.dimY]);
    %             RedIMG(:,:,i)  = RedImg';
    %             fclose(fh1);
            
        end
        
        I = (mean(RedIMG(:,:,1:length(batchSettings.frameBlock)),3)); % Mean image
        fixed = (I - min(I(:)))./range(I(:)); % scale the intensities [0 1]
        imTemplate_red = fft2(fixed);
            
        for j = 1:length(batchSettings.frameBlock)
            % using Fourier transformation of images for registration
            error = dftregistration(imTemplate_red,fft2(RedIMG(:,:,j)),inputParams.dftResolution);
            ty(j) = error(3);
            tx(j) = error(4);
        end
            
        meanIMG = mean(RedIMG,3); % Mean image for cell center clicking
        normMeanRedImg = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));
            
        clear RedIMG RedImg imTemplate_red
    end
    
    % offsets = [ty' tx'];
       
    
    clear batchResults.IMG I fixed 
end