function BW = poly2mask(x,y,M,N)

% Modified version of original version of poly2mask to enable mex compilation 01/20/22 DZ
%POLY2MASK Convert region-of-interest polygon to mask.
%   BW = POLY2MASK(X,Y,M,N) computes a binary region-of-interest mask,
%   BW, from a region-of-interest polygon, represented by the vectors X
%   and Y.  The size of BW is M-by-N.  Pixels in BW that are inside
%   the polygon (X,Y) are 1; pixels outside the polygon are 0.  The class
%   of BW is logical.
%
%   POLY2MASK closes the polygon automatically if it isn't already
%   closed.
%
%   Example 1:
%   ---------
%       x = [63 186 54 190 63];
%       y = [60 60 209 204 60];
%       bw = poly2mask(x,y,256,256);
%       imshow(bw)
%       hold on
%       plot(x,y,'b','LineWidth',2)
%       hold off
%
%   Example 2:
%   ---------
%  % Or using random points:
%
%       x = 256*rand(1,4);
%       y = 256*rand(1,4);
%       x(end+1) = x(1);
%       y(end+1) = y(1);
%       bw = poly2mask(x,y,256,256);
%       imshow(bw)
%       hold on
%       plot(x,y,'b','LineWidth',2)
%       hold off
%
%   See also ROIPOLY.

%   Copyright 1993-2018 The MathWorks, Inc.

if isempty(x)
    BW = false(M,N);
    return;
end

% if (x(end) ~= x(1)) || (y(end) ~= y(1)) % moved to computeDFF_new_coders
%     x(end+1) = x(1);
%     y(end+1) = y(1);
% end

% [xe,ye] = poly2edgelist(x,y);

scale = 5; % default value

% Scale and quantize (x,y) locations to the higher resolution grid.
x = round(scale*(x - 0.5) + 1);
y = round(scale*(y - 0.5) + 1);

num_segments = length(x) - 1;
x_segments = cell(num_segments,1);
y_segments = cell(num_segments,1);
totElements = 0;

for k = 1:num_segments
    %[x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1));
    
    % iptui.intline does not support Matlab coder
    maxRange = max(abs(x(k+1)-x(k)),abs(y(k+1)-y(k)));
    nm = (0:maxRange)';
    totElements = totElements + length(nm);
    difX = x(k+1) - x(k);
    difY = y(k+1) - y(k);
    x_segments{k} = round(x(k) + (difX/maxRange)*nm);
    y_segments{k} = round(y(k) + (difY/maxRange)*nm);
    
end

% Concatenate segment vertices.
% x = cat(1,x_segments{:});
% y = cat(1,y_segments{:});

x = zeros(totElements,1);
y = zeros(totElements,1);
index = 1;

for j = 1:num_segments
    len = length(x_segments{j});
    x(index:index+len-1) = x_segments{j};
    y(index:index+len-1) = y_segments{j};
    index = index + len;
end

% Horizontal edges are located where the x-value changes.
d = diff(x);
edge_indices = find(d);
xe = x(edge_indices);

% Wherever the diff is negative, the x-coordinate should be x-1 instead of
% x.
shift = find(d(edge_indices) < 0);
xe(shift) = xe(shift) - 1;

% In order for the result to be the same no matter which direction we are
% tracing the polynomial, the y-value for a diagonal transition has to be
% biased the same way no matter what.  We'll always chooser the smaller
% y-value associated with diagonal transitions.
ye = min(y(edge_indices), y(edge_indices+1));

% BW = edgelist2mask(M,N,xe,ye);

shift = (scale - 1)/2;

% Scale x values, throwing away edgelist points that aren't on a pixel's
% center column.
xe = (xe+shift)/scale;
idx = xe == floor(xe);
xe = xe(idx);
ye = ye(idx);

% Scale y values.
ye = ceil((ye + shift)/scale);

% Throw away horizontal edges that are too far left, too far right, or below the image.
bad_indices = find((xe < 1) | (xe > N) | (ye > M));
xe(bad_indices) = [];
ye(bad_indices) = [];

% Treat horizontal edges above the top of the image as they are along the
% upper edge.
ye = max(1,ye);

% Insert the edge list locations into a sparse matrix, taking
% advantage of the accumulation behavior of the SPARSE function.
S = sparse(ye,xe,1,M,N);

% We reduce the memory consumption of edgelist2mask by processing only a
% group of columns at a time (g274577); this does not compromise speed.
BW = false(size(S));
numCols = size(S,2);
columnChunk = 50;

% for m = 1:columnChunk:numCols
%   firstColumn = m;
%   lastColumn = min(m + columnChunk - 1, numCols);
%   columns = full(S(:, firstColumn:lastColumn));
%   BW1(:, firstColumn:lastColumn) = builtin('_parityscan', columns); % not supported for matlab coder
% end

for m = 1:columnChunk:numCols
    firstColumn = m;
    lastColumn = min(m + columnChunk - 1, numCols);
    columns = full(S(:, firstColumn:lastColumn));
    BW(:,firstColumn:lastColumn) = mod(cumsum(columns,1),2)~=0;
end


