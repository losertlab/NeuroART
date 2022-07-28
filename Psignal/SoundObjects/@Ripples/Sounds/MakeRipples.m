function MakeRipples(Duration, SavePath, RippleRates, RippleFrequencies, f0, BW, SF)
%Initialize Ripple Parameters
if nargin < 2
    warning('Must enter a SavePath')
    return
end
if nargin < 3 || isempty(RippleRates)
    RippleRates = (4:4:24)';
end
if nargin < 4 || isempty(RippleFrequencies)
    RippleFrequencies = (1.4:-0.2:-1.4);
end
if nargin < 5 || isempty(f0)
    f0 = 250;
end
if nargin < 6 || isempty(BW)
    BW = 5;
end
if nargin < 7 || isempty(SF)
    SF = 80000;
end
T = round(1000/4);
RateNum = length(RippleRates);
FrqNum = length(RippleFrequencies);
AM = ones(RateNum,FrqNum);
PH = rand(RateNum,FrqNum)*360 - 180;
RippleRates = RippleRates*ones(1,FrqNum);
RippleFrequencies = ones(RateNum,1)*RippleFrequencies;
%Phase optimization
NumIterations=250;
StepSize=5;
for s = 1:FrqNum,
    disp(['Phase optimization: Stimulus ' num2str(s)])
    [yo,PH(:,s),mi,mo,mvec] = PhaseOptimization(AM(:,s),RippleRates(:,s),RippleFrequencies(:,s),PH(:,s),NumIterations,StepSize,T,1000);
end
PH=round(PH);
%Inverse-polarity
AM = [AM AM];
RippleRates = [RippleRates RippleRates];
RippleFrequencies = [RippleFrequencies RippleFrequencies];
PH = [PH PH-180];
%Generate ripples
T0 = Duration; 	% actual duration in seconds
CF = 1;	% component-spacing flag, 1->log spacing, 0->harmonic
df = 1/100;	% freq. spacing, in oct (CF=1) or in Hz (CF=0)
RO = 0;	% roll-off in dB/Oct
AF = 1;	% amplitude flag, 1->linear, 0->log (dB)
Mo = 0.9;	% amp. total mod: 0<Mo<1 (Af=1); 0<Mo dB (Af=0)
wM = 120;%Maximum temporal velocity to consider in Hz (DEFAULT = 120)
PhFlag = 1;% Flag which determines how to set the compnent flags
fname  = 'TORC_424';
nstim = size(AM,2);
ripParams = [T0 f0 BW SF CF df RO AF Mo wM PhFlag];
for i = 1:nstim
    rippleList =  [AM(:,i),RippleRates(:,i),RippleFrequencies(:,i),PH(:,i)];
    rippleList_rad =  [AM(:,i),RippleRates(:,i),RippleFrequencies(:,i),PH(:,i)./180.*pi];
    s = RippleGenerator(rippleList_rad, ripParams);
    wavname=[SavePath '\' fname '_' num2str(i) '.wav'];
    disp(['Saving ' wavname])
    audiowrite(wavname,.999.*(s./max(abs(s))),SF);
    %save parameters with phase in degrees to be compatible with ststims code
    a = writeTorcInfo([SavePath '\' fname '_' num2str(i) '.txt'],rippleList,ripParams);
end


