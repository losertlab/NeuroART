function EstimateTF
%% This program uses the recorded response to a noise played through a speaker to estimate (1) the transfer function from the speaker to the
%% microphone and (2) the "Inverse" transfer function, which is the equalizing spectrum that flatens the spectral output of the speaker. The
%% inverse transfer function is found by divding (subtracting for dB) an idealized flat spectrum by the forward transfer function.
global globalparams CalbBand Response
R = globalparams.R;
R.Fs = globalparams.SR;
[b a] = butter(6,[globalparams.Fband(1) globalparams.Fband(2)]./(R.Fs/2),'bandpass');
H = abs(freqz(b,a,length(Response),R.Fs,'whole'));
ResponseSpec =(1/length(Response))*abs(fft(Response));
ResponseSpec=smooth(ResponseSpec,2^10);
ResponseSpecdB = VolumeConversion(ResponseSpec,'V2dB',globalparams.microphone);
%Estimate whitening filter
f = linspace(0,R.Fs,length(ResponseSpecdB));
fidx = find(f>=globalparams.Fband(1) & f<=globalparams.Fband(2));
WhiteSpecdB = min(ResponseSpecdB(fidx))-ResponseSpecdB;
R.WhiteningSpec = VolumeConversion(WhiteSpecdB','dB2V',globalparams.microphone);
R.WhiteningSpec = R.WhiteningSpec.*H';
figure(globalparams.Fig)
subplot(2,3,1:2)
cla
plot(f/1000,ResponseSpecdB,'b','linewidth',2);
set(gca,'Xscale','log')
hold on;
grid on
xlim(globalparams.Fband./1000)
aa=axis;
ylim([0 aa(4)+1]);
ylabel('dB SPL')
xlabel('Frequency (kHz)');
title('Recorded Noise Spectrum Level','fontsize',10);
legend('boxoff')
R.ResponseSpecdB=ResponseSpecdB;
globalparams.R = R;