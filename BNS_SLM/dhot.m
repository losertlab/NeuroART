function [mask, intensity] = dhot(P, opts, calculateIntensity)
% 
% P: [N x 2] matrix of locations of optical traps in range [-1, 1]x[-1, 1]
% This is not true the range is between ~ [-2.4, 2.4], scaling issue needs to be fixed
% 
% opts: struct containing parameters for function (see 'dhot_opts' function)

%% Variables in the Paper vs. variables in the code
  
% r --> P
% rho --> sqrt(x^2 + y^2)

%%

    assert(size(P, 2) == 2); % check whether P has both x and y coordinates
    N = size(P, 1);  % The number of optical traps / beamlets, this is equal to the number of selected rows in the table of the GUI

    assert(nargin >= 1);
    if nargin < 2, opts = dhot_opts(); end
    
    assert(all(mod(opts.slm_dims, 2) == 0));

    lambda = opts.lambda; % wavelength of the beam in mm
    f = opts.f; % effective focal length of the optical train in mm

    d = min(opts.slm_res); % temporary, nonmeasured size of the image plane, assessed via square region of the SLM in mm
    n = opts.slm_dims(1);  % size of the square region of the SLM in pixels (1536)
    m = opts.slm_dims(2);  % size of the square region of the SLM in pixels (1536)
    
    %VVV SA(042121): Is this added to double the size of the phase mask?
    % We have to do a similar thing when calculating the lens phase mask
    % otherwise the center of the lens phase mask will not be at [0 0]. I
    % fixed this in the code. The focal length calculations might have been
    % affected as well
    
    %% Double the size of the phase mask
%     d = 2*d; % Commented by samira 05/04/21, multiplication is done in dhot_opts
%     m = 2*m;
    %%
    
    dA = 1/(m*m); % differential area used in Eq. (3) 
    
    % If d = 30.7 mm -->  map [0 30.7] to [-15.35 15.35]
    dmax = d/2;
    dmin = -d/2;

    % map the normalized coordinates in P to the actual measured size of the image plane (need to figure out the correct scaling)
    P(:, 1) = dmax*P(:, 1); 
    P(:, 2) = dmax*P(:, 2);

    % Compute meshgrid for use later
    lin = linspace(dmin, dmax, m);
    [x, y] = meshgrid(lin, lin);  % In the paper, rho^2 = x^2 + y^2

%     assignin('base','x',x);
%     assignin('base','y',y);
    
    % Parameter for modified Gerchberg-Saxton algorithm.
    xi = opts.xi;

    % Topological charge for optical vortices
    apply_vortex = ~isempty(opts.vortex_modes);
    if apply_vortex
        L = opts.vortex_modes;
        if isscalar(L)
           L = L*ones(N, 1); 
          
        else
            assert(isvector(L));
            assert(length(L) == N);
        end
    end
    
    % Axicons
    apply_axicons = ~isempty(opts.axicon_radii);
    if apply_axicons
        R = d; % R = "effective aperture radius in the SLM plane"
        gamma = opts.axicon_radii;
        if isscalar(gamma)
            gamma = gamma*ones(N, 1);
        else
            assert(isvector(gamma));
            assert(length(gamma) == N);
        end
    end
    
    % Individual lenses
    apply_trap_lenses = ~isempty(opts.z_offsets);
    if apply_trap_lenses
        z = opts.z_offsets;
        if isscalar(z)
            z = z*ones(N, 1);
        else
            assert(isvector(z));
            assert(length(z) == N);
        end
    end
    
    % Precompute some values to speed things up:
    rho = sqrt(x.*x + y.*y); % Changed (Samira Dec072020) 
    rho_sq = rho.^2; 
    
    theta = atan2(y, x);
    
    dimX = size(rho_sq,1);
    dimY = size(rho_sq,2);
    
    % Initialize epsilon to so that energy is normalized.
    % normfactor = m*n*h*h/N;
    %*** we do not understand this normalization above, which is not in the parent paper 
    
    epsilon = zeros(N,opts.niters,'single');
    alpha = zeros(N,opts.niters,'single');
    phi = zeros(dimX,dimY,N,'single'); % single precision for fast calculations
    
%     epsilon = zeros(N,opts.niters);
%     alpha = zeros(N,opts.niters);
%     phi = zeros(dimX,dimY,N); % added by Dulara
    
    alpha(:, 1) = ones(N, 1)/sqrt(N); % epsilon_j = alpha_j * exp(i* phi_j)
    
    r_dot_rho = bsxfun(@times,x,reshape(P(:,1),1,1,N))+bsxfun(@times,y,reshape(P(:,2),1,1,N));
