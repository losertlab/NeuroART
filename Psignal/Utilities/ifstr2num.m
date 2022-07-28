function a = ifstr2num (b);
% function a = ifstr2num (b);
% 
% this simple function is str2num, but works also when the input is a
% numeric, which in that case its not changed.

% nima , nov 2005

if isnumeric(b)
    a = b;
elseif ischar(b)
    a = str2num(b);
end    