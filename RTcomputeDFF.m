function [newDFF, newRawFluo,t1,t2,t3,t4] = RTcomputeDFF(roiBW2,npBWout,RegImg,winsize,percent,Fraw, T, symmFLAG, smoothwin, minNpSubFluo,maxAdjF)
% if isempty(gcp('nocreate'))
%     parpool('local'); %parallel channel
% end
%% PREALLOCATE FLUO AND NPFLUO MATRICES
tst = tic;

n = size(roiBW2,1);

rawFluo = zeros(n,1); % inverted dimensions for efficiency -- Dulara 05/24/2020
npFluo = zeros(n,1);
percNP = 0.7; %DW 11232015- added for Npil correction - percentage of neuropil signal to substract
% dimensions = size(RegImg);

t1 = toc(tst);
tst = tic;
%% CALCULATE FLUORESCENCE TRACES for ALL STIMS

% for nn = 1:n
%     [r,c,~]=find(roiBW2{nn});
%     rawFluo(nn) = mean(RegImg(sub2ind(dimensions,r,c)));
%     [rNp,cNp,~] = find(npBWout{nn});
%     npFluo(nn) = mean(RegImg(sub2ind(dimensions,rNp,cNp)));
% end

for nn = 1:n
    [r,c]=find(roiBW2{nn}~=0);
    
    Fluo = zeros(numel(r),1);
    for i=1:numel(r)
        Fluo(i) = RegImg(r(i),c(i));
    end
    rawFluo(nn) = mean(Fluo);
end

for nn = 1:n
    [rNp,cNp] = find(npBWout{nn} ~= 0 );
    
    FluoNp = zeros(numel(rNp),1);
    for i=1:numel(rNp)
        FluoNp(i) = RegImg(rNp(i),cNp(i));
    end
    npFluo(nn) = mean(FluoNp);
end

rawFluo = (rawFluo - (percNP * npFluo) - minNpSubFluo)./maxAdjF; % Normalize to get rid of negative values
Fraw = horzcat(Fraw,rawFluo); % append the fluorencence of the latest frame to the end of the existing fluorenscence matrix

t2 = toc(tst);
tst = tic;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% PUT THE DE-NOISING, PARSING AND DFF CALCULATIONS HERE - %% DW05192016 added for trialBased analyses

%% apply assymetrical (exponential) filter to traces - smoothing - MODIFIED FROM PLENZ LAB AVALANCHE CODE

% fluoAllSmooth = zeros(n,smoothwin+1); % e.g. smooth fluorecence values of the last 10 frames

for j = 1:n % n = number of good ROIs
    smraw = smooth2(Fraw(j,end-smoothwin:end),symmFLAG,smoothwin,T,1.5); 
    Fraw(j,end-smoothwin+1:end) = smraw(1:smoothwin);
    
%     if FrameID < smoothwin
%         smraw = smooth2(Fraw(j,end-smoothwin:end),symmFLAG,smoothwin,T,1.5); 
%         Fraw(j,end-FrameID:end) = smraw(end);
%     else
%         Fraw(j,end-smoothwin:end) = smooth2(Fraw(j,end-smoothwin:end),symmFLAG,smoothwin,T,1.5); %Maybe smooth after DFF calc 
%     end
    
%     fluoAllSmooth(j,:) = smraw(end-winsize+1:end);
end

newDFF = zeros(n,smoothwin);

t3 = toc(tst);
tst = tic;

for j = 1:n
%     f = fluoAllSmooth(j,:);
    f = Fraw(j,:);
    percentWin = floor(winsize*percent/100); % because current frame is the last frame
    tWin = sort(f,'ascend');
    F0 = mean(tWin(1:percentWin));
    
    for k = 1:smoothwin
        newDFF(j,k) = 100 * (f(end-k+1)- F0) / F0; 
    end
      
end

newRawFluo = Fraw(:,end-smoothwin+1:end);

t4 = toc(tst);
end
