function error = calcError(imgFolder,liveimg_savepath, ellipticity, num_Axis, type, c_spot, selectedROI, backgroundROI, manualRing, intensity_threshold, lineCoordinates)

% type = 0 --> calculate error  based on ellipticity
% type = 1 --> calculate error  based on uniformity
% ellipticity --> expected value of the ratio,(ellipse_t.long_axis - ellipse_t.short_axis)/ellipse_t.long_axis
% liveimg_savepath = 'C:\Users\Sauron\Desktop\BK Testing\New folder\';
pause(0.1)
tiffiles = dir([imgFolder, '*.tiff']); % '.tiff' for pylon viewer, '.tif' for micromanager
imNum = length(tiffiles);

imgType = 0; % 0: Tiff (Basler), 1: Tif (Hamamatsu)
numPoints = 1;

IMG = imread([imgFolder tiffiles(end).name]);
IMG1 = IMG;

if imgType
    IMG1(IMG1<7000)= 0;
    normFactor = 40000; % Intensity normalization factor
else
    normFactor = 255;
end

% IMG(IMG>20000) = 20000;
% IMG(IMG<7000)= 7000;
% IMG = IMG - 7000;

% IMG(IMG>255) = 255;
% IMG(IMG<50)= 50;
% IMG = IMG - 50;

% IMG = double(IMG)./6000; % Normalize Intensity

if isempty(backgroundROI)
    if ~isempty(c_spot)
        IMG(c_spot(1):c_spot(2), c_spot(3):c_spot(4)) = zeros(c_spot(2)-c_spot(1)+1, c_spot(4)-c_spot(3)+1);
    end

    if ~isempty(selectedROI)
        temp = IMG(selectedROI(1):selectedROI(2), selectedROI(3):selectedROI(4));
        IMG = zeros(size(IMG,1), size(IMG,2));
        IMG(selectedROI(1):selectedROI(2), selectedROI(3):selectedROI(4)) = temp;
    end

   
%     try  

        if type == 0
             IB = imbinarize(IMG); % convert to binary
             bw = imclearborder(IB);
             % bw = bwareaopen(IB,30); % DZ 06-10-21
             biggest = bwareafilt(bw,1); % detect connected components
             [I,J] = find(biggest); % find the biggest object, I, J are the x and y coordinates of the pixels in the ring
             ellipse_t = fit_ellipse(I,J);
            
            s = regionprops(biggest,{'Orientation'});
