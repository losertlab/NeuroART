function Pos = HF_fusePos(varargin)
PosA = zeros(length(varargin),4);
for i=1:length(varargin)
  if ~isnumeric(varargin{i}) | length(varargin{i})~=4
    error('Inputs must be valid position vectors!');
  end
  PosA(i,:) = varargin{i}(:); 
end

XLocs = unique(PosA(:,1));
YLocs = unique(PosA(:,2));
if length(XLocs)==1 Mode = 'Vertical';
elseif length(YLocs)==1 Mode = 'Horizontal';
else error('Axes have to be next to each other or above each other');
end

% LOCATION
Pos(1) = min(XLocs); Pos(2) = min(YLocs);

% SIZE
switch Mode
  case 'Vertical';
    Pos(3) = PosA(1,3);     Pos(4) = max(sum(PosA(:,[2,4]),2)) - Pos(2);
  case 'Horizontal'
    Pos(3) = max(sum(PosA(:,[1,3]),2)) - Pos(1);   Pos(4) = PosA(1,4);
end