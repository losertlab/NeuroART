function opts = dhot_opts()
    opts = struct;
    
    % the dimensions and pixels are multiplied by 2 to calculate the phase
    % mask for an imaginary double-sized SLM. The GUI code will crop this to size after dhot
    
    opts.slm_res = [30.7 30.7]*1.5; % size of the slm surface in mm, multiply by 1.5 for high res mode
    opts.slm_dims = [1536 1536]; % size of the slm surface in pixels
    opts.lambda = 1064*1e-6;  %1040*1e-6; % wavelength of beam in mm
    opts.f = 250; % % effective focal length for equation 1 in Curtis, Koss Grier ref paper, in mm
%     opts.f = 9; % effective focal length for 20x top objective
    opts.xi = 0.5; % modified Gerchberg-Saxton parameter
    opts.niters = 3; % number of Gerchberg-Saxton iterations, universal parameter and can be changed using the slider in the GUI
    opts.lens = -602; % initial global focal length of virtual lens. This can be changed using the slider in the GUI
    opts.apply_axicons = false;
    opts.axicon_radii = []; % radii for individual axicons---if just a
                            % number, then apply the same radius to
                            % all axicons
    opts.vortex_modes = []; % modes (l) for individual vortices---inputted within the GUI for each beamlet, given on each row in the GUI table,
                            % if just a single number in the coded array, then apply the same radius to
                            % all vortices, values are specified using the table in the GUI
                            % l is in arbitrary units and does not necesssarily indicate actual physical distance
    opts.z_offsets = [];
    opts.eff_aper_rad = 1; % TODO: effective aperture radius---see
                           % last page of DHOT paper
                           
    % Zernike coefficients for abberation correction, added by Dulara for the abberation correction routine
    
    opts.zernike = [0,0,0,0,0,0,0,0,0,0,0,0,0,0];
%     
    opts.abberationCorrection = 1; % If this value is equal to 1, we are doing abberation correction, otherwise no abberation correction
    
%     opts.rRadius=0.04;%0.0039; % 0.023 is equivalent to vortex mode 15; 0.018 equivalent to vortex_mode 10; 
%                           % 0.014 is equivalent to vortex mode 5
%     opts.rDensity=1.5;%  Use 10 for a full circle, use 1 for points on a ring                       
end