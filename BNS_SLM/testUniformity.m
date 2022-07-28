% imgFolder = 'C:\Users\Sauron\Desktop\BK Testing\06-17-21 Automated AO test\Test2-4Rings\';
% imgFolder = 'E:\BK Testing\06-29-AO Second Full Scope Test\SingleRing_atCenter\';
imgFolder = 'E:\BK Testing\06-29-AO Second Full Scope Test\Results\';
% imgFolder = 'C:\Users\Sauron\Desktop\BK Testing\06-17-21 Automated AO test\Test3-5Rings\';

num_Axis = 50;

tiffiles = dir([imgFolder, '*.tiff']);

err_uni = [];
err_ellipticity = [];

tst = tic;

for idx = 1:2

    IMG = imread([imgFolder tiffiles(idx).name]);
    tiffiles(idx).name
   
    IB = imbinarize(IMG); % convert to binary
    bb = imclearborder(IB);
%     bw = bwareaopen(bb,50); % DZ 06-10-21
    biggest = bwareafilt(bb,1); % detect connected components

%     bw = logical(bw.*(~biggest));
%     biggest = bwareafilt(bw,1);
    
    [I,J] = find(biggest); % find the biggest object, I, J are the x and y coordinates of the pixels in the ring

    ellipse_t = fit_ellipse(I,J);
    
    s = regionprops(biggest,{'Orientation'});
%     s.Centroid = [ellipse_t.Y0_in, ellipse_t.X0_in]
    s.Centroid = [189, 296];
    
    s.MajorAxisLength = ellipse_t.long_axis;
    s.MinorAxisLength = ellipse_t.short_axis;

    % Calculate the ellipse line
    theta = linspace(0,2*pi, num_Axis);
    col = (s.MajorAxisLength/2)*cos(theta);
    row = (s.MinorAxisLength/2)*sin(theta);
    M = makehgtform('translate',[s.Centroid, 0],'zrotate',deg2rad(-1*s.Orientation));
    D = M*[col;row;zeros(1,numel(row));ones(1,numel(row))];

    figure(1);
    imshow(IMG)
%     hold on
%     plot(D(1,:),D(2,:),'r','LineWidth',2)
%     plot(D(1,:),D(2,:),'r','LineWidth',2)
%     hold off
    pause(0.5)
    
    intensities = IMG(sub2ind(size(IMG), round(D(2,:)),round(D(1,:))));
    figure(2);
    plot(intensities);
end

toc(tst)
