% imgFolder = 'E:\BK Testing\06-30-AO Third Full Scope Test\Results\';
% tiffiles = dir([imgFolder, '*.tiff']);
imgFolder ='E:\BK Testing\07-01-AO Fourth Full Scope Test\IMG_1\Default\';
tiffiles = dir([imgFolder, '*.tif']);
 
IMG = imread([imgFolder tiffiles(316).name]);
% imshow(IMG);

% Annulus mask

imageSizeX = 512;
imageSizeY = 512;
[columnsInImage, rowsInImage] = meshgrid(1:imageSizeX, 1:imageSizeY);
% Next create the circle in the image.
centerX = 214;
centerY = 275;
innerRadius = 10;
outerRadius = 18;
array2D = (rowsInImage - centerY).^2 ...
+ (columnsInImage - centerX).^2;
ringPixels = array2D >= innerRadius.^2 & array2D <= outerRadius.^2;

ringMask = ringPixels./sum(ringPixels(:));

Conv = conv2(double(IMG),ringMask,'same');
imagesc(Conv);
max(Conv(:))