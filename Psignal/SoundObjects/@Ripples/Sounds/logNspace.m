function y = logNspace(N,fl,fh,n);
% y = logNspace(N,fl,fh,n);
%
% N base of logarithm
% fl : lowest frequency
% fh : highest frequency
% n  : number of components (default: 50)

fl = log(fl)/log(N);
fh = log(fh)/log(N);

if nargin == 2
    n = 50;
end
if fh == pi
	eval(['fh = log' num2str(N) '(pi);'])
end
y = (N).^ [fl+(0:n-2)*(fh-fl)/(n-1), fh];
