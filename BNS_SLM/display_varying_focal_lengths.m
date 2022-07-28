function display_varying_focal_lengths(P, opts, focal_lengths)
    assert(size(P, 2) == 2);
    
    if nargin < 2
        opts = dhot_opts();
    end
    
    for k = 1:length(focal_lengths)
        f = focal_lengths(k);
        fprintf('generating mask for k = %d, f = %g\n', k, f);
        mask = dhot(P, opts);
        display_mask(mask, f);
    end
end
