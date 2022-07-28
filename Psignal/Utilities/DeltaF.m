function [baseline DeltaFF] = DeltaF(I, pfs, homof, fc, prestimulussilence, illum)
if nargin<6
    illum=0;
end
%Global params
if homof
    homoimg=zeros(size(I));
    for i = 1:size(I,3)
        disp(['Filtering Image ' num2str(i)]);
        I_temp = I(:,:,i);
        I_temp = im2double(I_temp);
        I_temp = log(1 + I_temp);
        M = 2*size(I_temp,1);
        N = 2*size(I_temp,2);
        sigma =fc; %May need to be adjusted per experiment.The bigger this umber is, the higher is the high-pss cutoff. If it is too high, then there isn't eoungh reflectance signal to generate an image.
        [X, Y] = meshgrid(1:N,1:M);
        centerX = ceil(N/2);
        centerY = ceil(M/2);
        gaussianNumerator = (X - centerX).^2 + (Y - centerY).^2;
        H = exp(-gaussianNumerator./(2*sigma.^2));
        if ~illum
            H=1-H;
        end
        H = fftshift(H);
        Ir = padarray(I_temp,[ceil(size(I_temp,1)/2) ceil(size(I_temp,2)/2)],'symmetric');
        If = fft2(Ir, M, N);
        Iout = real(ifft2(H.*If));
        Iout = Iout(ceil(size(I_temp,1)/2)+1:size(Iout,1)-ceil(size(I_temp,1)/2),ceil(size(I_temp,2)/2)+1:(size(Iout,2)-ceil(size(I_temp,2)/2)));
        homoimg(:,:,i) = exp(Iout) - 1;
    end
else
    homoimg = I;
end
clear I
baseline = squeeze(mean(homoimg(:,:,1:pfs*prestimulussilence),3));
DeltaFF=(homoimg-repmat(baseline,[1 1 size(homoimg,3)]))./repmat(baseline,[1 1 size(homoimg,3)]);
