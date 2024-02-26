function [fvals, freqs] = calculateImageSliceFreq(img, theta, offset, swidth)
% Calculate the frequency spectrum of an image slice
%
% Usage
%   [fvals, freqs] = calculateImageSliceFreq(img, theta, offset, swidth)
%   calculates the frequency spectrum of a slice through an image.
%
% Parameters
%   - img     -- Real valued image to calculate spectrum from
%   - theta   -- Angle of slice (radians)
%   - offset  -- Offset of slice (pixels)
%   - swidth  -- width of slice (pixels) to average over
%
% Returns
%   - fvals   -- Calculated amplitudes
%   - freqs   -- Corresponding frequencies
%
% This function is used for the power spectrum plots in the calibration
% functions. The function samples a slice of pixels from an image.
% Arguments control the slice position, width and angle. The function
% returns the spatial frequencies and complex amplitudes. For example
% usage, see :class:`ui.utils.CalibrationStepFarfield`.

% Copyright 2019 Isaac Lenton
% This file is part of OTSLM, see LICENSE.md for information about
% using/distributing this file.

% TODO: Should this function move elsewhere?

imwidth = size(img, 2);
imheight = size(img, 1);
len = sqrt(imwidth.^2 + imheight.^2);

% Calculate frequency of slice
[xx0, yy0] = meshgrid(1:imwidth, 1:imheight);
[xx, yy] = meshgrid(-len/2:len/2, (0.5:(swidth-0.5)) + offset);
xxR = xx .* cos(theta) + imwidth/2 - yy .* sin(theta);
yyR = xx .* sin(theta) + imheight/2 + yy .* cos(theta);
vals = interp2(xx0, yy0, img, xxR, yyR);
vals(isnan(vals)) = 0.0;
vals = sum(vals, 1);
fvals = fft(vals);
fvals = fvals(1:ceil(length(fvals)/2));
freqs = 1:length(fvals);

end
