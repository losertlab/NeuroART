clear;

% Hi, I'm Samira, and I made a change.

% P = [0 0];
P = [-0.5 0; 0.5 0];

opts = dhot_opts();
opts.apply_axicons = true;
opts.axicon_radii = 100;

mask = dhot(P, opts);