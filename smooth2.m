function F1 = smooth2(tmp,cenflag,win,T,tau)
%cenflag ->  1 == symmetric with peak in middle, 0 asym with peak in 1st pos
%win size should be odd

if mod(win,2) == 0
    %window is even so add 1
    win = win +1;
end

%create window
mid = floor(win/2)+1;
wf = ones(1,win);

tauwin = round(tau/T);
for j = mid:win
    wf(j) = wf(j-1)*exp(-tau*T);
end
wf(1:mid-1) = wf(end:-1:mid+1);
wf = wf./sum(wf);
if ~cenflag    
    wf(1:mid-1) = 0;
end
%figure; plot(wf);   

%multiply raw data at each frame by window
nsamps = numel(tmp);

tmp =[tmp, tmp(1:win+1)];
F1 = zeros(1,nsamps); % added to allow mex compilation

for j = 1 : nsamps
    F1(j) = sum(tmp(j:j+win-1).*wf); 
end

% F1 = F1(1:nsamps); % commented on 07/08/2020


%figure; plot(tmp); hold on; plot(F1,'k');
