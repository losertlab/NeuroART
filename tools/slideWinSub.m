% Sliding window dF/F calculation
% Inputs: F       - raw fluorecense
%         winsize - baseline window size
%         percent - lower percent of baseline values to average
% Output: dFF     - relative change in fluorescense in percent
function dFF = slideWinSub(F,winsize,percent)
nRois = size(F,1);
nFrames = size(F,2);
dFF = zeros(size(F));
%M = zeros(1, nrois);
%SD = zeros(1, nrois);
for j = 1 : nRois
  
    for k = 1 : nFrames
            lWin = 1;
            rWin = min(k+winsize, nFrames);
            if k > winsize
                lWin = max(1,k-2*winsize);
                rWin = k;    
            end
            percentWin = floor(percent/100*(rWin-lWin));
            tWin = sort(F(j,(lWin:rWin)),'ascend');
            F0 = mean(tWin(1:percentWin));
            dFF(j,k) = 100 * (F(j,k)- F0) / F0;  
%             if dFF(j,k) > 700
%                 dFF(j,k) = F0; % added by DZ 031423 to handle glitches occured due to stimulation
%             end
    end
    
%     for k = 1 : nFrames
%             lWin = max(1,k-winsize);
%             rWin = min(k+winsize, nFrames);
%             percentWin = floor(percent/100*(rWin-lWin));
%             tWin = sort(F(j,(lWin:rWin)),'ascend');
%             F0 = mean(tWin(1:percentWin));
%             dFF(j,k) = 100 * (F(j,k)- F0) / F0;   
%     end
    
    %twin = sort(nDFF(j,:));
    %pcentwin = floor(percent/100*(nframes));
    %M(j) = mean(twin(1:pcentwin));
    %SD(j) = std(twin(1:pcentwin));
    
end