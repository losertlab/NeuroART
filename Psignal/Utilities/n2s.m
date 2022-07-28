function out = n2s(in,n)

if nargin==1
 out = num2str(in);
else
 out = num2str(in,n);  
end
