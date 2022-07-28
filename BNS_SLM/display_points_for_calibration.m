function display_points_for_calibration(P, opts, focal_length)
    assert(size(P, 2) == 2);
    
    if nargin < 2
        opts = dhot_opts();
    end
    
    if nargin < 3
        focal_length = -60;
    end
    
    for k = 1:size(P, 1)
        p = P(k, :);
        fprintf('generating mask for k = %d, p = (%g, %g)\n', k, ...
                p(1), p(2));
        mask = dhot(p, opts);
        display_mask(mask, focal_length);
    end
end