%     assignin('base','r_dot_rho',r_dot_rho);
    phi_trap = 2*pi*r_dot_rho/(lambda*f);
    
    if apply_vortex
        phi_LG = bsxfun(@times,theta,reshape(L,1,1,N));
        phi = phi + phi_LG;
        max_rho = max(rho(:)); % maximum value of rho matrix, we need this to normalize rho such that 0<rho<1
        
        if opts.abberationCorrection  % abberation correction routine, added by Dulara, 07/21
            if opts.zernike(1) ~= 0
                C = opts.zernike(1);
                phi = phi - 2*pi*C*zernike(1,-1,theta,rho./max_rho)./lambda; 
            end 
            if opts.zernike(2) ~= 0
                C = opts.zernike(2);
                phi = phi - 2*pi*C*zernike(1,1,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(3) ~= 0
                C = opts.zernike(3);
                phi = phi - 2*pi*C*zernike(2,-2,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(4) ~= 0
                C = opts.zernike(4);
                phi = phi - 2*pi*C*zernike(2,0,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(5) ~= 0
                C = opts.zernike(5);
                phi = phi - 2*pi*C*zernike(2,2,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(6) ~= 0
                C = opts.zernike(6);
                phi = phi - 2*pi*C*zernike(3,-3,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(7) ~= 0
                C = opts.zernike(7);
                phi = phi - 2*pi*C*zernike(3,-1,theta,rho./max_rho)./lambda; 
            end 
            if opts.zernike(8) ~= 0
                C = opts.zernike(8);
                phi = phi - 2*pi*C*zernike(3,1,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(9) ~= 0
                C = opts.zernike(9);
                phi = phi - 2*pi*C*zernike(3,3,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(10) ~= 0
                C = opts.zernike(10);
                phi = phi - 2*pi*C*zernike(4,-4,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(11) ~= 0
                C = opts.zernike(11);
                phi = phi - 2*pi*C*zernike(4,-2,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(12) ~= 0
                C = opts.zernike(12);
                phi = phi - 2*pi*C*zernike(4,0,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(13) ~= 0
                C = opts.zernike(13);
                phi = phi - 2*pi*C*zernike(4,2,theta,rho./max_rho)./lambda; 
            end  
            if opts.zernike(14) ~= 0
                C = opts.zernike(14);
                phi = phi - 2*pi*C*zernike(4,4,theta,rho./max_rho)./lambda; 
            end  
        end
    end
    if apply_axicons % Fixed by Samira (Dec072020)
       
        phi_ax = bsxfun(@times,max(0,  rho/R),reshape(gamma,1,1,N));
        phi = phi + phi_ax;
       
    end
    if apply_trap_lenses
        phi_trap_lens = bsxfun(@times,2*pi*rho_sq/(lambda*f*f),reshape(z,1,1,N)); % add comments to explain these improvements
        phi = phi + phi_trap_lens;
    end

%     fprintf('iter = 1\n');
    
%     epsilon(:,1) = alpha(:, 1).*exp(1i*pi/2*rand(N,1)); % random initial phases
    
    epsilon(:,1) = alpha(:, 1);
    exp_sum = exp(1i*(phi_trap-phi));
    
    %%
    
    E = sum(bsxfun(@times,exp_sum,reshape(epsilon(:,1),1,1,N)),3); % Eq. (1) of the dhot paper
    alpha1_sq = ones(N,1)/N; % added by Dulara, since alpha(:, 1) = ones(N,1)/sqrt(N)
    
    
    for iter = 2:opts.niters % iterations in the GS algorithm, Eq. (3) of the dhot paper
%         fprintf('iter = %d\n', iter);

        % beginning of GS algorithm
        
        % Compute phi^{(n)} from equation (2) and E.
        
        phi_sum = (E./abs(E))./exp_sum;  
        
        % Compute next epsilon values using equation (3) and phi.
        
        epsilon(:, iter) = reshape(sum(sum(phi_sum,1),2),[1,N])*dA;
            
        % Replace amplitudes using equation (4).
        
        alpha(:, iter) = abs(epsilon(:, iter));
        new_alpha = (1 - xi)*alpha(:, 1) + xi*alpha1_sq./alpha(:, iter);
        
        epsilon(:,iter) = new_alpha.*exp(1i*angle(epsilon(:,iter)));
        
        % end of GS algorithm
        
        E = sum(bsxfun(@times,exp_sum,reshape(epsilon(:,iter),1,1,N)),3); % Compute E from equation (1) which is the early form of the phase mask
        
    end
    
%     intensity = abs(ifft2(E(:, :)));
    mask = (angle(E) + pi)/(2*pi); % Ensure that the values are in the range [0,1]
%     A = exp(-(1/25)*(x.*x + y.*y)); % Gaussian function to blur the componenets in the peripheri of the phase mask
%     mask = mask.*A;

 if calculateIntensity
    intensity = abs(fftshift(fft2(exp(1i*mask*2*pi)))); %%SA 042121 To look at the intensity at the object plane
 else
     intensity = [];     
 end
 
%% SA 042121 (entire section) Virtual lens added to the entire phase mask 
    % SA 050421 Modified again by Samira to eliminate the lens_opts function
  % is_square_lens = true; %SA commented out 050421 
  % lopts = lens_opts(is_square_lens); %SA commented out 050421
    
%   mask = add_virtualLens(mask, opts); % uncomment this line if you need to add the virtual lens inside the dhot function

%% 05/09/21 Dulara moved this section to the GUI code for speeding up the calculations
%     lopts = struct;
%     lopts.focal_length = opts.lens;
%     lopts.lambda = 1040;
%     lopts.slm_res = opts.slm_dims; %SA050421 multiplication by 2 was taken out; SA042121 multiplied by 2 to create a lens phase mask with double the size->> note that dims is mislabeled in dhot_opts
%     lopts.slm_dims= opts.slm_res;
%     lopts.origin = (opts.slm_dims + 1)/2;
%     mask = mod(mask + lens(lopts), 1); % 2pi modulo (samira) 

   %%%%% Output %%%%%%
    
%      filename=['Vmode' num2str(opts.vortex_modes) '_Ring_' num2str(opts.rRadius) '_res_' num2str(stepSize) '.tif'];
% %     % savepath='/Users/Samira/Desktop/ringOpmtimization/';
%      testImage=(fftshift(intensity));
%      testImage=imcrop(testImage, [500 500 80 80]);
%      imwrite(testImage, filename);
%     
%   imwrite(fftshift(intensity),'theoriticalPattern.tif') %writes the theoretical intensity
% %     % image for calibration purposes with the intensity pattern yielded at the sample (08/19/2019-Samira)
% 
% %     subplot(1, 3, 3);
% %     plot(1:iter, sum(alpha.^2, 1), '-x');
% %     xlim([0 iter+1]);
% 
%     % imwrite(mask, 'mask.png');

end
