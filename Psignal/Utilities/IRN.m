%IRN generates iterated ripple noises with inputs:
%delay: delay betweeen iterations
%weight: delay filter coefficients
%RandPhase: randomize IRN phase
%BW: bandwith of IRN
%Duration: IRN duration

function [yfilt ytonestack y2] = IRN(delay, weight, RandPhase, f, BW, Duration, fs, itr, edgecut)
if nargin < 1
    delay = 0.0025;
    weight = 1;
    RandPhase = 0;
    f=4000;
%     BW = f*2.^[-.25 .25];
BW = [2000 20000];
    Duration = 1;
    fs = 50000;
    itr=16;n
    edgecut = .32;
end
t=0:1/fs:Duration-(1/fs);
x = rand(length(t),1);
shift = delay*fs;
if ~RandPhase 
    y=zeros(length(x)+shift*itr,1);
    for i=0:itr
        y(i*shift+1:i*shift+length(x),:) = y(i*shift+1:i*shift+length(x),:)+x;
    end
end
[b a] = butter(3,BW/(fs/2));
yfilt = filtfilt(b,a,y);
yfilt=yfilt(edgecut*fs:end-edgecut*fs-1);
yfilt = yfilt./max(abs(yfilt));
f0=1/delay;
y2 = sin(2*pi*f0.*t);
y2=y2(edgecut*fs:end-edgecut*fs-1);
ytonestack=0;
for i = 1:20
    ytonestack = ytonestack+sin(2*pi*(f0*i).*t);
end
ytonestack = filtfilt(b,a,ytonestack);
ytonestack=ytonestack(edgecut*fs:end-edgecut*fs-1);
ytonestack=ytonestack/max(abs(ytonestack));


    