% imgFolder = 'C:\Users\Sauron\Desktop\BK Testing\06-17-21 Automated AO test\Test2-4Rings\';
% imgFolder = 'E:\BK Testing\06-29-AO Second Full Scope Test\SingleRing_atCenter\';
imgFolder = 'E:\BK Testing\06-29-AO Second Full Scope Test\Results\';
% imgFolder = 'C:\Users\Sauron\Desktop\BK Testing\06-17-21 Automated AO test\Test3-5Rings\';

num_Axis = 50;

tiffiles = dir([imgFolder, '*.tiff']);

err_uni = [];
err_ellipticity = [];

tst = tic;

for idx = 1:25

    IMG = imread([imgFolder tiffiles(idx).name]);
    [centers, radii, metric] = imfindcircles(IMG,[10 30]);
  
    figure(1);
    imshow(IMG)
    hold on
    viscircles(centers, radii,'EdgeColor','b');
end

toc(tst)
