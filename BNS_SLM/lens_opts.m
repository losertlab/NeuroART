function opts = lens_opts(square)
% 
% lambda: wavelength in nm, default: 1040
% slm_res: pixel resolution of slm (height by width), default: 1536x1536
% slm_dims: size of slm in mm (height by width), default: 30.7x30.7
% focal_length: of lens (in mm), default: 150
% origin: center of lens function, in pixels (y, x), default: 768x768
%
    opts = struct;
    opts.lambda = 1064; % wavelength of the beam in nm
    if nargin < 1 || ~square
        opts.slm_res = [1536, 1536];
        opts.slm_dims = [30.7 30.7];
    elseif nargin >= 1 && square
        opts.slm_res = [1536, 1536];
        opts.slm_dims = [30.7 30.7];
    end
    
    opts.origin = (opts.slm_res + 1)/2;
end
