function display_squares(side_lengths, focal_length)
    P = [1 1; 1 -1; -1 1; -1 -1]/2;
    
    for k = 1:length(side_lengths)
        l = side_lengths(k);
        fprintf('generating mask for k = %d, l = %g\n', k, l);
        mask = dhot(l*P);
        display_mask(mask, focal_length);
    end
end
