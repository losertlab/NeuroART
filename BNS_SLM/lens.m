function L = lens(opts)
% creates a hyperbolic fresnel lens for SLM
% for arguments, see lens_opts.m function
    
    if nargin == 0
        opts = lens_opts()
    end
    % TODO: validate options struct
    
    lambda = opts.lambda;
    slm_res = opts.slm_res;
    slm_dims = opts.slm_dims;
    focal_length = opts.focal_length;
    origin = opts.origin;
    
    % Set up grid in the DOE plane for computing lens function 
    y_max = slm_dims(1);
    x_max = slm_dims(2);
   % M = 2*slm_res(1); %SA 042121 To fix the lens origin mismatch and avoid hard coding in multiple places
   % N = 2*slm_res(2); %SA042121 
    M = slm_res(1);%SA 042121 To fix the lens origin mismatch and avoid hard coding in multiple places
    N = slm_res(2);%SA 042121
    dy = y_max/M;
    dx = x_max/N;
    ys = linspace(dy, y_max, M);
    xs = linspace(dx, x_max, N);
    [X, Y] = meshgrid(xs, ys);
    y0 = dy*origin(1);
    x0 = dx*origin(2);
    
    % Precompute quantities.
    f_abs = abs(focal_length);
    f_sgn = sign(focal_length);
    f_sqr = focal_length.^2;
    wl = lambda*1e-6; % convert to mm
    
    % Compute lens function.
    L = (f_sgn/wl)*(sqrt(f_sqr + (Y - y0).^2 + (X - x0).^2) - f_abs);
end
