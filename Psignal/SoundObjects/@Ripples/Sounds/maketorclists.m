
a1rv = (4:4:48)';
T = round(1000/4); % base period

a1rf = (1.4:-0.2:-1.4);

velnum = length(a1rv);
frqnum = length(a1rf);

a1am = ones(velnum,frqnum);
a1ph = rand(velnum,frqnum)*360 - 180;
a1rv = a1rv*ones(1,frqnum); 
a1rf = ones(velnum,1)*a1rf; 

% Phase optimization
for s = 1:frqnum,
 disp(['Phase optimization: Stimulus ' num2str(s)])
 PLOT = 1;
 [yo,a1ph(:,s),mi,mo,mvec] = phsopt(a1am(:,s),a1rv(:,s),a1rf(:,s),a1ph(:,s),250,5,T,1000,PLOT);
end

% Inverse-polarity
a1am = [a1am a1am];
a1rv = [a1rv a1rv];
a1rf = [a1rf a1rf];
a1ph = [a1ph a1ph-180];

% Write ripple lists
fname = 'TORC2';
ripwrite(fname,a1am,a1rf,a1ph,a1rv);
