function [regFrame, tx, ty] = regseqRT_coderGPU(template,currentFrame)
    
    % Add kernelfun pragma to trigger GPU kernel creation
    coder.gpu.kernelfun;
    
    [tx, ty] = phase_corr_reg(template, fft2(currentFrame));
    regFrame = imtranslate(currentFrame, [tx, ty]);
    
end

function [tx, ty] = phase_corr_reg(F0, F)
    X = ifftn(F0 .* conj(F), 'nonsymmetric');
    [max1, argmax1] = max(X); % find column maximums
    [~, argmax2] = max(max1);
    tx = argmax2;
    ty = argmax1(argmax2);
    [m, n] = size(F0);
    tx = adjust_t(tx, m);
    ty = adjust_t(ty, n);
end

function t = adjust_t(t0, dim)
    if t0 > fix(dim / 2)
        t = t0 - dim - 1;
    else
        t = t0 - 1;
    end
end
