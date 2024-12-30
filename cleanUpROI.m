clean_start = tic;

DFF.smoothClean = DFF.smooth((min(DFF.smooth')> 2*median(min(DFF.smooth))),:); % DW09232016 changed this to be data-based rather than hardcoded / ZB09302016 changed mean to median
xcRaw = xc;
ycRaw = yc;

% convert to sparse matrices for efficiency during real time analysis
n = size(roiBW2,1);
for nn = 1:n
    roiBW2{nn} = sparse(roiBW2{nn});
    npBWout{nn} = sparse(npBWout{nn});
end

clean_finish = toc(clean_start);
fprintf('Removing bad ROIs took %.1f minutes\n',clean_finish/60)

total_finish = toc(total_start);
fprintf('Script completed successfully in %.1f minutes\n',total_finish/60);