%             s.Centroid = [ellipse_t.Y0_in, ellipse_t.X0_in];
            s.Centroid = [ manualRing(1) + manualRing(3)/2,  manualRing(2) + manualRing(4)/2];
            s.MajorAxisLength = ellipse_t.long_axis;
            s.MinorAxisLength = ellipse_t.short_axis;

            % Calculate the ellipse line
            theta = linspace(0,2*pi, num_Axis);
            col = (s.MajorAxisLength/2)*cos(theta);
            row = (s.MinorAxisLength/2)*sin(theta);
            M = makehgtform('translate',[s.Centroid, 0],'zrotate',deg2rad(-1*s.Orientation));
            D = M*[col;row;zeros(1,numel(row));ones(1,numel(row))];

            Tr = D';
            ellipse = fit_ellipse(round(Tr(:,1)),round(Tr(:,2)));
            error = abs((ellipse.long_axis - ellipse.short_axis)/ellipse.long_axis - ellipticity)

            fname = sprintf('IMG%d.tiff',imNum);
            imwrite(IMG, [liveimg_savepath,fname]);

        elseif type == 1
            IB = imbinarize(IMG); % convert to binary
             bw = imclearborder(IB);
             % bw = bwareaopen(IB,30); % DZ 06-10-21
             biggest = bwareafilt(bw,1); % detect connected components
             [I,J] = find(biggest); % find the biggest object, I, J are the x and y coordinates of the pixels in the ring
             ellipse_t = fit_ellipse(I,J);

            s = regionprops(biggest,{'Orientation'});
            s.Centroid = [ellipse_t.Y0_in, ellipse_t.X0_in];
            s.MajorAxisLength = ellipse_t.long_axis;
            s.MinorAxisLength = ellipse_t.short_axis;

            % Calculate the ellipse line
            theta = linspace(0,2*pi, num_Axis);
            col = (s.MajorAxisLength/2)*cos(theta);
            row = (s.MinorAxisLength/2)*sin(theta);
            M = makehgtform('translate',[s.Centroid, 0],'zrotate',deg2rad(-1*s.Orientation));
            D = M*[col;row;zeros(1,numel(row));ones(1,numel(row))];

            % Visualize the result

            intensities = IMG(sub2ind(size(IMG), round(D(2,:)),round(D(1,:))));
            figure(2)
            imshow(IMG)
            hold on
            plot(D(1,:),D(2,:),'r','LineWidth',2)
        %     plot(D(1,:),D(2,:),'r','LineWidth',2)

            error = std(double(intensities));
            
        elseif type == 3
            
            bw = imbinarize(IMG1); % convert to binary
            IB = bwareaopen(bw,5);
            IMG = double(IMG)./normFactor; % Normalize Intensity
            s = regionprops(IB,'centroid'); % Find the centroids of the points
            error = 0;
            length(s);
            
            xmin = Inf;
            xmax = -Inf;
            ymin = Inf;
            ymax = -Inf;
            for itr = 1:length(s)
                ctr= s(itr).Centroid;
                x = ctr(1);
                y = ctr(2);
                if x<xmin
                    xmin = x;
                end
                if x>xmax
                    xmax = x;
                end
                if y<ymin
                    ymin = y;
                end
                if y>ymax
                    ymax = y;
                end
            end
            
            for i = 1:length(s)
                
                centroid = s(i).Centroid;
                
                if length(s)==2 && i ~= 1 && numPoints==1
                    continue;
                end
                
                if length(s)>2 && i ~= 1 && numPoints==1
                    continue;
                end
                
                if numPoints == 4 && abs(centroid(1)-xmin) > 15 && abs(xmax-centroid(1))>15
                    continue;
                end
                if numPoints == 4 && abs(centroid(2)-ymin) > 15 && abs(ymax-centroid(2))>15
                    continue;
                end
                
                
                if imgType % Hamamatsu
                    imageSizeX = 512;
                    imageSizeY = 512; 
                else % Basler
                    imageSizeX = 640;
                    imageSizeY = 480;
                end
                [columnsInImage, rowsInImage] = meshgrid(1:imageSizeX, 1:imageSizeY);
                % Next create the circle in the image.
                centerX = centroid(1);
                centerY = centroid(2);
                innerRadius = 0;
                outerRadius = 5;
                array2D = (rowsInImage - centerY).^2 ...
                + (columnsInImage - centerX).^2;
                ringPixels = array2D >= innerRadius.^2 & array2D <= outerRadius.^2;

                overlap = double(IMG).*double(ringPixels);

                intensities = overlap(overlap>0);
                

%                 error = error + sum(intensities.*log2(intensities));     %Entropy
                
%                 err = sum(intensities)^2/sum(intensities.^2);  % Sharpness
                
                if numPoints == 4 && length(s) == 2 && i == 2
                    error1 = -error;
                    error2 = mean(intensities);
                    error = -min(error1, error2);
                elseif numPoints == 4 && length(s) == 1
                    error = 0;
                else
                    error = error - mean(intensities);
                end
                
                theta = linspace(0,2*pi, num_Axis);
                col = (5)*cos(theta);
                row = (5)*sin(theta);
                M = makehgtform('translate',[centroid, 0],'zrotate',deg2rad(0));
                D = M*[col;row;zeros(1,numel(row));ones(1,numel(row))];

                % Visualize the result

                figure(5)
                imshow(IB)
                hold on
                plot(D(1,:),D(2,:),'r','LineWidth',1)
                pause(0.1)
            end
            fname = sprintf('IMG%d.mat',imNum);
%             imwrite(IMG, [liveimg_savepath,fname]);
            save([liveimg_savepath,fname],'IMG');
%             error
%             error = error/length(s) % average error      
        elseif type == 4
            
            IMG = double(IMG)./normFactor; % Normalize Intensity
            [centers, radii, metric] = imfindcircles(IMG,[15 30]);
            length(radii);
            error = 0;
            
            for i = 1:length(radii)
                
                centroid = centers(i,:);
                
                if i > numPoints
                    break;
                end
                
                if imgType % Hamamatsu
                    imageSizeX = 512;
                    imageSizeY = 512; 
                else % Basler
                    imageSizeX = 640;
                    imageSizeY = 480;
                end
                [columnsInImage, rowsInImage] = meshgrid(1:imageSizeX, 1:imageSizeY);
                % Next create the circle in the image.
                centerX = centroid(1);
                centerY = centroid(2);
                innerRadius = radii(i)-2; % thickness of the annulus =  pixels
                outerRadius = radii(i)+2;
                array2D = (rowsInImage - centerY).^2 ...
                + (columnsInImage - centerX).^2;
                ringPixels = array2D >= innerRadius.^2 & array2D <= outerRadius.^2;

                overlap = double(IMG).*double(ringPixels);

                intensities = overlap(overlap>0);
                
                if numPoints == 4 && length(s) == 2 && i == 2
                    error1 = -error;
                    error2 = mean(intensities);
                    error = -min(error1, error2);
                elseif numPoints == 4 && length(s) == 1
                    error = 0;
                else
                    error = error - mean(intensities);
