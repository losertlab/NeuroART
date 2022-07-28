/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * smooth.c
 *
 * Code generation for function 'smooth'
 *
 */

/* Include files */
#include "smooth.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Declarations */
static void moving(const double y[360], double c[360]);

/* Function Definitions */
static void moving(const double y[360], double c[360])
{
  int i;
  boolean_T ynan[360];
  boolean_T b_y;
  boolean_T exitg1;
  double yy[360];
  int k;
  int j;
  double b_ynan[360];
  double cbegin[7];
  double cend[7];

  /* -------------------------------------------------------------------- */
  /*  moving average of the data. */
  for (i = 0; i < 360; i++) {
    ynan[i] = rtIsNaN(y[i]);
  }

  /*  force it to be odd */
  b_y = false;
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i < 360)) {
    if (ynan[i]) {
      b_y = true;
      exitg1 = true;
    } else {
      i++;
    }
  }

  if (!b_y) {
    /*  simplest method for most common case */
    memset(&c[0], 0, 360U * sizeof(double));
    for (i = 0; i < 9; i++) {
      k = i + 1;
      for (j = k; j < 361; j++) {
        c[j - 1] += 0.1111111111111111 * y[(j - i) - 1];
      }
    }

    for (i = 0; i < 7; i++) {
      cbegin[i] = y[i];
    }

    for (i = 0; i < 6; i++) {
      cbegin[i + 1] += cbegin[i];
    }

    for (i = 0; i < 7; i++) {
      cend[i] = y[359 - i];
    }

    for (i = 0; i < 6; i++) {
      cend[i + 1] += cend[i];
    }

    yy[0] = cbegin[0];
    yy[1] = cbegin[2] / 3.0;
    yy[2] = cbegin[4] / 5.0;
    yy[3] = cbegin[6] / 7.0;
    memcpy(&yy[4], &c[8], 352U * sizeof(double));
    yy[356] = cend[6] / 7.0;
    yy[357] = cend[4] / 5.0;
    yy[358] = cend[2] / 3.0;
    yy[359] = cend[0];
    memcpy(&c[0], &yy[0], 360U * sizeof(double));
  } else {
    /*  with no x repeats, can take ratio of two smoothed sequences */
    for (i = 0; i < 360; i++) {
      yy[i] = y[i];
      if (ynan[i]) {
        yy[i] = 0.0;
      }
    }

    moving(yy, c);
    for (i = 0; i < 360; i++) {
      b_ynan[i] = !ynan[i];
    }

    moving(b_ynan, yy);
    for (i = 0; i < 360; i++) {
      c[i] /= yy[i];
    }
  }
}

