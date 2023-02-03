function b = de2bi(varargin)
%DE2BI Convert decimal numbers to binary numbers.
%   B = DE2BI(D) converts a nonnegative integer decimal vector D to a
%   binary matrix B. Each row of the binary matrix B corresponds to one
%   element of D. The default orientation of the binary output is
%   Right-MSB; the first element in B represents the lowest bit.
%
%   In addition to the vector input, three optional parameters can be
%   given:
%
%   B = DE2BI(...,N) uses N to define how many digits (columns) are output.
%
%   B = DE2BI(...,N,P) uses P to define which base to convert the decimal
%   elements to.
%
%   B = DE2BI(...,MSBFLAG) uses MSBFLAG to determine the output
%   orientation.  MSBFLAG has two possible values, 'right-msb' and
%   'left-msb'.  Giving a 'right-msb' MSBFLAG does not change the
%   function's default behavior.  Giving a 'left-msb' MSBFLAG flips the
%   output orientation to display the MSB to the left.
%
%   Examples:
%       E = [12; 5];
%
%       A = de2bi(E)                
%       B = de2bi(E,5)
%       C = de2bi(E,[],3)            
%       D = de2bi(E,5,'left-msb')
%
%   See also BI2DE.

%   Copyright 1996-2018 The MathWorks, Inc.

% Typical error checking.
narginchk(1,4);

% --- Placeholder for the signature string.
sigStr = '';
msbFlag = '';
p = [];
n = [];

% --- Identify string and numeric arguments
for i=1:nargin
   if(i>1)
      sigStr(size(sigStr,2)+1) = '/';
   end
   % --- Assign the string and numeric flags
   if(ischar(varargin{i}) || isStringScalar(varargin{i})) % added by DZ 10/26/22 because to resolve previous function name in Matlab 2021b
      sigStr(size(sigStr,2)+1) = 's';
   elseif(isnumeric(varargin{i}))
      sigStr(size(sigStr,2)+1) = 'n';
   else
      error(message('comm:de2bi:InvalidArg'));
   end
end

% --- Identify parameter signatures and assign values to variables
switch sigStr
   % --- de2bi(d)
   case 'n'
      d		= varargin{1};

	% --- de2bi(d, n)
	case 'n/n'
      d		= varargin{1};
      n		= varargin{2};

	% --- de2bi(d, msbFlag)
	case 'n/s'
      d		= varargin{1};
      msbFlag	= varargin{2};

	% --- de2bi(d, n, msbFlag)
	case 'n/n/s'
      d		= varargin{1};
      n		= varargin{2};
      msbFlag	= varargin{3};

	% --- de2bi(d, msbFlag, n)
	case 'n/s/n'
      d		= varargin{1};
      msbFlag	= varargin{2};
      n		= varargin{3};

	% --- de2bi(d, n, p)
	case 'n/n/n'
      d		= varargin{1};
      n		= varargin{2};
      p  	= varargin{3};

	% --- de2bi(d, n, p, msbFlag)
	case 'n/n/n/s'
      d		= varargin{1};
      n		= varargin{2};
      p  	= varargin{3};
      msbFlag	= varargin{4};

	% --- de2bi(d, n, msbFlag, p)
	case 'n/n/s/n'
      d		= varargin{1};
      n		= varargin{2};
      msbFlag	= varargin{3};
      p  	= varargin{4};

	% --- de2bi(d, msbFlag, n, p)
	case 'n/s/n/n'
      d		= varargin{1};
      msbFlag	= varargin{2};
      n		= varargin{3};
      p  	= varargin{4};

   % --- If the parameter list does not match one of these signatures.
   otherwise
      disp('error: comm:de2bi:InvalidArgSeq');
end

if isempty(d)
   disp('error: comm:de2bi:NoInput');
end

protoType = d(1);
d = double(d(:));

if any(d(:) < 0) || any(~isfinite(d(:))) || ~isreal(d) || ~isequal(floor(d),d)
   disp('error: comm:de2bi:InvalidInput');
end

% Assign the base to convert to.
if isempty(p)
    p = 2;
elseif max(size(p) ~= 1)
   disp('error: comm:de2bi:NonScalarBase');
elseif (~isfinite(p)) || (~isreal(p)) || (floor(p) ~= p)
   disp('error: comm:de2bi:InvalidBase');
elseif p < 2
   disp('errpr: comm:de2bi:BaseLessThan2');
end

% Determine minimum length required.
tmp = max(d);
if tmp ~= 0 				% Want base-p log of tmp.
   ntmp = floor( log(tmp) / log(p) ) + 1;
   
   % This takes care of any round off error that occurs for really big inputs.
   if p^(ntmp-1) > tmp
      ntmp = ntmp - 1;
   end
else 							% Since you can't take log(0).
   % 1 bit to represent 0
   ntmp = 1;
end

% Assign number of columns in output matrix.
if isempty(n)
   n = ntmp;
elseif max(size(n) ~= 1)
   disp('error: comm:de2bi:NonScalarN');
elseif (~isfinite(n)) || (~isreal(n)) || (floor(n) ~= n)
   disp('error: comm:de2bi:InvalidN');
elseif n < ntmp
   disp('error: comm:de2bi:SmallN');
end

% Check if the string msbFlag is valid.
if isempty(msbFlag)
   msbFlag = 'right-msb';
elseif ~(strcmp(msbFlag, 'right-msb') || strcmp(msbFlag, 'left-msb'))
   disp('error: comm:de2bi:InvalidMsbFlag');
end

b = de2bi_internal(d, n, p, msbFlag, protoType);

end

% [EOF]
