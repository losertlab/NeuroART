function [phase, amplitude] = lgmode(sz, amode, rmode, varargin)
% Generates the phase pattern for a LG beam
%
% Usage
%   pattern = lgmode(sz, amode, rmode, ...) generates phase pattern.
%
%   [phase, amplitude] = lgmode(...) also generates the amplitude pattern.
%
% Parameters
%   - sz -- size of the pattern
%   - amode -- azimuthal order
%   - rmode -- radial order
%
% Optional named parameters
%   - 'radius'    radius   -- scaling factor for radial mode rings
%   - 'p0'        p0       -- incident amplitude correction factor
%     Should be 1.0 (default) for plane wave illumination (w_i = Inf),
%     for Gaussian beams should be p0 = 1 - radius^2/w_i^2.
%     See `Lerner et al. (2012) <https://doi.org/10.1364/OL.37.004826>`__
%     for details.
%
%   - 'centre'      [x, y] --   centre location for lens (default: sz/2)
%   - 'offset'      [x, y] --   offset after applying transformations
%   - 'aspect'      aspect --   aspect ratio of lens (default: 1.0)
%   - 'angle'       angle  --   Rotation angle about axis (radians)
%   - 'angle_deg'   angle  --   Rotation angle about axis (degrees)
%   - 'gpuArray'    bool   --   If the result should be a gpuArray
%
% In order to generate pure LG beams it is necessary to control both the
% beam amplitude and phase. However, if the purity of the beam is not
% important then the phase pattern is often sufficient to generate the
% desired beam shape.

% Copyright 2018 Isaac Lenton
% This file is part of OTSLM, see LICENSE.md for information about
% using/distributing this file.

% Check mode numbers
assert(rmode >= 0, 'Radial mode must be >= 0');
assert(floor(rmode) == rmode, 'Radial mode must be integer');
assert(floor(amode) == amode, 'Azimuthal mode must be integer');

% Parse inputs
p = inputParser;
p = addGridParameters(p, sz, 'skip', 'type');
p.addParameter('radius', min(sz(1), sz(2))/10);
p.addParameter('p0', 1.0);
p.parse(varargin{:});

% Generate coordinates
gridParameters = expandGridParameters(p);
[~, ~, rho, phi] = otslm.simple.grid(sz, gridParameters{:});

% Calculate azimuthal part of pattern
phase = amode .* phi ./ (2.0*pi);

% Calculate laguerre polynomials in radial direction
maxrho = max(rho(:));
if p.Results.gpuArray
  maxrho = gather(maxrho);
end
rr = linspace(0.0, maxrho, round(2*maxrho));
Lpoly_rr = laguerreL(rmode, abs(amode), (rr./p.Results.radius).^2);

% Interpolate between points using splines
if p.Results.gpuArray
  Lpoly_rho = interp1((rr./p.Results.radius).^2, Lpoly_rr, ...
    (rho./p.Results.radius).^2, 'linear');
else
  Lpoly_rho = interp1((rr./p.Results.radius).^2, Lpoly_rr, ...
      (rho./p.Results.radius).^2, 'spline');
end

% Calculate radial part of phase
phase = phase + (sign(Lpoly_rho) > 0)*0.5;

% Calculate the amplitude too
if nargout == 2

  amplitude = (rho./p.Results.radius).^abs(2*amode) ...
      .* Lpoly_rho .* exp(-p.Results.p0*0.5*(rho./p.Results.radius).^2);

  % Normalize amplitude maximum value
  amplitude = amplitude ./ max(abs(amplitude(:)));

end

end

