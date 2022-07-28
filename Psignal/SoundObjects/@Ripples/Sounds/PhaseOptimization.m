function  [yo,pho,mi,mo,mvec] = PhaseOptimization(AM,RippleRates,RippleFrequencies,PH,NumIterations,StepSize,T,saf);
mvec = zeros(1,NumIterations);
l = length(RippleRates);
pho = PH;
% Initial stimulus construction
[waveParams,W,Omega] = makewaveparams({AM'},{RippleFrequencies'},{PH'},{RippleRates'});
[y,freqs] = ststruct(waveParams,W,Omega,125,4000,100,T,saf);	
mi = max(max(abs(y)));  % Initial maximum absolute value
mvec(1) = mi;	
for iter = 2:NumIterations,	
   purt = round(rand(l,1))*StepSize - StepSize/2;   % Purturbation vector
   % New stimulus construction
   [waveParams,W,Omega] = makewaveparams({AM'},{RippleFrequencies'},{(PH+purt)'},{RippleRates'});
   [ytry,freqs] = ststruct(waveParams,W,Omega,125,4000,100,T,saf);	
   mvec(iter) = max(max(abs(ytry)));
   % If maximum absolute value decreases, keep new phases
   if mvec(iter) < max(max(abs(y))),
      PH = PH + purt;
      y = ytry;
      pho = PH;
   end
end
yo = y;
mo = max(max(abs(yo))); 
