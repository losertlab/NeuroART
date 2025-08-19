function verify_Cam2SLM(cameraToSlm, cameraPointPatternImg, ...
                           fakePointPatternImg)
    % "fake" calibration image for test pattern
    fakePointPattern = imread(fakePointPatternImg);

    % camera image of test pattern
    cameraPointPattern = imread(cameraPointPatternImg);
    
    % PMT image of a number seven (replaced with a calibration image)
  
    Q = [0 0; 1 0; 1 1; 0 1; 0 0].*(ones(5, 1)*size(fakePointPattern));
    
    tform1 = affine2d(cameraToSlm.tdata.T);
    
    %[p, q] = transformPointsForward(affine2d(cameraToSlm.tdata.T), Q(:, 1), Q(:, 2));
    [p, q] = transformPointsForward(affine2d(cameraToSlm.tdata.T), Q(:, 1), Q(:, 2));
    figure; 
    hold on; 
    imshow(cameraPointPattern); 
    hold on;
    plot(p, q);
    uiwait;
    
   
end
