function hologram = volume2hologram(volume, varargin)
% Generate hologram from 3-D volume by un-mapping the Ewald sphere
%
% This function is only the inverse of :func:`hologram2volume` when
% interpolation use_weight is disabled and there is no blurring.
%
% Usage
%   hologram = volume2hologram(volume, ...) calculates the overlap
%   of the Ewald sphere with the volume and projects it to a 2-D hologram.
%
% Parameters
%   - volume (numeric) -- 3-D volume to un-map
%
% Optional named arguments
%   - 'interpolate' (logical) -- interpolate between the nearest two
%     pixels in the z-direction.  (default: True)
%   - 'padding' (numeric)     -- padding in the axial direction (default 0).
%   - 'focal_length' (numeric) -- focal length in pixels (default:
%     estimated from z)
%   - 'use_weight' (logical)  -- use weights when sampling interpolated pixels
%
% See also :func:`hologram2volume` and :class:`prop.FftEwaldForward`.

% Copyright 2018 Isaac Lenton
% This file is part of OTSLM, see LICENSE.md for information about
% using/distributing this file.

p = inputParser;
p.addParameter('interpolate', true);
p.addParameter('focal_length', []);
p.addParameter('padding', 0);
p.addParameter('use_weight', false);
p.parse(varargin{:});

% Calculate the focal length of the lens (assuming no padding)
xsize = min(size(volume, 1), size(volume, 2));
zsize = size(volume, 3) - 2*p.Results.padding;

focallength = p.Results.focal_length;
if isempty(focallength)
  focallength = ((xsize/2).^2 + zsize.^2)/(2*zsize);
  zsize_min = zsize;
else
  zsize_min = focallength - sqrt(focallength.^2 - (xsize/2).^2);
end

% Calculate padding due to zsize
zoffset = max((zsize - zsize_min)/2, 0);

% Allocate memory for hologram
hologram = zeros(size(volume, 1), size(volume, 2));

% Assign volume values to hologram
for ii = 1:size(hologram, 2)
  for jj = 1:size(hologram, 1)

    % Calculate x and y locations in 3-D space
    xloc = ii - 0.5 - size(hologram, 2)/2;
    yloc = jj - 0.5 - size(hologram, 1)/2;

    % Calculate z location
    zloc = sqrt(focallength.^2 - xloc.^2 - yloc.^2);

    if isreal(zloc)

      zidx = focallength - zloc + zoffset;
      value = 0.0;

      if p.Results.interpolate

        % Interpolate between neighbouring pixels
        zidx_low = floor(zidx + 0.5) + p.Results.padding;
        zidx_high = ceil(zidx + 0.5) + p.Results.padding;
        
        if p.Results.use_weight
          weight = mod(zidx, 1);

          if zidx_low >= 1 && zidx_high <= size(volume, 3)
            value = weight*volume(jj, ii, zidx_low);
            value = value + (1.0-weight)*volume(jj, ii, zidx_high);
          elseif zidx_low >= 1 && zidx_low <= size(volume, 3)
            value = volume(jj, ii, zidx_low);
          elseif zidx_high >= 1 && zidx_high <= size(volume, 3)
            value = volume(jj, ii, zidx_high);
          end
        else
          if zidx_low >= 1 && zidx_high <= size(volume, 3)
            value = volume(jj, ii, zidx_low);
            value = value + volume(jj, ii, zidx_high);
          elseif zidx_low >= 1 && zidx_low <= size(volume, 3)
            value = volume(jj, ii, zidx_low);
          elseif zidx_high >= 1 && zidx_high <= size(volume, 3)
            value = volume(jj, ii, zidx_high);
          end
        end
      else
        zidx = round(zidx + 0.5) + p.Results.padding;

        % If point within volume, assign it
        if zidx >= 1 && zidx <= size(volume, 3)
          value = volume(jj, ii, zidx);
        end
      end

      hologram(jj, ii) = value;
    end

  end
end