%                     error = error + (1-metric(i));
                end
                
                theta = linspace(0,2*pi, num_Axis);
                col = (radii(i))*cos(theta);
                row = (radii(i))*sin(theta);
                M = makehgtform('translate',[centroid, 0],'zrotate',deg2rad(0));
                D = M*[col;row;zeros(1,numel(row));ones(1,numel(row))];

                % Visualize the result

%                 figure(5)
%                 imshow(IMG, [0,1]);
%                 hold on
%                 plot(D(1,:),D(2,:),'r','LineWidth',1)
%                 pause(0.1)
            end
            fname = sprintf('IMG%d.mat',imNum);
%             imwrite(IMG, [liveimg_savepath,fname]);
            save([liveimg_savepath,fname],'IMG');
            
        else
%             bw = imbinarize(IMG); % convert to binary
%             IB = bwareaopen(bw,5);
%             s = regionprops(IB,'centroid','EquivDiameter');
            
            thet = linspace(0,2*pi, 50);
            col = (manualRing(3)/2)*cos(thet);
            row = (manualRing(3)/2)*sin(thet); % manualRing(4)/2
            centroid = [manualRing(1) + manualRing(3)/2, manualRing(2) + manualRing(4)/2];
            M = makehgtform('translate',[centroid, 0],'zrotate',deg2rad(0));
            D = M*[col;row;zeros(1,numel(row));ones(1,numel(row))];
            
            col1 = (manualRing(3)/2 - 5)*cos(thet);
            row1 = (manualRing(3)/2 - 5)*sin(thet); % manualRing(4)/2
            D1 = M*[col1;row1;zeros(1,numel(row1));ones(1,numel(row1))];
            
            figure(20)
            imshow(IMG,[3500,40000])
            hold on
            plot(D(1,:),D(2,:),'r','LineWidth',2)
            hold on
            plot(D1(1,:),D1(2,:),'b','LineWidth',2)
%             hold off;
%             
            if imgType % Hamamatsu
                imageSizeX = 512;
                imageSizeY = 512; 
            else % Basler
                imageSizeX = 640;
                imageSizeY = 480;
            end
            [columnsInImage, rowsInImage] = meshgrid(1:imageSizeX, 1:imageSizeY);
            % Next create the circle in the image.
            centerX = manualRing(1) + manualRing(3)/2;
            centerY = manualRing(2) + manualRing(4)/2;
            innerRadius = manualRing(3)/2 - 5;
            outerRadius = manualRing(3)/2;
            array2D = (rowsInImage - centerY).^2 ...
            + (columnsInImage - centerX).^2;
            ringPixels = array2D >= innerRadius.^2 & array2D <= outerRadius.^2;
            
            overlap = double(IMG).*double(ringPixels)./normFactor;
            
            intensities = overlap(overlap>0);
            
            error = -mean(intensities);

%             error = sum(intensities.*log2(intensities)) % Entropy
            
            
%             lower_bound = prctile(intensities,25);
            
%             darkRegions = intensities(intensities<lower_bound);
            
            
%             x = round([lineCoordinates(1,1), lineCoordinates(2,1)]); % x-coordinates of the endpoints
%             y = round([lineCoordinates(1,2), lineCoordinates(2,2)]); % y-coordinates of the endpoints
%             line = improfile(IMG, x, y);
% 
%             npixels = length(line)
%             q1 = round(0.4*npixels) + 1;
%             q3 = round(0.6*npixels);
%                 
%             error = abs(max(line(1:q1)) - max(line(q3:end)))
            
%             if type == 2
%                 error = 100/mean(overlap(:)) 
%             else
%                 error = mean(overlap(:))/std(overlap(:))
%             end
            
%             error = 100/mean(darkRegions(:))

%             error = mean(overlap(:))/std(overlap(:))

%             IB = imbinarize(overlap); % convert to binary
%             bw = imclearborder(IB);
%             biggest = bwareafilt(bw,1);
%             s = regionprops(biggest,overlap,{'WeightedCentroid'});
%             error = (centerX - s.WeightedCentroid(1))^2 + (centerY - s.WeightedCentroid(2))^2
            
%             intensities = overlap(overlap > 0);
%             error1 = std(intensities)
%             if type == 2
%                 error = 100/mean(overlap(:))
%             else
%                 intensities = overlap(overlap > 0);
%                 error = std(intensities)
% 
%             end
          
