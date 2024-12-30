function [JstatIm,ii] = SyncAnalysisOnline(F10,Wi,Wa,Ws,Rmax)

%% Synchrony Analysis -- History-Independent Online Implementation
%
% This script is a demonstration of the online implementation of
% history-independent synchrony analysis. Simulated (or real) data that
% are loaded in their entirety are then segmented and inputted to the
% algorithm to mimic closed-loop usage.
%
% A key assumption is that all spiking events in the mark space up to
% Rmax-order marks occur with sufficient frequency to be modelled with a
% constant rate over window length Wa -- Wa and Rmax should be set
% accordingly
%
% Shoutik Mukherjee, smukher2@umd.edu

% Input parameters
% Spks: NxT matrix deconvolved spikes (N: number of cells, T: number of time bins)
% Wa : analysis window -- # bins over which parameters are constant   % try increasing Wa
% Wi : inference window -- # analysis windows after which J-stats are updated
% Ws : Number of inference windows displayed at a time, Ws < Wsm
% Rmax : maximum order of synchrony considered for analysis

% Output parameters
% JstatIm: J statistic evaluated on the last Ws*Wi analysis windows
% ii: number of inference windows for which Jstats were computed


%% Spike deconvolution using OASIS-Matlab
nCells = size(F10,1);
syncAnalysisWinSize = size(F10,2);
s = zeros(syncAnalysisWinSize,nCells);  
for cellInd = 1:nCells
    [~, s(:,cellInd), ~] = deconvolveCa(F10(cellInd,:)');
end
                            
Spks = double(s > 0.005); % convert to binary, 1: spike 0: no spike
Spks = Spks';
%% Analysis Hyperparameters -- User inputs
%%% Statistical Inference Hyperparameters
% Wi = 10; % inference window -- # analysis windows after which J-stats are updated
alpha = 0.01; % False Discovery Rate
Wsm = 100; % # analysis windows over which J-stats are filtered/smoothed
% Ws = Wsm/5; % Number of inference windows displayed at a time, Ws < Wsm

%%% MkPP Model Estimation Hyperparameters
% Rmax = 4;
% Wa = 20; % analysis window -- # bins over which parameters are constant
beta = 0.92; % forgetting factor   % try beta = 0.9, 0.95  wa = 10,20

%% Analysis Hyperparameters -- Can be kept fixed
%%% NoncentChi2FiltSmooth Hyperparameters
initF = 1e-1;
rho = 1;
NN = 20;
Nem = 10;

%%% MkPP Model Estimation Hyperparameters
C = size(Spks,1);
C_star = 0;
for rr=1:Rmax
    C_star = C_star + nchoosek(C, rr);
end
incl_idx = [1:C_star]';

kappa = 1e-3; % GD step size
GD_Iter = 2000;

T = size(Spks,2);
I = T / (Wi * Wa);

%% Analysis Loop
%%% Initialization
X = zeros(C_star,1);
Devs = [];
h = [];
Ms = [];
n = [];
nR = [];

nIm = zeros(C, Ws*Wi*Wa);
nR_Im = zeros(Rmax, Ws*Wi*Wa);
JstatIm = zeros(Rmax, Ws*Wi);
% J = []; % DZ added for testing

%%% Main Loop
for ii=1:I
    for jj=1:Wi        
        
        n = [ n, Spks(:, (ii-1)*Wi*Wa + (jj-1)*Wa + [1:Wa]) ];
        nIm = circshift(nIm, -Wa, 2);
        nIm(:, end+1-Wa:end) = Spks(:, (ii-1)*Wi*Wa + (jj-1)*Wa + [1:Wa]);
        
        tmp = 2.^([0 : C-1]) * Spks(:, (ii-1)*Wi*Wa + (jj-1)*Wa + [1:Wa]); % DZ: sum(spike*(2^cellID))
        ord_idx = sum(de2bi( 2.^([0 : C-1]) * Spks(:, (ii-1)*Wi*Wa + (jj-1)*Wa + [1:Wa]), C ), 2); % DZ: sum of digits after converting to binary (i.e. no: of spikes in a bin)

        nk_star = zeros(C_star, Wa); %DZ: classify into different spiking events (marked point representation)
        nkR = zeros(Rmax, Wa); %DZ: classify into different spike counts (range: 1:Rmax)
        for kk=1:Wa
            if ord_idx(kk)<=Rmax && ord_idx(kk)>0 && tmp(kk)<= C_star % DZ added last condition to make sure the size of nk_star does not increase 
                nk_star(tmp(kk), kk) = 1;
                nkR(ord_idx(kk), kk) = 1;
            end
        end
        
        nR = [nR, nkR];
        nR_Im = circshift(nR_Im, -Wa, 2);
        nR_Im(:, end+1-Wa:end) = nkR;
        
        Xbar = sum( nk_star, 2 ) / Wa;
        X = beta*X + Xbar;
        
        [Devk, Mk, hk] = MkPP_Updates(X, Wa, beta, Rmax, C, incl_idx, kappa, GD_Iter, (ii-1)*Wi + jj );
        
        Devs = [Devs, Devk];
        h = [h, hk];
        if isempty(Ms)
            Ms = Mk;
        end
    end    
    
    if size(Devs,2)>Wsm
        Devs(:, 1:Wsm) = [];
        h(:, 1:Wsm) = [];
    end
    
    %%% J-statistic computation
    Jstat = zeros(size(h));
    for r=2:Rmax
        sz0 = initF / var(Devs(r,:) - Ms(r));
        [nukSmth, ~ , ~ , ~, ~ , ~] = NoncentChi2FiltSmooth(Devs(r,:),Ms(r),sz0,rho,NN,Nem);
        Jstat(r,:) = h(r,:) .* ( (1 - alpha) < chi2cdf(Devs(r,:), Ms(r)) ) .* ( 1 - alpha - ncx2cdf( chi2inv( 1-alpha, Ms(r)), Ms(r), nukSmth(:)' ) );
    end
    
    JstatIm = circshift(JstatIm, -Wi ,2);
    JstatIm(:, end+1-Wi:end) = Jstat(:, end+1-Wi:end);
    
%     J = [J, Jstat(:, end+1-Wi:end)]; %DZ added for testing
   
end

% In this example, JstatIm contains the Jstatistic value evaluated on
% the last Ws*Wi = 200 analysis windows --> i.e 200xWa = 4000 time bins
% Gets updated at each inference window --> i.e circular shifted at each WixWa = 200 time bins