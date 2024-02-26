% Example of more advanced beams
%
% Copyright 2018 Isaac Lenton
% This file is part of OTSLM, see LICENSE.md for information about
% using/distributing this file.

% Add toolbox to the path
addpath('../');

sz = [512, 512];      % Size of pattern
padding = 500;        % Padding for FFT

% incident = [];        % Incident beam (use default in visualize)
incident = otslm.simple.gaussian(sz, 150);  % Incident beam (gaussian)
% incident = ones(sz);  % Incident beam (use uniform illumination)

% Calculate beam correction amplitude
beamCorrection = 1.0 - incident + 0.5;
beamCorrection(beamCorrection > 1.0) = 1.0;

% Functions used for generating figures
zoom = @(im, o) im(round(size(im, 1)/2)+(-o:o), round(size(im, 2)/2)+(-o:o));
visualize = @(pattern, o) zoom(abs(otslm.tools.visualise(pattern, ...
    'method', 'fft', 'padding', padding, 'incident', incident)).^2, o);

hf = figure();

%% Adding beam phase to shift beams

pattern = otslm.simple.lgmode(sz, 3, 2, 'radius', 50);
pattern = pattern + otslm.simple.linear(sz, 30);
pattern = otslm.tools.finalize(pattern);

figure(hf);
subplot(4, 4, 1);
imagesc(pattern);

subplot(4, 4, 2);
imagesc(visualize(pattern, 100));

%% Mixing beams with superposition

pattern1 = otslm.simple.linear(sz, 30, 'angle_deg', 90);
pattern2 = otslm.simple.linear(sz, 30, 'angle_deg', 0);

pattern = otslm.tools.combine({pattern1, pattern2}, ...
    'method', 'super');

pattern = otslm.tools.finalize(pattern);

figure(hf);
subplot(4, 4, 5);
imagesc(pattern);

subplot(4, 4, 6);
imagesc(visualize(pattern, 100));

%% Gerchberg-Saxton demonstration

% Generate the target image
im = zeros(sz);
if exist('insertText', 'var')
  im = insertText(im,[7 0; 0 25] + [ 230, 225 ], {'UQ', 'OMG'}, ...
      'FontSize', 18, 'BoxColor', 'black', 'TextColor', 'white', 'BoxOpacity', 0);
  im = im(:, :, 1);
else
  im = otslm.simple.aperture(sz, sz(1)/20);
end

% Setup the propagator methods for GS with the incident beam
prop = otslm.tools.prop.FftForward.simpleProp(im, ...
    'gpuArray', isa(im, 'gpuArray'));
vismethod = @(U) prop.propagate(U .* incident);

% Setup the GS object and then run for 20 iterations
gs = otslm.iter.GerchbergSaxton(im, 'adaptive', 1.0, ...
    'vismethod', vismethod);
gs.run(20);

figure(hf);
subplot(4, 4, 9);
imagesc(gs.phase);

subplot(4, 4, 10);
imagesc(visualize(gs.phase, 100));

%% HG beam with amplitude correction for Gaussian illumination

[pattern, amplitude] = otslm.simple.hgmode(sz, 3, 2, 'scale', 50);

pattern = otslm.tools.finalize(pattern, ...
    'amplitude', beamCorrection.*abs(amplitude));

figure(hf);
subplot(4, 4, 13);
imagesc(pattern);

subplot(4, 4, 14);
imagesc(visualize(pattern, 20));

%% Bessel beam

pattern = otslm.simple.aperture(sz, [ 100, 110 ], 'shape', 'ring');

% Coorect for amplitude of beam
pattern = pattern .* beamCorrection;

% Finalize pattern
pattern = otslm.tools.finalize(zeros(sz), 'amplitude', pattern);

figure(hf);
subplot(4, 4, 3);
imagesc(pattern);

subplot(4, 4, 4);
imagesc(visualize(pattern, 50));

%% Array of LG beams

lgpattern = otslm.simple.lgmode(sz, 5, 0);
grating = otslm.simple.sinusoid(sz, 50, 'type', '2dcart');

