%Load sound
[w fs] = audioread('C:\Users\Psygnal\Dropbox\Psignal\SoundObjects\@SpectrumShift\wheelbarrow\wheelbarrow.wav');
fsn=200000;
[P,Q] = rat(fsn/fs);
w = resample(w,P,Q);
fs = fsn;
%Filter
[b a]=butter(3,[1000 6000]./(fs/2));
w = filtfilt(b,a,w);
%Compute FFT
f=linspace(0,fs,length(w));
W=fft(w);
%Frequency shift spectrum peak 
fshift = 15000;
[junk m]=max(W(1:find(f<6000,1,'last')));
fmax = f(m);
fdiff=fshift-fmax;
idxdiff = find(f<fdiff,1,'last');
W2 = circshift(W,round(idxdiff),1);
%Reconstruct frequency shifted signal
W2(1:round(idxdiff)-1)=0;
W2=fft(real(ifft(W2)).*2);
w2 = ifft(W2);
%Frequency down-shift spectrum peak to original frequency
W3 = circshift(W2,-round(idxdiff),1);
%Reconstruct frequency down-shifted signal
idxdiff = find(f>20000,1,'first');
W3(idxdiff:end)=0;
W3=fft(real(ifft(W3)).*2);
w3 = ifft(W3);
%Plot signals
figure
subplot(2,1,1)
plot(f,abs(W))
hold on
plot(f,abs(W2),'r')
plot(f,abs(W3),'g')
subplot(2,1,2)
t=0:1/fs:length(w)/fs-(1/fs);
plot(t,w)
hold on
plot(t,w2,'r')
plot(t,w3,'g')
