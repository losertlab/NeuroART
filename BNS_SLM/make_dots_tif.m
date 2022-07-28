function make_dots_tif
    P = [-0.6 0; -0.6 0.6; 0 0.6; 0.6 0];
    P(:, 2) = -P(:, 2); % need to flip this
    L = linspace(-1, 1, 500);
    [x y] = meshgrid(L, L);
    F = zeros(length(L));
    for k = 1:size(P, 1)
        F = F + exp(-((x - P(k, 1)).^2 + (y - P(k, 2)).^2)/(2*0.00001*pi));
    end
    F = dre(F);
    imwrite(F, 'dots.tif');
end
