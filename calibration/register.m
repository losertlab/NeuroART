function transform = register(path1, path2) %path1 ->fixed image, path 2->moving image moving image will be transformed to the fixed image coordinates. 
    img1 = im2double(imread(path1));
    if ndims(img1) > 2
        assert(ndims(img1) == 3);
        img1 = mean(img1, 3);
    end
    
    img2 = im2double(imread(path2));
    if ndims(img2) > 2
        assert(ndims(img2) == 3);
        img2 = mean(img2, 3);
    end
    
    fixed_img = adapthisteq(imsmooth(im2single(img1)));
    moving_img = single(mat2gray(img2));
    
    [moving_pts, fixed_pts] = cpselect(moving_img, fixed_img, 'Wait', ...
                                       true);

%     transform = cp2tform(moving_pts, fixed_pts, 'similarity');
    
    transform = cp2tform(moving_pts, fixed_pts, 'affine');
    
    [reg_img, xdata, ydata] = imtransform(moving_img, transform, ...
                                          'Fill', 255);
    
    figure;
    imshow(fixed_img);
    hold on;
    h = imshow(reg_img, 'XData', xdata, 'YData', ydata);
    set(h, 'AlphaData', 0.5);
    h = imshow(fixed_img);
    set(h, 'AlphaData', 0.5);
    
    uiwait;
end

function img = imsmooth(img)
    img = impyramid(impyramid(img, 'reduce'), 'expand');
end
