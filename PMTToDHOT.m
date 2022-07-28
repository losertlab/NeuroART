function P_DHOT = PMTToDHOT(xc,yc)
    
    calibrationFile = load('PMTToSlm_x7936y9462_3x.mat');
    cameraToSlm = calibrationFile.cameraToSlm;
    neuronXY = [xc yc];
   
    [xSLM, ySLM]=tforminv(neuronXY,cameraToSlm);
    
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
    