pattern = lgpattern + grating;
pattern = otslm.tools.finalize(pattern, 'amplitude', beamCorrection);

figure(hf);
subplot(4, 4, 7);
imagesc(pattern);

subplot(4, 4, 8);
imagesc(visualize(pattern, 100));

%% Binary amplitude LG beam
% This is similar to Lerner et al., Opt. Lett.37 (23) 4826-4828 (2012) 
% https://doi.org/10.1364/OL.37.004826

dmdsz = [512, 1024];
aspect = 2;

% Calculate phase and amplitude for LG beam
[phase, amplitude] = otslm.simple.lgmode(dmdsz, 3, 0, ...
    'aspect', aspect, 'radius', 100);

% Shift the pattern away from the zero-th order (rotate away from artifacts)
phase = phase + otslm.simple.linear(dmdsz, 40, ...
    'angle_deg', 62, 'aspect', aspect);
  
% Finalize the pattern: encoding amplitude and phase into amplitude pattern
pattern = otslm.tools.finalize(phase, 'amplitude', amplitude, ...
    'device', 'dmd', 'colormap', 'gray', 'rpack', 'none');

% Dither the resulting pattern
%   tools.finalize could do this, be we want more control
%   we need to do rpack after dithering, so we call finalize again
pattern = otslm.tools.dither(pattern, 0.5, 'method', 'random');
patternVis = otslm.tools.finalize(pattern, ...
    'colormap', 'gray', 'rpack', '45deg', 'modulo', 'none');

dmdincident = ones(size(patternVis));

figure(hf);
subplot(4, 4, 11);
imagesc(pattern);

subplot(4, 4, 12);
visOutput = abs(otslm.tools.visualise([], 'amplitude', patternVis, ...
    'method', 'fft', 'padding', padding, 'incident', dmdincident)).^2;
visOutput = visOutput(ceil(size(visOutput, 1)/2)-50+(-40:40), ...
    ceil(size(visOutput, 2)/2 +(-40:40)));
imagesc(visOutput);

%% Selecting regions of interest

loc1 = [ 170, 150 ];
radius1 = 75;
pattern1 = otslm.simple.lgmode(sz, 3, 0, 'centre', loc1);
pattern1 = pattern1 + otslm.simple.linear(sz, 20);
pattern1 = otslm.tools.finalize(pattern1, 'amplitude', beamCorrection, ...
    'colormap', 'gray');

loc2 = [ 320, 170 ];
radius2 = 35;
pattern2 = zeros(sz);

loc3 = [ 270, 300 ];
radius3 = 50;
pattern3 = otslm.simple.linear(sz, -20, 'angle_deg', 45);
pattern3 = otslm.tools.finalize(pattern3, 'amplitude', 0.4, ...
    'colormap', 'gray');

background = otslm.simple.checkerboard(sz);

pattern = otslm.tools.mask_regions(background, ...
    {pattern1, pattern2, pattern3}, {loc1, loc2, loc3}, ...
    {radius1, radius2, radius3}, 'shape', 'circle');
  
pattern = otslm.tools.finalize(pattern);

figure(hf);
subplot(4, 4, 15);
imagesc(pattern);

subplot(4, 4, 16);
imagesc(visualize(pattern, 110));

%% Change properties of all figures

for ii = 1:16
  subplot(4, 4, ii);
  axis('image');
  colormap('gray');
  if mod(ii, 2) == 1
    caxis([-pi, pi]);
  end
  set(gca,'YTickLabel', [], 'XTickLabels', []);
end

subplot(4, 4, 11);
caxis([0, 1]);

% function imagesc(im)
% 
%   global kk;
% 
%   maxval = max(im(:)) - min(im(:));
%   if maxval == 0.0
%     maxval = 1.0;
%   end
%   
%   im = (im - min(im(:))) ./ maxval;
% 
%   imwrite(im, ['advanced' num2str(kk) '.png']);
%   kk = kk + 1;
% 
% end