void smooth(const double y[360], double c[360])
{
  int i;
  boolean_T b_y;
  boolean_T b[360];
  boolean_T exitg1;
  char method[6];
  static const char cv[6] = { 'l', 'o', 'w', 'e', 's', 's' };

  static const char b_b[6] = { 'm', 'o', 'v', 'i', 'n', 'g' };

  /* SMOOTH  Smooth data. */
  /*    Modified version to allow mex compilation, 01/14/22 DZ */
  /*    Z = SMOOTH(Y) smooths data Y using a 5-point moving average. */
  /*  */
  /*    Z = SMOOTH(Y,SPAN) smooths data Y using SPAN as the number of points used */
  /*    to compute each element of Z. */
  /*  */
  /*    Z = SMOOTH(Y,SPAN,METHOD) smooths data Y with specified METHOD. The */
  /*    available methods are: */
  /*  */
  /*            'moving'   - Moving average (default) */
  /*            'lowess'   - Lowess (linear fit) */
  /*            'loess'    - Loess (quadratic fit) */
  /*            'sgolay'   - Savitzky-Golay */
  /*            'rlowess'  - Robust Lowess (linear fit) */
  /*            'rloess'   - Robust Loess (quadratic fit) */
  /*  */
  /*    Z = SMOOTH(Y,METHOD) uses the default SPAN 5. */
  /*  */
  /*    Z = SMOOTH(Y,SPAN,'sgolay',DEGREE) and Z = SMOOTH(Y,'sgolay',DEGREE) */
  /*    additionally specify the degree of the polynomial to be used in the */
  /*    Savitzky-Golay method. The default DEGREE is 2. DEGREE must be smaller */
  /*    than SPAN. */
  /*  */
  /*    Z = SMOOTH(X,Y,...) additionally specifies the X coordinates.  If X is */
  /*    not provided, methods that require X coordinates assume X = 1:N, where */
  /*    N is the length of Y. */
  /*  */
  /*    Notes: */
  /*    1. When X is given and X is not uniformly distributed, the default method */
  /*    is 'lowess'.  The 'moving' method is not recommended. */
  /*  */
  /*    2. For the 'moving' and 'sgolay' methods, SPAN must be odd. */
  /*    If an even SPAN is specified, it is reduced by 1. */
  /*  */
  /*    3. If SPAN is greater than the length of Y, it is reduced to the */
  /*    length of Y. */
  /*  */
  /*    4. In the case of (robust) lowess and (robust) loess, it is also */
  /*    possible to specify the SPAN as a percentage of the total number */
  /*    of data points. When SPAN is less than or equal to 1, it is */
  /*    treated as a percentage. */
  /*  */
  /*    For example: */
  /*  */
  /*    Z = SMOOTH(Y) uses the moving average method with span 5 and */
  /*    X=1:length(Y). */
  /*  */
  /*    Z = SMOOTH(Y,7) uses the moving average method with span 7 and */
  /*    X=1:length(Y). */
  /*  */
  /*    Z = SMOOTH(Y,'sgolay') uses the Savitzky-Golay method with DEGREE=2, */
  /*    SPAN = 5, X = 1:length(Y). */
  /*  */
  /*    Z = SMOOTH(X,Y,'lowess') uses the lowess method with SPAN=5. */
  /*  */
  /*    Z = SMOOTH(X,Y,SPAN,'rloess') uses the robust loess method. */
  /*  */
  /*    Z = SMOOTH(X,Y) where X is unevenly distributed uses the */
  /*    'lowess' method with span 5. */
  /*  */
  /*    Z = SMOOTH(X,Y,8,'sgolay') uses the Savitzky-Golay method with */
  /*    span 7 (8 is reduced by 1 to make it odd). */
  /*  */
  /*    Z = SMOOTH(X,Y,0.3,'loess') uses the loess method where span is */
  /*    30% of the data, i.e. span = ceil(0.3*length(Y)). */
  /*  */
  /*    See also SPLINE. */
  /*    Copyright 2001-2018 The MathWorks, Inc. */
  /* -------------------------------------------------------------------- */
  /*  function c = lowess(x,y, span, method, robust, iter) */
  /*  % LOWESS  Smooth data using Lowess or Loess method. */
  /*  % */
  /*  % The difference between LOWESS and LOESS is that LOWESS uses a */
  /*  % linear model to do the local fitting whereas LOESS uses a */
  /*  % quadratic model to do the local fitting. Some other software */
  /*  % may not have LOWESS, instead, they use LOESS with order 1 or 2 to */
  /*  % represent these two smoothing methods. */
  /*  % */
  /*  % Reference:  */
  /*  % [C79] W.S.Cleveland, "Robust Locally Weighted Regression and Smoothing */
  /*  %    Scatterplots", _J. of the American Statistical Ass._, Vol 74, No. 368  */
  /*  %    (Dec.,1979), pp. 829-836. */
  /*  %    http://www.math.tau.ac.il/~yekutiel/MA%20seminar/Cleveland%201979.pdf */
  /*   */
  /*   */
  /*  n = length(y); */
  /*  span = floor(span); */
  /*  span = min(span,n); */
  /*  c = y; */
  /*  if span == 1 */
  /*      return; */
  /*  end */
  /*   */
  /*  useLoess = false; */
  /*  if isequal(method,'loess') */
  /*      useLoess = true; */
  /*  end */
  /*   */
  /*  diffx = diff(x); */
  /*   */
  /*  % For problems where x is uniform, there's a faster way */
  /*  isuniform = uniformx(diffx,x,y); */
  /*  if isuniform */
  /*      % For uniform data, an even span actually covers an odd number of */
  /*      % points.  For example, the four closest points to 5 in the */
  /*      % sequence 1:10 are {3,4,5,6}, but 7 is as close as 3. */
  /*      % Therefore force an odd span. */
  /*      span = 2*floor(span/2) + 1; */
  /*   */
  /*      c = unifloess(y,span,useLoess); */
  /*      if ~robust || span<=2 */
  /*          return; */
  /*      end */
  /*  end */
  /*   */
  /*   */
  /*  ynan = isnan(y); */
  /*  anyNans = any(ynan(:)); */
  /*  seps = sqrt(eps); */
  /*  theDiffs = [1; diffx; 1]; */
  /*   */
  /*  if isuniform */
  /*      % We've already computed the non-robust smooth, so in preparation for */
  /*      % the robust smooth, compute the following arrays directly */
  /*      halfw = floor(span/2); */
  /*       */
  /*      % Each local interval is from |halfw| below the current index to |halfw| */
  /*      % above */
  /*      lbound = (1:n)-halfw; */
  /*      rbound = (1:n)+halfw; */
  /*      % However, there always has to be at least |span| points to the right of the */
  /*      % left bound */
  /*      lbound = min( n+1-span, lbound ); */
  /*      % ... and at least |span| points to the left of the right bound */
  /*      rbound = max( span, rbound ); */
  /*      % Furthermore, because these bounds index into vectors of length n, they */
  /*      % must contain valid indices */
  /*      lbound = max( 1, lbound ); */
  /*      rbound = min( n, rbound ); */
  /*       */
  /*      % Since the input is uniform we can use natural numbers for the input when */
  /*      % we need them. */
  /*      x = (1:numel(x))'; */
  /*  else */
  /*      if robust */
  /*          % pre-allocate space for lower and upper indices for each fit, */
  /*          % to avoid re-computing this information in robust iterations */
  /*          lbound = zeros(n,1,'like',y); */
  /*          rbound = zeros(n,1,'like',y); */
  /*      end */
  /*   */
  /*      % Compute the non-robust smooth for non-uniform x */
  /*      for i=1:n */
  /*          % if x(i) and x(i-1) are equal we just use the old value. */
  /*          if theDiffs(i) == 0 */
  /*              c(i) = c(i-1); */
  /*              if robust */
  /*                  lbound(i) = lbound(i-1); */
  /*                  rbound(i) = rbound(i-1); */
  /*              end */
  /*              continue; */
  /*          end */
  /*           */
  /*          % Find nearest neighbours */
  /*          idx = iKNearestNeighbours( span, i, x, ~ynan ); */
  /*          if robust */
  /*              % Need to store neighborhoods for robust loop */
  /*              lbound(i) = min(idx); */
  /*              rbound(i) = max(idx); */
  /*          end */
  /*           */
  /*          if isempty(idx) */
  /*              c(i) = NaN; */
  /*              continue */
  /*          end */
  /*   */
  /*          x1 = x(idx)-x(i); % center around current point to improve conditioning */
  /*          d1 = abs(x1); */
  /*          y1 = y(idx); */
  /*   */
  /*          weight = iTricubeWeights( d1 ); */
  /*          if all(weight<seps) */
  /*              weight(:) = 1;    % if all weights are 0, just skip weighting */
  /*          end */
  /*   */
  /*          v = [ones(size(x1)) x1]; */
  /*          if useLoess */
  /*              v = [v x1.*x1]; %#ok<AGROW> There is no significant growth here */
  /*          end */
  /*           */
  /*          v = weight(:,ones(1,size(v,2))).*v; */
  /*          y1 = weight.*y1; */
  /*          if size(v,1)==size(v,2) */
  /*              % Square v may give infs in the \ solution, so force least squares */
  /*              b = [v;zeros(1,size(v,2))]\[y1;0]; */
  /*          else */
  /*              b = v\y1; */
  /*          end */
  /*          c(i) = b(1); */
  /*      end */
  /*  end */
  /*   */
  /*  % now that we have a non-robust fit, we can compute the residual and do */
  /*  % the robust fit if required */
  /*  maxabsyXeps = max(abs(y))*eps; */
  /*  if robust */
  /*      for k = 1:iter */
  /*          r = y-c; */
  /*           */
  /*          % Compute robust weights */
  /*          rweight = iBisquareWeights( r, maxabsyXeps );  */
  /*           */
  /*          % Find new value for each point. */
  /*          for i=1:n */
  /*              if i>1 && x(i)==x(i-1) */
  /*                  c(i) = c(i-1); */
  /*                  continue; */
  /*              end */
  /*              if isnan(c(i)),  */
  /*                  continue;  */
  /*              end */
  /*               */
  /*              idx = lbound(i):rbound(i); */
  /*              if anyNans */
  /*                  idx = idx(~ynan(idx)); */
  /*              end */
  /*              % check robust weights for removed points */
  /*              if any( rweight(idx) <= 0 ) */
  /*                  idx = iKNearestNeighbours( span, i, x, (rweight > 0) ); */
  /*              end */
  /*               */
  /*              x1 = x(idx) - x(i); */
  /*              d1 = abs(x1); */
  /*              y1 = y(idx); */
  /*   */
  /*              weight = iTricubeWeights( d1 ); */
  /*              if all(weight<seps) */
  /*                  weight(:) = 1;    % if all weights 0, just skip weighting */
  /*              end */
  /*   */
  /*              v = [ones(size(x1)) x1]; */
  /*              if useLoess */
  /*                  v = [v x1.*x1]; %#ok<AGROW> There is no significant growth here */
  /*              end */
  /*               */
  /*              % Modify the weights based on x values by multiplying them by */
  /*              % robust weights. */
  /*              weight = weight.*rweight(idx); */
  /*               */
  /*              v = weight(:,ones(1,size(v,2))).*v; */
  /*              y1 = weight.*y1; */
  /*              if size(v,1)==size(v,2) */
  /*                  % Square v may give infs in the \ solution, so force least squares */
  /*                  b = [v;zeros(1,size(v,2))]\[y1;0]; */
  /*              else */
  /*                  b = v\y1; */
  /*              end */
  /*              c(i) = b(1); */
  /*          end */
  /*      end */
  /*  end */
  /* -------------------------------------------------------------------- */
  /*  function ys = unifloess(y,span,useLoess) */
  /*  %UNIFLOESS Apply loess on uniformly spaced X values */
  /*   */
  /*  y = y(:); */
  /*   */
  /*  % Omit points at the extremes, which have zero weight */
  /*  halfw = (span-1)/2;              % halfwidth of entire span */
  /*  d = abs((1-halfw:halfw-1));      % distances to pts with nonzero weight */
  /*  dmax = halfw;                    % max distance for tri-cubic weight */
  /*   */
  /*  % Set up weighted Vandermonde matrix using equally spaced X values */
  /*  x1 = (2:span-1)-(halfw+1); */
  /*  weight = (1 - (d/dmax).^3).^1.5; % tri-cubic weight */
  /*  v = [ones(length(x1),1) x1(:)]; */
  /*  if useLoess */
  /*      v = [v x1(:).^2]; */
  /*  end */
  /*  V = v .* repmat(weight',1,size(v,2)); */
  /*   */
  /*  % Do QR decomposition */
  /*  [Q,~] = qr(V,0); */
  /*   */
  /*  % The projection matrix is Q*Q'.  We want to project onto the middle */
  /*  % point, so we can take just one row of the first factor. */
  /*  alpha = Q(halfw,:)*Q'; */
  /*   */
  /*  % This alpha defines the linear combination of the weighted y values that */
  /*  % yields the desired smooth values.  Incorporate the weights into the */
  /*  % coefficients of the linear combination, then apply filter. */
  /*  alpha = alpha .* weight; */
  /*  ys = filter(alpha,1,y); */
  /*   */
  /*  % We need to slide the values into the center of the array. */
  /*  ys(halfw+1:end-halfw) = ys(span-1:end-1); */
  /*   */
  /*  % Now we have taken care of everything except the end effects.  Loop over */
  /*  % the points where we don't have a complete span.  Now the Vandermonde */
  /*  % matrix has span-1 points, because only 1 has zero weight. */
  /*  x1 = 1:span-1; */
  /*  v = [ones(length(x1),1) x1(:)]; */
  /*  if useLoess */
  /*      v = [v x1(:).^2]; */
  /*  end */
  /*  for j=1:halfw */
  /*      % Compute weights based on deviations from the jth point, */
  /*      % then compute weights and apply them as above. */
  /*      d = abs((1:span-1) - j); */
  /*      weight = (1 - (d/(span-j)).^3).^1.5; */
  /*      V = v .* repmat(weight(:),1,size(v,2)); */
  /*      [Q,~] = qr(V,0); */
  /*      alpha = Q(j,:)*Q'; */
  /*      alpha = alpha .* weight; */
  /*      ys(j) = alpha * y(1:span-1); */
  /*   */
  /*      % These coefficients can be applied to the other end as well */
  /*      ys(end+1-j) = alpha * y(end:-1:end-span+2); */
  /*  end */
  /* -------------------------------------------------------------------- */
  /* ISUNIFORM True if x is of the form a:b:c */
  for (i = 0; i < 360; i++) {
    b[i] = rtIsNaN(y[i]);
  }

  b_y = false;
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i < 360)) {
    if (b[i]) {
      b_y = true;
      exitg1 = true;
    } else {
      i++;
    }
  }

  b_y = !b_y;

  /* -------------------------------------------------------------------- */
  /*  function idx = iKNearestNeighbours( k, i, x, in ) */
  /*  % Find the k points from x(in) closest to x(i) */
  /*   */
  /*  if nnz( in ) <= k */
  /*      % If we have k points or fewer, then return them all */
  /*      idx = find( in ); */
  /*  else */
  /*      % Find the distance to the k closest point */
  /*      d = abs( x - x(i) ); */
  /*      ds = sort( d(in) ); */
  /*      dk = ds(k); */
  /*       */
  /*      % Find all points that are as close as or closer than the k closest point */
  /*      close = (d <= dk); */
  /*       */
  /*      % The required indices are those points that are both close and "in" */
  /*      idx = find( close & in ); */
  /*  end */
  /*  %-------------------------------------------------------------------- */
  /*  % Bi-square (robust) weight function */
  /*  function delta = iBisquareWeights( r, myeps ) */
  /*  % Convert residuals to weights using the bi-square weight function. */
  /*  % NOTE that this function returns the square root of the weights */
  /*   */
  /*  % Only use non-NaN residuals to compute median */
  /*  idx = ~isnan( r ); */
  /*  % And bound the median away from zero */
  /*  s = max( 1e8 * myeps, median( abs( r(idx) ) ) ); */
  /*  % Covert the residuals to weights */
  /*  delta = iBisquare( r/(6*s) ); */
  /*  % Everything with NaN residual should have zero weight */
  /*  delta(~idx) = 0; */
  /*  function b = iBisquare( x ) */
  /*  % This is this bi-square function defined at the top of the left hand */
  /*  % column of page 831 in [C79] */
  /*  % NOTE that this function returns the square root of the weights */
  /*  b = zeros( size( x ) , 'like', x); */
  /*  idx = abs( x ) < 1; */
  /*  b(idx) = abs( 1 - x(idx).^2 ); */
  /*  %-------------------------------------------------------------------- */
  /*  % Tri-cubic weight function */
  /*  function w = iTricubeWeights( d ) */
  /*  % Convert distances into weights using tri-cubic weight function. */
  /*  % NOTE that this function returns the square-root of the weights. */
  /*  % */
  /*  % Protect against divide-by-zero. This can happen if more points than the span */
  /*  % are coincident. */
  /*  maxD = max( d ); */
  /*  if maxD > 0 */
  /*      d = d/max( d ); */
  /*  end */
  /*  w = (1 - d.^3).^1.5; */
  if (b_y) {
    for (i = 0; i < 6; i++) {
      method[i] = b_b[i];
    }

    /*  uniformly distributed X. */
  } else {
    for (i = 0; i < 6; i++) {
      method[i] = cv[i];
    }
  }

  /*  if NaNs not all at end */
  for (i = 0; i < 360; i++) {
    c[i] = rtNaN;
  }

  i = memcmp(&method[0], &b_b[0], 6);
  if (i == 0) {
    i = 0;
  } else {
    i = -1;
  }

  switch (i) {
   case 0:
    moving(y, c);

    /*      case {'lowess','loess','rlowess','rloess'} */
    /*          robust = 0; */
    /*          iter = 5; */
    /*          if method(1)=='r' */
    /*              robust = 1; */
    /*              method = method(2:end); */
    /*          end */
    /*          c(ok) = lowess(x(ok),y(ok),span, method,robust,iter); */
    break;
  }
}

/* End of code generation (smooth.c) */
