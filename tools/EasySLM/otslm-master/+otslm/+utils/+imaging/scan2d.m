function im = scan2d(slm, cam, varargin)
% Scans a 2-D region region across device.
%
% This function is similar to :func:`scan1d` except it scans a rectangular
% region in a raster pattern across the surface of the SLM to form a 2-D
% image of the intensity.
%
% Usage
%   im = scan2d(slm, cam, ...) scans a bar region across the device
%   and returns a matrix representing the intensities at each location.
%
% Parameters
%   - slm (:class:`Showable`) -- device to display pattern on.
%     The slm.showComplex function is used to display the pattern.
%     The pattern used for pixels outside the main region depends on
%     the SLM configuration.
%   - cam (:class:`Viewable`) -- device viewing the display.  This device
%     could be a single photo-diode or the average intensity from all
%     pixels on a camera.
%
% Optional named arguments
%   - width [x,y] (numeric)   -- width of the region to scan across the device
%   - stride [x,y] (numeric)  -- number of pixels to step
%   - padding [x0 x1 y0 y1] (numeric) -- offset for initial window position
%   - delay (numeric)     -- number of seconds to delay after displaying the
%     image on the SLM before imaging (default: [], i.e. none)
%   - angle (numeric)     -- direction to scan in (rad)
%   - angle_deg (numeric) -- direction to scan in (deg)
%   - verbose (logical)   -- display additional information about run
%
% See also :func:`image1d`.

% Copyright 2018 Isaac Lenton
% This file is part of OTSLM, see LICENSE.md for information about
% using/distributing this file.

assert(isa(slm, 'otslm.utils.Showable') && isvalid(slm), ...
  'slm must be a valid otslm.utils.Showable');
assert(isa(cam, 'otslm.utils.Viewable') && isvalid(cam), ...
  'cam must be a valid otslm.utils.Viewable');

p = inputParser;
p.addParameter('width', [10, 10]);
p.addParameter('stride', [1, 1]);
p.addParameter('padding', []);
p.addParameter('delay', []);
p.addParameter('angle', []);
p.addParameter('angle_deg', []);
p.addParameter('verbose', true);
p.parse(varargin{:});

% Parse width
rwidth = p.Results.width;
if numel(rwidth) == 1
	rwidth = [rwidth, rwidth];
end

% Parse stride
stride = p.Results.stride;
if numel(stride) == 1
	stride = [stride, stride];
end

% Parse padding
padding = p.Results.padding;
if isempty(padding)
	padding = rwidth;
elseif numel(padding) == 1
	padding = [padding, padding];
end

% Parse angle arguments
angle_rad = [];
if isempty(angle_rad)
	angle_rad = p.Results.angle;
end
if isempty(angle_rad)
	angle_rad = p.Results.angle_deg * pi/180;
end
if isempty(angle_rad)
	angle_rad = 0;
end
angle_rad = angle(exp(1i*angle_rad));

% Calculate width of device (in rotated coordinates)
width(1) = slm.size(1)*sin(angle_rad) + slm.size(2)*cos(angle_rad);
width(2) = slm.size(1)*cos(angle_rad) + slm.size(2)*sin(angle_rad);

offsetx = 1:stride(1):abs(width(1))+padding(1);
offsetx = offsetx - 1 + rwidth(1)/2 - padding(1) - abs(width(1))/2;

offsety = 1:stride(2):abs(width(2))+padding(2);
offsety = offsety - 1 + rwidth(2)/2 - padding(2) - abs(width(2))/2;

im = zeros(length(offsety), length(offsetx));

for ii = 1:length(offsetx)
	for jj = 1:length(offsety)

		if p.Results.verbose
			disp(['Scan2d: [' num2str(jj) ', ' num2str(ii) '] / [' ...
        num2str(length(offsety)) ', ' num2str(length(offsetx)) ']']);
		end

		% Generate pattern
		pattern = otslm.simple.aperture(slm.size, rwidth/2, ...
				'shape', 'rect', 'angle', angle_rad, ...
				'offset', [offsetx(ii), offsety(jj)]);

		% Display the pattern and acquire image
		slm.showComplex(pattern);
    if ~isempty(p.Results.delay)
      pause(p.Results.delay);
    end
		camim = cam.viewTarget();

		% Store the result
		im(jj, ii) = sum(camim(:));
	end
end

