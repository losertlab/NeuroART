tStartDN = tic;
disp('Denoising fluorescence traces with Plenz Lab approach.....');
[nrois , ~] = size(npSubFluo');
T = 1/exptVars.frameRate;

% Normalizing raw F to get rid of negative vals before dF/F calc % 7/2020
minNpSubFluo = min(npSubFluo(:));
adjF = npSubFluo - minNpSubFluo;
maxAdjF = max(adjF(:));
normF = adjF ./ maxAdjF;

%smoothed raw dR/R or dF/F - commented out 9/16, uncommented 6/2020 for slower volume data
if T >= 0.2
    smoothwin = 3;
elseif T >= 0.13 && T <= 0.2
    smoothwin = 5; %7
elseif T <=0.13
    smoothwin = 9; %13
end
symmFLAG = 1; % 1 ->symmetric, 2->asym, T = frame time, Tau -> 1.5

for j = 1 : nrois % NEED FOR IMPROVEMENT - ELIMINATE FOR LOOP?
    
    %%%%% SYMMETRIC (new as of Sept2016)
    tmp = normF(:,j)';
    npSubFluoSmooth(:,j) = smooth2(tmp,symmFLAG,smoothwin,T,1.5); %Maybe smooth after DFF calc?
    
end

normF = normF'; % added by dulara on 05/24/2020

tElapsedDN = toc(tStartDN);
disp(['     Time elapsed for denoising was ', num2str(tElapsedDN/60) , ' minutes']);

percent = percentBaselineSub;
DFF.smooth = slideWinSub(npSubFluoSmooth',winsize,percent,exptVars.scope);
DFF.raw = slideWinSub(normF,winsize,percent,exptVars.scope);