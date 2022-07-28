%% This program synthesizes RSS stimuli
fs = 200000; %Sampling Rate
f0 = 4000; %4kHz
maxOct = 4; %Max number of octave steps from f0
octStep = 1/64; %Step size for carrier frequency
binSize = 1/16; % Bin size in octave, 
nFreqPerBin = binSize/octStep;
nBin = maxOct/binSize;
x = 0:octStep:maxOct-octStep; %Octave step index
f = f0.*2.^x; %List of carrier frequencies
nCarrier = length(f);
rss_dur = 0.2; %Stimulus duration
nSamps = floor(rss_dur * fs);
nRSS = 500;
%Generate gaussian distributed level for each bin
mean_atten = 0;
atten_sd = 15;
[i,j] = meshgrid(1:nBin,1:nBin);
sigma_p = 2;
C = exp(-(i-j).^2 / sigma_p^2);
atten_seq = mvnrnd(mean_atten*ones(nBin,1),atten_sd^2*C, nRSS)';
atten_seq2 = zeros(nCarrier,nRSS);
for i = 1:nBin
    rng = (i-1)*nFreqPerBin+1:i*nFreqPerBin;
    atten_seq2(rng,:) = repmat(atten_seq(i,:) ,nFreqPerBin,1);
end
phase_rnd = 2*pi*rand(nCarrier,nRSS);
%10 ms ramp
ramp_dur = 0.01;
nRamp = floor(ramp_dur*fs);
ramp = ones(1,nSamps);
ramp(1:nRamp) = sin(linspace(0,pi/2,nRamp));
ramp(end-nRamp+1:end) = sin(linspace(pi/2,0,nRamp));
%Fill in param structure and save
param.f0 = f0;
param.maxOct = maxOct;
param.octStep = octStep;
param.f = f;
param.rss_dur = rss_dur;
param.fs = fs;
param.nSamps = nSamps;
param.atten_seq = atten_seq;
param.phase_rnd = phase_rnd;
param.nFreqPerBin = nFreqPerBin;
param.mean_atten = mean_atten;
param.atten_sd = atten_sd;
param.nBin = nBin;
param.sigma_p = sigma_p;
param.C = C;
home=fileparts(which('Psignal'));
folder_name = [home '\Waveforms\RSS'];
mkdir(folder_name)
fn = 'Params.mat';
save([folder_name '\' fn],'param');
%% Generate RSS
t = repmat(0:1/fs:(nSamps-1)/fs,nCarrier,1);
freq = repmat(f',1,nSamps);
for i = 1:nRSS
    fprintf('generating rss %d\n',i);
    phase_tmp = repmat(phase_rnd(:,i),1,nSamps);
    atten_tmp = diag(10.^(-atten_seq2(:,i)/20));
    c_wave = atten_tmp * cos(2*pi*freq.*t + phase_tmp);
    wave = sum(c_wave)/nCarrier.*ramp;
    fn = ['RSS_' num2str(i) '.mat'];
    save([folder_name '\' fn],'wave');
end
