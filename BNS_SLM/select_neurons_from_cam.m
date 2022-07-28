function [P_DHOT, neuronXY] = select_neurons_from_cam(cam_img, cameraToSlm)

 [~, ~, ext] = fileparts(cam_img);
    if strcmp(ext, '.tif')
        img = imread(cam_img);
    elseif strcmp(ext, '.raw')
        mapped = memmapfile(cam_img, 'Format', format);
        img = reshape(mapped.data, m, n);
    else
        error(fprintf('unsupported file type: %s', cam_img));
    end
    img = mat2gray(img);
h(1)=1;
    figure(h(1))
    imshow(img);
    [xCam, yCam] = getpts; %  manually select centers of the neurons 
    ptsIdx = [(1:length(xCam))' xCam yCam];
    
    for nn = 1:length(xCam)
    hold on
    figure(h(1))
    neuronNumb = num2str(ptsIdx(nn));
    text(xCam(nn), yCam(nn),neuronNumb,'FontSize',14,'Color','w')
    end
    neuronXY = [xCam yCam];
   
    [xSLM, ySLM]=tforminv(neuronXY,cameraToSlm);
    
%     Tinv = affine2d(cameraToSlm.tdata.Tinv); %for inverse transformation 
%     T = affine2d(cameraToSlm.tdata.T);
%     [x2Cam y2Cam]= transformPointsForward(T, x1, y1);
%     
%     [xSLM, ySLM] = transformPointsInverse(Tinv, xCam, yCam);


    % There's a reflection in both x and y axis and a scaling of~2.245 that
    % is taking place this part has been hard coded to correct for that.
    %
    
%     xDHOT=-2.245*(xSLM-541)/540; % hard coded  by Samira 20191001
%    
%     yDHOT=-2.245* (ySLM-541)/540; 
%     P_DHOT = [xDHOT yDHOT];
%     

%% 20211028

% 30 units in the table --> 122 pixels in theoritical intensity image (2304x2304)
% 1 pixel --> 30/122 units

    xDHOT= 0.2833*(xSLM-1153)/1152; %DZ 10/21
    yDHOT= 0.2833* (ySLM-1153)/1152; 
    P_DHOT = [xDHOT yDHOT];
    
    
%     xDHOT= -2.245*(xSLM-1153)/1152; %DZ 10/21
%     yDHOT= -2.245* (ySLM-1153)/1152; 
%     P_DHOT = [xDHOT yDHOT];

end
    