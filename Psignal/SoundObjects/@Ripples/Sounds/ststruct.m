function [ststims,freqs]=ststruct(waveParams,W,Omega,...
				 lfreq,hfreq,numcomp,T,saf);
% [ststims,freqs] = ststruct(waveParams,W,Omega,lfreq,hfreq,numcomp,T,saf);
%
% STSTRUCT : Spectro-temporal stimulus constructor.
%
% waveParams : 5-D matrix containing amplitude and phase pairs for 
%              all components of all waveforms (see MAKEWAVEPARAMS)
% W          : Vector of ripple velocities
% Omega      : Matrix of ripple frequencies
% lfreq      : lowest frequency component
% hfreq      : highest frequency component
% numcomp    : number of components (frequency length)
% T          : stimulus duration (ms)
% saf 	     : sampling frequency (default = 1000 Hz)

[ap,ws,omegs,lr,numstims] = size(waveParams);
[a,b]=size(Omega);
[c,d]=size(W);

if nargin==7, saf = 1000; end

if a*b*c*d ~= omegs*ws*lr,
	error('Omega and/or W don''t match waveParams')
end

freqs = logNspace(2,lfreq,hfreq,numcomp+1);
sffact = saf/1000;
leng = round(T*sffact);
t = (1:leng)-1;

%w0 = saf/T;
w0 = 1000/T;
W0 = 1/(log2(hfreq/lfreq));

k = round(W/w0);
l = round(Omega/W0);

ststims = zeros(numcomp,leng,numstims);

c = [floor(numcomp/2)+1,floor(leng/2)+1];

for wnum = 1:numstims,
	count = 0;
	stimHolder = zeros(numcomp,leng);
        %fprintf(1,'%s','Record '),fprintf(1,'%2d',wnum),
        %fprintf(1,'\b\b\b\b\b\b\b\b\b')

	for row = 1:ws,
		%dblu = W(row);
		for sgn = 1:lr,
			for col = 1:omegs,
			      	%omeg = Omega(col,sgn);

				count = count + 1;

				amp = waveParams(1,row,col,sgn,wnum);
				phs = waveParams(2,row,col,sgn,wnum);

				if amp,

				stimHolder(c(1)+l(col,sgn),c(2)+k(row))=...
				(amp/2)*exp(i*(phs-90)*pi/180);
				stimHolder(c(1)-l(col,sgn),c(2)-k(row))=...
				(amp/2)*exp(-i*(phs-90)*pi/180);
				
				end
			end
		end
	end
	ststims(:,:,wnum) = real(ifft2(ifftshift(stimHolder*(leng*numcomp))));
end
%fprintf(1,'\n')
% Plotting...
% surf(t,freqs,rip); shading interp; view(2)
