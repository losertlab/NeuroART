%% OASIS-spike deconvolution setup

addpath(fullfile('OASIS_matlab','functions'));
addpath(fullfile('OASIS_matlab', 'packages', 'oasis'));
addpath(fullfile('OASIS_matlab', 'packages', 'oasis_kernel'));
addpath(fullfile('OASIS_matlab', 'packages', 'constrained-foopsi'));
addpath(fullfile('OASIS_matlab', 'packages', 'MCMC'));
addpath(fullfile('OASIS_matlab', 'packages', 'MCMC', 'utilities'));
addpath(fullfile('OASIS_matlab'));


%% Spike deconvolution

winSize = 1600;

s = zeros(1600,10);
for i = 1:10
[~, s(:,i), ~] = deconvolveCa(F(i,end-winSize+1:end)');
end

Spks = double(s > 0.004);
Spks = Spks';

%% Online Synchrony Analysis

SyncAnalysisOnline;

T = size(Spks,2);
Rmax = 4;
Wi = 10;
Wa = 20;
Ws = winsize/(Wi*Wa);
ii = floor(T / (Wi * Wa));

%% Figure update

imagesc(kron(JstatIm(2:end,:), ones(1,1)), [-1 1]); colormap redblue;
yticks( 1:size(nR,1)-1 ); yticklabels( num2str( [2:size(nR,1)]' ) );
ylabel({'r^{th}-Order','Synchrony'});
tspacing = Wi*4;
T0 = (ii-Ws)*Wi*Wa;
xlim([0, T-T0]/Wa); xticks([0:tspacing*Wa:(T-T0)]/Wa); xticklabels( num2str([T0:tspacing*Wa:T]') );
xlabel('Time Bin');
colorbar;