%             k = find(overlap(overlap > 0.5*intensity_threshold));
%             error = 1000/length(k)
        
%             error = error2;
%             error = error1*error2*error2*error2;
            fname = sprintf('IMG%d.mat',imNum);
%             imwrite(IMG, [liveimg_savepath,fname]);
            save([liveimg_savepath,fname],'IMG');

%             ringMask = ringPixels./sum(ringPixels(:));
%             Conv = conv2(double(IMG),ringMask,'same');
%             error =  10000/(max(Conv(:))-5000)
            
%             intensities = IMG(sub2ind(size(IMG), round(D(2,:)),round(D(1,:))));
            
%             maxInt = [];
%             rx = manualRing(3)/2 + 1;
%             ry = manualRing(4)/2 + 1;
% 
%             for ax = 0:num_Axis-1 % 4 axis
%                 theta = (pi/num_Axis)*ax;
%                 y = round([centroid(2) - ry*sin(theta), centroid(2) + ry*sin(theta)]); % x-coordinates of the endpoints
%                 x = round([centroid(1) - rx*cos(theta), centroid(1) + rx*cos(theta)]); % y-coordinates of the endpoints
%                 line = improfile(IMG, x, y);
% 
%                 npixels = length(line);
%                 q1 = round(0.4*npixels) + 1;
%                 q3 = round(0.6*npixels);
% 
%                 
%                 maxInt = [maxInt, max(line(1:q1)), max(line(q3:end))];
                
%             end
%             maxInt
%             error = 10000/mean(double(maxInt))

%             error = std(double(maxInt))/mean(double(maxInt))
        end
%     catch
%         if type == 0
%             error = 1;
%         else
%             error = 255;
%         end
%     end
    
else
%     error = sum(sum(IMG(backgroundROI(1):backgroundROI(2), backgroundROI(3):backgroundROI(4)))); 
end


%     BWout = ~biggest;
%     BWout = imfill(BWout,'holes');
%     statOuter = regionprops(BWout,{'EquivDiameter','Centroid'});
%     outerRadius = statOuter.EquivDiameter/2;
    % Measure the inner radius
%     BWin = imclearborder(biggest);
%     BWin = imopen(BWin, strel('disk',1)); % Remove noise
    
%     statInner = regionprops(biggest,{'EquivDiameter','Area'})
%     innerRadius = statInner.EquivDiameter/2;
%     perimeter = 2*pi*innerRadius;
%     area  = statInner.Area;
    
%     error = 1 - 4*pi*area/perimeter^2;
    
%     bw = bwareaopen(biggest,30);
%     se = strel('disk',2);
%     bw = imclose(bw,se);
%     bw = imfill(bw,'holes');
%     [B,L] = bwboundaries(bw,'noholes');
%     
%     stats = regionprops(L,'Area','Centroid');
%     boundary = B{1};
% 
%     % compute a simple estimate of the object's perimeter
%     delta_sq = diff(boundary).^2;    
%     perimeter = sum(sqrt(sum(delta_sq,2)));
%   
%     % obtain the area calculation corresponding to label 'k'
%     area = stats.Area;
%   
%     % compute the roundness metric
%     error = 1 - 4*pi*area/perimeter^2;
    
%     stats = regionprops(biggest, 'Orientation');
%     error = 90 - abs(stats.Orientation);
    
%     stats = regionprops(biggest, 'Eccentricity');
%     error = stats.Eccentricity;
    
%     error = abs((ellipse_t.long_axis - ellipse_t.short_axis)/ellipse_t.long_axis - ellipticity);

%     Xcenter = ellipse_t.X0_in;
%     Ycenter = ellipse_t.Y0_in;
%     r = ellipse_t.long_axis/2;
%     maxInt = [];
% 
%     for ax = 0:num_Axis-1
%         theta = (pi/num_Axis)*ax;
%         y = round([Xcenter - r*sin(theta), Xcenter + r*sin(theta)]); % x-coordinates of the endpoints
%         x = round([Ycenter - r*cos(theta), Ycenter + r*cos(theta)]); % y-coordinates of the endpoints
%         line = improfile(IMG, x, y);
% 
%         npixels = length(line);
%         q1 = round(0.25*npixels) + 1;
%         q3 = round(0.75*npixels);
% 
%         if npixels > 4
%             maxInt = [maxInt, max(line(1:q1)), max(line(q3:end))];
%         elseif npeaks > 0
%             maxInt = [maxInt, line(1), line(end)];
%         end
%     end
%     maxInt
