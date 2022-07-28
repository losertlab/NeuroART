function error = errorFunAO(imgFolder,liveimg_savepath,imgType,numBeamlets)

% err = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
% imgType = 0; % 0: Tiff (Basler), 1: Tif (Hamamatsu)
% numBeamlets = number of Vortices/Points;
% imgFolder = 'E:\BK Testing\09-16 AO Test\Results_R1_180to180_stepSize_15\';

pause(0.1);

%%

if imgType
    normFactor = 10000; % Intensity normalization factor
    tiffiles = dir([imgFolder, '*.tif']);
    imageSizeX = 512;
    imageSizeY = 512;
else
    normFactor = 255;
    tiffiles = dir([imgFolder, '*.tiff']);
    imageSizeX = 640;
    imageSizeY = 480;
end
IMG = imread([imgFolder tiffiles(end).name]);
imNum = length(tiffiles);
% tiffiles = dir([imgFolder, '*.mat']); % '.tiff' for pylon viewer, '.tif' for micromanager
% IMG = imread([imgFolder tiffiles(x).name]);
% f = load([imgFolder tiffiles(ceil(x(1)*200 + x(2)*200)).name]);
% IMG = f.IMG;


IMG = double(IMG)./normFactor; % Normalize Intensity
% [centers, radii, metric] = imfindcircles(IMG,[10 20]); % for Hamamatsu L = 15, for L=20 consider [15 25]
[centers, radii, metric] = imfindcircles(IMG,[8 16], 'Sensitivity',0.9); % 4 rings simultaneous

error = 0;
            
for i = 1:length(radii)               
    centroid = centers(i,:);      
    if i > numBeamlets
        break;
    end
    
    [columnsInImage, rowsInImage] = meshgrid(1:imageSizeX, 1:imageSizeY);
                
    % Next create the circle in the image.
                
    centerX = centroid(1);
    centerY = centroid(2);
                
    innerRadius = radii(i)-3; % thickness of the annulus =  pixels, radii is closer to the outer boundary
    outerRadius = radii(i);
                
    array2D = (rowsInImage - centerY).^2  + (columnsInImage - centerX).^2;

    ringPixels = array2D >= innerRadius.^2 & array2D <= outerRadius.^2;

                
    overlap = double(IMG).*double(ringPixels);

                
    intensities = overlap(overlap>0); 
    error = error - mean(intensities)/std(intensities);
    
%     theta = linspace(0,2*pi, 30);
%     col = (radii(i))*cos(theta);
%     row = (radii(i))*sin(theta);
%     M = makehgtform('translate',[centroid, 0],'zrotate',deg2rad(0));
%     D = M*[col;row;zeros(1,numel(row));ones(1,numel(row))];
%     
% %     Visualize the result
% 
%     figure(5)
%     imshow(IMG, [0,1]);
%     hold on
%     plot(D(1,:),D(2,:),'r','LineWidth',1)
%     pause(0.1)
%                
    

end

fname = sprintf('IMG%d.mat',imNum);    
save([liveimg_savepath,fname],'IMG');