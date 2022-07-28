clear;

P = [-0.5 0; 0.5 0];

opts = dhot_opts();
opts.apply_vortex = true;
opts.vortex_modes = 1;

mask = dhot(P, opts);