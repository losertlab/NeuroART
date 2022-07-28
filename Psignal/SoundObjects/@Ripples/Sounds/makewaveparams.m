function  [waveParams,W,Omega,N,T,k] = makewaveparams(a1am,a1rf,a1ph,a1rv);
% [waveParams,W,Omega,N,T,k] = makewaveparams(a1am,a1rf,a1ph,a1rv);
%
% MAKEWAVEPARAMS: This is an important program which transforms the input 
%                 matrices (dim: #components X #waveforms) into the
%		  universal waveParams representation used in STSTRUCT.

%clear W Omega waveParams

numstims = size(a1am,2);

% Transfer negative velocities to Omega

vels = cat(2,a1rv{:});
frqs = cat(2,a1rf{:});
frqs(find(vels<0)) = -frqs(find(vels<0));
vels = unique(abs(vels));
frqs = unique(frqs);

N = size(unique([cat(2,a1rv{:});cat(2,a1rf{:})]','rows'),1);
W = vels(:);
T = round(1000/min(abs(diff([0;unique(abs(W(find(W))))]))));
%k = round(abs(cat(1,a1rv{:}))'*T/1000 + 1);

Onegs = frqs(find(frqs<0));
Oposs = frqs(find(frqs>=0));
Onegs = Onegs(:); Oposs = Oposs(:);

Omega(1:length(Oposs),1) = Oposs; 
Omega(1:length(Onegs),2) = Onegs; 

Omega = sort(Omega);
if size(Omega,2) == 2,
	Omega(:,2) = flipud(Omega(:,2));
elseif size(Omega,2) == 1,
	Omega(:,2) = zeros(size(Omega));
end
numvels = length(W);
numfrqs = size(Omega,1);

waveParams = zeros(2,numvels,numfrqs,2,numstims);

for fnum = 1:numstims,
	for cnum = 1:length(a1am{fnum}),
		temp = a1am{fnum};
		amp = temp(cnum);
		temp = a1ph{fnum};
		phs = temp(cnum);
		if amp < 0,
			amp = abs(amp);
			phs = phs - 180;
		end	
		temp = a1rv{fnum}(cnum);
                %ii = find(temp<0);
                ii = temp<0;
		%veli = find(W==abs(temp(cnum)));
                veli = find(W==abs(temp));
                temp = a1rf{fnum}(cnum); 
                if ii, %~isempty(ii), 
                   temp = -temp; 
                   %phs(ii) = phs(ii)-180;
                   phs = -(phs+90)-90;
                end
		if temp %temp(cnum),
			%[frqi,sgn]=find(Omega==temp(cnum));
                        [frqi,sgn]=find(Omega==temp);
		else,
			frqi = 1; sgn = 1;
		end
                %veli,frqi,sgn,fnum,amp,phs
		waveParams(:,veli,frqi,sgn,fnum) = [amp phs];
	end
end
		
