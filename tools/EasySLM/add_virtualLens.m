function m = add_virtualLens(mask, opts)

 % 05/09/21 Dulara moved this section to the GUI code for speeding up the calculations
 lopts = struct;
 lopts.focal_length = opts.lens;
 lopts.lambda = 1064;
 lopts.slm_res = opts.slm_dims; %SA050421 multiplication by 2 was taken out; SA042121 multiplied by 2 to create a lens phase mask with double the size->> note that dims is mislabeled in dhot_opts
 lopts.slm_dims= opts.slm_res;
 lopts.origin = (opts.slm_dims + 1)/2;
 m = mod(mask + lens(lopts), 1); % 2pi modulo (samira), add virtual lens
 
end