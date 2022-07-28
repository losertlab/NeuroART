function display_slm_points_in_camera_space(P, cameraToSlm, cameraToPmt, ...
                                            cameraPointPatternImg, ...
                                            fakePointPatternImg)
    dots_calib_img = imread(fakePointPatternImg);
    dots_img = imread(cameraPointPatternImg);
    P = P.*(ones(size(P, 1), 1)*size(dots_calib_img));
    T = affine2d(cameraToSlm.tdata.T);
    [p, q] = transformPointsForward(affine2d(cameraToSlm.tdata.T), P(:, 1), P(:, 2));
    figure; 
    hold on; 
    imshow(dots_img); 
    hold on; 
    scatter(p, q);
    uiwait;
end
