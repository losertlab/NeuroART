function Spks = spikeDeconv(F)


addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

%% OASIS-spike deconvolution setup

addpath(fullfile('OASIS_matlab','functions'));
addpath(fullfile('OASIS_matlab', 'packages', 'oasis'));
addpath(fullfile('OASIS_matlab', 'packages', 'oasis_kernel'));
addpath(fullfile('OASIS_matlab', 'packages', 'constrained-foopsi'));
addpath(fullfile('OASIS_matlab', 'packages', 'MCMC'));
addpath(fullfile('OASIS_matlab', 'packages', 'MCMC', 'utilities'));
addpath(fullfile('OASIS_matlab'));


nCells = size(F,1);
T = size(F,2);
s = zeros(T,nCells);  
for cellInd = 1:nCells
    [~, s(:,cellInd), ~] = deconvolveCa(F(cellInd,:)');
end
                            
% Spks = double(s > 0.005); % convert to binary, 1: spike 0: no spike
Spks = s';

end