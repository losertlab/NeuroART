/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * poly2mask.c
 *
 * Code generation for function 'poly2mask'
 *
 */

/* Include files */
#include "poly2mask.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "computeDFF_new_coder_rtwutil.h"
#include "diff.h"
#include "find.h"
#include "mod.h"
#include "nullAssignment.h"
#include "rt_nonfinite.h"
#include "sparse.h"
#include "sparse1.h"
#include "useConstantDim.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void b_poly2mask(double x[360], double y[360], double M, double N,
                 emxArray_boolean_T *BW)
{
  int lastColumn;
  emxArray_real_T *b_y;
  double b_index;
  double maxRange_tmp;
  double maxRange;
  int i;
  int loop_ub;
  emxArray_real_T *nm;
  cell_wrap_3 x_segments[359];
  int varargin_1;
  int nx;
  cell_wrap_3 y_segments[359];
  int b_varargin_1;
  double totElements;
  emxArray_real_T *c_y;
  emxArray_real_T *d;
  emxArray_int32_T *edge_indices;
  int i1;
  emxArray_int32_T *b_nm;
  emxArray_real_T *xe;
  emxArray_boolean_T *idx;
  emxArray_real_T *b_xe;
  emxArray_real_T *ye;
  int trueCount;
  coder_internal_sparse expl_temp;
  int S_n;
  emxArray_real_T *columns;
  emxArray_int32_T *t1_colidx;
  emxArray_int32_T *t1_rowidx;
  emxArray_real_T *r;
  int m;
  unsigned int b_m;
  int y_size[2];
  double y_data[512];

  /*  Modified version of original version of poly2mask to enable mex compilation 01/20/22 DZ */
  /* POLY2MASK Convert region-of-interest polygon to mask. */
  /*    BW = POLY2MASK(X,Y,M,N) computes a binary region-of-interest mask, */
  /*    BW, from a region-of-interest polygon, represented by the vectors X */
  /*    and Y.  The size of BW is M-by-N.  Pixels in BW that are inside */
  /*    the polygon (X,Y) are 1; pixels outside the polygon are 0.  The class */
  /*    of BW is logical. */
  /*  */
  /*    POLY2MASK closes the polygon automatically if it isn't already */
  /*    closed. */
  /*  */
  /*    Example 1: */
  /*    --------- */
  /*        x = [63 186 54 190 63]; */
  /*        y = [60 60 209 204 60]; */
  /*        bw = poly2mask(x,y,256,256); */
  /*        imshow(bw) */
  /*        hold on */
  /*        plot(x,y,'b','LineWidth',2) */
  /*        hold off */
  /*  */
  /*    Example 2: */
  /*    --------- */
  /*   % Or using random points: */
  /*  */
  /*        x = 256*rand(1,4); */
  /*        y = 256*rand(1,4); */
  /*        x(end+1) = x(1); */
  /*        y(end+1) = y(1); */
  /*        bw = poly2mask(x,y,256,256); */
  /*        imshow(bw) */
  /*        hold on */
  /*        plot(x,y,'b','LineWidth',2) */
  /*        hold off */
  /*  */
  /*    See also ROIPOLY. */
  /*    Copyright 1993-2018 The MathWorks, Inc. */
  /*  if (x(end) ~= x(1)) || (y(end) ~= y(1)) % moved to computeDFF_new_coders */
  /*      x(end+1) = x(1); */
  /*      y(end+1) = y(1); */
  /*  end */
  /*  [xe,ye] = poly2edgelist(x,y); */
  /*  default value */
  /*  Scale and quantize (x,y) locations to the higher resolution grid. */
  for (lastColumn = 0; lastColumn < 360; lastColumn++) {
    x[lastColumn] = rt_roundd_snf(5.0 * (x[lastColumn] - 0.5) + 1.0);
    y[lastColumn] = rt_roundd_snf(5.0 * (y[lastColumn] - 0.5) + 1.0);
  }

  emxInit_real_T(&b_y, 2);

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[1] - x[0];
  maxRange_tmp = y[1] - y[0];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  emxInit_real_T(&nm, 1);
  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  emxInitMatrix_cell_wrap_3(x_segments);
  varargin_1 = nm->size[0];
  b_index /= maxRange;
  i = x_segments[0].f1->size[0];
  x_segments[0].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[0].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[0].f1->data[i] = x[0] + b_index * nm->data[i];
  }

  nx = x_segments[0].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[0].f1->data[lastColumn] = rt_roundd_snf(x_segments[0].f1->
      data[lastColumn]);
  }

  emxInitMatrix_cell_wrap_3(y_segments);
  b_index = maxRange_tmp / maxRange;
  i = y_segments[0].f1->size[0];
  y_segments[0].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[0].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[0].f1->data[i] = y[0] + b_index * nm->data[i];
  }

  nx = y_segments[0].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[0].f1->data[lastColumn] = rt_roundd_snf(y_segments[0].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[2] - x[1];
  maxRange_tmp = y[2] - y[1];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  b_varargin_1 = nm->size[0];
  b_index /= maxRange;
  i = x_segments[1].f1->size[0];
  x_segments[1].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[1].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[1].f1->data[i] = x[1] + b_index * nm->data[i];
  }

  nx = x_segments[1].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[1].f1->data[lastColumn] = rt_roundd_snf(x_segments[1].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[1].f1->size[0];
  y_segments[1].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[1].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[1].f1->data[i] = y[1] + b_index * nm->data[i];
  }

  nx = y_segments[1].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[1].f1->data[lastColumn] = rt_roundd_snf(y_segments[1].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[3] - x[2];
  maxRange_tmp = y[3] - y[2];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements = (double)((unsigned int)varargin_1 + b_varargin_1) + (double)
    nm->size[0];
  b_index /= maxRange;
  i = x_segments[2].f1->size[0];
  x_segments[2].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[2].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[2].f1->data[i] = x[2] + b_index * nm->data[i];
  }

  nx = x_segments[2].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[2].f1->data[lastColumn] = rt_roundd_snf(x_segments[2].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[2].f1->size[0];
  y_segments[2].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[2].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[2].f1->data[i] = y[2] + b_index * nm->data[i];
  }

  nx = y_segments[2].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[2].f1->data[lastColumn] = rt_roundd_snf(y_segments[2].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[4] - x[3];
  maxRange_tmp = y[4] - y[3];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[3].f1->size[0];
  x_segments[3].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[3].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[3].f1->data[i] = x[3] + b_index * nm->data[i];
  }

  nx = x_segments[3].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[3].f1->data[lastColumn] = rt_roundd_snf(x_segments[3].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[3].f1->size[0];
  y_segments[3].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[3].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[3].f1->data[i] = y[3] + b_index * nm->data[i];
  }

  nx = y_segments[3].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[3].f1->data[lastColumn] = rt_roundd_snf(y_segments[3].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[5] - x[4];
  maxRange_tmp = y[5] - y[4];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[4].f1->size[0];
  x_segments[4].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[4].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[4].f1->data[i] = x[4] + b_index * nm->data[i];
  }

  nx = x_segments[4].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[4].f1->data[lastColumn] = rt_roundd_snf(x_segments[4].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[4].f1->size[0];
  y_segments[4].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[4].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[4].f1->data[i] = y[4] + b_index * nm->data[i];
  }

  nx = y_segments[4].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[4].f1->data[lastColumn] = rt_roundd_snf(y_segments[4].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[6] - x[5];
  maxRange_tmp = y[6] - y[5];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[5].f1->size[0];
  x_segments[5].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[5].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[5].f1->data[i] = x[5] + b_index * nm->data[i];
  }

  nx = x_segments[5].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[5].f1->data[lastColumn] = rt_roundd_snf(x_segments[5].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[5].f1->size[0];
  y_segments[5].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[5].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[5].f1->data[i] = y[5] + b_index * nm->data[i];
  }

  nx = y_segments[5].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[5].f1->data[lastColumn] = rt_roundd_snf(y_segments[5].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[7] - x[6];
  maxRange_tmp = y[7] - y[6];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[6].f1->size[0];
  x_segments[6].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[6].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[6].f1->data[i] = x[6] + b_index * nm->data[i];
  }

  nx = x_segments[6].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[6].f1->data[lastColumn] = rt_roundd_snf(x_segments[6].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[6].f1->size[0];
  y_segments[6].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[6].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[6].f1->data[i] = y[6] + b_index * nm->data[i];
  }

  nx = y_segments[6].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[6].f1->data[lastColumn] = rt_roundd_snf(y_segments[6].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[8] - x[7];
  maxRange_tmp = y[8] - y[7];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[7].f1->size[0];
  x_segments[7].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[7].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[7].f1->data[i] = x[7] + b_index * nm->data[i];
  }

  nx = x_segments[7].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[7].f1->data[lastColumn] = rt_roundd_snf(x_segments[7].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[7].f1->size[0];
  y_segments[7].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[7].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[7].f1->data[i] = y[7] + b_index * nm->data[i];
  }

  nx = y_segments[7].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[7].f1->data[lastColumn] = rt_roundd_snf(y_segments[7].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[9] - x[8];
  maxRange_tmp = y[9] - y[8];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[8].f1->size[0];
  x_segments[8].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[8].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[8].f1->data[i] = x[8] + b_index * nm->data[i];
  }

  nx = x_segments[8].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[8].f1->data[lastColumn] = rt_roundd_snf(x_segments[8].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[8].f1->size[0];
  y_segments[8].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[8].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[8].f1->data[i] = y[8] + b_index * nm->data[i];
  }

  nx = y_segments[8].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[8].f1->data[lastColumn] = rt_roundd_snf(y_segments[8].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[10] - x[9];
  maxRange_tmp = y[10] - y[9];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[9].f1->size[0];
  x_segments[9].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[9].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[9].f1->data[i] = x[9] + b_index * nm->data[i];
  }

  nx = x_segments[9].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[9].f1->data[lastColumn] = rt_roundd_snf(x_segments[9].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[9].f1->size[0];
  y_segments[9].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[9].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[9].f1->data[i] = y[9] + b_index * nm->data[i];
  }

  nx = y_segments[9].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[9].f1->data[lastColumn] = rt_roundd_snf(y_segments[9].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[11] - x[10];
  maxRange_tmp = y[11] - y[10];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[10].f1->size[0];
  x_segments[10].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[10].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[10].f1->data[i] = x[10] + b_index * nm->data[i];
  }

  nx = x_segments[10].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[10].f1->data[lastColumn] = rt_roundd_snf(x_segments[10].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[10].f1->size[0];
  y_segments[10].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[10].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[10].f1->data[i] = y[10] + b_index * nm->data[i];
  }

  nx = y_segments[10].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[10].f1->data[lastColumn] = rt_roundd_snf(y_segments[10].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[12] - x[11];
  maxRange_tmp = y[12] - y[11];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[11].f1->size[0];
  x_segments[11].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[11].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[11].f1->data[i] = x[11] + b_index * nm->data[i];
  }

  nx = x_segments[11].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[11].f1->data[lastColumn] = rt_roundd_snf(x_segments[11].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[11].f1->size[0];
  y_segments[11].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[11].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[11].f1->data[i] = y[11] + b_index * nm->data[i];
  }

  nx = y_segments[11].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[11].f1->data[lastColumn] = rt_roundd_snf(y_segments[11].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[13] - x[12];
  maxRange_tmp = y[13] - y[12];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[12].f1->size[0];
  x_segments[12].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[12].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[12].f1->data[i] = x[12] + b_index * nm->data[i];
  }

  nx = x_segments[12].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[12].f1->data[lastColumn] = rt_roundd_snf(x_segments[12].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[12].f1->size[0];
  y_segments[12].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[12].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[12].f1->data[i] = y[12] + b_index * nm->data[i];
  }

  nx = y_segments[12].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[12].f1->data[lastColumn] = rt_roundd_snf(y_segments[12].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[14] - x[13];
  maxRange_tmp = y[14] - y[13];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[13].f1->size[0];
  x_segments[13].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[13].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[13].f1->data[i] = x[13] + b_index * nm->data[i];
  }

  nx = x_segments[13].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[13].f1->data[lastColumn] = rt_roundd_snf(x_segments[13].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[13].f1->size[0];
  y_segments[13].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[13].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[13].f1->data[i] = y[13] + b_index * nm->data[i];
  }

  nx = y_segments[13].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[13].f1->data[lastColumn] = rt_roundd_snf(y_segments[13].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[15] - x[14];
  maxRange_tmp = y[15] - y[14];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[14].f1->size[0];
  x_segments[14].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[14].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[14].f1->data[i] = x[14] + b_index * nm->data[i];
  }

  nx = x_segments[14].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[14].f1->data[lastColumn] = rt_roundd_snf(x_segments[14].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[14].f1->size[0];
  y_segments[14].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[14].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[14].f1->data[i] = y[14] + b_index * nm->data[i];
  }

  nx = y_segments[14].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[14].f1->data[lastColumn] = rt_roundd_snf(y_segments[14].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[16] - x[15];
  maxRange_tmp = y[16] - y[15];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[15].f1->size[0];
  x_segments[15].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[15].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[15].f1->data[i] = x[15] + b_index * nm->data[i];
  }

  nx = x_segments[15].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[15].f1->data[lastColumn] = rt_roundd_snf(x_segments[15].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[15].f1->size[0];
  y_segments[15].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[15].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[15].f1->data[i] = y[15] + b_index * nm->data[i];
  }

  nx = y_segments[15].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[15].f1->data[lastColumn] = rt_roundd_snf(y_segments[15].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[17] - x[16];
  maxRange_tmp = y[17] - y[16];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[16].f1->size[0];
  x_segments[16].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[16].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[16].f1->data[i] = x[16] + b_index * nm->data[i];
  }

  nx = x_segments[16].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[16].f1->data[lastColumn] = rt_roundd_snf(x_segments[16].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[16].f1->size[0];
  y_segments[16].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[16].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[16].f1->data[i] = y[16] + b_index * nm->data[i];
  }

  nx = y_segments[16].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[16].f1->data[lastColumn] = rt_roundd_snf(y_segments[16].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[18] - x[17];
  maxRange_tmp = y[18] - y[17];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[17].f1->size[0];
  x_segments[17].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[17].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[17].f1->data[i] = x[17] + b_index * nm->data[i];
  }

  nx = x_segments[17].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[17].f1->data[lastColumn] = rt_roundd_snf(x_segments[17].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[17].f1->size[0];
  y_segments[17].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[17].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[17].f1->data[i] = y[17] + b_index * nm->data[i];
  }

  nx = y_segments[17].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[17].f1->data[lastColumn] = rt_roundd_snf(y_segments[17].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[19] - x[18];
  maxRange_tmp = y[19] - y[18];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[18].f1->size[0];
  x_segments[18].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[18].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[18].f1->data[i] = x[18] + b_index * nm->data[i];
  }

  nx = x_segments[18].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[18].f1->data[lastColumn] = rt_roundd_snf(x_segments[18].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[18].f1->size[0];
  y_segments[18].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[18].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[18].f1->data[i] = y[18] + b_index * nm->data[i];
  }

  nx = y_segments[18].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[18].f1->data[lastColumn] = rt_roundd_snf(y_segments[18].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[20] - x[19];
  maxRange_tmp = y[20] - y[19];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[19].f1->size[0];
  x_segments[19].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[19].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[19].f1->data[i] = x[19] + b_index * nm->data[i];
  }

  nx = x_segments[19].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[19].f1->data[lastColumn] = rt_roundd_snf(x_segments[19].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[19].f1->size[0];
  y_segments[19].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[19].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[19].f1->data[i] = y[19] + b_index * nm->data[i];
  }

  nx = y_segments[19].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[19].f1->data[lastColumn] = rt_roundd_snf(y_segments[19].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[21] - x[20];
  maxRange_tmp = y[21] - y[20];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[20].f1->size[0];
  x_segments[20].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[20].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[20].f1->data[i] = x[20] + b_index * nm->data[i];
  }

  nx = x_segments[20].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[20].f1->data[lastColumn] = rt_roundd_snf(x_segments[20].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[20].f1->size[0];
  y_segments[20].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[20].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[20].f1->data[i] = y[20] + b_index * nm->data[i];
  }

  nx = y_segments[20].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[20].f1->data[lastColumn] = rt_roundd_snf(y_segments[20].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[22] - x[21];
  maxRange_tmp = y[22] - y[21];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[21].f1->size[0];
  x_segments[21].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[21].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[21].f1->data[i] = x[21] + b_index * nm->data[i];
  }

  nx = x_segments[21].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[21].f1->data[lastColumn] = rt_roundd_snf(x_segments[21].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[21].f1->size[0];
  y_segments[21].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[21].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[21].f1->data[i] = y[21] + b_index * nm->data[i];
  }

  nx = y_segments[21].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[21].f1->data[lastColumn] = rt_roundd_snf(y_segments[21].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[23] - x[22];
  maxRange_tmp = y[23] - y[22];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[22].f1->size[0];
  x_segments[22].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[22].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[22].f1->data[i] = x[22] + b_index * nm->data[i];
  }

  nx = x_segments[22].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[22].f1->data[lastColumn] = rt_roundd_snf(x_segments[22].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[22].f1->size[0];
  y_segments[22].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[22].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[22].f1->data[i] = y[22] + b_index * nm->data[i];
  }

  nx = y_segments[22].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[22].f1->data[lastColumn] = rt_roundd_snf(y_segments[22].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[24] - x[23];
  maxRange_tmp = y[24] - y[23];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[23].f1->size[0];
  x_segments[23].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[23].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[23].f1->data[i] = x[23] + b_index * nm->data[i];
  }

  nx = x_segments[23].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[23].f1->data[lastColumn] = rt_roundd_snf(x_segments[23].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[23].f1->size[0];
  y_segments[23].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[23].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[23].f1->data[i] = y[23] + b_index * nm->data[i];
  }

  nx = y_segments[23].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[23].f1->data[lastColumn] = rt_roundd_snf(y_segments[23].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[25] - x[24];
  maxRange_tmp = y[25] - y[24];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[24].f1->size[0];
  x_segments[24].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[24].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[24].f1->data[i] = x[24] + b_index * nm->data[i];
  }

  nx = x_segments[24].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[24].f1->data[lastColumn] = rt_roundd_snf(x_segments[24].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[24].f1->size[0];
  y_segments[24].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[24].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[24].f1->data[i] = y[24] + b_index * nm->data[i];
  }

  nx = y_segments[24].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[24].f1->data[lastColumn] = rt_roundd_snf(y_segments[24].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[26] - x[25];
  maxRange_tmp = y[26] - y[25];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[25].f1->size[0];
  x_segments[25].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[25].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[25].f1->data[i] = x[25] + b_index * nm->data[i];
  }

  nx = x_segments[25].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[25].f1->data[lastColumn] = rt_roundd_snf(x_segments[25].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[25].f1->size[0];
  y_segments[25].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[25].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[25].f1->data[i] = y[25] + b_index * nm->data[i];
  }

  nx = y_segments[25].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[25].f1->data[lastColumn] = rt_roundd_snf(y_segments[25].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[27] - x[26];
  maxRange_tmp = y[27] - y[26];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[26].f1->size[0];
  x_segments[26].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[26].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[26].f1->data[i] = x[26] + b_index * nm->data[i];
  }

  nx = x_segments[26].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[26].f1->data[lastColumn] = rt_roundd_snf(x_segments[26].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[26].f1->size[0];
  y_segments[26].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[26].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[26].f1->data[i] = y[26] + b_index * nm->data[i];
  }

  nx = y_segments[26].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[26].f1->data[lastColumn] = rt_roundd_snf(y_segments[26].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[28] - x[27];
  maxRange_tmp = y[28] - y[27];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[27].f1->size[0];
  x_segments[27].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[27].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[27].f1->data[i] = x[27] + b_index * nm->data[i];
  }

  nx = x_segments[27].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[27].f1->data[lastColumn] = rt_roundd_snf(x_segments[27].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[27].f1->size[0];
  y_segments[27].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[27].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[27].f1->data[i] = y[27] + b_index * nm->data[i];
  }

  nx = y_segments[27].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[27].f1->data[lastColumn] = rt_roundd_snf(y_segments[27].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[29] - x[28];
  maxRange_tmp = y[29] - y[28];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[28].f1->size[0];
  x_segments[28].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[28].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[28].f1->data[i] = x[28] + b_index * nm->data[i];
  }

  nx = x_segments[28].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[28].f1->data[lastColumn] = rt_roundd_snf(x_segments[28].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[28].f1->size[0];
  y_segments[28].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[28].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[28].f1->data[i] = y[28] + b_index * nm->data[i];
  }

  nx = y_segments[28].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[28].f1->data[lastColumn] = rt_roundd_snf(y_segments[28].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[30] - x[29];
  maxRange_tmp = y[30] - y[29];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[29].f1->size[0];
  x_segments[29].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[29].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[29].f1->data[i] = x[29] + b_index * nm->data[i];
  }

  nx = x_segments[29].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[29].f1->data[lastColumn] = rt_roundd_snf(x_segments[29].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[29].f1->size[0];
  y_segments[29].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[29].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[29].f1->data[i] = y[29] + b_index * nm->data[i];
  }

  nx = y_segments[29].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[29].f1->data[lastColumn] = rt_roundd_snf(y_segments[29].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[31] - x[30];
  maxRange_tmp = y[31] - y[30];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[30].f1->size[0];
  x_segments[30].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[30].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[30].f1->data[i] = x[30] + b_index * nm->data[i];
  }

  nx = x_segments[30].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[30].f1->data[lastColumn] = rt_roundd_snf(x_segments[30].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[30].f1->size[0];
  y_segments[30].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[30].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[30].f1->data[i] = y[30] + b_index * nm->data[i];
  }

  nx = y_segments[30].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[30].f1->data[lastColumn] = rt_roundd_snf(y_segments[30].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[32] - x[31];
  maxRange_tmp = y[32] - y[31];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[31].f1->size[0];
  x_segments[31].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[31].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[31].f1->data[i] = x[31] + b_index * nm->data[i];
  }

  nx = x_segments[31].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[31].f1->data[lastColumn] = rt_roundd_snf(x_segments[31].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[31].f1->size[0];
  y_segments[31].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[31].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[31].f1->data[i] = y[31] + b_index * nm->data[i];
  }

  nx = y_segments[31].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[31].f1->data[lastColumn] = rt_roundd_snf(y_segments[31].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[33] - x[32];
  maxRange_tmp = y[33] - y[32];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[32].f1->size[0];
  x_segments[32].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[32].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[32].f1->data[i] = x[32] + b_index * nm->data[i];
  }

  nx = x_segments[32].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[32].f1->data[lastColumn] = rt_roundd_snf(x_segments[32].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[32].f1->size[0];
  y_segments[32].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[32].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[32].f1->data[i] = y[32] + b_index * nm->data[i];
  }

  nx = y_segments[32].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[32].f1->data[lastColumn] = rt_roundd_snf(y_segments[32].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[34] - x[33];
  maxRange_tmp = y[34] - y[33];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[33].f1->size[0];
  x_segments[33].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[33].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[33].f1->data[i] = x[33] + b_index * nm->data[i];
  }

  nx = x_segments[33].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[33].f1->data[lastColumn] = rt_roundd_snf(x_segments[33].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[33].f1->size[0];
  y_segments[33].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[33].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[33].f1->data[i] = y[33] + b_index * nm->data[i];
  }

  nx = y_segments[33].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[33].f1->data[lastColumn] = rt_roundd_snf(y_segments[33].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[35] - x[34];
  maxRange_tmp = y[35] - y[34];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[34].f1->size[0];
  x_segments[34].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[34].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[34].f1->data[i] = x[34] + b_index * nm->data[i];
  }

  nx = x_segments[34].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[34].f1->data[lastColumn] = rt_roundd_snf(x_segments[34].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[34].f1->size[0];
  y_segments[34].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[34].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[34].f1->data[i] = y[34] + b_index * nm->data[i];
  }

  nx = y_segments[34].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[34].f1->data[lastColumn] = rt_roundd_snf(y_segments[34].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[36] - x[35];
  maxRange_tmp = y[36] - y[35];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[35].f1->size[0];
  x_segments[35].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[35].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[35].f1->data[i] = x[35] + b_index * nm->data[i];
  }

  nx = x_segments[35].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[35].f1->data[lastColumn] = rt_roundd_snf(x_segments[35].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[35].f1->size[0];
  y_segments[35].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[35].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[35].f1->data[i] = y[35] + b_index * nm->data[i];
  }

  nx = y_segments[35].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[35].f1->data[lastColumn] = rt_roundd_snf(y_segments[35].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[37] - x[36];
  maxRange_tmp = y[37] - y[36];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[36].f1->size[0];
  x_segments[36].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[36].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[36].f1->data[i] = x[36] + b_index * nm->data[i];
  }

  nx = x_segments[36].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[36].f1->data[lastColumn] = rt_roundd_snf(x_segments[36].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[36].f1->size[0];
  y_segments[36].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[36].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[36].f1->data[i] = y[36] + b_index * nm->data[i];
  }

  nx = y_segments[36].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[36].f1->data[lastColumn] = rt_roundd_snf(y_segments[36].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[38] - x[37];
  maxRange_tmp = y[38] - y[37];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[37].f1->size[0];
  x_segments[37].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[37].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[37].f1->data[i] = x[37] + b_index * nm->data[i];
  }

  nx = x_segments[37].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[37].f1->data[lastColumn] = rt_roundd_snf(x_segments[37].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[37].f1->size[0];
  y_segments[37].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[37].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[37].f1->data[i] = y[37] + b_index * nm->data[i];
  }

  nx = y_segments[37].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[37].f1->data[lastColumn] = rt_roundd_snf(y_segments[37].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[39] - x[38];
  maxRange_tmp = y[39] - y[38];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[38].f1->size[0];
  x_segments[38].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[38].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[38].f1->data[i] = x[38] + b_index * nm->data[i];
  }

  nx = x_segments[38].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[38].f1->data[lastColumn] = rt_roundd_snf(x_segments[38].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[38].f1->size[0];
  y_segments[38].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[38].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[38].f1->data[i] = y[38] + b_index * nm->data[i];
  }

  nx = y_segments[38].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[38].f1->data[lastColumn] = rt_roundd_snf(y_segments[38].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[40] - x[39];
  maxRange_tmp = y[40] - y[39];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[39].f1->size[0];
  x_segments[39].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[39].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[39].f1->data[i] = x[39] + b_index * nm->data[i];
  }

  nx = x_segments[39].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[39].f1->data[lastColumn] = rt_roundd_snf(x_segments[39].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[39].f1->size[0];
  y_segments[39].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[39].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[39].f1->data[i] = y[39] + b_index * nm->data[i];
  }

  nx = y_segments[39].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[39].f1->data[lastColumn] = rt_roundd_snf(y_segments[39].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[41] - x[40];
  maxRange_tmp = y[41] - y[40];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[40].f1->size[0];
  x_segments[40].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[40].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[40].f1->data[i] = x[40] + b_index * nm->data[i];
  }

  nx = x_segments[40].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[40].f1->data[lastColumn] = rt_roundd_snf(x_segments[40].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[40].f1->size[0];
  y_segments[40].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[40].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[40].f1->data[i] = y[40] + b_index * nm->data[i];
  }

  nx = y_segments[40].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[40].f1->data[lastColumn] = rt_roundd_snf(y_segments[40].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[42] - x[41];
  maxRange_tmp = y[42] - y[41];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[41].f1->size[0];
  x_segments[41].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[41].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[41].f1->data[i] = x[41] + b_index * nm->data[i];
  }

  nx = x_segments[41].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[41].f1->data[lastColumn] = rt_roundd_snf(x_segments[41].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[41].f1->size[0];
  y_segments[41].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[41].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[41].f1->data[i] = y[41] + b_index * nm->data[i];
  }

  nx = y_segments[41].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[41].f1->data[lastColumn] = rt_roundd_snf(y_segments[41].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[43] - x[42];
  maxRange_tmp = y[43] - y[42];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[42].f1->size[0];
  x_segments[42].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[42].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[42].f1->data[i] = x[42] + b_index * nm->data[i];
  }

  nx = x_segments[42].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[42].f1->data[lastColumn] = rt_roundd_snf(x_segments[42].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[42].f1->size[0];
  y_segments[42].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[42].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[42].f1->data[i] = y[42] + b_index * nm->data[i];
  }

  nx = y_segments[42].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[42].f1->data[lastColumn] = rt_roundd_snf(y_segments[42].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[44] - x[43];
  maxRange_tmp = y[44] - y[43];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[43].f1->size[0];
  x_segments[43].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[43].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[43].f1->data[i] = x[43] + b_index * nm->data[i];
  }

  nx = x_segments[43].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[43].f1->data[lastColumn] = rt_roundd_snf(x_segments[43].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[43].f1->size[0];
  y_segments[43].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[43].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[43].f1->data[i] = y[43] + b_index * nm->data[i];
  }

  nx = y_segments[43].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[43].f1->data[lastColumn] = rt_roundd_snf(y_segments[43].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[45] - x[44];
  maxRange_tmp = y[45] - y[44];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[44].f1->size[0];
  x_segments[44].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[44].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[44].f1->data[i] = x[44] + b_index * nm->data[i];
  }

  nx = x_segments[44].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[44].f1->data[lastColumn] = rt_roundd_snf(x_segments[44].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[44].f1->size[0];
  y_segments[44].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[44].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[44].f1->data[i] = y[44] + b_index * nm->data[i];
  }

  nx = y_segments[44].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[44].f1->data[lastColumn] = rt_roundd_snf(y_segments[44].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[46] - x[45];
  maxRange_tmp = y[46] - y[45];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[45].f1->size[0];
  x_segments[45].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[45].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[45].f1->data[i] = x[45] + b_index * nm->data[i];
  }

  nx = x_segments[45].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[45].f1->data[lastColumn] = rt_roundd_snf(x_segments[45].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[45].f1->size[0];
  y_segments[45].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[45].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[45].f1->data[i] = y[45] + b_index * nm->data[i];
  }

  nx = y_segments[45].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[45].f1->data[lastColumn] = rt_roundd_snf(y_segments[45].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[47] - x[46];
  maxRange_tmp = y[47] - y[46];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[46].f1->size[0];
  x_segments[46].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[46].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[46].f1->data[i] = x[46] + b_index * nm->data[i];
  }

  nx = x_segments[46].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[46].f1->data[lastColumn] = rt_roundd_snf(x_segments[46].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[46].f1->size[0];
  y_segments[46].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[46].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[46].f1->data[i] = y[46] + b_index * nm->data[i];
  }

  nx = y_segments[46].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[46].f1->data[lastColumn] = rt_roundd_snf(y_segments[46].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[48] - x[47];
  maxRange_tmp = y[48] - y[47];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[47].f1->size[0];
  x_segments[47].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[47].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[47].f1->data[i] = x[47] + b_index * nm->data[i];
  }

  nx = x_segments[47].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[47].f1->data[lastColumn] = rt_roundd_snf(x_segments[47].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[47].f1->size[0];
  y_segments[47].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[47].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[47].f1->data[i] = y[47] + b_index * nm->data[i];
  }

  nx = y_segments[47].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[47].f1->data[lastColumn] = rt_roundd_snf(y_segments[47].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[49] - x[48];
  maxRange_tmp = y[49] - y[48];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[48].f1->size[0];
  x_segments[48].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[48].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[48].f1->data[i] = x[48] + b_index * nm->data[i];
  }

  nx = x_segments[48].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[48].f1->data[lastColumn] = rt_roundd_snf(x_segments[48].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[48].f1->size[0];
  y_segments[48].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[48].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[48].f1->data[i] = y[48] + b_index * nm->data[i];
  }

  nx = y_segments[48].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[48].f1->data[lastColumn] = rt_roundd_snf(y_segments[48].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[50] - x[49];
  maxRange_tmp = y[50] - y[49];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[49].f1->size[0];
  x_segments[49].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[49].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[49].f1->data[i] = x[49] + b_index * nm->data[i];
  }

  nx = x_segments[49].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[49].f1->data[lastColumn] = rt_roundd_snf(x_segments[49].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[49].f1->size[0];
  y_segments[49].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[49].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[49].f1->data[i] = y[49] + b_index * nm->data[i];
  }

  nx = y_segments[49].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[49].f1->data[lastColumn] = rt_roundd_snf(y_segments[49].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[51] - x[50];
  maxRange_tmp = y[51] - y[50];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[50].f1->size[0];
  x_segments[50].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[50].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[50].f1->data[i] = x[50] + b_index * nm->data[i];
  }

  nx = x_segments[50].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[50].f1->data[lastColumn] = rt_roundd_snf(x_segments[50].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[50].f1->size[0];
  y_segments[50].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[50].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[50].f1->data[i] = y[50] + b_index * nm->data[i];
  }

  nx = y_segments[50].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[50].f1->data[lastColumn] = rt_roundd_snf(y_segments[50].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[52] - x[51];
  maxRange_tmp = y[52] - y[51];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[51].f1->size[0];
  x_segments[51].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[51].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[51].f1->data[i] = x[51] + b_index * nm->data[i];
  }

  nx = x_segments[51].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[51].f1->data[lastColumn] = rt_roundd_snf(x_segments[51].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[51].f1->size[0];
  y_segments[51].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[51].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[51].f1->data[i] = y[51] + b_index * nm->data[i];
  }

  nx = y_segments[51].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[51].f1->data[lastColumn] = rt_roundd_snf(y_segments[51].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[53] - x[52];
  maxRange_tmp = y[53] - y[52];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[52].f1->size[0];
  x_segments[52].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[52].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[52].f1->data[i] = x[52] + b_index * nm->data[i];
  }

  nx = x_segments[52].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[52].f1->data[lastColumn] = rt_roundd_snf(x_segments[52].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[52].f1->size[0];
  y_segments[52].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[52].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[52].f1->data[i] = y[52] + b_index * nm->data[i];
  }

  nx = y_segments[52].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[52].f1->data[lastColumn] = rt_roundd_snf(y_segments[52].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[54] - x[53];
  maxRange_tmp = y[54] - y[53];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[53].f1->size[0];
  x_segments[53].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[53].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[53].f1->data[i] = x[53] + b_index * nm->data[i];
  }

  nx = x_segments[53].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[53].f1->data[lastColumn] = rt_roundd_snf(x_segments[53].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[53].f1->size[0];
  y_segments[53].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[53].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[53].f1->data[i] = y[53] + b_index * nm->data[i];
  }

  nx = y_segments[53].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[53].f1->data[lastColumn] = rt_roundd_snf(y_segments[53].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[55] - x[54];
  maxRange_tmp = y[55] - y[54];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[54].f1->size[0];
  x_segments[54].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[54].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[54].f1->data[i] = x[54] + b_index * nm->data[i];
  }

  nx = x_segments[54].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[54].f1->data[lastColumn] = rt_roundd_snf(x_segments[54].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[54].f1->size[0];
  y_segments[54].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[54].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[54].f1->data[i] = y[54] + b_index * nm->data[i];
  }

  nx = y_segments[54].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[54].f1->data[lastColumn] = rt_roundd_snf(y_segments[54].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[56] - x[55];
  maxRange_tmp = y[56] - y[55];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[55].f1->size[0];
  x_segments[55].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[55].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[55].f1->data[i] = x[55] + b_index * nm->data[i];
  }

  nx = x_segments[55].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[55].f1->data[lastColumn] = rt_roundd_snf(x_segments[55].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[55].f1->size[0];
  y_segments[55].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[55].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[55].f1->data[i] = y[55] + b_index * nm->data[i];
  }

  nx = y_segments[55].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[55].f1->data[lastColumn] = rt_roundd_snf(y_segments[55].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[57] - x[56];
  maxRange_tmp = y[57] - y[56];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[56].f1->size[0];
  x_segments[56].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[56].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[56].f1->data[i] = x[56] + b_index * nm->data[i];
  }

  nx = x_segments[56].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[56].f1->data[lastColumn] = rt_roundd_snf(x_segments[56].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[56].f1->size[0];
  y_segments[56].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[56].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[56].f1->data[i] = y[56] + b_index * nm->data[i];
  }

  nx = y_segments[56].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[56].f1->data[lastColumn] = rt_roundd_snf(y_segments[56].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[58] - x[57];
  maxRange_tmp = y[58] - y[57];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[57].f1->size[0];
  x_segments[57].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[57].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[57].f1->data[i] = x[57] + b_index * nm->data[i];
  }

  nx = x_segments[57].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[57].f1->data[lastColumn] = rt_roundd_snf(x_segments[57].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[57].f1->size[0];
  y_segments[57].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[57].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[57].f1->data[i] = y[57] + b_index * nm->data[i];
  }

  nx = y_segments[57].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[57].f1->data[lastColumn] = rt_roundd_snf(y_segments[57].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[59] - x[58];
  maxRange_tmp = y[59] - y[58];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[58].f1->size[0];
  x_segments[58].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[58].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[58].f1->data[i] = x[58] + b_index * nm->data[i];
  }

  nx = x_segments[58].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[58].f1->data[lastColumn] = rt_roundd_snf(x_segments[58].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[58].f1->size[0];
  y_segments[58].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[58].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[58].f1->data[i] = y[58] + b_index * nm->data[i];
  }

  nx = y_segments[58].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[58].f1->data[lastColumn] = rt_roundd_snf(y_segments[58].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[60] - x[59];
  maxRange_tmp = y[60] - y[59];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[59].f1->size[0];
  x_segments[59].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[59].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[59].f1->data[i] = x[59] + b_index * nm->data[i];
  }

  nx = x_segments[59].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[59].f1->data[lastColumn] = rt_roundd_snf(x_segments[59].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[59].f1->size[0];
  y_segments[59].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[59].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[59].f1->data[i] = y[59] + b_index * nm->data[i];
  }

  nx = y_segments[59].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[59].f1->data[lastColumn] = rt_roundd_snf(y_segments[59].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[61] - x[60];
  maxRange_tmp = y[61] - y[60];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[60].f1->size[0];
  x_segments[60].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[60].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[60].f1->data[i] = x[60] + b_index * nm->data[i];
  }

  nx = x_segments[60].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[60].f1->data[lastColumn] = rt_roundd_snf(x_segments[60].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[60].f1->size[0];
  y_segments[60].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[60].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[60].f1->data[i] = y[60] + b_index * nm->data[i];
  }

  nx = y_segments[60].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[60].f1->data[lastColumn] = rt_roundd_snf(y_segments[60].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[62] - x[61];
  maxRange_tmp = y[62] - y[61];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[61].f1->size[0];
  x_segments[61].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[61].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[61].f1->data[i] = x[61] + b_index * nm->data[i];
  }

  nx = x_segments[61].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[61].f1->data[lastColumn] = rt_roundd_snf(x_segments[61].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[61].f1->size[0];
  y_segments[61].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[61].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[61].f1->data[i] = y[61] + b_index * nm->data[i];
  }

  nx = y_segments[61].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[61].f1->data[lastColumn] = rt_roundd_snf(y_segments[61].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[63] - x[62];
  maxRange_tmp = y[63] - y[62];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[62].f1->size[0];
  x_segments[62].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[62].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[62].f1->data[i] = x[62] + b_index * nm->data[i];
  }

  nx = x_segments[62].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[62].f1->data[lastColumn] = rt_roundd_snf(x_segments[62].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[62].f1->size[0];
  y_segments[62].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[62].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[62].f1->data[i] = y[62] + b_index * nm->data[i];
  }

  nx = y_segments[62].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[62].f1->data[lastColumn] = rt_roundd_snf(y_segments[62].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[64] - x[63];
  maxRange_tmp = y[64] - y[63];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[63].f1->size[0];
  x_segments[63].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[63].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[63].f1->data[i] = x[63] + b_index * nm->data[i];
  }

  nx = x_segments[63].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[63].f1->data[lastColumn] = rt_roundd_snf(x_segments[63].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[63].f1->size[0];
  y_segments[63].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[63].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[63].f1->data[i] = y[63] + b_index * nm->data[i];
  }

  nx = y_segments[63].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[63].f1->data[lastColumn] = rt_roundd_snf(y_segments[63].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[65] - x[64];
  maxRange_tmp = y[65] - y[64];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[64].f1->size[0];
  x_segments[64].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[64].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[64].f1->data[i] = x[64] + b_index * nm->data[i];
  }

  nx = x_segments[64].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[64].f1->data[lastColumn] = rt_roundd_snf(x_segments[64].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[64].f1->size[0];
  y_segments[64].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[64].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[64].f1->data[i] = y[64] + b_index * nm->data[i];
  }

  nx = y_segments[64].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[64].f1->data[lastColumn] = rt_roundd_snf(y_segments[64].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[66] - x[65];
  maxRange_tmp = y[66] - y[65];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[65].f1->size[0];
  x_segments[65].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[65].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[65].f1->data[i] = x[65] + b_index * nm->data[i];
  }

  nx = x_segments[65].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[65].f1->data[lastColumn] = rt_roundd_snf(x_segments[65].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[65].f1->size[0];
  y_segments[65].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[65].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[65].f1->data[i] = y[65] + b_index * nm->data[i];
  }

  nx = y_segments[65].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[65].f1->data[lastColumn] = rt_roundd_snf(y_segments[65].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[67] - x[66];
  maxRange_tmp = y[67] - y[66];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[66].f1->size[0];
  x_segments[66].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[66].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[66].f1->data[i] = x[66] + b_index * nm->data[i];
  }

  nx = x_segments[66].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[66].f1->data[lastColumn] = rt_roundd_snf(x_segments[66].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[66].f1->size[0];
  y_segments[66].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[66].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[66].f1->data[i] = y[66] + b_index * nm->data[i];
  }

  nx = y_segments[66].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[66].f1->data[lastColumn] = rt_roundd_snf(y_segments[66].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[68] - x[67];
  maxRange_tmp = y[68] - y[67];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[67].f1->size[0];
  x_segments[67].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[67].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[67].f1->data[i] = x[67] + b_index * nm->data[i];
  }

  nx = x_segments[67].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[67].f1->data[lastColumn] = rt_roundd_snf(x_segments[67].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[67].f1->size[0];
  y_segments[67].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[67].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[67].f1->data[i] = y[67] + b_index * nm->data[i];
  }

  nx = y_segments[67].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[67].f1->data[lastColumn] = rt_roundd_snf(y_segments[67].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[69] - x[68];
  maxRange_tmp = y[69] - y[68];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[68].f1->size[0];
  x_segments[68].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[68].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[68].f1->data[i] = x[68] + b_index * nm->data[i];
  }

  nx = x_segments[68].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[68].f1->data[lastColumn] = rt_roundd_snf(x_segments[68].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[68].f1->size[0];
  y_segments[68].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[68].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[68].f1->data[i] = y[68] + b_index * nm->data[i];
  }

  nx = y_segments[68].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[68].f1->data[lastColumn] = rt_roundd_snf(y_segments[68].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[70] - x[69];
  maxRange_tmp = y[70] - y[69];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[69].f1->size[0];
  x_segments[69].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[69].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[69].f1->data[i] = x[69] + b_index * nm->data[i];
  }

  nx = x_segments[69].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[69].f1->data[lastColumn] = rt_roundd_snf(x_segments[69].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[69].f1->size[0];
  y_segments[69].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[69].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[69].f1->data[i] = y[69] + b_index * nm->data[i];
  }

  nx = y_segments[69].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[69].f1->data[lastColumn] = rt_roundd_snf(y_segments[69].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[71] - x[70];
  maxRange_tmp = y[71] - y[70];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[70].f1->size[0];
  x_segments[70].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[70].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[70].f1->data[i] = x[70] + b_index * nm->data[i];
  }

  nx = x_segments[70].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[70].f1->data[lastColumn] = rt_roundd_snf(x_segments[70].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[70].f1->size[0];
  y_segments[70].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[70].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[70].f1->data[i] = y[70] + b_index * nm->data[i];
  }

  nx = y_segments[70].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[70].f1->data[lastColumn] = rt_roundd_snf(y_segments[70].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[72] - x[71];
  maxRange_tmp = y[72] - y[71];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[71].f1->size[0];
  x_segments[71].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[71].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[71].f1->data[i] = x[71] + b_index * nm->data[i];
  }

  nx = x_segments[71].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[71].f1->data[lastColumn] = rt_roundd_snf(x_segments[71].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[71].f1->size[0];
  y_segments[71].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[71].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[71].f1->data[i] = y[71] + b_index * nm->data[i];
  }

  nx = y_segments[71].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[71].f1->data[lastColumn] = rt_roundd_snf(y_segments[71].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[73] - x[72];
  maxRange_tmp = y[73] - y[72];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[72].f1->size[0];
  x_segments[72].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[72].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[72].f1->data[i] = x[72] + b_index * nm->data[i];
  }

  nx = x_segments[72].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[72].f1->data[lastColumn] = rt_roundd_snf(x_segments[72].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[72].f1->size[0];
  y_segments[72].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[72].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[72].f1->data[i] = y[72] + b_index * nm->data[i];
  }

  nx = y_segments[72].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[72].f1->data[lastColumn] = rt_roundd_snf(y_segments[72].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[74] - x[73];
  maxRange_tmp = y[74] - y[73];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[73].f1->size[0];
  x_segments[73].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[73].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[73].f1->data[i] = x[73] + b_index * nm->data[i];
  }

  nx = x_segments[73].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[73].f1->data[lastColumn] = rt_roundd_snf(x_segments[73].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[73].f1->size[0];
  y_segments[73].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[73].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[73].f1->data[i] = y[73] + b_index * nm->data[i];
  }

  nx = y_segments[73].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[73].f1->data[lastColumn] = rt_roundd_snf(y_segments[73].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[75] - x[74];
  maxRange_tmp = y[75] - y[74];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[74].f1->size[0];
  x_segments[74].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[74].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[74].f1->data[i] = x[74] + b_index * nm->data[i];
  }

  nx = x_segments[74].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[74].f1->data[lastColumn] = rt_roundd_snf(x_segments[74].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[74].f1->size[0];
  y_segments[74].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[74].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[74].f1->data[i] = y[74] + b_index * nm->data[i];
  }

  nx = y_segments[74].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[74].f1->data[lastColumn] = rt_roundd_snf(y_segments[74].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[76] - x[75];
  maxRange_tmp = y[76] - y[75];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[75].f1->size[0];
  x_segments[75].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[75].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[75].f1->data[i] = x[75] + b_index * nm->data[i];
  }

  nx = x_segments[75].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[75].f1->data[lastColumn] = rt_roundd_snf(x_segments[75].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[75].f1->size[0];
  y_segments[75].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[75].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[75].f1->data[i] = y[75] + b_index * nm->data[i];
  }

  nx = y_segments[75].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[75].f1->data[lastColumn] = rt_roundd_snf(y_segments[75].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[77] - x[76];
  maxRange_tmp = y[77] - y[76];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[76].f1->size[0];
  x_segments[76].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[76].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[76].f1->data[i] = x[76] + b_index * nm->data[i];
  }

  nx = x_segments[76].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[76].f1->data[lastColumn] = rt_roundd_snf(x_segments[76].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[76].f1->size[0];
  y_segments[76].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[76].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[76].f1->data[i] = y[76] + b_index * nm->data[i];
  }

  nx = y_segments[76].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[76].f1->data[lastColumn] = rt_roundd_snf(y_segments[76].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[78] - x[77];
  maxRange_tmp = y[78] - y[77];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[77].f1->size[0];
  x_segments[77].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[77].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[77].f1->data[i] = x[77] + b_index * nm->data[i];
  }

  nx = x_segments[77].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[77].f1->data[lastColumn] = rt_roundd_snf(x_segments[77].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[77].f1->size[0];
  y_segments[77].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[77].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[77].f1->data[i] = y[77] + b_index * nm->data[i];
  }

  nx = y_segments[77].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[77].f1->data[lastColumn] = rt_roundd_snf(y_segments[77].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[79] - x[78];
  maxRange_tmp = y[79] - y[78];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[78].f1->size[0];
  x_segments[78].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[78].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[78].f1->data[i] = x[78] + b_index * nm->data[i];
  }

  nx = x_segments[78].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[78].f1->data[lastColumn] = rt_roundd_snf(x_segments[78].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[78].f1->size[0];
  y_segments[78].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[78].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[78].f1->data[i] = y[78] + b_index * nm->data[i];
  }

  nx = y_segments[78].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[78].f1->data[lastColumn] = rt_roundd_snf(y_segments[78].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[80] - x[79];
  maxRange_tmp = y[80] - y[79];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[79].f1->size[0];
  x_segments[79].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[79].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[79].f1->data[i] = x[79] + b_index * nm->data[i];
  }

  nx = x_segments[79].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[79].f1->data[lastColumn] = rt_roundd_snf(x_segments[79].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[79].f1->size[0];
  y_segments[79].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[79].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[79].f1->data[i] = y[79] + b_index * nm->data[i];
  }

  nx = y_segments[79].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[79].f1->data[lastColumn] = rt_roundd_snf(y_segments[79].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[81] - x[80];
  maxRange_tmp = y[81] - y[80];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[80].f1->size[0];
  x_segments[80].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[80].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[80].f1->data[i] = x[80] + b_index * nm->data[i];
  }

  nx = x_segments[80].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[80].f1->data[lastColumn] = rt_roundd_snf(x_segments[80].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[80].f1->size[0];
  y_segments[80].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[80].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[80].f1->data[i] = y[80] + b_index * nm->data[i];
  }

  nx = y_segments[80].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[80].f1->data[lastColumn] = rt_roundd_snf(y_segments[80].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[82] - x[81];
  maxRange_tmp = y[82] - y[81];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[81].f1->size[0];
  x_segments[81].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[81].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[81].f1->data[i] = x[81] + b_index * nm->data[i];
  }

  nx = x_segments[81].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[81].f1->data[lastColumn] = rt_roundd_snf(x_segments[81].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[81].f1->size[0];
  y_segments[81].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[81].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[81].f1->data[i] = y[81] + b_index * nm->data[i];
  }

  nx = y_segments[81].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[81].f1->data[lastColumn] = rt_roundd_snf(y_segments[81].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[83] - x[82];
  maxRange_tmp = y[83] - y[82];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[82].f1->size[0];
  x_segments[82].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[82].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[82].f1->data[i] = x[82] + b_index * nm->data[i];
  }

  nx = x_segments[82].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[82].f1->data[lastColumn] = rt_roundd_snf(x_segments[82].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[82].f1->size[0];
  y_segments[82].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[82].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[82].f1->data[i] = y[82] + b_index * nm->data[i];
  }

  nx = y_segments[82].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[82].f1->data[lastColumn] = rt_roundd_snf(y_segments[82].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[84] - x[83];
  maxRange_tmp = y[84] - y[83];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[83].f1->size[0];
  x_segments[83].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[83].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[83].f1->data[i] = x[83] + b_index * nm->data[i];
  }

  nx = x_segments[83].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[83].f1->data[lastColumn] = rt_roundd_snf(x_segments[83].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[83].f1->size[0];
  y_segments[83].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[83].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[83].f1->data[i] = y[83] + b_index * nm->data[i];
  }

  nx = y_segments[83].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[83].f1->data[lastColumn] = rt_roundd_snf(y_segments[83].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[85] - x[84];
  maxRange_tmp = y[85] - y[84];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[84].f1->size[0];
  x_segments[84].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[84].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[84].f1->data[i] = x[84] + b_index * nm->data[i];
  }

  nx = x_segments[84].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[84].f1->data[lastColumn] = rt_roundd_snf(x_segments[84].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[84].f1->size[0];
  y_segments[84].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[84].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[84].f1->data[i] = y[84] + b_index * nm->data[i];
  }

  nx = y_segments[84].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[84].f1->data[lastColumn] = rt_roundd_snf(y_segments[84].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[86] - x[85];
  maxRange_tmp = y[86] - y[85];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[85].f1->size[0];
  x_segments[85].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[85].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[85].f1->data[i] = x[85] + b_index * nm->data[i];
  }

  nx = x_segments[85].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[85].f1->data[lastColumn] = rt_roundd_snf(x_segments[85].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[85].f1->size[0];
  y_segments[85].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[85].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[85].f1->data[i] = y[85] + b_index * nm->data[i];
  }

  nx = y_segments[85].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[85].f1->data[lastColumn] = rt_roundd_snf(y_segments[85].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[87] - x[86];
  maxRange_tmp = y[87] - y[86];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[86].f1->size[0];
  x_segments[86].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[86].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[86].f1->data[i] = x[86] + b_index * nm->data[i];
  }

  nx = x_segments[86].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[86].f1->data[lastColumn] = rt_roundd_snf(x_segments[86].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[86].f1->size[0];
  y_segments[86].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[86].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[86].f1->data[i] = y[86] + b_index * nm->data[i];
  }

  nx = y_segments[86].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[86].f1->data[lastColumn] = rt_roundd_snf(y_segments[86].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[88] - x[87];
  maxRange_tmp = y[88] - y[87];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[87].f1->size[0];
  x_segments[87].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[87].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[87].f1->data[i] = x[87] + b_index * nm->data[i];
  }

  nx = x_segments[87].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[87].f1->data[lastColumn] = rt_roundd_snf(x_segments[87].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[87].f1->size[0];
  y_segments[87].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[87].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[87].f1->data[i] = y[87] + b_index * nm->data[i];
  }

  nx = y_segments[87].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[87].f1->data[lastColumn] = rt_roundd_snf(y_segments[87].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[89] - x[88];
  maxRange_tmp = y[89] - y[88];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[88].f1->size[0];
  x_segments[88].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[88].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[88].f1->data[i] = x[88] + b_index * nm->data[i];
  }

  nx = x_segments[88].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[88].f1->data[lastColumn] = rt_roundd_snf(x_segments[88].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[88].f1->size[0];
  y_segments[88].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[88].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[88].f1->data[i] = y[88] + b_index * nm->data[i];
  }

  nx = y_segments[88].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[88].f1->data[lastColumn] = rt_roundd_snf(y_segments[88].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[90] - x[89];
  maxRange_tmp = y[90] - y[89];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[89].f1->size[0];
  x_segments[89].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[89].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[89].f1->data[i] = x[89] + b_index * nm->data[i];
  }

  nx = x_segments[89].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[89].f1->data[lastColumn] = rt_roundd_snf(x_segments[89].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[89].f1->size[0];
  y_segments[89].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[89].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[89].f1->data[i] = y[89] + b_index * nm->data[i];
  }

  nx = y_segments[89].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[89].f1->data[lastColumn] = rt_roundd_snf(y_segments[89].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[91] - x[90];
  maxRange_tmp = y[91] - y[90];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[90].f1->size[0];
  x_segments[90].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[90].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[90].f1->data[i] = x[90] + b_index * nm->data[i];
  }

  nx = x_segments[90].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[90].f1->data[lastColumn] = rt_roundd_snf(x_segments[90].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[90].f1->size[0];
  y_segments[90].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[90].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[90].f1->data[i] = y[90] + b_index * nm->data[i];
  }

  nx = y_segments[90].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[90].f1->data[lastColumn] = rt_roundd_snf(y_segments[90].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[92] - x[91];
  maxRange_tmp = y[92] - y[91];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[91].f1->size[0];
  x_segments[91].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[91].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[91].f1->data[i] = x[91] + b_index * nm->data[i];
  }

  nx = x_segments[91].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[91].f1->data[lastColumn] = rt_roundd_snf(x_segments[91].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[91].f1->size[0];
  y_segments[91].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[91].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[91].f1->data[i] = y[91] + b_index * nm->data[i];
  }

  nx = y_segments[91].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[91].f1->data[lastColumn] = rt_roundd_snf(y_segments[91].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[93] - x[92];
  maxRange_tmp = y[93] - y[92];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[92].f1->size[0];
  x_segments[92].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[92].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[92].f1->data[i] = x[92] + b_index * nm->data[i];
  }

  nx = x_segments[92].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[92].f1->data[lastColumn] = rt_roundd_snf(x_segments[92].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[92].f1->size[0];
  y_segments[92].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[92].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[92].f1->data[i] = y[92] + b_index * nm->data[i];
  }

  nx = y_segments[92].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[92].f1->data[lastColumn] = rt_roundd_snf(y_segments[92].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[94] - x[93];
  maxRange_tmp = y[94] - y[93];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[93].f1->size[0];
  x_segments[93].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[93].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[93].f1->data[i] = x[93] + b_index * nm->data[i];
  }

  nx = x_segments[93].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[93].f1->data[lastColumn] = rt_roundd_snf(x_segments[93].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[93].f1->size[0];
  y_segments[93].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[93].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[93].f1->data[i] = y[93] + b_index * nm->data[i];
  }

  nx = y_segments[93].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[93].f1->data[lastColumn] = rt_roundd_snf(y_segments[93].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[95] - x[94];
  maxRange_tmp = y[95] - y[94];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[94].f1->size[0];
  x_segments[94].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[94].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[94].f1->data[i] = x[94] + b_index * nm->data[i];
  }

  nx = x_segments[94].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[94].f1->data[lastColumn] = rt_roundd_snf(x_segments[94].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[94].f1->size[0];
  y_segments[94].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[94].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[94].f1->data[i] = y[94] + b_index * nm->data[i];
  }

  nx = y_segments[94].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[94].f1->data[lastColumn] = rt_roundd_snf(y_segments[94].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[96] - x[95];
  maxRange_tmp = y[96] - y[95];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[95].f1->size[0];
  x_segments[95].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[95].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[95].f1->data[i] = x[95] + b_index * nm->data[i];
  }

  nx = x_segments[95].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[95].f1->data[lastColumn] = rt_roundd_snf(x_segments[95].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[95].f1->size[0];
  y_segments[95].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[95].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[95].f1->data[i] = y[95] + b_index * nm->data[i];
  }

  nx = y_segments[95].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[95].f1->data[lastColumn] = rt_roundd_snf(y_segments[95].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[97] - x[96];
  maxRange_tmp = y[97] - y[96];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[96].f1->size[0];
  x_segments[96].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[96].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[96].f1->data[i] = x[96] + b_index * nm->data[i];
  }

  nx = x_segments[96].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[96].f1->data[lastColumn] = rt_roundd_snf(x_segments[96].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[96].f1->size[0];
  y_segments[96].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[96].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[96].f1->data[i] = y[96] + b_index * nm->data[i];
  }

  nx = y_segments[96].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[96].f1->data[lastColumn] = rt_roundd_snf(y_segments[96].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[98] - x[97];
  maxRange_tmp = y[98] - y[97];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[97].f1->size[0];
  x_segments[97].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[97].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[97].f1->data[i] = x[97] + b_index * nm->data[i];
  }

  nx = x_segments[97].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[97].f1->data[lastColumn] = rt_roundd_snf(x_segments[97].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[97].f1->size[0];
  y_segments[97].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[97].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[97].f1->data[i] = y[97] + b_index * nm->data[i];
  }

  nx = y_segments[97].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[97].f1->data[lastColumn] = rt_roundd_snf(y_segments[97].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[99] - x[98];
  maxRange_tmp = y[99] - y[98];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[98].f1->size[0];
  x_segments[98].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[98].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[98].f1->data[i] = x[98] + b_index * nm->data[i];
  }

  nx = x_segments[98].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[98].f1->data[lastColumn] = rt_roundd_snf(x_segments[98].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[98].f1->size[0];
  y_segments[98].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[98].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[98].f1->data[i] = y[98] + b_index * nm->data[i];
  }

  nx = y_segments[98].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[98].f1->data[lastColumn] = rt_roundd_snf(y_segments[98].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[100] - x[99];
  maxRange_tmp = y[100] - y[99];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[99].f1->size[0];
  x_segments[99].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[99].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[99].f1->data[i] = x[99] + b_index * nm->data[i];
  }

  nx = x_segments[99].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[99].f1->data[lastColumn] = rt_roundd_snf(x_segments[99].f1->
      data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[99].f1->size[0];
  y_segments[99].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[99].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[99].f1->data[i] = y[99] + b_index * nm->data[i];
  }

  nx = y_segments[99].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[99].f1->data[lastColumn] = rt_roundd_snf(y_segments[99].f1->
      data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[101] - x[100];
  maxRange_tmp = y[101] - y[100];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[100].f1->size[0];
  x_segments[100].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[100].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[100].f1->data[i] = x[100] + b_index * nm->data[i];
  }

  nx = x_segments[100].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[100].f1->data[lastColumn] = rt_roundd_snf(x_segments[100]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[100].f1->size[0];
  y_segments[100].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[100].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[100].f1->data[i] = y[100] + b_index * nm->data[i];
  }

  nx = y_segments[100].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[100].f1->data[lastColumn] = rt_roundd_snf(y_segments[100]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[102] - x[101];
  maxRange_tmp = y[102] - y[101];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[101].f1->size[0];
  x_segments[101].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[101].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[101].f1->data[i] = x[101] + b_index * nm->data[i];
  }

  nx = x_segments[101].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[101].f1->data[lastColumn] = rt_roundd_snf(x_segments[101]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[101].f1->size[0];
  y_segments[101].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[101].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[101].f1->data[i] = y[101] + b_index * nm->data[i];
  }

  nx = y_segments[101].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[101].f1->data[lastColumn] = rt_roundd_snf(y_segments[101]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[103] - x[102];
  maxRange_tmp = y[103] - y[102];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[102].f1->size[0];
  x_segments[102].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[102].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[102].f1->data[i] = x[102] + b_index * nm->data[i];
  }

  nx = x_segments[102].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[102].f1->data[lastColumn] = rt_roundd_snf(x_segments[102]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[102].f1->size[0];
  y_segments[102].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[102].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[102].f1->data[i] = y[102] + b_index * nm->data[i];
  }

  nx = y_segments[102].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[102].f1->data[lastColumn] = rt_roundd_snf(y_segments[102]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[104] - x[103];
  maxRange_tmp = y[104] - y[103];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[103].f1->size[0];
  x_segments[103].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[103].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[103].f1->data[i] = x[103] + b_index * nm->data[i];
  }

  nx = x_segments[103].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[103].f1->data[lastColumn] = rt_roundd_snf(x_segments[103]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[103].f1->size[0];
  y_segments[103].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[103].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[103].f1->data[i] = y[103] + b_index * nm->data[i];
  }

  nx = y_segments[103].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[103].f1->data[lastColumn] = rt_roundd_snf(y_segments[103]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[105] - x[104];
  maxRange_tmp = y[105] - y[104];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[104].f1->size[0];
  x_segments[104].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[104].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[104].f1->data[i] = x[104] + b_index * nm->data[i];
  }

  nx = x_segments[104].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[104].f1->data[lastColumn] = rt_roundd_snf(x_segments[104]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[104].f1->size[0];
  y_segments[104].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[104].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[104].f1->data[i] = y[104] + b_index * nm->data[i];
  }

  nx = y_segments[104].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[104].f1->data[lastColumn] = rt_roundd_snf(y_segments[104]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[106] - x[105];
  maxRange_tmp = y[106] - y[105];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[105].f1->size[0];
  x_segments[105].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[105].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[105].f1->data[i] = x[105] + b_index * nm->data[i];
  }

  nx = x_segments[105].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[105].f1->data[lastColumn] = rt_roundd_snf(x_segments[105]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[105].f1->size[0];
  y_segments[105].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[105].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[105].f1->data[i] = y[105] + b_index * nm->data[i];
  }

  nx = y_segments[105].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[105].f1->data[lastColumn] = rt_roundd_snf(y_segments[105]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[107] - x[106];
  maxRange_tmp = y[107] - y[106];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[106].f1->size[0];
  x_segments[106].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[106].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[106].f1->data[i] = x[106] + b_index * nm->data[i];
  }

  nx = x_segments[106].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[106].f1->data[lastColumn] = rt_roundd_snf(x_segments[106]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[106].f1->size[0];
  y_segments[106].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[106].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[106].f1->data[i] = y[106] + b_index * nm->data[i];
  }

  nx = y_segments[106].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[106].f1->data[lastColumn] = rt_roundd_snf(y_segments[106]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[108] - x[107];
  maxRange_tmp = y[108] - y[107];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[107].f1->size[0];
  x_segments[107].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[107].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[107].f1->data[i] = x[107] + b_index * nm->data[i];
  }

  nx = x_segments[107].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[107].f1->data[lastColumn] = rt_roundd_snf(x_segments[107]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[107].f1->size[0];
  y_segments[107].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[107].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[107].f1->data[i] = y[107] + b_index * nm->data[i];
  }

  nx = y_segments[107].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[107].f1->data[lastColumn] = rt_roundd_snf(y_segments[107]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[109] - x[108];
  maxRange_tmp = y[109] - y[108];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[108].f1->size[0];
  x_segments[108].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[108].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[108].f1->data[i] = x[108] + b_index * nm->data[i];
  }

  nx = x_segments[108].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[108].f1->data[lastColumn] = rt_roundd_snf(x_segments[108]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[108].f1->size[0];
  y_segments[108].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[108].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[108].f1->data[i] = y[108] + b_index * nm->data[i];
  }

  nx = y_segments[108].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[108].f1->data[lastColumn] = rt_roundd_snf(y_segments[108]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[110] - x[109];
  maxRange_tmp = y[110] - y[109];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[109].f1->size[0];
  x_segments[109].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[109].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[109].f1->data[i] = x[109] + b_index * nm->data[i];
  }

  nx = x_segments[109].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[109].f1->data[lastColumn] = rt_roundd_snf(x_segments[109]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[109].f1->size[0];
  y_segments[109].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[109].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[109].f1->data[i] = y[109] + b_index * nm->data[i];
  }

  nx = y_segments[109].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[109].f1->data[lastColumn] = rt_roundd_snf(y_segments[109]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[111] - x[110];
  maxRange_tmp = y[111] - y[110];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[110].f1->size[0];
  x_segments[110].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[110].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[110].f1->data[i] = x[110] + b_index * nm->data[i];
  }

  nx = x_segments[110].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[110].f1->data[lastColumn] = rt_roundd_snf(x_segments[110]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[110].f1->size[0];
  y_segments[110].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[110].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[110].f1->data[i] = y[110] + b_index * nm->data[i];
  }

  nx = y_segments[110].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[110].f1->data[lastColumn] = rt_roundd_snf(y_segments[110]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[112] - x[111];
  maxRange_tmp = y[112] - y[111];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[111].f1->size[0];
  x_segments[111].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[111].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[111].f1->data[i] = x[111] + b_index * nm->data[i];
  }

  nx = x_segments[111].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[111].f1->data[lastColumn] = rt_roundd_snf(x_segments[111]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[111].f1->size[0];
  y_segments[111].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[111].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[111].f1->data[i] = y[111] + b_index * nm->data[i];
  }

  nx = y_segments[111].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[111].f1->data[lastColumn] = rt_roundd_snf(y_segments[111]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[113] - x[112];
  maxRange_tmp = y[113] - y[112];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[112].f1->size[0];
  x_segments[112].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[112].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[112].f1->data[i] = x[112] + b_index * nm->data[i];
  }

  nx = x_segments[112].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[112].f1->data[lastColumn] = rt_roundd_snf(x_segments[112]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[112].f1->size[0];
  y_segments[112].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[112].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[112].f1->data[i] = y[112] + b_index * nm->data[i];
  }

  nx = y_segments[112].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[112].f1->data[lastColumn] = rt_roundd_snf(y_segments[112]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[114] - x[113];
  maxRange_tmp = y[114] - y[113];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[113].f1->size[0];
  x_segments[113].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[113].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[113].f1->data[i] = x[113] + b_index * nm->data[i];
  }

  nx = x_segments[113].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[113].f1->data[lastColumn] = rt_roundd_snf(x_segments[113]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[113].f1->size[0];
  y_segments[113].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[113].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[113].f1->data[i] = y[113] + b_index * nm->data[i];
  }

  nx = y_segments[113].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[113].f1->data[lastColumn] = rt_roundd_snf(y_segments[113]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[115] - x[114];
  maxRange_tmp = y[115] - y[114];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[114].f1->size[0];
  x_segments[114].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[114].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[114].f1->data[i] = x[114] + b_index * nm->data[i];
  }

  nx = x_segments[114].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[114].f1->data[lastColumn] = rt_roundd_snf(x_segments[114]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[114].f1->size[0];
  y_segments[114].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[114].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[114].f1->data[i] = y[114] + b_index * nm->data[i];
  }

  nx = y_segments[114].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[114].f1->data[lastColumn] = rt_roundd_snf(y_segments[114]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[116] - x[115];
  maxRange_tmp = y[116] - y[115];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[115].f1->size[0];
  x_segments[115].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[115].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[115].f1->data[i] = x[115] + b_index * nm->data[i];
  }

  nx = x_segments[115].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[115].f1->data[lastColumn] = rt_roundd_snf(x_segments[115]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[115].f1->size[0];
  y_segments[115].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[115].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[115].f1->data[i] = y[115] + b_index * nm->data[i];
  }

  nx = y_segments[115].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[115].f1->data[lastColumn] = rt_roundd_snf(y_segments[115]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[117] - x[116];
  maxRange_tmp = y[117] - y[116];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[116].f1->size[0];
  x_segments[116].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[116].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[116].f1->data[i] = x[116] + b_index * nm->data[i];
  }

  nx = x_segments[116].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[116].f1->data[lastColumn] = rt_roundd_snf(x_segments[116]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[116].f1->size[0];
  y_segments[116].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[116].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[116].f1->data[i] = y[116] + b_index * nm->data[i];
  }

  nx = y_segments[116].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[116].f1->data[lastColumn] = rt_roundd_snf(y_segments[116]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[118] - x[117];
  maxRange_tmp = y[118] - y[117];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[117].f1->size[0];
  x_segments[117].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[117].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[117].f1->data[i] = x[117] + b_index * nm->data[i];
  }

  nx = x_segments[117].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[117].f1->data[lastColumn] = rt_roundd_snf(x_segments[117]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[117].f1->size[0];
  y_segments[117].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[117].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[117].f1->data[i] = y[117] + b_index * nm->data[i];
  }

  nx = y_segments[117].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[117].f1->data[lastColumn] = rt_roundd_snf(y_segments[117]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[119] - x[118];
  maxRange_tmp = y[119] - y[118];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[118].f1->size[0];
  x_segments[118].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[118].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[118].f1->data[i] = x[118] + b_index * nm->data[i];
  }

  nx = x_segments[118].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[118].f1->data[lastColumn] = rt_roundd_snf(x_segments[118]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[118].f1->size[0];
  y_segments[118].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[118].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[118].f1->data[i] = y[118] + b_index * nm->data[i];
  }

  nx = y_segments[118].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[118].f1->data[lastColumn] = rt_roundd_snf(y_segments[118]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[120] - x[119];
  maxRange_tmp = y[120] - y[119];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[119].f1->size[0];
  x_segments[119].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[119].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[119].f1->data[i] = x[119] + b_index * nm->data[i];
  }

  nx = x_segments[119].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[119].f1->data[lastColumn] = rt_roundd_snf(x_segments[119]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[119].f1->size[0];
  y_segments[119].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[119].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[119].f1->data[i] = y[119] + b_index * nm->data[i];
  }

  nx = y_segments[119].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[119].f1->data[lastColumn] = rt_roundd_snf(y_segments[119]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[121] - x[120];
  maxRange_tmp = y[121] - y[120];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[120].f1->size[0];
  x_segments[120].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[120].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[120].f1->data[i] = x[120] + b_index * nm->data[i];
  }

  nx = x_segments[120].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[120].f1->data[lastColumn] = rt_roundd_snf(x_segments[120]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[120].f1->size[0];
  y_segments[120].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[120].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[120].f1->data[i] = y[120] + b_index * nm->data[i];
  }

  nx = y_segments[120].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[120].f1->data[lastColumn] = rt_roundd_snf(y_segments[120]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[122] - x[121];
  maxRange_tmp = y[122] - y[121];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[121].f1->size[0];
  x_segments[121].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[121].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[121].f1->data[i] = x[121] + b_index * nm->data[i];
  }

  nx = x_segments[121].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[121].f1->data[lastColumn] = rt_roundd_snf(x_segments[121]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[121].f1->size[0];
  y_segments[121].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[121].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[121].f1->data[i] = y[121] + b_index * nm->data[i];
  }

  nx = y_segments[121].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[121].f1->data[lastColumn] = rt_roundd_snf(y_segments[121]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[123] - x[122];
  maxRange_tmp = y[123] - y[122];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[122].f1->size[0];
  x_segments[122].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[122].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[122].f1->data[i] = x[122] + b_index * nm->data[i];
  }

  nx = x_segments[122].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[122].f1->data[lastColumn] = rt_roundd_snf(x_segments[122]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[122].f1->size[0];
  y_segments[122].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[122].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[122].f1->data[i] = y[122] + b_index * nm->data[i];
  }

  nx = y_segments[122].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[122].f1->data[lastColumn] = rt_roundd_snf(y_segments[122]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[124] - x[123];
  maxRange_tmp = y[124] - y[123];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[123].f1->size[0];
  x_segments[123].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[123].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[123].f1->data[i] = x[123] + b_index * nm->data[i];
  }

  nx = x_segments[123].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[123].f1->data[lastColumn] = rt_roundd_snf(x_segments[123]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[123].f1->size[0];
  y_segments[123].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[123].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[123].f1->data[i] = y[123] + b_index * nm->data[i];
  }

  nx = y_segments[123].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[123].f1->data[lastColumn] = rt_roundd_snf(y_segments[123]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[125] - x[124];
  maxRange_tmp = y[125] - y[124];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[124].f1->size[0];
  x_segments[124].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[124].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[124].f1->data[i] = x[124] + b_index * nm->data[i];
  }

  nx = x_segments[124].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[124].f1->data[lastColumn] = rt_roundd_snf(x_segments[124]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[124].f1->size[0];
  y_segments[124].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[124].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[124].f1->data[i] = y[124] + b_index * nm->data[i];
  }

  nx = y_segments[124].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[124].f1->data[lastColumn] = rt_roundd_snf(y_segments[124]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[126] - x[125];
  maxRange_tmp = y[126] - y[125];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[125].f1->size[0];
  x_segments[125].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[125].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[125].f1->data[i] = x[125] + b_index * nm->data[i];
  }

  nx = x_segments[125].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[125].f1->data[lastColumn] = rt_roundd_snf(x_segments[125]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[125].f1->size[0];
  y_segments[125].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[125].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[125].f1->data[i] = y[125] + b_index * nm->data[i];
  }

  nx = y_segments[125].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[125].f1->data[lastColumn] = rt_roundd_snf(y_segments[125]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[127] - x[126];
  maxRange_tmp = y[127] - y[126];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[126].f1->size[0];
  x_segments[126].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[126].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[126].f1->data[i] = x[126] + b_index * nm->data[i];
  }

  nx = x_segments[126].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[126].f1->data[lastColumn] = rt_roundd_snf(x_segments[126]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[126].f1->size[0];
  y_segments[126].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[126].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[126].f1->data[i] = y[126] + b_index * nm->data[i];
  }

  nx = y_segments[126].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[126].f1->data[lastColumn] = rt_roundd_snf(y_segments[126]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[128] - x[127];
  maxRange_tmp = y[128] - y[127];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[127].f1->size[0];
  x_segments[127].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[127].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[127].f1->data[i] = x[127] + b_index * nm->data[i];
  }

  nx = x_segments[127].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[127].f1->data[lastColumn] = rt_roundd_snf(x_segments[127]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[127].f1->size[0];
  y_segments[127].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[127].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[127].f1->data[i] = y[127] + b_index * nm->data[i];
  }

  nx = y_segments[127].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[127].f1->data[lastColumn] = rt_roundd_snf(y_segments[127]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[129] - x[128];
  maxRange_tmp = y[129] - y[128];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[128].f1->size[0];
  x_segments[128].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[128].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[128].f1->data[i] = x[128] + b_index * nm->data[i];
  }

  nx = x_segments[128].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[128].f1->data[lastColumn] = rt_roundd_snf(x_segments[128]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[128].f1->size[0];
  y_segments[128].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[128].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[128].f1->data[i] = y[128] + b_index * nm->data[i];
  }

  nx = y_segments[128].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[128].f1->data[lastColumn] = rt_roundd_snf(y_segments[128]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[130] - x[129];
  maxRange_tmp = y[130] - y[129];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[129].f1->size[0];
  x_segments[129].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[129].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[129].f1->data[i] = x[129] + b_index * nm->data[i];
  }

  nx = x_segments[129].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[129].f1->data[lastColumn] = rt_roundd_snf(x_segments[129]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[129].f1->size[0];
  y_segments[129].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[129].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[129].f1->data[i] = y[129] + b_index * nm->data[i];
  }

  nx = y_segments[129].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[129].f1->data[lastColumn] = rt_roundd_snf(y_segments[129]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[131] - x[130];
  maxRange_tmp = y[131] - y[130];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[130].f1->size[0];
  x_segments[130].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[130].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[130].f1->data[i] = x[130] + b_index * nm->data[i];
  }

  nx = x_segments[130].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[130].f1->data[lastColumn] = rt_roundd_snf(x_segments[130]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[130].f1->size[0];
  y_segments[130].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[130].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[130].f1->data[i] = y[130] + b_index * nm->data[i];
  }

  nx = y_segments[130].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[130].f1->data[lastColumn] = rt_roundd_snf(y_segments[130]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[132] - x[131];
  maxRange_tmp = y[132] - y[131];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[131].f1->size[0];
  x_segments[131].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[131].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[131].f1->data[i] = x[131] + b_index * nm->data[i];
  }

  nx = x_segments[131].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[131].f1->data[lastColumn] = rt_roundd_snf(x_segments[131]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[131].f1->size[0];
  y_segments[131].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[131].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[131].f1->data[i] = y[131] + b_index * nm->data[i];
  }

  nx = y_segments[131].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[131].f1->data[lastColumn] = rt_roundd_snf(y_segments[131]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[133] - x[132];
  maxRange_tmp = y[133] - y[132];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[132].f1->size[0];
  x_segments[132].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[132].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[132].f1->data[i] = x[132] + b_index * nm->data[i];
  }

  nx = x_segments[132].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[132].f1->data[lastColumn] = rt_roundd_snf(x_segments[132]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[132].f1->size[0];
  y_segments[132].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[132].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[132].f1->data[i] = y[132] + b_index * nm->data[i];
  }

  nx = y_segments[132].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[132].f1->data[lastColumn] = rt_roundd_snf(y_segments[132]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[134] - x[133];
  maxRange_tmp = y[134] - y[133];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[133].f1->size[0];
  x_segments[133].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[133].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[133].f1->data[i] = x[133] + b_index * nm->data[i];
  }

  nx = x_segments[133].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[133].f1->data[lastColumn] = rt_roundd_snf(x_segments[133]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[133].f1->size[0];
  y_segments[133].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[133].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[133].f1->data[i] = y[133] + b_index * nm->data[i];
  }

  nx = y_segments[133].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[133].f1->data[lastColumn] = rt_roundd_snf(y_segments[133]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[135] - x[134];
  maxRange_tmp = y[135] - y[134];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[134].f1->size[0];
  x_segments[134].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[134].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[134].f1->data[i] = x[134] + b_index * nm->data[i];
  }

  nx = x_segments[134].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[134].f1->data[lastColumn] = rt_roundd_snf(x_segments[134]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[134].f1->size[0];
  y_segments[134].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[134].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[134].f1->data[i] = y[134] + b_index * nm->data[i];
  }

  nx = y_segments[134].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[134].f1->data[lastColumn] = rt_roundd_snf(y_segments[134]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[136] - x[135];
  maxRange_tmp = y[136] - y[135];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[135].f1->size[0];
  x_segments[135].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[135].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[135].f1->data[i] = x[135] + b_index * nm->data[i];
  }

  nx = x_segments[135].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[135].f1->data[lastColumn] = rt_roundd_snf(x_segments[135]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[135].f1->size[0];
  y_segments[135].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[135].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[135].f1->data[i] = y[135] + b_index * nm->data[i];
  }

  nx = y_segments[135].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[135].f1->data[lastColumn] = rt_roundd_snf(y_segments[135]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[137] - x[136];
  maxRange_tmp = y[137] - y[136];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[136].f1->size[0];
  x_segments[136].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[136].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[136].f1->data[i] = x[136] + b_index * nm->data[i];
  }

  nx = x_segments[136].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[136].f1->data[lastColumn] = rt_roundd_snf(x_segments[136]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[136].f1->size[0];
  y_segments[136].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[136].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[136].f1->data[i] = y[136] + b_index * nm->data[i];
  }

  nx = y_segments[136].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[136].f1->data[lastColumn] = rt_roundd_snf(y_segments[136]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[138] - x[137];
  maxRange_tmp = y[138] - y[137];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[137].f1->size[0];
  x_segments[137].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[137].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[137].f1->data[i] = x[137] + b_index * nm->data[i];
  }

  nx = x_segments[137].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[137].f1->data[lastColumn] = rt_roundd_snf(x_segments[137]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[137].f1->size[0];
  y_segments[137].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[137].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[137].f1->data[i] = y[137] + b_index * nm->data[i];
  }

  nx = y_segments[137].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[137].f1->data[lastColumn] = rt_roundd_snf(y_segments[137]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[139] - x[138];
  maxRange_tmp = y[139] - y[138];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[138].f1->size[0];
  x_segments[138].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[138].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[138].f1->data[i] = x[138] + b_index * nm->data[i];
  }

  nx = x_segments[138].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[138].f1->data[lastColumn] = rt_roundd_snf(x_segments[138]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[138].f1->size[0];
  y_segments[138].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[138].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[138].f1->data[i] = y[138] + b_index * nm->data[i];
  }

  nx = y_segments[138].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[138].f1->data[lastColumn] = rt_roundd_snf(y_segments[138]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[140] - x[139];
  maxRange_tmp = y[140] - y[139];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[139].f1->size[0];
  x_segments[139].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[139].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[139].f1->data[i] = x[139] + b_index * nm->data[i];
  }

  nx = x_segments[139].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[139].f1->data[lastColumn] = rt_roundd_snf(x_segments[139]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[139].f1->size[0];
  y_segments[139].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[139].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[139].f1->data[i] = y[139] + b_index * nm->data[i];
  }

  nx = y_segments[139].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[139].f1->data[lastColumn] = rt_roundd_snf(y_segments[139]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[141] - x[140];
  maxRange_tmp = y[141] - y[140];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[140].f1->size[0];
  x_segments[140].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[140].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[140].f1->data[i] = x[140] + b_index * nm->data[i];
  }

  nx = x_segments[140].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[140].f1->data[lastColumn] = rt_roundd_snf(x_segments[140]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[140].f1->size[0];
  y_segments[140].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[140].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[140].f1->data[i] = y[140] + b_index * nm->data[i];
  }

  nx = y_segments[140].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[140].f1->data[lastColumn] = rt_roundd_snf(y_segments[140]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[142] - x[141];
  maxRange_tmp = y[142] - y[141];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[141].f1->size[0];
  x_segments[141].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[141].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[141].f1->data[i] = x[141] + b_index * nm->data[i];
  }

  nx = x_segments[141].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[141].f1->data[lastColumn] = rt_roundd_snf(x_segments[141]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[141].f1->size[0];
  y_segments[141].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[141].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[141].f1->data[i] = y[141] + b_index * nm->data[i];
  }

  nx = y_segments[141].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[141].f1->data[lastColumn] = rt_roundd_snf(y_segments[141]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[143] - x[142];
  maxRange_tmp = y[143] - y[142];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[142].f1->size[0];
  x_segments[142].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[142].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[142].f1->data[i] = x[142] + b_index * nm->data[i];
  }

  nx = x_segments[142].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[142].f1->data[lastColumn] = rt_roundd_snf(x_segments[142]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[142].f1->size[0];
  y_segments[142].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[142].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[142].f1->data[i] = y[142] + b_index * nm->data[i];
  }

  nx = y_segments[142].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[142].f1->data[lastColumn] = rt_roundd_snf(y_segments[142]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[144] - x[143];
  maxRange_tmp = y[144] - y[143];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[143].f1->size[0];
  x_segments[143].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[143].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[143].f1->data[i] = x[143] + b_index * nm->data[i];
  }

  nx = x_segments[143].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[143].f1->data[lastColumn] = rt_roundd_snf(x_segments[143]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[143].f1->size[0];
  y_segments[143].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[143].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[143].f1->data[i] = y[143] + b_index * nm->data[i];
  }

  nx = y_segments[143].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[143].f1->data[lastColumn] = rt_roundd_snf(y_segments[143]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[145] - x[144];
  maxRange_tmp = y[145] - y[144];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[144].f1->size[0];
  x_segments[144].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[144].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[144].f1->data[i] = x[144] + b_index * nm->data[i];
  }

  nx = x_segments[144].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[144].f1->data[lastColumn] = rt_roundd_snf(x_segments[144]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[144].f1->size[0];
  y_segments[144].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[144].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[144].f1->data[i] = y[144] + b_index * nm->data[i];
  }

  nx = y_segments[144].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[144].f1->data[lastColumn] = rt_roundd_snf(y_segments[144]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[146] - x[145];
  maxRange_tmp = y[146] - y[145];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[145].f1->size[0];
  x_segments[145].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[145].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[145].f1->data[i] = x[145] + b_index * nm->data[i];
  }

  nx = x_segments[145].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[145].f1->data[lastColumn] = rt_roundd_snf(x_segments[145]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[145].f1->size[0];
  y_segments[145].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[145].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[145].f1->data[i] = y[145] + b_index * nm->data[i];
  }

  nx = y_segments[145].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[145].f1->data[lastColumn] = rt_roundd_snf(y_segments[145]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[147] - x[146];
  maxRange_tmp = y[147] - y[146];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[146].f1->size[0];
  x_segments[146].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[146].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[146].f1->data[i] = x[146] + b_index * nm->data[i];
  }

  nx = x_segments[146].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[146].f1->data[lastColumn] = rt_roundd_snf(x_segments[146]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[146].f1->size[0];
  y_segments[146].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[146].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[146].f1->data[i] = y[146] + b_index * nm->data[i];
  }

  nx = y_segments[146].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[146].f1->data[lastColumn] = rt_roundd_snf(y_segments[146]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[148] - x[147];
  maxRange_tmp = y[148] - y[147];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[147].f1->size[0];
  x_segments[147].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[147].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[147].f1->data[i] = x[147] + b_index * nm->data[i];
  }

  nx = x_segments[147].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[147].f1->data[lastColumn] = rt_roundd_snf(x_segments[147]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[147].f1->size[0];
  y_segments[147].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[147].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[147].f1->data[i] = y[147] + b_index * nm->data[i];
  }

  nx = y_segments[147].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[147].f1->data[lastColumn] = rt_roundd_snf(y_segments[147]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[149] - x[148];
  maxRange_tmp = y[149] - y[148];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[148].f1->size[0];
  x_segments[148].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[148].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[148].f1->data[i] = x[148] + b_index * nm->data[i];
  }

  nx = x_segments[148].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[148].f1->data[lastColumn] = rt_roundd_snf(x_segments[148]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[148].f1->size[0];
  y_segments[148].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[148].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[148].f1->data[i] = y[148] + b_index * nm->data[i];
  }

  nx = y_segments[148].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[148].f1->data[lastColumn] = rt_roundd_snf(y_segments[148]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[150] - x[149];
  maxRange_tmp = y[150] - y[149];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[149].f1->size[0];
  x_segments[149].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[149].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[149].f1->data[i] = x[149] + b_index * nm->data[i];
  }

  nx = x_segments[149].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[149].f1->data[lastColumn] = rt_roundd_snf(x_segments[149]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[149].f1->size[0];
  y_segments[149].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[149].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[149].f1->data[i] = y[149] + b_index * nm->data[i];
  }

  nx = y_segments[149].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[149].f1->data[lastColumn] = rt_roundd_snf(y_segments[149]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[151] - x[150];
  maxRange_tmp = y[151] - y[150];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[150].f1->size[0];
  x_segments[150].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[150].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[150].f1->data[i] = x[150] + b_index * nm->data[i];
  }

  nx = x_segments[150].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[150].f1->data[lastColumn] = rt_roundd_snf(x_segments[150]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[150].f1->size[0];
  y_segments[150].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[150].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[150].f1->data[i] = y[150] + b_index * nm->data[i];
  }

  nx = y_segments[150].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[150].f1->data[lastColumn] = rt_roundd_snf(y_segments[150]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[152] - x[151];
  maxRange_tmp = y[152] - y[151];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[151].f1->size[0];
  x_segments[151].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[151].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[151].f1->data[i] = x[151] + b_index * nm->data[i];
  }

  nx = x_segments[151].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[151].f1->data[lastColumn] = rt_roundd_snf(x_segments[151]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[151].f1->size[0];
  y_segments[151].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[151].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[151].f1->data[i] = y[151] + b_index * nm->data[i];
  }

  nx = y_segments[151].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[151].f1->data[lastColumn] = rt_roundd_snf(y_segments[151]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[153] - x[152];
  maxRange_tmp = y[153] - y[152];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[152].f1->size[0];
  x_segments[152].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[152].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[152].f1->data[i] = x[152] + b_index * nm->data[i];
  }

  nx = x_segments[152].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[152].f1->data[lastColumn] = rt_roundd_snf(x_segments[152]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[152].f1->size[0];
  y_segments[152].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[152].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[152].f1->data[i] = y[152] + b_index * nm->data[i];
  }

  nx = y_segments[152].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[152].f1->data[lastColumn] = rt_roundd_snf(y_segments[152]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[154] - x[153];
  maxRange_tmp = y[154] - y[153];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[153].f1->size[0];
  x_segments[153].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[153].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[153].f1->data[i] = x[153] + b_index * nm->data[i];
  }

  nx = x_segments[153].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[153].f1->data[lastColumn] = rt_roundd_snf(x_segments[153]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[153].f1->size[0];
  y_segments[153].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[153].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[153].f1->data[i] = y[153] + b_index * nm->data[i];
  }

  nx = y_segments[153].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[153].f1->data[lastColumn] = rt_roundd_snf(y_segments[153]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[155] - x[154];
  maxRange_tmp = y[155] - y[154];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[154].f1->size[0];
  x_segments[154].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[154].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[154].f1->data[i] = x[154] + b_index * nm->data[i];
  }

  nx = x_segments[154].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[154].f1->data[lastColumn] = rt_roundd_snf(x_segments[154]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[154].f1->size[0];
  y_segments[154].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[154].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[154].f1->data[i] = y[154] + b_index * nm->data[i];
  }

  nx = y_segments[154].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[154].f1->data[lastColumn] = rt_roundd_snf(y_segments[154]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[156] - x[155];
  maxRange_tmp = y[156] - y[155];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[155].f1->size[0];
  x_segments[155].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[155].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[155].f1->data[i] = x[155] + b_index * nm->data[i];
  }

  nx = x_segments[155].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[155].f1->data[lastColumn] = rt_roundd_snf(x_segments[155]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[155].f1->size[0];
  y_segments[155].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[155].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[155].f1->data[i] = y[155] + b_index * nm->data[i];
  }

  nx = y_segments[155].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[155].f1->data[lastColumn] = rt_roundd_snf(y_segments[155]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[157] - x[156];
  maxRange_tmp = y[157] - y[156];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[156].f1->size[0];
  x_segments[156].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[156].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[156].f1->data[i] = x[156] + b_index * nm->data[i];
  }

  nx = x_segments[156].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[156].f1->data[lastColumn] = rt_roundd_snf(x_segments[156]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[156].f1->size[0];
  y_segments[156].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[156].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[156].f1->data[i] = y[156] + b_index * nm->data[i];
  }

  nx = y_segments[156].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[156].f1->data[lastColumn] = rt_roundd_snf(y_segments[156]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[158] - x[157];
  maxRange_tmp = y[158] - y[157];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[157].f1->size[0];
  x_segments[157].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[157].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[157].f1->data[i] = x[157] + b_index * nm->data[i];
  }

  nx = x_segments[157].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[157].f1->data[lastColumn] = rt_roundd_snf(x_segments[157]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[157].f1->size[0];
  y_segments[157].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[157].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[157].f1->data[i] = y[157] + b_index * nm->data[i];
  }

  nx = y_segments[157].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[157].f1->data[lastColumn] = rt_roundd_snf(y_segments[157]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[159] - x[158];
  maxRange_tmp = y[159] - y[158];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[158].f1->size[0];
  x_segments[158].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[158].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[158].f1->data[i] = x[158] + b_index * nm->data[i];
  }

  nx = x_segments[158].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[158].f1->data[lastColumn] = rt_roundd_snf(x_segments[158]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[158].f1->size[0];
  y_segments[158].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[158].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[158].f1->data[i] = y[158] + b_index * nm->data[i];
  }

  nx = y_segments[158].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[158].f1->data[lastColumn] = rt_roundd_snf(y_segments[158]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[160] - x[159];
  maxRange_tmp = y[160] - y[159];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[159].f1->size[0];
  x_segments[159].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[159].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[159].f1->data[i] = x[159] + b_index * nm->data[i];
  }

  nx = x_segments[159].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[159].f1->data[lastColumn] = rt_roundd_snf(x_segments[159]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[159].f1->size[0];
  y_segments[159].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[159].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[159].f1->data[i] = y[159] + b_index * nm->data[i];
  }

  nx = y_segments[159].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[159].f1->data[lastColumn] = rt_roundd_snf(y_segments[159]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[161] - x[160];
  maxRange_tmp = y[161] - y[160];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[160].f1->size[0];
  x_segments[160].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[160].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[160].f1->data[i] = x[160] + b_index * nm->data[i];
  }

  nx = x_segments[160].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[160].f1->data[lastColumn] = rt_roundd_snf(x_segments[160]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[160].f1->size[0];
  y_segments[160].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[160].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[160].f1->data[i] = y[160] + b_index * nm->data[i];
  }

  nx = y_segments[160].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[160].f1->data[lastColumn] = rt_roundd_snf(y_segments[160]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[162] - x[161];
  maxRange_tmp = y[162] - y[161];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[161].f1->size[0];
  x_segments[161].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[161].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[161].f1->data[i] = x[161] + b_index * nm->data[i];
  }

  nx = x_segments[161].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[161].f1->data[lastColumn] = rt_roundd_snf(x_segments[161]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[161].f1->size[0];
  y_segments[161].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[161].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[161].f1->data[i] = y[161] + b_index * nm->data[i];
  }

  nx = y_segments[161].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[161].f1->data[lastColumn] = rt_roundd_snf(y_segments[161]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[163] - x[162];
  maxRange_tmp = y[163] - y[162];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[162].f1->size[0];
  x_segments[162].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[162].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[162].f1->data[i] = x[162] + b_index * nm->data[i];
  }

  nx = x_segments[162].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[162].f1->data[lastColumn] = rt_roundd_snf(x_segments[162]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[162].f1->size[0];
  y_segments[162].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[162].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[162].f1->data[i] = y[162] + b_index * nm->data[i];
  }

  nx = y_segments[162].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[162].f1->data[lastColumn] = rt_roundd_snf(y_segments[162]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[164] - x[163];
  maxRange_tmp = y[164] - y[163];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[163].f1->size[0];
  x_segments[163].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[163].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[163].f1->data[i] = x[163] + b_index * nm->data[i];
  }

  nx = x_segments[163].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[163].f1->data[lastColumn] = rt_roundd_snf(x_segments[163]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[163].f1->size[0];
  y_segments[163].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[163].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[163].f1->data[i] = y[163] + b_index * nm->data[i];
  }

  nx = y_segments[163].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[163].f1->data[lastColumn] = rt_roundd_snf(y_segments[163]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[165] - x[164];
  maxRange_tmp = y[165] - y[164];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[164].f1->size[0];
  x_segments[164].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[164].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[164].f1->data[i] = x[164] + b_index * nm->data[i];
  }

  nx = x_segments[164].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[164].f1->data[lastColumn] = rt_roundd_snf(x_segments[164]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[164].f1->size[0];
  y_segments[164].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[164].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[164].f1->data[i] = y[164] + b_index * nm->data[i];
  }

  nx = y_segments[164].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[164].f1->data[lastColumn] = rt_roundd_snf(y_segments[164]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[166] - x[165];
  maxRange_tmp = y[166] - y[165];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[165].f1->size[0];
  x_segments[165].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[165].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[165].f1->data[i] = x[165] + b_index * nm->data[i];
  }

  nx = x_segments[165].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[165].f1->data[lastColumn] = rt_roundd_snf(x_segments[165]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[165].f1->size[0];
  y_segments[165].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[165].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[165].f1->data[i] = y[165] + b_index * nm->data[i];
  }

  nx = y_segments[165].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[165].f1->data[lastColumn] = rt_roundd_snf(y_segments[165]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[167] - x[166];
  maxRange_tmp = y[167] - y[166];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[166].f1->size[0];
  x_segments[166].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[166].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[166].f1->data[i] = x[166] + b_index * nm->data[i];
  }

  nx = x_segments[166].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[166].f1->data[lastColumn] = rt_roundd_snf(x_segments[166]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[166].f1->size[0];
  y_segments[166].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[166].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[166].f1->data[i] = y[166] + b_index * nm->data[i];
  }

  nx = y_segments[166].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[166].f1->data[lastColumn] = rt_roundd_snf(y_segments[166]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[168] - x[167];
  maxRange_tmp = y[168] - y[167];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[167].f1->size[0];
  x_segments[167].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[167].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[167].f1->data[i] = x[167] + b_index * nm->data[i];
  }

  nx = x_segments[167].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[167].f1->data[lastColumn] = rt_roundd_snf(x_segments[167]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[167].f1->size[0];
  y_segments[167].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[167].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[167].f1->data[i] = y[167] + b_index * nm->data[i];
  }

  nx = y_segments[167].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[167].f1->data[lastColumn] = rt_roundd_snf(y_segments[167]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[169] - x[168];
  maxRange_tmp = y[169] - y[168];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[168].f1->size[0];
  x_segments[168].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[168].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[168].f1->data[i] = x[168] + b_index * nm->data[i];
  }

  nx = x_segments[168].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[168].f1->data[lastColumn] = rt_roundd_snf(x_segments[168]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[168].f1->size[0];
  y_segments[168].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[168].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[168].f1->data[i] = y[168] + b_index * nm->data[i];
  }

  nx = y_segments[168].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[168].f1->data[lastColumn] = rt_roundd_snf(y_segments[168]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[170] - x[169];
  maxRange_tmp = y[170] - y[169];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[169].f1->size[0];
  x_segments[169].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[169].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[169].f1->data[i] = x[169] + b_index * nm->data[i];
  }

  nx = x_segments[169].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[169].f1->data[lastColumn] = rt_roundd_snf(x_segments[169]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[169].f1->size[0];
  y_segments[169].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[169].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[169].f1->data[i] = y[169] + b_index * nm->data[i];
  }

  nx = y_segments[169].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[169].f1->data[lastColumn] = rt_roundd_snf(y_segments[169]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[171] - x[170];
  maxRange_tmp = y[171] - y[170];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[170].f1->size[0];
  x_segments[170].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[170].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[170].f1->data[i] = x[170] + b_index * nm->data[i];
  }

  nx = x_segments[170].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[170].f1->data[lastColumn] = rt_roundd_snf(x_segments[170]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[170].f1->size[0];
  y_segments[170].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[170].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[170].f1->data[i] = y[170] + b_index * nm->data[i];
  }

  nx = y_segments[170].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[170].f1->data[lastColumn] = rt_roundd_snf(y_segments[170]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[172] - x[171];
  maxRange_tmp = y[172] - y[171];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[171].f1->size[0];
  x_segments[171].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[171].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[171].f1->data[i] = x[171] + b_index * nm->data[i];
  }

  nx = x_segments[171].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[171].f1->data[lastColumn] = rt_roundd_snf(x_segments[171]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[171].f1->size[0];
  y_segments[171].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[171].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[171].f1->data[i] = y[171] + b_index * nm->data[i];
  }

  nx = y_segments[171].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[171].f1->data[lastColumn] = rt_roundd_snf(y_segments[171]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[173] - x[172];
  maxRange_tmp = y[173] - y[172];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[172].f1->size[0];
  x_segments[172].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[172].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[172].f1->data[i] = x[172] + b_index * nm->data[i];
  }

  nx = x_segments[172].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[172].f1->data[lastColumn] = rt_roundd_snf(x_segments[172]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[172].f1->size[0];
  y_segments[172].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[172].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[172].f1->data[i] = y[172] + b_index * nm->data[i];
  }

  nx = y_segments[172].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[172].f1->data[lastColumn] = rt_roundd_snf(y_segments[172]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[174] - x[173];
  maxRange_tmp = y[174] - y[173];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[173].f1->size[0];
  x_segments[173].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[173].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[173].f1->data[i] = x[173] + b_index * nm->data[i];
  }

  nx = x_segments[173].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[173].f1->data[lastColumn] = rt_roundd_snf(x_segments[173]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[173].f1->size[0];
  y_segments[173].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[173].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[173].f1->data[i] = y[173] + b_index * nm->data[i];
  }

  nx = y_segments[173].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[173].f1->data[lastColumn] = rt_roundd_snf(y_segments[173]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[175] - x[174];
  maxRange_tmp = y[175] - y[174];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[174].f1->size[0];
  x_segments[174].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[174].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[174].f1->data[i] = x[174] + b_index * nm->data[i];
  }

  nx = x_segments[174].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[174].f1->data[lastColumn] = rt_roundd_snf(x_segments[174]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[174].f1->size[0];
  y_segments[174].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[174].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[174].f1->data[i] = y[174] + b_index * nm->data[i];
  }

  nx = y_segments[174].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[174].f1->data[lastColumn] = rt_roundd_snf(y_segments[174]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[176] - x[175];
  maxRange_tmp = y[176] - y[175];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[175].f1->size[0];
  x_segments[175].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[175].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[175].f1->data[i] = x[175] + b_index * nm->data[i];
  }

  nx = x_segments[175].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[175].f1->data[lastColumn] = rt_roundd_snf(x_segments[175]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[175].f1->size[0];
  y_segments[175].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[175].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[175].f1->data[i] = y[175] + b_index * nm->data[i];
  }

  nx = y_segments[175].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[175].f1->data[lastColumn] = rt_roundd_snf(y_segments[175]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[177] - x[176];
  maxRange_tmp = y[177] - y[176];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[176].f1->size[0];
  x_segments[176].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[176].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[176].f1->data[i] = x[176] + b_index * nm->data[i];
  }

  nx = x_segments[176].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[176].f1->data[lastColumn] = rt_roundd_snf(x_segments[176]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[176].f1->size[0];
  y_segments[176].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[176].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[176].f1->data[i] = y[176] + b_index * nm->data[i];
  }

  nx = y_segments[176].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[176].f1->data[lastColumn] = rt_roundd_snf(y_segments[176]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[178] - x[177];
  maxRange_tmp = y[178] - y[177];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[177].f1->size[0];
  x_segments[177].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[177].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[177].f1->data[i] = x[177] + b_index * nm->data[i];
  }

  nx = x_segments[177].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[177].f1->data[lastColumn] = rt_roundd_snf(x_segments[177]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[177].f1->size[0];
  y_segments[177].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[177].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[177].f1->data[i] = y[177] + b_index * nm->data[i];
  }

  nx = y_segments[177].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[177].f1->data[lastColumn] = rt_roundd_snf(y_segments[177]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[179] - x[178];
  maxRange_tmp = y[179] - y[178];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[178].f1->size[0];
  x_segments[178].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[178].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[178].f1->data[i] = x[178] + b_index * nm->data[i];
  }

  nx = x_segments[178].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[178].f1->data[lastColumn] = rt_roundd_snf(x_segments[178]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[178].f1->size[0];
  y_segments[178].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[178].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[178].f1->data[i] = y[178] + b_index * nm->data[i];
  }

  nx = y_segments[178].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[178].f1->data[lastColumn] = rt_roundd_snf(y_segments[178]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[180] - x[179];
  maxRange_tmp = y[180] - y[179];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[179].f1->size[0];
  x_segments[179].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[179].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[179].f1->data[i] = x[179] + b_index * nm->data[i];
  }

  nx = x_segments[179].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[179].f1->data[lastColumn] = rt_roundd_snf(x_segments[179]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[179].f1->size[0];
  y_segments[179].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[179].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[179].f1->data[i] = y[179] + b_index * nm->data[i];
  }

  nx = y_segments[179].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[179].f1->data[lastColumn] = rt_roundd_snf(y_segments[179]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[181] - x[180];
  maxRange_tmp = y[181] - y[180];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[180].f1->size[0];
  x_segments[180].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[180].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[180].f1->data[i] = x[180] + b_index * nm->data[i];
  }

  nx = x_segments[180].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[180].f1->data[lastColumn] = rt_roundd_snf(x_segments[180]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[180].f1->size[0];
  y_segments[180].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[180].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[180].f1->data[i] = y[180] + b_index * nm->data[i];
  }

  nx = y_segments[180].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[180].f1->data[lastColumn] = rt_roundd_snf(y_segments[180]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[182] - x[181];
  maxRange_tmp = y[182] - y[181];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[181].f1->size[0];
  x_segments[181].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[181].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[181].f1->data[i] = x[181] + b_index * nm->data[i];
  }

  nx = x_segments[181].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[181].f1->data[lastColumn] = rt_roundd_snf(x_segments[181]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[181].f1->size[0];
  y_segments[181].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[181].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[181].f1->data[i] = y[181] + b_index * nm->data[i];
  }

  nx = y_segments[181].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[181].f1->data[lastColumn] = rt_roundd_snf(y_segments[181]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[183] - x[182];
  maxRange_tmp = y[183] - y[182];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[182].f1->size[0];
  x_segments[182].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[182].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[182].f1->data[i] = x[182] + b_index * nm->data[i];
  }

  nx = x_segments[182].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[182].f1->data[lastColumn] = rt_roundd_snf(x_segments[182]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[182].f1->size[0];
  y_segments[182].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[182].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[182].f1->data[i] = y[182] + b_index * nm->data[i];
  }

  nx = y_segments[182].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[182].f1->data[lastColumn] = rt_roundd_snf(y_segments[182]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[184] - x[183];
  maxRange_tmp = y[184] - y[183];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[183].f1->size[0];
  x_segments[183].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[183].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[183].f1->data[i] = x[183] + b_index * nm->data[i];
  }

  nx = x_segments[183].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[183].f1->data[lastColumn] = rt_roundd_snf(x_segments[183]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[183].f1->size[0];
  y_segments[183].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[183].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[183].f1->data[i] = y[183] + b_index * nm->data[i];
  }

  nx = y_segments[183].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[183].f1->data[lastColumn] = rt_roundd_snf(y_segments[183]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[185] - x[184];
  maxRange_tmp = y[185] - y[184];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[184].f1->size[0];
  x_segments[184].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[184].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[184].f1->data[i] = x[184] + b_index * nm->data[i];
  }

  nx = x_segments[184].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[184].f1->data[lastColumn] = rt_roundd_snf(x_segments[184]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[184].f1->size[0];
  y_segments[184].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[184].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[184].f1->data[i] = y[184] + b_index * nm->data[i];
  }

  nx = y_segments[184].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[184].f1->data[lastColumn] = rt_roundd_snf(y_segments[184]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[186] - x[185];
  maxRange_tmp = y[186] - y[185];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[185].f1->size[0];
  x_segments[185].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[185].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[185].f1->data[i] = x[185] + b_index * nm->data[i];
  }

  nx = x_segments[185].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[185].f1->data[lastColumn] = rt_roundd_snf(x_segments[185]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[185].f1->size[0];
  y_segments[185].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[185].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[185].f1->data[i] = y[185] + b_index * nm->data[i];
  }

  nx = y_segments[185].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[185].f1->data[lastColumn] = rt_roundd_snf(y_segments[185]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[187] - x[186];
  maxRange_tmp = y[187] - y[186];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[186].f1->size[0];
  x_segments[186].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[186].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[186].f1->data[i] = x[186] + b_index * nm->data[i];
  }

  nx = x_segments[186].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[186].f1->data[lastColumn] = rt_roundd_snf(x_segments[186]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[186].f1->size[0];
  y_segments[186].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[186].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[186].f1->data[i] = y[186] + b_index * nm->data[i];
  }

  nx = y_segments[186].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[186].f1->data[lastColumn] = rt_roundd_snf(y_segments[186]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[188] - x[187];
  maxRange_tmp = y[188] - y[187];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[187].f1->size[0];
  x_segments[187].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[187].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[187].f1->data[i] = x[187] + b_index * nm->data[i];
  }

  nx = x_segments[187].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[187].f1->data[lastColumn] = rt_roundd_snf(x_segments[187]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[187].f1->size[0];
  y_segments[187].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[187].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[187].f1->data[i] = y[187] + b_index * nm->data[i];
  }

  nx = y_segments[187].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[187].f1->data[lastColumn] = rt_roundd_snf(y_segments[187]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[189] - x[188];
  maxRange_tmp = y[189] - y[188];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[188].f1->size[0];
  x_segments[188].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[188].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[188].f1->data[i] = x[188] + b_index * nm->data[i];
  }

  nx = x_segments[188].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[188].f1->data[lastColumn] = rt_roundd_snf(x_segments[188]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[188].f1->size[0];
  y_segments[188].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[188].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[188].f1->data[i] = y[188] + b_index * nm->data[i];
  }

  nx = y_segments[188].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[188].f1->data[lastColumn] = rt_roundd_snf(y_segments[188]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[190] - x[189];
  maxRange_tmp = y[190] - y[189];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[189].f1->size[0];
  x_segments[189].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[189].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[189].f1->data[i] = x[189] + b_index * nm->data[i];
  }

  nx = x_segments[189].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[189].f1->data[lastColumn] = rt_roundd_snf(x_segments[189]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[189].f1->size[0];
  y_segments[189].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[189].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[189].f1->data[i] = y[189] + b_index * nm->data[i];
  }

  nx = y_segments[189].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[189].f1->data[lastColumn] = rt_roundd_snf(y_segments[189]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[191] - x[190];
  maxRange_tmp = y[191] - y[190];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[190].f1->size[0];
  x_segments[190].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[190].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[190].f1->data[i] = x[190] + b_index * nm->data[i];
  }

  nx = x_segments[190].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[190].f1->data[lastColumn] = rt_roundd_snf(x_segments[190]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[190].f1->size[0];
  y_segments[190].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[190].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[190].f1->data[i] = y[190] + b_index * nm->data[i];
  }

  nx = y_segments[190].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[190].f1->data[lastColumn] = rt_roundd_snf(y_segments[190]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[192] - x[191];
  maxRange_tmp = y[192] - y[191];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[191].f1->size[0];
  x_segments[191].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[191].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[191].f1->data[i] = x[191] + b_index * nm->data[i];
  }

  nx = x_segments[191].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[191].f1->data[lastColumn] = rt_roundd_snf(x_segments[191]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[191].f1->size[0];
  y_segments[191].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[191].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[191].f1->data[i] = y[191] + b_index * nm->data[i];
  }

  nx = y_segments[191].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[191].f1->data[lastColumn] = rt_roundd_snf(y_segments[191]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[193] - x[192];
  maxRange_tmp = y[193] - y[192];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[192].f1->size[0];
  x_segments[192].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[192].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[192].f1->data[i] = x[192] + b_index * nm->data[i];
  }

  nx = x_segments[192].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[192].f1->data[lastColumn] = rt_roundd_snf(x_segments[192]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[192].f1->size[0];
  y_segments[192].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[192].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[192].f1->data[i] = y[192] + b_index * nm->data[i];
  }

  nx = y_segments[192].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[192].f1->data[lastColumn] = rt_roundd_snf(y_segments[192]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[194] - x[193];
  maxRange_tmp = y[194] - y[193];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[193].f1->size[0];
  x_segments[193].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[193].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[193].f1->data[i] = x[193] + b_index * nm->data[i];
  }

  nx = x_segments[193].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[193].f1->data[lastColumn] = rt_roundd_snf(x_segments[193]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[193].f1->size[0];
  y_segments[193].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[193].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[193].f1->data[i] = y[193] + b_index * nm->data[i];
  }

  nx = y_segments[193].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[193].f1->data[lastColumn] = rt_roundd_snf(y_segments[193]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[195] - x[194];
  maxRange_tmp = y[195] - y[194];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[194].f1->size[0];
  x_segments[194].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[194].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[194].f1->data[i] = x[194] + b_index * nm->data[i];
  }

  nx = x_segments[194].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[194].f1->data[lastColumn] = rt_roundd_snf(x_segments[194]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[194].f1->size[0];
  y_segments[194].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[194].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[194].f1->data[i] = y[194] + b_index * nm->data[i];
  }

  nx = y_segments[194].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[194].f1->data[lastColumn] = rt_roundd_snf(y_segments[194]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[196] - x[195];
  maxRange_tmp = y[196] - y[195];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[195].f1->size[0];
  x_segments[195].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[195].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[195].f1->data[i] = x[195] + b_index * nm->data[i];
  }

  nx = x_segments[195].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[195].f1->data[lastColumn] = rt_roundd_snf(x_segments[195]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[195].f1->size[0];
  y_segments[195].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[195].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[195].f1->data[i] = y[195] + b_index * nm->data[i];
  }

  nx = y_segments[195].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[195].f1->data[lastColumn] = rt_roundd_snf(y_segments[195]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[197] - x[196];
  maxRange_tmp = y[197] - y[196];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[196].f1->size[0];
  x_segments[196].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[196].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[196].f1->data[i] = x[196] + b_index * nm->data[i];
  }

  nx = x_segments[196].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[196].f1->data[lastColumn] = rt_roundd_snf(x_segments[196]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[196].f1->size[0];
  y_segments[196].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[196].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[196].f1->data[i] = y[196] + b_index * nm->data[i];
  }

  nx = y_segments[196].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[196].f1->data[lastColumn] = rt_roundd_snf(y_segments[196]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[198] - x[197];
  maxRange_tmp = y[198] - y[197];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[197].f1->size[0];
  x_segments[197].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[197].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[197].f1->data[i] = x[197] + b_index * nm->data[i];
  }

  nx = x_segments[197].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[197].f1->data[lastColumn] = rt_roundd_snf(x_segments[197]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[197].f1->size[0];
  y_segments[197].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[197].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[197].f1->data[i] = y[197] + b_index * nm->data[i];
  }

  nx = y_segments[197].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[197].f1->data[lastColumn] = rt_roundd_snf(y_segments[197]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[199] - x[198];
  maxRange_tmp = y[199] - y[198];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[198].f1->size[0];
  x_segments[198].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[198].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[198].f1->data[i] = x[198] + b_index * nm->data[i];
  }

  nx = x_segments[198].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[198].f1->data[lastColumn] = rt_roundd_snf(x_segments[198]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[198].f1->size[0];
  y_segments[198].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[198].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[198].f1->data[i] = y[198] + b_index * nm->data[i];
  }

  nx = y_segments[198].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[198].f1->data[lastColumn] = rt_roundd_snf(y_segments[198]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[200] - x[199];
  maxRange_tmp = y[200] - y[199];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[199].f1->size[0];
  x_segments[199].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[199].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[199].f1->data[i] = x[199] + b_index * nm->data[i];
  }

  nx = x_segments[199].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[199].f1->data[lastColumn] = rt_roundd_snf(x_segments[199]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[199].f1->size[0];
  y_segments[199].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[199].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[199].f1->data[i] = y[199] + b_index * nm->data[i];
  }

  nx = y_segments[199].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[199].f1->data[lastColumn] = rt_roundd_snf(y_segments[199]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[201] - x[200];
  maxRange_tmp = y[201] - y[200];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[200].f1->size[0];
  x_segments[200].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[200].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[200].f1->data[i] = x[200] + b_index * nm->data[i];
  }

  nx = x_segments[200].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[200].f1->data[lastColumn] = rt_roundd_snf(x_segments[200]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[200].f1->size[0];
  y_segments[200].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[200].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[200].f1->data[i] = y[200] + b_index * nm->data[i];
  }

  nx = y_segments[200].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[200].f1->data[lastColumn] = rt_roundd_snf(y_segments[200]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[202] - x[201];
  maxRange_tmp = y[202] - y[201];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[201].f1->size[0];
  x_segments[201].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[201].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[201].f1->data[i] = x[201] + b_index * nm->data[i];
  }

  nx = x_segments[201].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[201].f1->data[lastColumn] = rt_roundd_snf(x_segments[201]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[201].f1->size[0];
  y_segments[201].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[201].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[201].f1->data[i] = y[201] + b_index * nm->data[i];
  }

  nx = y_segments[201].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[201].f1->data[lastColumn] = rt_roundd_snf(y_segments[201]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[203] - x[202];
  maxRange_tmp = y[203] - y[202];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[202].f1->size[0];
  x_segments[202].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[202].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[202].f1->data[i] = x[202] + b_index * nm->data[i];
  }

  nx = x_segments[202].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[202].f1->data[lastColumn] = rt_roundd_snf(x_segments[202]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[202].f1->size[0];
  y_segments[202].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[202].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[202].f1->data[i] = y[202] + b_index * nm->data[i];
  }

  nx = y_segments[202].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[202].f1->data[lastColumn] = rt_roundd_snf(y_segments[202]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[204] - x[203];
  maxRange_tmp = y[204] - y[203];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[203].f1->size[0];
  x_segments[203].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[203].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[203].f1->data[i] = x[203] + b_index * nm->data[i];
  }

  nx = x_segments[203].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[203].f1->data[lastColumn] = rt_roundd_snf(x_segments[203]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[203].f1->size[0];
  y_segments[203].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[203].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[203].f1->data[i] = y[203] + b_index * nm->data[i];
  }

  nx = y_segments[203].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[203].f1->data[lastColumn] = rt_roundd_snf(y_segments[203]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[205] - x[204];
  maxRange_tmp = y[205] - y[204];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[204].f1->size[0];
  x_segments[204].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[204].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[204].f1->data[i] = x[204] + b_index * nm->data[i];
  }

  nx = x_segments[204].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[204].f1->data[lastColumn] = rt_roundd_snf(x_segments[204]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[204].f1->size[0];
  y_segments[204].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[204].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[204].f1->data[i] = y[204] + b_index * nm->data[i];
  }

  nx = y_segments[204].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[204].f1->data[lastColumn] = rt_roundd_snf(y_segments[204]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[206] - x[205];
  maxRange_tmp = y[206] - y[205];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[205].f1->size[0];
  x_segments[205].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[205].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[205].f1->data[i] = x[205] + b_index * nm->data[i];
  }

  nx = x_segments[205].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[205].f1->data[lastColumn] = rt_roundd_snf(x_segments[205]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[205].f1->size[0];
  y_segments[205].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[205].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[205].f1->data[i] = y[205] + b_index * nm->data[i];
  }

  nx = y_segments[205].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[205].f1->data[lastColumn] = rt_roundd_snf(y_segments[205]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[207] - x[206];
  maxRange_tmp = y[207] - y[206];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[206].f1->size[0];
  x_segments[206].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[206].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[206].f1->data[i] = x[206] + b_index * nm->data[i];
  }

  nx = x_segments[206].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[206].f1->data[lastColumn] = rt_roundd_snf(x_segments[206]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[206].f1->size[0];
  y_segments[206].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[206].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[206].f1->data[i] = y[206] + b_index * nm->data[i];
  }

  nx = y_segments[206].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[206].f1->data[lastColumn] = rt_roundd_snf(y_segments[206]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[208] - x[207];
  maxRange_tmp = y[208] - y[207];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[207].f1->size[0];
  x_segments[207].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[207].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[207].f1->data[i] = x[207] + b_index * nm->data[i];
  }

  nx = x_segments[207].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[207].f1->data[lastColumn] = rt_roundd_snf(x_segments[207]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[207].f1->size[0];
  y_segments[207].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[207].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[207].f1->data[i] = y[207] + b_index * nm->data[i];
  }

  nx = y_segments[207].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[207].f1->data[lastColumn] = rt_roundd_snf(y_segments[207]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[209] - x[208];
  maxRange_tmp = y[209] - y[208];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[208].f1->size[0];
  x_segments[208].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[208].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[208].f1->data[i] = x[208] + b_index * nm->data[i];
  }

  nx = x_segments[208].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[208].f1->data[lastColumn] = rt_roundd_snf(x_segments[208]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[208].f1->size[0];
  y_segments[208].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[208].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[208].f1->data[i] = y[208] + b_index * nm->data[i];
  }

  nx = y_segments[208].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[208].f1->data[lastColumn] = rt_roundd_snf(y_segments[208]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[210] - x[209];
  maxRange_tmp = y[210] - y[209];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[209].f1->size[0];
  x_segments[209].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[209].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[209].f1->data[i] = x[209] + b_index * nm->data[i];
  }

  nx = x_segments[209].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[209].f1->data[lastColumn] = rt_roundd_snf(x_segments[209]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[209].f1->size[0];
  y_segments[209].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[209].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[209].f1->data[i] = y[209] + b_index * nm->data[i];
  }

  nx = y_segments[209].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[209].f1->data[lastColumn] = rt_roundd_snf(y_segments[209]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[211] - x[210];
  maxRange_tmp = y[211] - y[210];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[210].f1->size[0];
  x_segments[210].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[210].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[210].f1->data[i] = x[210] + b_index * nm->data[i];
  }

  nx = x_segments[210].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[210].f1->data[lastColumn] = rt_roundd_snf(x_segments[210]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[210].f1->size[0];
  y_segments[210].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[210].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[210].f1->data[i] = y[210] + b_index * nm->data[i];
  }

  nx = y_segments[210].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[210].f1->data[lastColumn] = rt_roundd_snf(y_segments[210]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[212] - x[211];
  maxRange_tmp = y[212] - y[211];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[211].f1->size[0];
  x_segments[211].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[211].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[211].f1->data[i] = x[211] + b_index * nm->data[i];
  }

  nx = x_segments[211].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[211].f1->data[lastColumn] = rt_roundd_snf(x_segments[211]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[211].f1->size[0];
  y_segments[211].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[211].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[211].f1->data[i] = y[211] + b_index * nm->data[i];
  }

  nx = y_segments[211].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[211].f1->data[lastColumn] = rt_roundd_snf(y_segments[211]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[213] - x[212];
  maxRange_tmp = y[213] - y[212];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[212].f1->size[0];
  x_segments[212].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[212].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[212].f1->data[i] = x[212] + b_index * nm->data[i];
  }

  nx = x_segments[212].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[212].f1->data[lastColumn] = rt_roundd_snf(x_segments[212]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[212].f1->size[0];
  y_segments[212].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[212].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[212].f1->data[i] = y[212] + b_index * nm->data[i];
  }

  nx = y_segments[212].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[212].f1->data[lastColumn] = rt_roundd_snf(y_segments[212]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[214] - x[213];
  maxRange_tmp = y[214] - y[213];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[213].f1->size[0];
  x_segments[213].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[213].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[213].f1->data[i] = x[213] + b_index * nm->data[i];
  }

  nx = x_segments[213].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[213].f1->data[lastColumn] = rt_roundd_snf(x_segments[213]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[213].f1->size[0];
  y_segments[213].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[213].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[213].f1->data[i] = y[213] + b_index * nm->data[i];
  }

  nx = y_segments[213].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[213].f1->data[lastColumn] = rt_roundd_snf(y_segments[213]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[215] - x[214];
  maxRange_tmp = y[215] - y[214];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[214].f1->size[0];
  x_segments[214].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[214].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[214].f1->data[i] = x[214] + b_index * nm->data[i];
  }

  nx = x_segments[214].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[214].f1->data[lastColumn] = rt_roundd_snf(x_segments[214]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[214].f1->size[0];
  y_segments[214].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[214].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[214].f1->data[i] = y[214] + b_index * nm->data[i];
  }

  nx = y_segments[214].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[214].f1->data[lastColumn] = rt_roundd_snf(y_segments[214]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[216] - x[215];
  maxRange_tmp = y[216] - y[215];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[215].f1->size[0];
  x_segments[215].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[215].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[215].f1->data[i] = x[215] + b_index * nm->data[i];
  }

  nx = x_segments[215].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[215].f1->data[lastColumn] = rt_roundd_snf(x_segments[215]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[215].f1->size[0];
  y_segments[215].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[215].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[215].f1->data[i] = y[215] + b_index * nm->data[i];
  }

  nx = y_segments[215].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[215].f1->data[lastColumn] = rt_roundd_snf(y_segments[215]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[217] - x[216];
  maxRange_tmp = y[217] - y[216];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[216].f1->size[0];
  x_segments[216].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[216].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[216].f1->data[i] = x[216] + b_index * nm->data[i];
  }

  nx = x_segments[216].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[216].f1->data[lastColumn] = rt_roundd_snf(x_segments[216]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[216].f1->size[0];
  y_segments[216].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[216].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[216].f1->data[i] = y[216] + b_index * nm->data[i];
  }

  nx = y_segments[216].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[216].f1->data[lastColumn] = rt_roundd_snf(y_segments[216]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[218] - x[217];
  maxRange_tmp = y[218] - y[217];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[217].f1->size[0];
  x_segments[217].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[217].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[217].f1->data[i] = x[217] + b_index * nm->data[i];
  }

  nx = x_segments[217].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[217].f1->data[lastColumn] = rt_roundd_snf(x_segments[217]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[217].f1->size[0];
  y_segments[217].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[217].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[217].f1->data[i] = y[217] + b_index * nm->data[i];
  }

  nx = y_segments[217].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[217].f1->data[lastColumn] = rt_roundd_snf(y_segments[217]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[219] - x[218];
  maxRange_tmp = y[219] - y[218];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[218].f1->size[0];
  x_segments[218].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[218].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[218].f1->data[i] = x[218] + b_index * nm->data[i];
  }

  nx = x_segments[218].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[218].f1->data[lastColumn] = rt_roundd_snf(x_segments[218]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[218].f1->size[0];
  y_segments[218].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[218].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[218].f1->data[i] = y[218] + b_index * nm->data[i];
  }

  nx = y_segments[218].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[218].f1->data[lastColumn] = rt_roundd_snf(y_segments[218]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[220] - x[219];
  maxRange_tmp = y[220] - y[219];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[219].f1->size[0];
  x_segments[219].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[219].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[219].f1->data[i] = x[219] + b_index * nm->data[i];
  }

  nx = x_segments[219].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[219].f1->data[lastColumn] = rt_roundd_snf(x_segments[219]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[219].f1->size[0];
  y_segments[219].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[219].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[219].f1->data[i] = y[219] + b_index * nm->data[i];
  }

  nx = y_segments[219].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[219].f1->data[lastColumn] = rt_roundd_snf(y_segments[219]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[221] - x[220];
  maxRange_tmp = y[221] - y[220];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[220].f1->size[0];
  x_segments[220].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[220].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[220].f1->data[i] = x[220] + b_index * nm->data[i];
  }

  nx = x_segments[220].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[220].f1->data[lastColumn] = rt_roundd_snf(x_segments[220]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[220].f1->size[0];
  y_segments[220].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[220].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[220].f1->data[i] = y[220] + b_index * nm->data[i];
  }

  nx = y_segments[220].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[220].f1->data[lastColumn] = rt_roundd_snf(y_segments[220]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[222] - x[221];
  maxRange_tmp = y[222] - y[221];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[221].f1->size[0];
  x_segments[221].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[221].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[221].f1->data[i] = x[221] + b_index * nm->data[i];
  }

  nx = x_segments[221].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[221].f1->data[lastColumn] = rt_roundd_snf(x_segments[221]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[221].f1->size[0];
  y_segments[221].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[221].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[221].f1->data[i] = y[221] + b_index * nm->data[i];
  }

  nx = y_segments[221].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[221].f1->data[lastColumn] = rt_roundd_snf(y_segments[221]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[223] - x[222];
  maxRange_tmp = y[223] - y[222];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[222].f1->size[0];
  x_segments[222].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[222].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[222].f1->data[i] = x[222] + b_index * nm->data[i];
  }

  nx = x_segments[222].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[222].f1->data[lastColumn] = rt_roundd_snf(x_segments[222]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[222].f1->size[0];
  y_segments[222].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[222].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[222].f1->data[i] = y[222] + b_index * nm->data[i];
  }

  nx = y_segments[222].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[222].f1->data[lastColumn] = rt_roundd_snf(y_segments[222]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[224] - x[223];
  maxRange_tmp = y[224] - y[223];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[223].f1->size[0];
  x_segments[223].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[223].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[223].f1->data[i] = x[223] + b_index * nm->data[i];
  }

  nx = x_segments[223].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[223].f1->data[lastColumn] = rt_roundd_snf(x_segments[223]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[223].f1->size[0];
  y_segments[223].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[223].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[223].f1->data[i] = y[223] + b_index * nm->data[i];
  }

  nx = y_segments[223].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[223].f1->data[lastColumn] = rt_roundd_snf(y_segments[223]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[225] - x[224];
  maxRange_tmp = y[225] - y[224];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[224].f1->size[0];
  x_segments[224].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[224].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[224].f1->data[i] = x[224] + b_index * nm->data[i];
  }

  nx = x_segments[224].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[224].f1->data[lastColumn] = rt_roundd_snf(x_segments[224]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[224].f1->size[0];
  y_segments[224].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[224].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[224].f1->data[i] = y[224] + b_index * nm->data[i];
  }

  nx = y_segments[224].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[224].f1->data[lastColumn] = rt_roundd_snf(y_segments[224]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[226] - x[225];
  maxRange_tmp = y[226] - y[225];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[225].f1->size[0];
  x_segments[225].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[225].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[225].f1->data[i] = x[225] + b_index * nm->data[i];
  }

  nx = x_segments[225].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[225].f1->data[lastColumn] = rt_roundd_snf(x_segments[225]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[225].f1->size[0];
  y_segments[225].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[225].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[225].f1->data[i] = y[225] + b_index * nm->data[i];
  }

  nx = y_segments[225].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[225].f1->data[lastColumn] = rt_roundd_snf(y_segments[225]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[227] - x[226];
  maxRange_tmp = y[227] - y[226];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[226].f1->size[0];
  x_segments[226].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[226].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[226].f1->data[i] = x[226] + b_index * nm->data[i];
  }

  nx = x_segments[226].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[226].f1->data[lastColumn] = rt_roundd_snf(x_segments[226]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[226].f1->size[0];
  y_segments[226].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[226].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[226].f1->data[i] = y[226] + b_index * nm->data[i];
  }

  nx = y_segments[226].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[226].f1->data[lastColumn] = rt_roundd_snf(y_segments[226]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[228] - x[227];
  maxRange_tmp = y[228] - y[227];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[227].f1->size[0];
  x_segments[227].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[227].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[227].f1->data[i] = x[227] + b_index * nm->data[i];
  }

  nx = x_segments[227].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[227].f1->data[lastColumn] = rt_roundd_snf(x_segments[227]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[227].f1->size[0];
  y_segments[227].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[227].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[227].f1->data[i] = y[227] + b_index * nm->data[i];
  }

  nx = y_segments[227].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[227].f1->data[lastColumn] = rt_roundd_snf(y_segments[227]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[229] - x[228];
  maxRange_tmp = y[229] - y[228];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[228].f1->size[0];
  x_segments[228].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[228].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[228].f1->data[i] = x[228] + b_index * nm->data[i];
  }

  nx = x_segments[228].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[228].f1->data[lastColumn] = rt_roundd_snf(x_segments[228]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[228].f1->size[0];
  y_segments[228].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[228].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[228].f1->data[i] = y[228] + b_index * nm->data[i];
  }

  nx = y_segments[228].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[228].f1->data[lastColumn] = rt_roundd_snf(y_segments[228]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[230] - x[229];
  maxRange_tmp = y[230] - y[229];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[229].f1->size[0];
  x_segments[229].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[229].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[229].f1->data[i] = x[229] + b_index * nm->data[i];
  }

  nx = x_segments[229].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[229].f1->data[lastColumn] = rt_roundd_snf(x_segments[229]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[229].f1->size[0];
  y_segments[229].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[229].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[229].f1->data[i] = y[229] + b_index * nm->data[i];
  }

  nx = y_segments[229].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[229].f1->data[lastColumn] = rt_roundd_snf(y_segments[229]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[231] - x[230];
  maxRange_tmp = y[231] - y[230];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[230].f1->size[0];
  x_segments[230].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[230].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[230].f1->data[i] = x[230] + b_index * nm->data[i];
  }

  nx = x_segments[230].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[230].f1->data[lastColumn] = rt_roundd_snf(x_segments[230]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[230].f1->size[0];
  y_segments[230].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[230].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[230].f1->data[i] = y[230] + b_index * nm->data[i];
  }

  nx = y_segments[230].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[230].f1->data[lastColumn] = rt_roundd_snf(y_segments[230]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[232] - x[231];
  maxRange_tmp = y[232] - y[231];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[231].f1->size[0];
  x_segments[231].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[231].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[231].f1->data[i] = x[231] + b_index * nm->data[i];
  }

  nx = x_segments[231].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[231].f1->data[lastColumn] = rt_roundd_snf(x_segments[231]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[231].f1->size[0];
  y_segments[231].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[231].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[231].f1->data[i] = y[231] + b_index * nm->data[i];
  }

  nx = y_segments[231].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[231].f1->data[lastColumn] = rt_roundd_snf(y_segments[231]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[233] - x[232];
  maxRange_tmp = y[233] - y[232];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[232].f1->size[0];
  x_segments[232].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[232].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[232].f1->data[i] = x[232] + b_index * nm->data[i];
  }

  nx = x_segments[232].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[232].f1->data[lastColumn] = rt_roundd_snf(x_segments[232]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[232].f1->size[0];
  y_segments[232].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[232].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[232].f1->data[i] = y[232] + b_index * nm->data[i];
  }

  nx = y_segments[232].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[232].f1->data[lastColumn] = rt_roundd_snf(y_segments[232]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[234] - x[233];
  maxRange_tmp = y[234] - y[233];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[233].f1->size[0];
  x_segments[233].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[233].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[233].f1->data[i] = x[233] + b_index * nm->data[i];
  }

  nx = x_segments[233].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[233].f1->data[lastColumn] = rt_roundd_snf(x_segments[233]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[233].f1->size[0];
  y_segments[233].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[233].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[233].f1->data[i] = y[233] + b_index * nm->data[i];
  }

  nx = y_segments[233].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[233].f1->data[lastColumn] = rt_roundd_snf(y_segments[233]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[235] - x[234];
  maxRange_tmp = y[235] - y[234];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[234].f1->size[0];
  x_segments[234].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[234].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[234].f1->data[i] = x[234] + b_index * nm->data[i];
  }

  nx = x_segments[234].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[234].f1->data[lastColumn] = rt_roundd_snf(x_segments[234]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[234].f1->size[0];
  y_segments[234].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[234].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[234].f1->data[i] = y[234] + b_index * nm->data[i];
  }

  nx = y_segments[234].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[234].f1->data[lastColumn] = rt_roundd_snf(y_segments[234]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[236] - x[235];
  maxRange_tmp = y[236] - y[235];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[235].f1->size[0];
  x_segments[235].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[235].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[235].f1->data[i] = x[235] + b_index * nm->data[i];
  }

  nx = x_segments[235].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[235].f1->data[lastColumn] = rt_roundd_snf(x_segments[235]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[235].f1->size[0];
  y_segments[235].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[235].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[235].f1->data[i] = y[235] + b_index * nm->data[i];
  }

  nx = y_segments[235].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[235].f1->data[lastColumn] = rt_roundd_snf(y_segments[235]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[237] - x[236];
  maxRange_tmp = y[237] - y[236];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[236].f1->size[0];
  x_segments[236].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[236].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[236].f1->data[i] = x[236] + b_index * nm->data[i];
  }

  nx = x_segments[236].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[236].f1->data[lastColumn] = rt_roundd_snf(x_segments[236]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[236].f1->size[0];
  y_segments[236].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[236].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[236].f1->data[i] = y[236] + b_index * nm->data[i];
  }

  nx = y_segments[236].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[236].f1->data[lastColumn] = rt_roundd_snf(y_segments[236]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[238] - x[237];
  maxRange_tmp = y[238] - y[237];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[237].f1->size[0];
  x_segments[237].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[237].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[237].f1->data[i] = x[237] + b_index * nm->data[i];
  }

  nx = x_segments[237].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[237].f1->data[lastColumn] = rt_roundd_snf(x_segments[237]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[237].f1->size[0];
  y_segments[237].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[237].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[237].f1->data[i] = y[237] + b_index * nm->data[i];
  }

  nx = y_segments[237].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[237].f1->data[lastColumn] = rt_roundd_snf(y_segments[237]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[239] - x[238];
  maxRange_tmp = y[239] - y[238];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[238].f1->size[0];
  x_segments[238].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[238].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[238].f1->data[i] = x[238] + b_index * nm->data[i];
  }

  nx = x_segments[238].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[238].f1->data[lastColumn] = rt_roundd_snf(x_segments[238]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[238].f1->size[0];
  y_segments[238].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[238].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[238].f1->data[i] = y[238] + b_index * nm->data[i];
  }

  nx = y_segments[238].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[238].f1->data[lastColumn] = rt_roundd_snf(y_segments[238]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[240] - x[239];
  maxRange_tmp = y[240] - y[239];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[239].f1->size[0];
  x_segments[239].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[239].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[239].f1->data[i] = x[239] + b_index * nm->data[i];
  }

  nx = x_segments[239].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[239].f1->data[lastColumn] = rt_roundd_snf(x_segments[239]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[239].f1->size[0];
  y_segments[239].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[239].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[239].f1->data[i] = y[239] + b_index * nm->data[i];
  }

  nx = y_segments[239].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[239].f1->data[lastColumn] = rt_roundd_snf(y_segments[239]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[241] - x[240];
  maxRange_tmp = y[241] - y[240];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[240].f1->size[0];
  x_segments[240].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[240].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[240].f1->data[i] = x[240] + b_index * nm->data[i];
  }

  nx = x_segments[240].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[240].f1->data[lastColumn] = rt_roundd_snf(x_segments[240]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[240].f1->size[0];
  y_segments[240].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[240].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[240].f1->data[i] = y[240] + b_index * nm->data[i];
  }

  nx = y_segments[240].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[240].f1->data[lastColumn] = rt_roundd_snf(y_segments[240]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[242] - x[241];
  maxRange_tmp = y[242] - y[241];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[241].f1->size[0];
  x_segments[241].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[241].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[241].f1->data[i] = x[241] + b_index * nm->data[i];
  }

  nx = x_segments[241].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[241].f1->data[lastColumn] = rt_roundd_snf(x_segments[241]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[241].f1->size[0];
  y_segments[241].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[241].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[241].f1->data[i] = y[241] + b_index * nm->data[i];
  }

  nx = y_segments[241].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[241].f1->data[lastColumn] = rt_roundd_snf(y_segments[241]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[243] - x[242];
  maxRange_tmp = y[243] - y[242];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[242].f1->size[0];
  x_segments[242].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[242].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[242].f1->data[i] = x[242] + b_index * nm->data[i];
  }

  nx = x_segments[242].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[242].f1->data[lastColumn] = rt_roundd_snf(x_segments[242]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[242].f1->size[0];
  y_segments[242].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[242].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[242].f1->data[i] = y[242] + b_index * nm->data[i];
  }

  nx = y_segments[242].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[242].f1->data[lastColumn] = rt_roundd_snf(y_segments[242]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[244] - x[243];
  maxRange_tmp = y[244] - y[243];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[243].f1->size[0];
  x_segments[243].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[243].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[243].f1->data[i] = x[243] + b_index * nm->data[i];
  }

  nx = x_segments[243].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[243].f1->data[lastColumn] = rt_roundd_snf(x_segments[243]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[243].f1->size[0];
  y_segments[243].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[243].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[243].f1->data[i] = y[243] + b_index * nm->data[i];
  }

  nx = y_segments[243].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[243].f1->data[lastColumn] = rt_roundd_snf(y_segments[243]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[245] - x[244];
  maxRange_tmp = y[245] - y[244];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[244].f1->size[0];
  x_segments[244].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[244].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[244].f1->data[i] = x[244] + b_index * nm->data[i];
  }

  nx = x_segments[244].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[244].f1->data[lastColumn] = rt_roundd_snf(x_segments[244]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[244].f1->size[0];
  y_segments[244].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[244].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[244].f1->data[i] = y[244] + b_index * nm->data[i];
  }

  nx = y_segments[244].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[244].f1->data[lastColumn] = rt_roundd_snf(y_segments[244]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[246] - x[245];
  maxRange_tmp = y[246] - y[245];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[245].f1->size[0];
  x_segments[245].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[245].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[245].f1->data[i] = x[245] + b_index * nm->data[i];
  }

  nx = x_segments[245].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[245].f1->data[lastColumn] = rt_roundd_snf(x_segments[245]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[245].f1->size[0];
  y_segments[245].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[245].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[245].f1->data[i] = y[245] + b_index * nm->data[i];
  }

  nx = y_segments[245].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[245].f1->data[lastColumn] = rt_roundd_snf(y_segments[245]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[247] - x[246];
  maxRange_tmp = y[247] - y[246];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[246].f1->size[0];
  x_segments[246].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[246].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[246].f1->data[i] = x[246] + b_index * nm->data[i];
  }

  nx = x_segments[246].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[246].f1->data[lastColumn] = rt_roundd_snf(x_segments[246]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[246].f1->size[0];
  y_segments[246].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[246].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[246].f1->data[i] = y[246] + b_index * nm->data[i];
  }

  nx = y_segments[246].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[246].f1->data[lastColumn] = rt_roundd_snf(y_segments[246]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[248] - x[247];
  maxRange_tmp = y[248] - y[247];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[247].f1->size[0];
  x_segments[247].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[247].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[247].f1->data[i] = x[247] + b_index * nm->data[i];
  }

  nx = x_segments[247].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[247].f1->data[lastColumn] = rt_roundd_snf(x_segments[247]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[247].f1->size[0];
  y_segments[247].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[247].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[247].f1->data[i] = y[247] + b_index * nm->data[i];
  }

  nx = y_segments[247].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[247].f1->data[lastColumn] = rt_roundd_snf(y_segments[247]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[249] - x[248];
  maxRange_tmp = y[249] - y[248];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[248].f1->size[0];
  x_segments[248].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[248].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[248].f1->data[i] = x[248] + b_index * nm->data[i];
  }

  nx = x_segments[248].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[248].f1->data[lastColumn] = rt_roundd_snf(x_segments[248]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[248].f1->size[0];
  y_segments[248].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[248].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[248].f1->data[i] = y[248] + b_index * nm->data[i];
  }

  nx = y_segments[248].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[248].f1->data[lastColumn] = rt_roundd_snf(y_segments[248]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[250] - x[249];
  maxRange_tmp = y[250] - y[249];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[249].f1->size[0];
  x_segments[249].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[249].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[249].f1->data[i] = x[249] + b_index * nm->data[i];
  }

  nx = x_segments[249].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[249].f1->data[lastColumn] = rt_roundd_snf(x_segments[249]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[249].f1->size[0];
  y_segments[249].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[249].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[249].f1->data[i] = y[249] + b_index * nm->data[i];
  }

  nx = y_segments[249].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[249].f1->data[lastColumn] = rt_roundd_snf(y_segments[249]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[251] - x[250];
  maxRange_tmp = y[251] - y[250];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[250].f1->size[0];
  x_segments[250].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[250].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[250].f1->data[i] = x[250] + b_index * nm->data[i];
  }

  nx = x_segments[250].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[250].f1->data[lastColumn] = rt_roundd_snf(x_segments[250]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[250].f1->size[0];
  y_segments[250].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[250].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[250].f1->data[i] = y[250] + b_index * nm->data[i];
  }

  nx = y_segments[250].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[250].f1->data[lastColumn] = rt_roundd_snf(y_segments[250]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[252] - x[251];
  maxRange_tmp = y[252] - y[251];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[251].f1->size[0];
  x_segments[251].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[251].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[251].f1->data[i] = x[251] + b_index * nm->data[i];
  }

  nx = x_segments[251].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[251].f1->data[lastColumn] = rt_roundd_snf(x_segments[251]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[251].f1->size[0];
  y_segments[251].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[251].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[251].f1->data[i] = y[251] + b_index * nm->data[i];
  }

  nx = y_segments[251].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[251].f1->data[lastColumn] = rt_roundd_snf(y_segments[251]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[253] - x[252];
  maxRange_tmp = y[253] - y[252];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[252].f1->size[0];
  x_segments[252].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[252].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[252].f1->data[i] = x[252] + b_index * nm->data[i];
  }

  nx = x_segments[252].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[252].f1->data[lastColumn] = rt_roundd_snf(x_segments[252]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[252].f1->size[0];
  y_segments[252].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[252].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[252].f1->data[i] = y[252] + b_index * nm->data[i];
  }

  nx = y_segments[252].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[252].f1->data[lastColumn] = rt_roundd_snf(y_segments[252]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[254] - x[253];
  maxRange_tmp = y[254] - y[253];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[253].f1->size[0];
  x_segments[253].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[253].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[253].f1->data[i] = x[253] + b_index * nm->data[i];
  }

  nx = x_segments[253].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[253].f1->data[lastColumn] = rt_roundd_snf(x_segments[253]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[253].f1->size[0];
  y_segments[253].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[253].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[253].f1->data[i] = y[253] + b_index * nm->data[i];
  }

  nx = y_segments[253].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[253].f1->data[lastColumn] = rt_roundd_snf(y_segments[253]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[255] - x[254];
  maxRange_tmp = y[255] - y[254];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[254].f1->size[0];
  x_segments[254].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[254].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[254].f1->data[i] = x[254] + b_index * nm->data[i];
  }

  nx = x_segments[254].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[254].f1->data[lastColumn] = rt_roundd_snf(x_segments[254]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[254].f1->size[0];
  y_segments[254].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[254].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[254].f1->data[i] = y[254] + b_index * nm->data[i];
  }

  nx = y_segments[254].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[254].f1->data[lastColumn] = rt_roundd_snf(y_segments[254]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[256] - x[255];
  maxRange_tmp = y[256] - y[255];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[255].f1->size[0];
  x_segments[255].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[255].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[255].f1->data[i] = x[255] + b_index * nm->data[i];
  }

  nx = x_segments[255].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[255].f1->data[lastColumn] = rt_roundd_snf(x_segments[255]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[255].f1->size[0];
  y_segments[255].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[255].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[255].f1->data[i] = y[255] + b_index * nm->data[i];
  }

  nx = y_segments[255].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[255].f1->data[lastColumn] = rt_roundd_snf(y_segments[255]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[257] - x[256];
  maxRange_tmp = y[257] - y[256];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[256].f1->size[0];
  x_segments[256].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[256].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[256].f1->data[i] = x[256] + b_index * nm->data[i];
  }

  nx = x_segments[256].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[256].f1->data[lastColumn] = rt_roundd_snf(x_segments[256]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[256].f1->size[0];
  y_segments[256].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[256].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[256].f1->data[i] = y[256] + b_index * nm->data[i];
  }

  nx = y_segments[256].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[256].f1->data[lastColumn] = rt_roundd_snf(y_segments[256]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[258] - x[257];
  maxRange_tmp = y[258] - y[257];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[257].f1->size[0];
  x_segments[257].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[257].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[257].f1->data[i] = x[257] + b_index * nm->data[i];
  }

  nx = x_segments[257].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[257].f1->data[lastColumn] = rt_roundd_snf(x_segments[257]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[257].f1->size[0];
  y_segments[257].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[257].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[257].f1->data[i] = y[257] + b_index * nm->data[i];
  }

  nx = y_segments[257].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[257].f1->data[lastColumn] = rt_roundd_snf(y_segments[257]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[259] - x[258];
  maxRange_tmp = y[259] - y[258];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[258].f1->size[0];
  x_segments[258].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[258].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[258].f1->data[i] = x[258] + b_index * nm->data[i];
  }

  nx = x_segments[258].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[258].f1->data[lastColumn] = rt_roundd_snf(x_segments[258]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[258].f1->size[0];
  y_segments[258].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[258].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[258].f1->data[i] = y[258] + b_index * nm->data[i];
  }

  nx = y_segments[258].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[258].f1->data[lastColumn] = rt_roundd_snf(y_segments[258]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[260] - x[259];
  maxRange_tmp = y[260] - y[259];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[259].f1->size[0];
  x_segments[259].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[259].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[259].f1->data[i] = x[259] + b_index * nm->data[i];
  }

  nx = x_segments[259].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[259].f1->data[lastColumn] = rt_roundd_snf(x_segments[259]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[259].f1->size[0];
  y_segments[259].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[259].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[259].f1->data[i] = y[259] + b_index * nm->data[i];
  }

  nx = y_segments[259].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[259].f1->data[lastColumn] = rt_roundd_snf(y_segments[259]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[261] - x[260];
  maxRange_tmp = y[261] - y[260];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[260].f1->size[0];
  x_segments[260].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[260].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[260].f1->data[i] = x[260] + b_index * nm->data[i];
  }

  nx = x_segments[260].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[260].f1->data[lastColumn] = rt_roundd_snf(x_segments[260]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[260].f1->size[0];
  y_segments[260].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[260].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[260].f1->data[i] = y[260] + b_index * nm->data[i];
  }

  nx = y_segments[260].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[260].f1->data[lastColumn] = rt_roundd_snf(y_segments[260]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[262] - x[261];
  maxRange_tmp = y[262] - y[261];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[261].f1->size[0];
  x_segments[261].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[261].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[261].f1->data[i] = x[261] + b_index * nm->data[i];
  }

  nx = x_segments[261].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[261].f1->data[lastColumn] = rt_roundd_snf(x_segments[261]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[261].f1->size[0];
  y_segments[261].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[261].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[261].f1->data[i] = y[261] + b_index * nm->data[i];
  }

  nx = y_segments[261].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[261].f1->data[lastColumn] = rt_roundd_snf(y_segments[261]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[263] - x[262];
  maxRange_tmp = y[263] - y[262];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[262].f1->size[0];
  x_segments[262].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[262].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[262].f1->data[i] = x[262] + b_index * nm->data[i];
  }

  nx = x_segments[262].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[262].f1->data[lastColumn] = rt_roundd_snf(x_segments[262]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[262].f1->size[0];
  y_segments[262].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[262].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[262].f1->data[i] = y[262] + b_index * nm->data[i];
  }

  nx = y_segments[262].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[262].f1->data[lastColumn] = rt_roundd_snf(y_segments[262]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[264] - x[263];
  maxRange_tmp = y[264] - y[263];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[263].f1->size[0];
  x_segments[263].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[263].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[263].f1->data[i] = x[263] + b_index * nm->data[i];
  }

  nx = x_segments[263].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[263].f1->data[lastColumn] = rt_roundd_snf(x_segments[263]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[263].f1->size[0];
  y_segments[263].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[263].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[263].f1->data[i] = y[263] + b_index * nm->data[i];
  }

  nx = y_segments[263].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[263].f1->data[lastColumn] = rt_roundd_snf(y_segments[263]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[265] - x[264];
  maxRange_tmp = y[265] - y[264];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[264].f1->size[0];
  x_segments[264].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[264].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[264].f1->data[i] = x[264] + b_index * nm->data[i];
  }

  nx = x_segments[264].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[264].f1->data[lastColumn] = rt_roundd_snf(x_segments[264]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[264].f1->size[0];
  y_segments[264].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[264].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[264].f1->data[i] = y[264] + b_index * nm->data[i];
  }

  nx = y_segments[264].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[264].f1->data[lastColumn] = rt_roundd_snf(y_segments[264]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[266] - x[265];
  maxRange_tmp = y[266] - y[265];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[265].f1->size[0];
  x_segments[265].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[265].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[265].f1->data[i] = x[265] + b_index * nm->data[i];
  }

  nx = x_segments[265].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[265].f1->data[lastColumn] = rt_roundd_snf(x_segments[265]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[265].f1->size[0];
  y_segments[265].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[265].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[265].f1->data[i] = y[265] + b_index * nm->data[i];
  }

  nx = y_segments[265].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[265].f1->data[lastColumn] = rt_roundd_snf(y_segments[265]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[267] - x[266];
  maxRange_tmp = y[267] - y[266];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[266].f1->size[0];
  x_segments[266].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[266].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[266].f1->data[i] = x[266] + b_index * nm->data[i];
  }

  nx = x_segments[266].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[266].f1->data[lastColumn] = rt_roundd_snf(x_segments[266]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[266].f1->size[0];
  y_segments[266].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[266].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[266].f1->data[i] = y[266] + b_index * nm->data[i];
  }

  nx = y_segments[266].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[266].f1->data[lastColumn] = rt_roundd_snf(y_segments[266]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[268] - x[267];
  maxRange_tmp = y[268] - y[267];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[267].f1->size[0];
  x_segments[267].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[267].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[267].f1->data[i] = x[267] + b_index * nm->data[i];
  }

  nx = x_segments[267].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[267].f1->data[lastColumn] = rt_roundd_snf(x_segments[267]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[267].f1->size[0];
  y_segments[267].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[267].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[267].f1->data[i] = y[267] + b_index * nm->data[i];
  }

  nx = y_segments[267].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[267].f1->data[lastColumn] = rt_roundd_snf(y_segments[267]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[269] - x[268];
  maxRange_tmp = y[269] - y[268];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[268].f1->size[0];
  x_segments[268].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[268].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[268].f1->data[i] = x[268] + b_index * nm->data[i];
  }

  nx = x_segments[268].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[268].f1->data[lastColumn] = rt_roundd_snf(x_segments[268]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[268].f1->size[0];
  y_segments[268].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[268].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[268].f1->data[i] = y[268] + b_index * nm->data[i];
  }

  nx = y_segments[268].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[268].f1->data[lastColumn] = rt_roundd_snf(y_segments[268]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[270] - x[269];
  maxRange_tmp = y[270] - y[269];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[269].f1->size[0];
  x_segments[269].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[269].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[269].f1->data[i] = x[269] + b_index * nm->data[i];
  }

  nx = x_segments[269].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[269].f1->data[lastColumn] = rt_roundd_snf(x_segments[269]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[269].f1->size[0];
  y_segments[269].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[269].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[269].f1->data[i] = y[269] + b_index * nm->data[i];
  }

  nx = y_segments[269].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[269].f1->data[lastColumn] = rt_roundd_snf(y_segments[269]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[271] - x[270];
  maxRange_tmp = y[271] - y[270];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[270].f1->size[0];
  x_segments[270].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[270].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[270].f1->data[i] = x[270] + b_index * nm->data[i];
  }

  nx = x_segments[270].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[270].f1->data[lastColumn] = rt_roundd_snf(x_segments[270]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[270].f1->size[0];
  y_segments[270].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[270].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[270].f1->data[i] = y[270] + b_index * nm->data[i];
  }

  nx = y_segments[270].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[270].f1->data[lastColumn] = rt_roundd_snf(y_segments[270]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[272] - x[271];
  maxRange_tmp = y[272] - y[271];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[271].f1->size[0];
  x_segments[271].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[271].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[271].f1->data[i] = x[271] + b_index * nm->data[i];
  }

  nx = x_segments[271].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[271].f1->data[lastColumn] = rt_roundd_snf(x_segments[271]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[271].f1->size[0];
  y_segments[271].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[271].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[271].f1->data[i] = y[271] + b_index * nm->data[i];
  }

  nx = y_segments[271].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[271].f1->data[lastColumn] = rt_roundd_snf(y_segments[271]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[273] - x[272];
  maxRange_tmp = y[273] - y[272];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[272].f1->size[0];
  x_segments[272].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[272].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[272].f1->data[i] = x[272] + b_index * nm->data[i];
  }

  nx = x_segments[272].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[272].f1->data[lastColumn] = rt_roundd_snf(x_segments[272]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[272].f1->size[0];
  y_segments[272].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[272].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[272].f1->data[i] = y[272] + b_index * nm->data[i];
  }

  nx = y_segments[272].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[272].f1->data[lastColumn] = rt_roundd_snf(y_segments[272]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[274] - x[273];
  maxRange_tmp = y[274] - y[273];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[273].f1->size[0];
  x_segments[273].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[273].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[273].f1->data[i] = x[273] + b_index * nm->data[i];
  }

  nx = x_segments[273].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[273].f1->data[lastColumn] = rt_roundd_snf(x_segments[273]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[273].f1->size[0];
  y_segments[273].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[273].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[273].f1->data[i] = y[273] + b_index * nm->data[i];
  }

  nx = y_segments[273].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[273].f1->data[lastColumn] = rt_roundd_snf(y_segments[273]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[275] - x[274];
  maxRange_tmp = y[275] - y[274];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[274].f1->size[0];
  x_segments[274].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[274].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[274].f1->data[i] = x[274] + b_index * nm->data[i];
  }

  nx = x_segments[274].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[274].f1->data[lastColumn] = rt_roundd_snf(x_segments[274]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[274].f1->size[0];
  y_segments[274].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[274].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[274].f1->data[i] = y[274] + b_index * nm->data[i];
  }

  nx = y_segments[274].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[274].f1->data[lastColumn] = rt_roundd_snf(y_segments[274]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[276] - x[275];
  maxRange_tmp = y[276] - y[275];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[275].f1->size[0];
  x_segments[275].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[275].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[275].f1->data[i] = x[275] + b_index * nm->data[i];
  }

  nx = x_segments[275].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[275].f1->data[lastColumn] = rt_roundd_snf(x_segments[275]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[275].f1->size[0];
  y_segments[275].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[275].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[275].f1->data[i] = y[275] + b_index * nm->data[i];
  }

  nx = y_segments[275].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[275].f1->data[lastColumn] = rt_roundd_snf(y_segments[275]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[277] - x[276];
  maxRange_tmp = y[277] - y[276];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[276].f1->size[0];
  x_segments[276].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[276].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[276].f1->data[i] = x[276] + b_index * nm->data[i];
  }

  nx = x_segments[276].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[276].f1->data[lastColumn] = rt_roundd_snf(x_segments[276]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[276].f1->size[0];
  y_segments[276].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[276].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[276].f1->data[i] = y[276] + b_index * nm->data[i];
  }

  nx = y_segments[276].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[276].f1->data[lastColumn] = rt_roundd_snf(y_segments[276]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[278] - x[277];
  maxRange_tmp = y[278] - y[277];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[277].f1->size[0];
  x_segments[277].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[277].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[277].f1->data[i] = x[277] + b_index * nm->data[i];
  }

  nx = x_segments[277].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[277].f1->data[lastColumn] = rt_roundd_snf(x_segments[277]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[277].f1->size[0];
  y_segments[277].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[277].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[277].f1->data[i] = y[277] + b_index * nm->data[i];
  }

  nx = y_segments[277].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[277].f1->data[lastColumn] = rt_roundd_snf(y_segments[277]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[279] - x[278];
  maxRange_tmp = y[279] - y[278];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[278].f1->size[0];
  x_segments[278].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[278].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[278].f1->data[i] = x[278] + b_index * nm->data[i];
  }

  nx = x_segments[278].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[278].f1->data[lastColumn] = rt_roundd_snf(x_segments[278]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[278].f1->size[0];
  y_segments[278].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[278].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[278].f1->data[i] = y[278] + b_index * nm->data[i];
  }

  nx = y_segments[278].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[278].f1->data[lastColumn] = rt_roundd_snf(y_segments[278]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[280] - x[279];
  maxRange_tmp = y[280] - y[279];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[279].f1->size[0];
  x_segments[279].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[279].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[279].f1->data[i] = x[279] + b_index * nm->data[i];
  }

  nx = x_segments[279].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[279].f1->data[lastColumn] = rt_roundd_snf(x_segments[279]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[279].f1->size[0];
  y_segments[279].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[279].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[279].f1->data[i] = y[279] + b_index * nm->data[i];
  }

  nx = y_segments[279].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[279].f1->data[lastColumn] = rt_roundd_snf(y_segments[279]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[281] - x[280];
  maxRange_tmp = y[281] - y[280];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[280].f1->size[0];
  x_segments[280].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[280].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[280].f1->data[i] = x[280] + b_index * nm->data[i];
  }

  nx = x_segments[280].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[280].f1->data[lastColumn] = rt_roundd_snf(x_segments[280]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[280].f1->size[0];
  y_segments[280].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[280].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[280].f1->data[i] = y[280] + b_index * nm->data[i];
  }

  nx = y_segments[280].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[280].f1->data[lastColumn] = rt_roundd_snf(y_segments[280]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[282] - x[281];
  maxRange_tmp = y[282] - y[281];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[281].f1->size[0];
  x_segments[281].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[281].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[281].f1->data[i] = x[281] + b_index * nm->data[i];
  }

  nx = x_segments[281].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[281].f1->data[lastColumn] = rt_roundd_snf(x_segments[281]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[281].f1->size[0];
  y_segments[281].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[281].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[281].f1->data[i] = y[281] + b_index * nm->data[i];
  }

  nx = y_segments[281].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[281].f1->data[lastColumn] = rt_roundd_snf(y_segments[281]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[283] - x[282];
  maxRange_tmp = y[283] - y[282];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[282].f1->size[0];
  x_segments[282].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[282].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[282].f1->data[i] = x[282] + b_index * nm->data[i];
  }

  nx = x_segments[282].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[282].f1->data[lastColumn] = rt_roundd_snf(x_segments[282]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[282].f1->size[0];
  y_segments[282].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[282].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[282].f1->data[i] = y[282] + b_index * nm->data[i];
  }

  nx = y_segments[282].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[282].f1->data[lastColumn] = rt_roundd_snf(y_segments[282]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[284] - x[283];
  maxRange_tmp = y[284] - y[283];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[283].f1->size[0];
  x_segments[283].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[283].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[283].f1->data[i] = x[283] + b_index * nm->data[i];
  }

  nx = x_segments[283].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[283].f1->data[lastColumn] = rt_roundd_snf(x_segments[283]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[283].f1->size[0];
  y_segments[283].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[283].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[283].f1->data[i] = y[283] + b_index * nm->data[i];
  }

  nx = y_segments[283].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[283].f1->data[lastColumn] = rt_roundd_snf(y_segments[283]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[285] - x[284];
  maxRange_tmp = y[285] - y[284];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[284].f1->size[0];
  x_segments[284].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[284].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[284].f1->data[i] = x[284] + b_index * nm->data[i];
  }

  nx = x_segments[284].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[284].f1->data[lastColumn] = rt_roundd_snf(x_segments[284]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[284].f1->size[0];
  y_segments[284].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[284].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[284].f1->data[i] = y[284] + b_index * nm->data[i];
  }

  nx = y_segments[284].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[284].f1->data[lastColumn] = rt_roundd_snf(y_segments[284]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[286] - x[285];
  maxRange_tmp = y[286] - y[285];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[285].f1->size[0];
  x_segments[285].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[285].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[285].f1->data[i] = x[285] + b_index * nm->data[i];
  }

  nx = x_segments[285].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[285].f1->data[lastColumn] = rt_roundd_snf(x_segments[285]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[285].f1->size[0];
  y_segments[285].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[285].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[285].f1->data[i] = y[285] + b_index * nm->data[i];
  }

  nx = y_segments[285].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[285].f1->data[lastColumn] = rt_roundd_snf(y_segments[285]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[287] - x[286];
  maxRange_tmp = y[287] - y[286];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[286].f1->size[0];
  x_segments[286].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[286].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[286].f1->data[i] = x[286] + b_index * nm->data[i];
  }

  nx = x_segments[286].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[286].f1->data[lastColumn] = rt_roundd_snf(x_segments[286]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[286].f1->size[0];
  y_segments[286].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[286].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[286].f1->data[i] = y[286] + b_index * nm->data[i];
  }

  nx = y_segments[286].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[286].f1->data[lastColumn] = rt_roundd_snf(y_segments[286]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[288] - x[287];
  maxRange_tmp = y[288] - y[287];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[287].f1->size[0];
  x_segments[287].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[287].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[287].f1->data[i] = x[287] + b_index * nm->data[i];
  }

  nx = x_segments[287].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[287].f1->data[lastColumn] = rt_roundd_snf(x_segments[287]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[287].f1->size[0];
  y_segments[287].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[287].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[287].f1->data[i] = y[287] + b_index * nm->data[i];
  }

  nx = y_segments[287].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[287].f1->data[lastColumn] = rt_roundd_snf(y_segments[287]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[289] - x[288];
  maxRange_tmp = y[289] - y[288];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[288].f1->size[0];
  x_segments[288].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[288].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[288].f1->data[i] = x[288] + b_index * nm->data[i];
  }

  nx = x_segments[288].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[288].f1->data[lastColumn] = rt_roundd_snf(x_segments[288]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[288].f1->size[0];
  y_segments[288].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[288].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[288].f1->data[i] = y[288] + b_index * nm->data[i];
  }

  nx = y_segments[288].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[288].f1->data[lastColumn] = rt_roundd_snf(y_segments[288]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[290] - x[289];
  maxRange_tmp = y[290] - y[289];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[289].f1->size[0];
  x_segments[289].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[289].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[289].f1->data[i] = x[289] + b_index * nm->data[i];
  }

  nx = x_segments[289].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[289].f1->data[lastColumn] = rt_roundd_snf(x_segments[289]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[289].f1->size[0];
  y_segments[289].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[289].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[289].f1->data[i] = y[289] + b_index * nm->data[i];
  }

  nx = y_segments[289].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[289].f1->data[lastColumn] = rt_roundd_snf(y_segments[289]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[291] - x[290];
  maxRange_tmp = y[291] - y[290];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[290].f1->size[0];
  x_segments[290].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[290].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[290].f1->data[i] = x[290] + b_index * nm->data[i];
  }

  nx = x_segments[290].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[290].f1->data[lastColumn] = rt_roundd_snf(x_segments[290]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[290].f1->size[0];
  y_segments[290].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[290].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[290].f1->data[i] = y[290] + b_index * nm->data[i];
  }

  nx = y_segments[290].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[290].f1->data[lastColumn] = rt_roundd_snf(y_segments[290]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[292] - x[291];
  maxRange_tmp = y[292] - y[291];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[291].f1->size[0];
  x_segments[291].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[291].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[291].f1->data[i] = x[291] + b_index * nm->data[i];
  }

  nx = x_segments[291].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[291].f1->data[lastColumn] = rt_roundd_snf(x_segments[291]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[291].f1->size[0];
  y_segments[291].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[291].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[291].f1->data[i] = y[291] + b_index * nm->data[i];
  }

  nx = y_segments[291].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[291].f1->data[lastColumn] = rt_roundd_snf(y_segments[291]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[293] - x[292];
  maxRange_tmp = y[293] - y[292];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[292].f1->size[0];
  x_segments[292].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[292].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[292].f1->data[i] = x[292] + b_index * nm->data[i];
  }

  nx = x_segments[292].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[292].f1->data[lastColumn] = rt_roundd_snf(x_segments[292]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[292].f1->size[0];
  y_segments[292].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[292].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[292].f1->data[i] = y[292] + b_index * nm->data[i];
  }

  nx = y_segments[292].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[292].f1->data[lastColumn] = rt_roundd_snf(y_segments[292]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[294] - x[293];
  maxRange_tmp = y[294] - y[293];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[293].f1->size[0];
  x_segments[293].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[293].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[293].f1->data[i] = x[293] + b_index * nm->data[i];
  }

  nx = x_segments[293].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[293].f1->data[lastColumn] = rt_roundd_snf(x_segments[293]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[293].f1->size[0];
  y_segments[293].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[293].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[293].f1->data[i] = y[293] + b_index * nm->data[i];
  }

  nx = y_segments[293].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[293].f1->data[lastColumn] = rt_roundd_snf(y_segments[293]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[295] - x[294];
  maxRange_tmp = y[295] - y[294];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[294].f1->size[0];
  x_segments[294].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[294].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[294].f1->data[i] = x[294] + b_index * nm->data[i];
  }

  nx = x_segments[294].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[294].f1->data[lastColumn] = rt_roundd_snf(x_segments[294]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[294].f1->size[0];
  y_segments[294].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[294].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[294].f1->data[i] = y[294] + b_index * nm->data[i];
  }

  nx = y_segments[294].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[294].f1->data[lastColumn] = rt_roundd_snf(y_segments[294]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[296] - x[295];
  maxRange_tmp = y[296] - y[295];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[295].f1->size[0];
  x_segments[295].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[295].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[295].f1->data[i] = x[295] + b_index * nm->data[i];
  }

  nx = x_segments[295].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[295].f1->data[lastColumn] = rt_roundd_snf(x_segments[295]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[295].f1->size[0];
  y_segments[295].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[295].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[295].f1->data[i] = y[295] + b_index * nm->data[i];
  }

  nx = y_segments[295].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[295].f1->data[lastColumn] = rt_roundd_snf(y_segments[295]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[297] - x[296];
  maxRange_tmp = y[297] - y[296];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[296].f1->size[0];
  x_segments[296].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[296].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[296].f1->data[i] = x[296] + b_index * nm->data[i];
  }

  nx = x_segments[296].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[296].f1->data[lastColumn] = rt_roundd_snf(x_segments[296]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[296].f1->size[0];
  y_segments[296].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[296].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[296].f1->data[i] = y[296] + b_index * nm->data[i];
  }

  nx = y_segments[296].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[296].f1->data[lastColumn] = rt_roundd_snf(y_segments[296]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[298] - x[297];
  maxRange_tmp = y[298] - y[297];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[297].f1->size[0];
  x_segments[297].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[297].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[297].f1->data[i] = x[297] + b_index * nm->data[i];
  }

  nx = x_segments[297].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[297].f1->data[lastColumn] = rt_roundd_snf(x_segments[297]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[297].f1->size[0];
  y_segments[297].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[297].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[297].f1->data[i] = y[297] + b_index * nm->data[i];
  }

  nx = y_segments[297].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[297].f1->data[lastColumn] = rt_roundd_snf(y_segments[297]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[299] - x[298];
  maxRange_tmp = y[299] - y[298];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[298].f1->size[0];
  x_segments[298].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[298].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[298].f1->data[i] = x[298] + b_index * nm->data[i];
  }

  nx = x_segments[298].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[298].f1->data[lastColumn] = rt_roundd_snf(x_segments[298]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[298].f1->size[0];
  y_segments[298].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[298].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[298].f1->data[i] = y[298] + b_index * nm->data[i];
  }

  nx = y_segments[298].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[298].f1->data[lastColumn] = rt_roundd_snf(y_segments[298]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[300] - x[299];
  maxRange_tmp = y[300] - y[299];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[299].f1->size[0];
  x_segments[299].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[299].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[299].f1->data[i] = x[299] + b_index * nm->data[i];
  }

  nx = x_segments[299].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[299].f1->data[lastColumn] = rt_roundd_snf(x_segments[299]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[299].f1->size[0];
  y_segments[299].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[299].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[299].f1->data[i] = y[299] + b_index * nm->data[i];
  }

  nx = y_segments[299].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[299].f1->data[lastColumn] = rt_roundd_snf(y_segments[299]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[301] - x[300];
  maxRange_tmp = y[301] - y[300];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[300].f1->size[0];
  x_segments[300].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[300].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[300].f1->data[i] = x[300] + b_index * nm->data[i];
  }

  nx = x_segments[300].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[300].f1->data[lastColumn] = rt_roundd_snf(x_segments[300]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[300].f1->size[0];
  y_segments[300].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[300].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[300].f1->data[i] = y[300] + b_index * nm->data[i];
  }

  nx = y_segments[300].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[300].f1->data[lastColumn] = rt_roundd_snf(y_segments[300]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[302] - x[301];
  maxRange_tmp = y[302] - y[301];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[301].f1->size[0];
  x_segments[301].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[301].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[301].f1->data[i] = x[301] + b_index * nm->data[i];
  }

  nx = x_segments[301].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[301].f1->data[lastColumn] = rt_roundd_snf(x_segments[301]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[301].f1->size[0];
  y_segments[301].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[301].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[301].f1->data[i] = y[301] + b_index * nm->data[i];
  }

  nx = y_segments[301].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[301].f1->data[lastColumn] = rt_roundd_snf(y_segments[301]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[303] - x[302];
  maxRange_tmp = y[303] - y[302];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[302].f1->size[0];
  x_segments[302].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[302].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[302].f1->data[i] = x[302] + b_index * nm->data[i];
  }

  nx = x_segments[302].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[302].f1->data[lastColumn] = rt_roundd_snf(x_segments[302]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[302].f1->size[0];
  y_segments[302].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[302].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[302].f1->data[i] = y[302] + b_index * nm->data[i];
  }

  nx = y_segments[302].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[302].f1->data[lastColumn] = rt_roundd_snf(y_segments[302]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[304] - x[303];
  maxRange_tmp = y[304] - y[303];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[303].f1->size[0];
  x_segments[303].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[303].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[303].f1->data[i] = x[303] + b_index * nm->data[i];
  }

  nx = x_segments[303].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[303].f1->data[lastColumn] = rt_roundd_snf(x_segments[303]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[303].f1->size[0];
  y_segments[303].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[303].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[303].f1->data[i] = y[303] + b_index * nm->data[i];
  }

  nx = y_segments[303].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[303].f1->data[lastColumn] = rt_roundd_snf(y_segments[303]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[305] - x[304];
  maxRange_tmp = y[305] - y[304];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[304].f1->size[0];
  x_segments[304].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[304].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[304].f1->data[i] = x[304] + b_index * nm->data[i];
  }

  nx = x_segments[304].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[304].f1->data[lastColumn] = rt_roundd_snf(x_segments[304]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[304].f1->size[0];
  y_segments[304].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[304].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[304].f1->data[i] = y[304] + b_index * nm->data[i];
  }

  nx = y_segments[304].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[304].f1->data[lastColumn] = rt_roundd_snf(y_segments[304]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[306] - x[305];
  maxRange_tmp = y[306] - y[305];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[305].f1->size[0];
  x_segments[305].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[305].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[305].f1->data[i] = x[305] + b_index * nm->data[i];
  }

  nx = x_segments[305].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[305].f1->data[lastColumn] = rt_roundd_snf(x_segments[305]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[305].f1->size[0];
  y_segments[305].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[305].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[305].f1->data[i] = y[305] + b_index * nm->data[i];
  }

  nx = y_segments[305].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[305].f1->data[lastColumn] = rt_roundd_snf(y_segments[305]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[307] - x[306];
  maxRange_tmp = y[307] - y[306];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[306].f1->size[0];
  x_segments[306].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[306].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[306].f1->data[i] = x[306] + b_index * nm->data[i];
  }

  nx = x_segments[306].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[306].f1->data[lastColumn] = rt_roundd_snf(x_segments[306]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[306].f1->size[0];
  y_segments[306].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[306].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[306].f1->data[i] = y[306] + b_index * nm->data[i];
  }

  nx = y_segments[306].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[306].f1->data[lastColumn] = rt_roundd_snf(y_segments[306]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[308] - x[307];
  maxRange_tmp = y[308] - y[307];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[307].f1->size[0];
  x_segments[307].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[307].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[307].f1->data[i] = x[307] + b_index * nm->data[i];
  }

  nx = x_segments[307].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[307].f1->data[lastColumn] = rt_roundd_snf(x_segments[307]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[307].f1->size[0];
  y_segments[307].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[307].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[307].f1->data[i] = y[307] + b_index * nm->data[i];
  }

  nx = y_segments[307].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[307].f1->data[lastColumn] = rt_roundd_snf(y_segments[307]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[309] - x[308];
  maxRange_tmp = y[309] - y[308];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[308].f1->size[0];
  x_segments[308].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[308].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[308].f1->data[i] = x[308] + b_index * nm->data[i];
  }

  nx = x_segments[308].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[308].f1->data[lastColumn] = rt_roundd_snf(x_segments[308]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[308].f1->size[0];
  y_segments[308].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[308].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[308].f1->data[i] = y[308] + b_index * nm->data[i];
  }

  nx = y_segments[308].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[308].f1->data[lastColumn] = rt_roundd_snf(y_segments[308]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[310] - x[309];
  maxRange_tmp = y[310] - y[309];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[309].f1->size[0];
  x_segments[309].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[309].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[309].f1->data[i] = x[309] + b_index * nm->data[i];
  }

  nx = x_segments[309].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[309].f1->data[lastColumn] = rt_roundd_snf(x_segments[309]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[309].f1->size[0];
  y_segments[309].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[309].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[309].f1->data[i] = y[309] + b_index * nm->data[i];
  }

  nx = y_segments[309].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[309].f1->data[lastColumn] = rt_roundd_snf(y_segments[309]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[311] - x[310];
  maxRange_tmp = y[311] - y[310];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[310].f1->size[0];
  x_segments[310].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[310].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[310].f1->data[i] = x[310] + b_index * nm->data[i];
  }

  nx = x_segments[310].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[310].f1->data[lastColumn] = rt_roundd_snf(x_segments[310]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[310].f1->size[0];
  y_segments[310].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[310].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[310].f1->data[i] = y[310] + b_index * nm->data[i];
  }

  nx = y_segments[310].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[310].f1->data[lastColumn] = rt_roundd_snf(y_segments[310]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[312] - x[311];
  maxRange_tmp = y[312] - y[311];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[311].f1->size[0];
  x_segments[311].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[311].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[311].f1->data[i] = x[311] + b_index * nm->data[i];
  }

  nx = x_segments[311].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[311].f1->data[lastColumn] = rt_roundd_snf(x_segments[311]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[311].f1->size[0];
  y_segments[311].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[311].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[311].f1->data[i] = y[311] + b_index * nm->data[i];
  }

  nx = y_segments[311].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[311].f1->data[lastColumn] = rt_roundd_snf(y_segments[311]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[313] - x[312];
  maxRange_tmp = y[313] - y[312];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[312].f1->size[0];
  x_segments[312].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[312].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[312].f1->data[i] = x[312] + b_index * nm->data[i];
  }

  nx = x_segments[312].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[312].f1->data[lastColumn] = rt_roundd_snf(x_segments[312]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[312].f1->size[0];
  y_segments[312].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[312].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[312].f1->data[i] = y[312] + b_index * nm->data[i];
  }

  nx = y_segments[312].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[312].f1->data[lastColumn] = rt_roundd_snf(y_segments[312]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[314] - x[313];
  maxRange_tmp = y[314] - y[313];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[313].f1->size[0];
  x_segments[313].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[313].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[313].f1->data[i] = x[313] + b_index * nm->data[i];
  }

  nx = x_segments[313].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[313].f1->data[lastColumn] = rt_roundd_snf(x_segments[313]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[313].f1->size[0];
  y_segments[313].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[313].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[313].f1->data[i] = y[313] + b_index * nm->data[i];
  }

  nx = y_segments[313].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[313].f1->data[lastColumn] = rt_roundd_snf(y_segments[313]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[315] - x[314];
  maxRange_tmp = y[315] - y[314];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[314].f1->size[0];
  x_segments[314].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[314].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[314].f1->data[i] = x[314] + b_index * nm->data[i];
  }

  nx = x_segments[314].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[314].f1->data[lastColumn] = rt_roundd_snf(x_segments[314]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[314].f1->size[0];
  y_segments[314].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[314].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[314].f1->data[i] = y[314] + b_index * nm->data[i];
  }

  nx = y_segments[314].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[314].f1->data[lastColumn] = rt_roundd_snf(y_segments[314]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[316] - x[315];
  maxRange_tmp = y[316] - y[315];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[315].f1->size[0];
  x_segments[315].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[315].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[315].f1->data[i] = x[315] + b_index * nm->data[i];
  }

  nx = x_segments[315].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[315].f1->data[lastColumn] = rt_roundd_snf(x_segments[315]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[315].f1->size[0];
  y_segments[315].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[315].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[315].f1->data[i] = y[315] + b_index * nm->data[i];
  }

  nx = y_segments[315].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[315].f1->data[lastColumn] = rt_roundd_snf(y_segments[315]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[317] - x[316];
  maxRange_tmp = y[317] - y[316];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[316].f1->size[0];
  x_segments[316].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[316].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[316].f1->data[i] = x[316] + b_index * nm->data[i];
  }

  nx = x_segments[316].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[316].f1->data[lastColumn] = rt_roundd_snf(x_segments[316]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[316].f1->size[0];
  y_segments[316].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[316].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[316].f1->data[i] = y[316] + b_index * nm->data[i];
  }

  nx = y_segments[316].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[316].f1->data[lastColumn] = rt_roundd_snf(y_segments[316]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[318] - x[317];
  maxRange_tmp = y[318] - y[317];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[317].f1->size[0];
  x_segments[317].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[317].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[317].f1->data[i] = x[317] + b_index * nm->data[i];
  }

  nx = x_segments[317].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[317].f1->data[lastColumn] = rt_roundd_snf(x_segments[317]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[317].f1->size[0];
  y_segments[317].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[317].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[317].f1->data[i] = y[317] + b_index * nm->data[i];
  }

  nx = y_segments[317].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[317].f1->data[lastColumn] = rt_roundd_snf(y_segments[317]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[319] - x[318];
  maxRange_tmp = y[319] - y[318];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[318].f1->size[0];
  x_segments[318].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[318].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[318].f1->data[i] = x[318] + b_index * nm->data[i];
  }

  nx = x_segments[318].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[318].f1->data[lastColumn] = rt_roundd_snf(x_segments[318]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[318].f1->size[0];
  y_segments[318].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[318].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[318].f1->data[i] = y[318] + b_index * nm->data[i];
  }

  nx = y_segments[318].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[318].f1->data[lastColumn] = rt_roundd_snf(y_segments[318]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[320] - x[319];
  maxRange_tmp = y[320] - y[319];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[319].f1->size[0];
  x_segments[319].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[319].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[319].f1->data[i] = x[319] + b_index * nm->data[i];
  }

  nx = x_segments[319].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[319].f1->data[lastColumn] = rt_roundd_snf(x_segments[319]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[319].f1->size[0];
  y_segments[319].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[319].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[319].f1->data[i] = y[319] + b_index * nm->data[i];
  }

  nx = y_segments[319].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[319].f1->data[lastColumn] = rt_roundd_snf(y_segments[319]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[321] - x[320];
  maxRange_tmp = y[321] - y[320];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[320].f1->size[0];
  x_segments[320].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[320].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[320].f1->data[i] = x[320] + b_index * nm->data[i];
  }

  nx = x_segments[320].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[320].f1->data[lastColumn] = rt_roundd_snf(x_segments[320]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[320].f1->size[0];
  y_segments[320].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[320].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[320].f1->data[i] = y[320] + b_index * nm->data[i];
  }

  nx = y_segments[320].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[320].f1->data[lastColumn] = rt_roundd_snf(y_segments[320]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[322] - x[321];
  maxRange_tmp = y[322] - y[321];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[321].f1->size[0];
  x_segments[321].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[321].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[321].f1->data[i] = x[321] + b_index * nm->data[i];
  }

  nx = x_segments[321].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[321].f1->data[lastColumn] = rt_roundd_snf(x_segments[321]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[321].f1->size[0];
  y_segments[321].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[321].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[321].f1->data[i] = y[321] + b_index * nm->data[i];
  }

  nx = y_segments[321].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[321].f1->data[lastColumn] = rt_roundd_snf(y_segments[321]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[323] - x[322];
  maxRange_tmp = y[323] - y[322];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[322].f1->size[0];
  x_segments[322].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[322].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[322].f1->data[i] = x[322] + b_index * nm->data[i];
  }

  nx = x_segments[322].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[322].f1->data[lastColumn] = rt_roundd_snf(x_segments[322]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[322].f1->size[0];
  y_segments[322].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[322].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[322].f1->data[i] = y[322] + b_index * nm->data[i];
  }

  nx = y_segments[322].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[322].f1->data[lastColumn] = rt_roundd_snf(y_segments[322]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[324] - x[323];
  maxRange_tmp = y[324] - y[323];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[323].f1->size[0];
  x_segments[323].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[323].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[323].f1->data[i] = x[323] + b_index * nm->data[i];
  }

  nx = x_segments[323].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[323].f1->data[lastColumn] = rt_roundd_snf(x_segments[323]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[323].f1->size[0];
  y_segments[323].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[323].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[323].f1->data[i] = y[323] + b_index * nm->data[i];
  }

  nx = y_segments[323].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[323].f1->data[lastColumn] = rt_roundd_snf(y_segments[323]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[325] - x[324];
  maxRange_tmp = y[325] - y[324];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[324].f1->size[0];
  x_segments[324].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[324].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[324].f1->data[i] = x[324] + b_index * nm->data[i];
  }

  nx = x_segments[324].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[324].f1->data[lastColumn] = rt_roundd_snf(x_segments[324]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[324].f1->size[0];
  y_segments[324].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[324].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[324].f1->data[i] = y[324] + b_index * nm->data[i];
  }

  nx = y_segments[324].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[324].f1->data[lastColumn] = rt_roundd_snf(y_segments[324]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[326] - x[325];
  maxRange_tmp = y[326] - y[325];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[325].f1->size[0];
  x_segments[325].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[325].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[325].f1->data[i] = x[325] + b_index * nm->data[i];
  }

  nx = x_segments[325].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[325].f1->data[lastColumn] = rt_roundd_snf(x_segments[325]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[325].f1->size[0];
  y_segments[325].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[325].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[325].f1->data[i] = y[325] + b_index * nm->data[i];
  }

  nx = y_segments[325].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[325].f1->data[lastColumn] = rt_roundd_snf(y_segments[325]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[327] - x[326];
  maxRange_tmp = y[327] - y[326];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[326].f1->size[0];
  x_segments[326].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[326].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[326].f1->data[i] = x[326] + b_index * nm->data[i];
  }

  nx = x_segments[326].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[326].f1->data[lastColumn] = rt_roundd_snf(x_segments[326]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[326].f1->size[0];
  y_segments[326].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[326].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[326].f1->data[i] = y[326] + b_index * nm->data[i];
  }

  nx = y_segments[326].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[326].f1->data[lastColumn] = rt_roundd_snf(y_segments[326]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[328] - x[327];
  maxRange_tmp = y[328] - y[327];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[327].f1->size[0];
  x_segments[327].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[327].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[327].f1->data[i] = x[327] + b_index * nm->data[i];
  }

  nx = x_segments[327].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[327].f1->data[lastColumn] = rt_roundd_snf(x_segments[327]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[327].f1->size[0];
  y_segments[327].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[327].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[327].f1->data[i] = y[327] + b_index * nm->data[i];
  }

  nx = y_segments[327].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[327].f1->data[lastColumn] = rt_roundd_snf(y_segments[327]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[329] - x[328];
  maxRange_tmp = y[329] - y[328];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[328].f1->size[0];
  x_segments[328].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[328].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[328].f1->data[i] = x[328] + b_index * nm->data[i];
  }

  nx = x_segments[328].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[328].f1->data[lastColumn] = rt_roundd_snf(x_segments[328]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[328].f1->size[0];
  y_segments[328].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[328].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[328].f1->data[i] = y[328] + b_index * nm->data[i];
  }

  nx = y_segments[328].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[328].f1->data[lastColumn] = rt_roundd_snf(y_segments[328]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[330] - x[329];
  maxRange_tmp = y[330] - y[329];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[329].f1->size[0];
  x_segments[329].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[329].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[329].f1->data[i] = x[329] + b_index * nm->data[i];
  }

  nx = x_segments[329].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[329].f1->data[lastColumn] = rt_roundd_snf(x_segments[329]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[329].f1->size[0];
  y_segments[329].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[329].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[329].f1->data[i] = y[329] + b_index * nm->data[i];
  }

  nx = y_segments[329].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[329].f1->data[lastColumn] = rt_roundd_snf(y_segments[329]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[331] - x[330];
  maxRange_tmp = y[331] - y[330];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[330].f1->size[0];
  x_segments[330].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[330].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[330].f1->data[i] = x[330] + b_index * nm->data[i];
  }

  nx = x_segments[330].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[330].f1->data[lastColumn] = rt_roundd_snf(x_segments[330]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[330].f1->size[0];
  y_segments[330].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[330].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[330].f1->data[i] = y[330] + b_index * nm->data[i];
  }

  nx = y_segments[330].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[330].f1->data[lastColumn] = rt_roundd_snf(y_segments[330]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[332] - x[331];
  maxRange_tmp = y[332] - y[331];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[331].f1->size[0];
  x_segments[331].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[331].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[331].f1->data[i] = x[331] + b_index * nm->data[i];
  }

  nx = x_segments[331].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[331].f1->data[lastColumn] = rt_roundd_snf(x_segments[331]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[331].f1->size[0];
  y_segments[331].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[331].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[331].f1->data[i] = y[331] + b_index * nm->data[i];
  }

  nx = y_segments[331].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[331].f1->data[lastColumn] = rt_roundd_snf(y_segments[331]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[333] - x[332];
  maxRange_tmp = y[333] - y[332];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[332].f1->size[0];
  x_segments[332].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[332].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[332].f1->data[i] = x[332] + b_index * nm->data[i];
  }

  nx = x_segments[332].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[332].f1->data[lastColumn] = rt_roundd_snf(x_segments[332]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[332].f1->size[0];
  y_segments[332].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[332].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[332].f1->data[i] = y[332] + b_index * nm->data[i];
  }

  nx = y_segments[332].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[332].f1->data[lastColumn] = rt_roundd_snf(y_segments[332]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[334] - x[333];
  maxRange_tmp = y[334] - y[333];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[333].f1->size[0];
  x_segments[333].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[333].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[333].f1->data[i] = x[333] + b_index * nm->data[i];
  }

  nx = x_segments[333].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[333].f1->data[lastColumn] = rt_roundd_snf(x_segments[333]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[333].f1->size[0];
  y_segments[333].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[333].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[333].f1->data[i] = y[333] + b_index * nm->data[i];
  }

  nx = y_segments[333].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[333].f1->data[lastColumn] = rt_roundd_snf(y_segments[333]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[335] - x[334];
  maxRange_tmp = y[335] - y[334];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[334].f1->size[0];
  x_segments[334].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[334].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[334].f1->data[i] = x[334] + b_index * nm->data[i];
  }

  nx = x_segments[334].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[334].f1->data[lastColumn] = rt_roundd_snf(x_segments[334]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[334].f1->size[0];
  y_segments[334].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[334].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[334].f1->data[i] = y[334] + b_index * nm->data[i];
  }

  nx = y_segments[334].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[334].f1->data[lastColumn] = rt_roundd_snf(y_segments[334]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[336] - x[335];
  maxRange_tmp = y[336] - y[335];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[335].f1->size[0];
  x_segments[335].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[335].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[335].f1->data[i] = x[335] + b_index * nm->data[i];
  }

  nx = x_segments[335].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[335].f1->data[lastColumn] = rt_roundd_snf(x_segments[335]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[335].f1->size[0];
  y_segments[335].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[335].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[335].f1->data[i] = y[335] + b_index * nm->data[i];
  }

  nx = y_segments[335].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[335].f1->data[lastColumn] = rt_roundd_snf(y_segments[335]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[337] - x[336];
  maxRange_tmp = y[337] - y[336];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[336].f1->size[0];
  x_segments[336].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[336].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[336].f1->data[i] = x[336] + b_index * nm->data[i];
  }

  nx = x_segments[336].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[336].f1->data[lastColumn] = rt_roundd_snf(x_segments[336]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[336].f1->size[0];
  y_segments[336].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[336].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[336].f1->data[i] = y[336] + b_index * nm->data[i];
  }

  nx = y_segments[336].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[336].f1->data[lastColumn] = rt_roundd_snf(y_segments[336]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[338] - x[337];
  maxRange_tmp = y[338] - y[337];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[337].f1->size[0];
  x_segments[337].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[337].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[337].f1->data[i] = x[337] + b_index * nm->data[i];
  }

  nx = x_segments[337].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[337].f1->data[lastColumn] = rt_roundd_snf(x_segments[337]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[337].f1->size[0];
  y_segments[337].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[337].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[337].f1->data[i] = y[337] + b_index * nm->data[i];
  }

  nx = y_segments[337].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[337].f1->data[lastColumn] = rt_roundd_snf(y_segments[337]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[339] - x[338];
  maxRange_tmp = y[339] - y[338];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[338].f1->size[0];
  x_segments[338].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[338].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[338].f1->data[i] = x[338] + b_index * nm->data[i];
  }

  nx = x_segments[338].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[338].f1->data[lastColumn] = rt_roundd_snf(x_segments[338]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[338].f1->size[0];
  y_segments[338].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[338].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[338].f1->data[i] = y[338] + b_index * nm->data[i];
  }

  nx = y_segments[338].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[338].f1->data[lastColumn] = rt_roundd_snf(y_segments[338]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[340] - x[339];
  maxRange_tmp = y[340] - y[339];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[339].f1->size[0];
  x_segments[339].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[339].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[339].f1->data[i] = x[339] + b_index * nm->data[i];
  }

  nx = x_segments[339].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[339].f1->data[lastColumn] = rt_roundd_snf(x_segments[339]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[339].f1->size[0];
  y_segments[339].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[339].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[339].f1->data[i] = y[339] + b_index * nm->data[i];
  }

  nx = y_segments[339].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[339].f1->data[lastColumn] = rt_roundd_snf(y_segments[339]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[341] - x[340];
  maxRange_tmp = y[341] - y[340];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[340].f1->size[0];
  x_segments[340].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[340].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[340].f1->data[i] = x[340] + b_index * nm->data[i];
  }

  nx = x_segments[340].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[340].f1->data[lastColumn] = rt_roundd_snf(x_segments[340]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[340].f1->size[0];
  y_segments[340].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[340].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[340].f1->data[i] = y[340] + b_index * nm->data[i];
  }

  nx = y_segments[340].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[340].f1->data[lastColumn] = rt_roundd_snf(y_segments[340]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[342] - x[341];
  maxRange_tmp = y[342] - y[341];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[341].f1->size[0];
  x_segments[341].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[341].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[341].f1->data[i] = x[341] + b_index * nm->data[i];
  }

  nx = x_segments[341].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[341].f1->data[lastColumn] = rt_roundd_snf(x_segments[341]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[341].f1->size[0];
  y_segments[341].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[341].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[341].f1->data[i] = y[341] + b_index * nm->data[i];
  }

  nx = y_segments[341].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[341].f1->data[lastColumn] = rt_roundd_snf(y_segments[341]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[343] - x[342];
  maxRange_tmp = y[343] - y[342];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[342].f1->size[0];
  x_segments[342].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[342].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[342].f1->data[i] = x[342] + b_index * nm->data[i];
  }

  nx = x_segments[342].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[342].f1->data[lastColumn] = rt_roundd_snf(x_segments[342]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[342].f1->size[0];
  y_segments[342].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[342].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[342].f1->data[i] = y[342] + b_index * nm->data[i];
  }

  nx = y_segments[342].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[342].f1->data[lastColumn] = rt_roundd_snf(y_segments[342]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[344] - x[343];
  maxRange_tmp = y[344] - y[343];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[343].f1->size[0];
  x_segments[343].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[343].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[343].f1->data[i] = x[343] + b_index * nm->data[i];
  }

  nx = x_segments[343].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[343].f1->data[lastColumn] = rt_roundd_snf(x_segments[343]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[343].f1->size[0];
  y_segments[343].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[343].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[343].f1->data[i] = y[343] + b_index * nm->data[i];
  }

  nx = y_segments[343].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[343].f1->data[lastColumn] = rt_roundd_snf(y_segments[343]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[345] - x[344];
  maxRange_tmp = y[345] - y[344];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[344].f1->size[0];
  x_segments[344].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[344].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[344].f1->data[i] = x[344] + b_index * nm->data[i];
  }

  nx = x_segments[344].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[344].f1->data[lastColumn] = rt_roundd_snf(x_segments[344]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[344].f1->size[0];
  y_segments[344].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[344].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[344].f1->data[i] = y[344] + b_index * nm->data[i];
  }

  nx = y_segments[344].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[344].f1->data[lastColumn] = rt_roundd_snf(y_segments[344]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[346] - x[345];
  maxRange_tmp = y[346] - y[345];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[345].f1->size[0];
  x_segments[345].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[345].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[345].f1->data[i] = x[345] + b_index * nm->data[i];
  }

  nx = x_segments[345].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[345].f1->data[lastColumn] = rt_roundd_snf(x_segments[345]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[345].f1->size[0];
  y_segments[345].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[345].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[345].f1->data[i] = y[345] + b_index * nm->data[i];
  }

  nx = y_segments[345].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[345].f1->data[lastColumn] = rt_roundd_snf(y_segments[345]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[347] - x[346];
  maxRange_tmp = y[347] - y[346];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[346].f1->size[0];
  x_segments[346].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[346].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[346].f1->data[i] = x[346] + b_index * nm->data[i];
  }

  nx = x_segments[346].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[346].f1->data[lastColumn] = rt_roundd_snf(x_segments[346]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[346].f1->size[0];
  y_segments[346].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[346].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[346].f1->data[i] = y[346] + b_index * nm->data[i];
  }

  nx = y_segments[346].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[346].f1->data[lastColumn] = rt_roundd_snf(y_segments[346]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[348] - x[347];
  maxRange_tmp = y[348] - y[347];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[347].f1->size[0];
  x_segments[347].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[347].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[347].f1->data[i] = x[347] + b_index * nm->data[i];
  }

  nx = x_segments[347].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[347].f1->data[lastColumn] = rt_roundd_snf(x_segments[347]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[347].f1->size[0];
  y_segments[347].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[347].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[347].f1->data[i] = y[347] + b_index * nm->data[i];
  }

  nx = y_segments[347].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[347].f1->data[lastColumn] = rt_roundd_snf(y_segments[347]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[349] - x[348];
  maxRange_tmp = y[349] - y[348];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[348].f1->size[0];
  x_segments[348].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[348].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[348].f1->data[i] = x[348] + b_index * nm->data[i];
  }

  nx = x_segments[348].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[348].f1->data[lastColumn] = rt_roundd_snf(x_segments[348]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[348].f1->size[0];
  y_segments[348].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[348].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[348].f1->data[i] = y[348] + b_index * nm->data[i];
  }

  nx = y_segments[348].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[348].f1->data[lastColumn] = rt_roundd_snf(y_segments[348]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[350] - x[349];
  maxRange_tmp = y[350] - y[349];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[349].f1->size[0];
  x_segments[349].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[349].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[349].f1->data[i] = x[349] + b_index * nm->data[i];
  }

  nx = x_segments[349].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[349].f1->data[lastColumn] = rt_roundd_snf(x_segments[349]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[349].f1->size[0];
  y_segments[349].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[349].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[349].f1->data[i] = y[349] + b_index * nm->data[i];
  }

  nx = y_segments[349].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[349].f1->data[lastColumn] = rt_roundd_snf(y_segments[349]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[351] - x[350];
  maxRange_tmp = y[351] - y[350];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[350].f1->size[0];
  x_segments[350].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[350].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[350].f1->data[i] = x[350] + b_index * nm->data[i];
  }

  nx = x_segments[350].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[350].f1->data[lastColumn] = rt_roundd_snf(x_segments[350]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[350].f1->size[0];
  y_segments[350].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[350].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[350].f1->data[i] = y[350] + b_index * nm->data[i];
  }

  nx = y_segments[350].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[350].f1->data[lastColumn] = rt_roundd_snf(y_segments[350]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[352] - x[351];
  maxRange_tmp = y[352] - y[351];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[351].f1->size[0];
  x_segments[351].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[351].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[351].f1->data[i] = x[351] + b_index * nm->data[i];
  }

  nx = x_segments[351].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[351].f1->data[lastColumn] = rt_roundd_snf(x_segments[351]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[351].f1->size[0];
  y_segments[351].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[351].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[351].f1->data[i] = y[351] + b_index * nm->data[i];
  }

  nx = y_segments[351].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[351].f1->data[lastColumn] = rt_roundd_snf(y_segments[351]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[353] - x[352];
  maxRange_tmp = y[353] - y[352];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[352].f1->size[0];
  x_segments[352].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[352].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[352].f1->data[i] = x[352] + b_index * nm->data[i];
  }

  nx = x_segments[352].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[352].f1->data[lastColumn] = rt_roundd_snf(x_segments[352]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[352].f1->size[0];
  y_segments[352].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[352].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[352].f1->data[i] = y[352] + b_index * nm->data[i];
  }

  nx = y_segments[352].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[352].f1->data[lastColumn] = rt_roundd_snf(y_segments[352]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[354] - x[353];
  maxRange_tmp = y[354] - y[353];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[353].f1->size[0];
  x_segments[353].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[353].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[353].f1->data[i] = x[353] + b_index * nm->data[i];
  }

  nx = x_segments[353].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[353].f1->data[lastColumn] = rt_roundd_snf(x_segments[353]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[353].f1->size[0];
  y_segments[353].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[353].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[353].f1->data[i] = y[353] + b_index * nm->data[i];
  }

  nx = y_segments[353].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[353].f1->data[lastColumn] = rt_roundd_snf(y_segments[353]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[355] - x[354];
  maxRange_tmp = y[355] - y[354];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[354].f1->size[0];
  x_segments[354].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[354].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[354].f1->data[i] = x[354] + b_index * nm->data[i];
  }

  nx = x_segments[354].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[354].f1->data[lastColumn] = rt_roundd_snf(x_segments[354]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[354].f1->size[0];
  y_segments[354].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[354].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[354].f1->data[i] = y[354] + b_index * nm->data[i];
  }

  nx = y_segments[354].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[354].f1->data[lastColumn] = rt_roundd_snf(y_segments[354]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[356] - x[355];
  maxRange_tmp = y[356] - y[355];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[355].f1->size[0];
  x_segments[355].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[355].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[355].f1->data[i] = x[355] + b_index * nm->data[i];
  }

  nx = x_segments[355].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[355].f1->data[lastColumn] = rt_roundd_snf(x_segments[355]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[355].f1->size[0];
  y_segments[355].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[355].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[355].f1->data[i] = y[355] + b_index * nm->data[i];
  }

  nx = y_segments[355].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[355].f1->data[lastColumn] = rt_roundd_snf(y_segments[355]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[357] - x[356];
  maxRange_tmp = y[357] - y[356];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[356].f1->size[0];
  x_segments[356].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[356].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[356].f1->data[i] = x[356] + b_index * nm->data[i];
  }

  nx = x_segments[356].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[356].f1->data[lastColumn] = rt_roundd_snf(x_segments[356]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[356].f1->size[0];
  y_segments[356].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[356].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[356].f1->data[i] = y[356] + b_index * nm->data[i];
  }

  nx = y_segments[356].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[356].f1->data[lastColumn] = rt_roundd_snf(y_segments[356]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[358] - x[357];
  maxRange_tmp = y[358] - y[357];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[357].f1->size[0];
  x_segments[357].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[357].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[357].f1->data[i] = x[357] + b_index * nm->data[i];
  }

  nx = x_segments[357].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[357].f1->data[lastColumn] = rt_roundd_snf(x_segments[357]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[357].f1->size[0];
  y_segments[357].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[357].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[357].f1->data[i] = y[357] + b_index * nm->data[i];
  }

  nx = y_segments[357].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[357].f1->data[lastColumn] = rt_roundd_snf(y_segments[357]
      .f1->data[lastColumn]);
  }

  /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
  /*  iptui.intline does not support Matlab coder */
  b_index = x[359] - x[358];
  maxRange_tmp = y[359] - y[358];
  maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
  if (rtIsNaN(maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    b_y->size[1] = 1;
    emxEnsureCapacity_real_T(b_y, i);
    b_y->data[0] = rtNaN;
  } else {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    loop_ub = (int)floor(maxRange);
    b_y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_y, i);
    for (i = 0; i <= loop_ub; i++) {
      b_y->data[i] = i;
    }
  }

  i = nm->size[0];
  nm->size[0] = b_y->size[1];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_y->data[i];
  }

  emxFree_real_T(&b_y);
  totElements += (double)nm->size[0];
  b_index /= maxRange;
  i = x_segments[358].f1->size[0];
  x_segments[358].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(x_segments[358].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    x_segments[358].f1->data[i] = x[358] + b_index * nm->data[i];
  }

  nx = x_segments[358].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    x_segments[358].f1->data[lastColumn] = rt_roundd_snf(x_segments[358]
      .f1->data[lastColumn]);
  }

  b_index = maxRange_tmp / maxRange;
  i = y_segments[358].f1->size[0];
  y_segments[358].f1->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(y_segments[358].f1, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    y_segments[358].f1->data[i] = y[358] + b_index * nm->data[i];
  }

  nx = y_segments[358].f1->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    y_segments[358].f1->data[lastColumn] = rt_roundd_snf(y_segments[358]
      .f1->data[lastColumn]);
  }

  /*  Concatenate segment vertices. */
  /*  x = cat(1,x_segments{:}); */
  /*  y = cat(1,y_segments{:}); */
  loop_ub = (int)totElements;
  i = nm->size[0];
  nm->size[0] = loop_ub;
  emxEnsureCapacity_real_T(nm, i);
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = 0.0;
  }

  emxInit_real_T(&c_y, 1);
  i = c_y->size[0];
  c_y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(c_y, i);
  for (i = 0; i < loop_ub; i++) {
    c_y->data[i] = 0.0;
  }

  b_index = 1.0;
  for (lastColumn = 0; lastColumn < 359; lastColumn++) {
    maxRange_tmp = (b_index + (double)x_segments[lastColumn].f1->size[0]) - 1.0;
    if (b_index > maxRange_tmp) {
      i = -1;
      i1 = 0;
    } else {
      i = (int)b_index - 2;
      i1 = (int)maxRange_tmp;
    }

    loop_ub = (i1 - i) - 1;
    for (i1 = 0; i1 < loop_ub; i1++) {
      nm->data[(i + i1) + 1] = x_segments[lastColumn].f1->data[i1];
    }

    maxRange_tmp = (b_index + (double)x_segments[lastColumn].f1->size[0]) - 1.0;
    if (b_index > maxRange_tmp) {
      i = -1;
      i1 = 0;
    } else {
      i = (int)b_index - 2;
      i1 = (int)maxRange_tmp;
    }

    loop_ub = (i1 - i) - 1;
    for (i1 = 0; i1 < loop_ub; i1++) {
      c_y->data[(i + i1) + 1] = y_segments[lastColumn].f1->data[i1];
    }

    b_index += (double)x_segments[lastColumn].f1->size[0];
  }

  emxFreeMatrix_cell_wrap_3(y_segments);
  emxFreeMatrix_cell_wrap_3(x_segments);
  emxInit_real_T(&d, 1);
  emxInit_int32_T(&edge_indices, 1);
  emxInit_int32_T(&b_nm, 1);

  /*  Horizontal edges are located where the x-value changes. */
  diff(nm, d);
  b_eml_find(d, b_nm);
  i = edge_indices->size[0];
  edge_indices->size[0] = b_nm->size[0];
  emxEnsureCapacity_int32_T(edge_indices, i);
  loop_ub = b_nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    edge_indices->data[i] = b_nm->data[i];
  }

  emxInit_real_T(&xe, 1);
  i = xe->size[0];
  xe->size[0] = edge_indices->size[0];
  emxEnsureCapacity_real_T(xe, i);
  loop_ub = edge_indices->size[0];
  for (i = 0; i < loop_ub; i++) {
    xe->data[i] = nm->data[edge_indices->data[i] - 1];
  }

  emxInit_boolean_T(&idx, 1);

  /*  Wherever the diff is negative, the x-coordinate should be x-1 instead of */
  /*  x. */
  i = idx->size[0];
  idx->size[0] = edge_indices->size[0];
  emxEnsureCapacity_boolean_T(idx, i);
  loop_ub = edge_indices->size[0];
  for (i = 0; i < loop_ub; i++) {
    idx->data[i] = (d->data[edge_indices->data[i] - 1] < 0.0);
  }

  eml_find(idx, b_nm);
  i = nm->size[0];
  nm->size[0] = b_nm->size[0];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_nm->data[i];
  }

  emxInit_real_T(&b_xe, 1);
  i = b_xe->size[0];
  b_xe->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(b_xe, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_xe->data[i] = xe->data[(int)nm->data[i] - 1] - 1.0;
  }

  loop_ub = b_xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    xe->data[(int)nm->data[i] - 1] = b_xe->data[i];
  }

  emxInit_real_T(&ye, 1);

  /*  In order for the result to be the same no matter which direction we are */
  /*  tracing the polynomial, the y-value for a diagonal transition has to be */
  /*  biased the same way no matter what.  We'll always chooser the smaller */
  /*  y-value associated with diagonal transitions. */
  i = ye->size[0];
  ye->size[0] = edge_indices->size[0];
  emxEnsureCapacity_real_T(ye, i);
  nx = edge_indices->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    ye->data[lastColumn] = fmin(c_y->data[edge_indices->data[lastColumn] - 1],
      c_y->data[edge_indices->data[lastColumn]]);
  }

  /*  BW = edgelist2mask(M,N,xe,ye); */
  /*  Scale x values, throwing away edgelist points that aren't on a pixel's */
  /*  center column. */
  loop_ub = xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    xe->data[i] = (xe->data[i] + 2.0) / 5.0;
  }

  i = nm->size[0];
  nm->size[0] = xe->size[0];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = xe->data[i];
  }

  nx = xe->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    nm->data[lastColumn] = floor(nm->data[lastColumn]);
  }

  i = idx->size[0];
  idx->size[0] = xe->size[0];
  emxEnsureCapacity_boolean_T(idx, i);
  loop_ub = xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    idx->data[i] = (xe->data[i] == nm->data[i]);
  }

  lastColumn = xe->size[0] - 1;
  trueCount = 0;
  for (nx = 0; nx <= lastColumn; nx++) {
    if (xe->data[nx] == nm->data[nx]) {
      trueCount++;
    }
  }

  varargin_1 = 0;
  for (nx = 0; nx <= lastColumn; nx++) {
    if (xe->data[nx] == nm->data[nx]) {
      xe->data[varargin_1] = xe->data[nx];
      varargin_1++;
    }
  }

  i = xe->size[0];
  xe->size[0] = trueCount;
  emxEnsureCapacity_real_T(xe, i);
  lastColumn = idx->size[0] - 1;
  b_varargin_1 = 0;
  varargin_1 = 0;
  for (nx = 0; nx <= lastColumn; nx++) {
    if (idx->data[nx]) {
      b_varargin_1++;
      ye->data[varargin_1] = ye->data[nx];
      varargin_1++;
    }
  }

  i = ye->size[0];
  ye->size[0] = b_varargin_1;
  emxEnsureCapacity_real_T(ye, i);

  /*  Scale y values. */
  for (i = 0; i < b_varargin_1; i++) {
    ye->data[i] = (ye->data[i] + 2.0) / 5.0;
  }

  nx = ye->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    ye->data[lastColumn] = ceil(ye->data[lastColumn]);
  }

  /*  Throw away horizontal edges that are too far left, too far right, or below the image. */
  i = idx->size[0];
  idx->size[0] = trueCount;
  emxEnsureCapacity_boolean_T(idx, i);
  for (i = 0; i < trueCount; i++) {
    idx->data[i] = ((xe->data[i] < 1.0) || (xe->data[i] > N) || (ye->data[i] > M));
  }

  eml_find(idx, b_nm);
  i = nm->size[0];
  nm->size[0] = b_nm->size[0];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_nm->size[0];
  emxFree_boolean_T(&idx);
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_nm->data[i];
  }

  i = b_nm->size[0];
  b_nm->size[0] = nm->size[0];
  emxEnsureCapacity_int32_T(b_nm, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_nm->data[i] = (int)nm->data[i];
  }

  nullAssignment(xe, b_nm);
  i = c_y->size[0];
  c_y->size[0] = ye->size[0];
  emxEnsureCapacity_real_T(c_y, i);
  loop_ub = ye->size[0];
  for (i = 0; i < loop_ub; i++) {
    c_y->data[i] = ye->data[i];
  }

  i = b_nm->size[0];
  b_nm->size[0] = nm->size[0];
  emxEnsureCapacity_int32_T(b_nm, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_nm->data[i] = (int)nm->data[i];
  }

  emxFree_real_T(&nm);
  nullAssignment(c_y, b_nm);

  /*  Treat horizontal edges above the top of the image as they are along the */
  /*  upper edge. */
  i = ye->size[0];
  ye->size[0] = c_y->size[0];
  emxEnsureCapacity_real_T(ye, i);
  nx = c_y->size[0];
  for (lastColumn = 0; lastColumn < nx; lastColumn++) {
    ye->data[lastColumn] = fmax(1.0, c_y->data[lastColumn]);
  }

  emxFree_real_T(&c_y);
  c_emxInitStruct_coder_internal_(&expl_temp);

  /*  Insert the edge list locations into a sparse matrix, taking */
  /*  advantage of the accumulation behavior of the SPARSE function. */
  sparse(ye, xe, M, N, &expl_temp);
  i = d->size[0];
  d->size[0] = expl_temp.d->size[0];
  emxEnsureCapacity_real_T(d, i);
  loop_ub = expl_temp.d->size[0];
  emxFree_real_T(&ye);
  emxFree_real_T(&xe);
  for (i = 0; i < loop_ub; i++) {
    d->data[i] = expl_temp.d->data[i];
  }

  i = b_nm->size[0];
  b_nm->size[0] = expl_temp.colidx->size[0];
  emxEnsureCapacity_int32_T(b_nm, i);
  loop_ub = expl_temp.colidx->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_nm->data[i] = expl_temp.colidx->data[i];
  }

  i = edge_indices->size[0];
  edge_indices->size[0] = expl_temp.rowidx->size[0];
  emxEnsureCapacity_int32_T(edge_indices, i);
  loop_ub = expl_temp.rowidx->size[0];
  for (i = 0; i < loop_ub; i++) {
    edge_indices->data[i] = expl_temp.rowidx->data[i];
  }

  trueCount = expl_temp.m;
  S_n = expl_temp.n;

  /*  We reduce the memory consumption of edgelist2mask by processing only a */
  /*  group of columns at a time (g274577); this does not compromise speed. */
  i = BW->size[0] * BW->size[1];
  BW->size[0] = expl_temp.m;
  BW->size[1] = expl_temp.n;
  emxEnsureCapacity_boolean_T(BW, i);
  loop_ub = expl_temp.m * expl_temp.n;
  for (i = 0; i < loop_ub; i++) {
    BW->data[i] = false;
  }

  /*  for m = 1:columnChunk:numCols */
  /*    firstColumn = m; */
  /*    lastColumn = min(m + columnChunk - 1, numCols); */
  /*    columns = full(S(:, firstColumn:lastColumn)); */
  /*    BW1(:, firstColumn:lastColumn) = builtin('_parityscan', columns); % not supported for matlab coder */
  /*  end */
  i = (int)(((double)expl_temp.n + 49.0) / 50.0);
  emxInit_real_T(&columns, 2);
  emxInit_int32_T(&t1_colidx, 1);
  emxInit_int32_T(&t1_rowidx, 1);
  emxInit_real_T(&r, 2);
  for (m = 0; m < i; m++) {
    b_m = m * 50U + 1U;
    lastColumn = (int)fmin(((double)b_m + 50.0) - 1.0, S_n);
    if ((double)lastColumn < b_m) {
      y_size[0] = 1;
      y_size[1] = 0;
    } else {
      y_size[0] = 1;
      loop_ub = lastColumn - (int)b_m;
      y_size[1] = loop_ub + 1;
      for (i1 = 0; i1 <= loop_ub; i1++) {
        y_data[i1] = b_m + i1;
      }
    }

    sparse_parenReference(d, b_nm, edge_indices, trueCount, y_data, y_size,
                          &expl_temp);
    i1 = b_xe->size[0];
    b_xe->size[0] = expl_temp.d->size[0];
    emxEnsureCapacity_real_T(b_xe, i1);
    loop_ub = expl_temp.d->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_xe->data[i1] = expl_temp.d->data[i1];
    }

    i1 = t1_colidx->size[0];
    t1_colidx->size[0] = expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(t1_colidx, i1);
    loop_ub = expl_temp.colidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      t1_colidx->data[i1] = expl_temp.colidx->data[i1];
    }

    i1 = t1_rowidx->size[0];
    t1_rowidx->size[0] = expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(t1_rowidx, i1);
    loop_ub = expl_temp.rowidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      t1_rowidx->data[i1] = expl_temp.rowidx->data[i1];
    }

    sparse_full(b_xe, t1_colidx, t1_rowidx, expl_temp.m, expl_temp.n, columns);
    if ((double)b_m > lastColumn) {
      i1 = 1;
    } else {
      i1 = (int)b_m;
    }

    useConstantDim(columns);
    b_mod(columns, r);
    loop_ub = r->size[1];
    for (nx = 0; nx < loop_ub; nx++) {
      varargin_1 = r->size[0];
      for (b_varargin_1 = 0; b_varargin_1 < varargin_1; b_varargin_1++) {
        BW->data[b_varargin_1 + BW->size[0] * ((i1 + nx) - 1)] = (r->
          data[b_varargin_1 + r->size[0] * nx] != 0.0);
      }
    }
  }

  emxFree_int32_T(&b_nm);
  emxFree_real_T(&b_xe);
  c_emxFreeStruct_coder_internal_(&expl_temp);
  emxFree_real_T(&r);
  emxFree_int32_T(&t1_rowidx);
  emxFree_int32_T(&t1_colidx);
  emxFree_real_T(&columns);
  emxFree_int32_T(&edge_indices);
  emxFree_real_T(&d);
}

void poly2mask(emxArray_real_T *x, emxArray_real_T *y, double M, double N,
               emxArray_boolean_T *BW)
{
  int loop_ub;
  int i;
  int nx;
  int k;
  emxArray_cell_wrap_3_360 x_segments;
  emxArray_cell_wrap_3_360 y_segments;
  int n;
  double totElements;
  int i1;
  emxArray_real_T *nm;
  emxArray_real_T *b_y;
  double b_index;
  double maxRange_tmp;
  double maxRange;
  int lastColumn;
  emxArray_real_T *edge_indices;
  emxArray_int32_T *b_nm;
  emxArray_real_T *xe;
  emxArray_boolean_T *idx;
  emxArray_real_T *b_xe;
  emxArray_real_T *ye;
  int trueCount;
  coder_internal_sparse expl_temp;
  emxArray_int32_T *S_rowidx;
  emxArray_real_T *columns;
  emxArray_int32_T *t0_colidx;
  emxArray_int32_T *t0_rowidx;
  emxArray_real_T *r;
  int m;
  unsigned int b_m;
  int y_size[2];
  double y_data[512];

  /*  Modified version of original version of poly2mask to enable mex compilation 01/20/22 DZ */
  /* POLY2MASK Convert region-of-interest polygon to mask. */
  /*    BW = POLY2MASK(X,Y,M,N) computes a binary region-of-interest mask, */
  /*    BW, from a region-of-interest polygon, represented by the vectors X */
  /*    and Y.  The size of BW is M-by-N.  Pixels in BW that are inside */
  /*    the polygon (X,Y) are 1; pixels outside the polygon are 0.  The class */
  /*    of BW is logical. */
  /*  */
  /*    POLY2MASK closes the polygon automatically if it isn't already */
  /*    closed. */
  /*  */
  /*    Example 1: */
  /*    --------- */
  /*        x = [63 186 54 190 63]; */
  /*        y = [60 60 209 204 60]; */
  /*        bw = poly2mask(x,y,256,256); */
  /*        imshow(bw) */
  /*        hold on */
  /*        plot(x,y,'b','LineWidth',2) */
  /*        hold off */
  /*  */
  /*    Example 2: */
  /*    --------- */
  /*   % Or using random points: */
  /*  */
  /*        x = 256*rand(1,4); */
  /*        y = 256*rand(1,4); */
  /*        x(end+1) = x(1); */
  /*        y(end+1) = y(1); */
  /*        bw = poly2mask(x,y,256,256); */
  /*        imshow(bw) */
  /*        hold on */
  /*        plot(x,y,'b','LineWidth',2) */
  /*        hold off */
  /*  */
  /*    See also ROIPOLY. */
  /*    Copyright 1993-2018 The MathWorks, Inc. */
  /*  if (x(end) ~= x(1)) || (y(end) ~= y(1)) % moved to computeDFF_new_coders */
  /*      x(end+1) = x(1); */
  /*      y(end+1) = y(1); */
  /*  end */
  /*  [xe,ye] = poly2edgelist(x,y); */
  /*  default value */
  /*  Scale and quantize (x,y) locations to the higher resolution grid. */
  loop_ub = x->size[0];
  for (i = 0; i < loop_ub; i++) {
    x->data[i] = 5.0 * (x->data[i] - 0.5) + 1.0;
  }

  nx = x->size[0];
  for (k = 0; k < nx; k++) {
    x->data[k] = rt_roundd_snf(x->data[k]);
  }

  loop_ub = y->size[0];
  for (i = 0; i < loop_ub; i++) {
    y->data[i] = 5.0 * (y->data[i] - 0.5) + 1.0;
  }

  nx = y->size[0];
  for (k = 0; k < nx; k++) {
    y->data[k] = rt_roundd_snf(y->data[k]);
  }

  emxInit_cell_wrap_3_360(&x_segments);
  emxInit_cell_wrap_3_360(&y_segments);
  n = x->size[0] - 2;
  totElements = 0.0;
  i = x->size[0];
  i1 = x_segments.size[0];
  x_segments.size[0] = (short)(x->size[0] - 1);
  emxEnsureCapacity_cell_wrap_3(x_segments.data, x_segments.size, i1);
  i1 = y_segments.size[0];
  y_segments.size[0] = (short)(x->size[0] - 1);
  emxEnsureCapacity_cell_wrap_3(y_segments.data, y_segments.size, i1);
  emxInit_real_T(&nm, 1);
  emxInit_real_T(&b_y, 2);
  for (k = 0; k <= i - 2; k++) {
    /* [x_segments{k},y_segments{k}] = iptui.intline(x(k),x(k+1),y(k),y(k+1)); */
    /*  iptui.intline does not support Matlab coder */
    b_index = x->data[k + 1] - x->data[k];
    maxRange_tmp = y->data[k + 1] - y->data[k];
    maxRange = fmax(fabs(b_index), fabs(maxRange_tmp));
    if (rtIsNaN(maxRange)) {
      i1 = b_y->size[0] * b_y->size[1];
      b_y->size[0] = 1;
      b_y->size[1] = 1;
      emxEnsureCapacity_real_T(b_y, i1);
      b_y->data[0] = rtNaN;
    } else if (rtIsInf(maxRange) && (0.0 == maxRange)) {
      i1 = b_y->size[0] * b_y->size[1];
      b_y->size[0] = 1;
      b_y->size[1] = 1;
      emxEnsureCapacity_real_T(b_y, i1);
      b_y->data[0] = rtNaN;
    } else {
      i1 = b_y->size[0] * b_y->size[1];
      b_y->size[0] = 1;
      loop_ub = (int)floor(maxRange);
      b_y->size[1] = loop_ub + 1;
      emxEnsureCapacity_real_T(b_y, i1);
      for (i1 = 0; i1 <= loop_ub; i1++) {
        b_y->data[i1] = i1;
      }
    }

    i1 = nm->size[0];
    nm->size[0] = b_y->size[1];
    emxEnsureCapacity_real_T(nm, i1);
    loop_ub = b_y->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      nm->data[i1] = b_y->data[i1];
    }

    loop_ub = nm->size[0];
    totElements += (double)nm->size[0];
    b_index /= maxRange;
    i1 = x_segments.data[k].f1->size[0];
    x_segments.data[k].f1->size[0] = nm->size[0];
    emxEnsureCapacity_real_T(x_segments.data[k].f1, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      x_segments.data[k].f1->data[i1] = x->data[k] + b_index * nm->data[i1];
    }

    nx = x_segments.data[k].f1->size[0];
    for (lastColumn = 0; lastColumn < nx; lastColumn++) {
      x_segments.data[k].f1->data[lastColumn] = rt_roundd_snf(x_segments.data[k]
        .f1->data[lastColumn]);
    }

    b_index = maxRange_tmp / maxRange;
    loop_ub = nm->size[0];
    i1 = y_segments.data[k].f1->size[0];
    y_segments.data[k].f1->size[0] = nm->size[0];
    emxEnsureCapacity_real_T(y_segments.data[k].f1, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      y_segments.data[k].f1->data[i1] = y->data[k] + b_index * nm->data[i1];
    }

    nx = y_segments.data[k].f1->size[0];
    for (lastColumn = 0; lastColumn < nx; lastColumn++) {
      y_segments.data[k].f1->data[lastColumn] = rt_roundd_snf(y_segments.data[k]
        .f1->data[lastColumn]);
    }
  }

  emxFree_real_T(&b_y);

  /*  Concatenate segment vertices. */
  /*  x = cat(1,x_segments{:}); */
  /*  y = cat(1,y_segments{:}); */
  loop_ub = (int)totElements;
  i = x->size[0];
  x->size[0] = loop_ub;
  emxEnsureCapacity_real_T(x, i);
  for (i = 0; i < loop_ub; i++) {
    x->data[i] = 0.0;
  }

  i = y->size[0];
  y->size[0] = loop_ub;
  emxEnsureCapacity_real_T(y, i);
  for (i = 0; i < loop_ub; i++) {
    y->data[i] = 0.0;
  }

  b_index = 1.0;
  for (lastColumn = 0; lastColumn <= n; lastColumn++) {
    totElements = (b_index + (double)x_segments.data[lastColumn].f1->size[0]) -
      1.0;
    if (b_index > totElements) {
      i = -1;
      i1 = 0;
    } else {
      i = (int)b_index - 2;
      i1 = (int)totElements;
    }

    loop_ub = (i1 - i) - 1;
    for (i1 = 0; i1 < loop_ub; i1++) {
      x->data[(i + i1) + 1] = x_segments.data[lastColumn].f1->data[i1];
    }

    totElements = (b_index + (double)x_segments.data[lastColumn].f1->size[0]) -
      1.0;
    if (b_index > totElements) {
      i = -1;
      i1 = 0;
    } else {
      i = (int)b_index - 2;
      i1 = (int)totElements;
    }

    loop_ub = (i1 - i) - 1;
    for (i1 = 0; i1 < loop_ub; i1++) {
      y->data[(i + i1) + 1] = y_segments.data[lastColumn].f1->data[i1];
    }

    b_index += (double)x_segments.data[lastColumn].f1->size[0];
  }

  emxFree_cell_wrap_3_360(&y_segments);
  emxFree_cell_wrap_3_360(&x_segments);
  emxInit_real_T(&edge_indices, 1);
  emxInit_int32_T(&b_nm, 1);

  /*  Horizontal edges are located where the x-value changes. */
  diff(x, nm);
  b_eml_find(nm, b_nm);
  i = edge_indices->size[0];
  edge_indices->size[0] = b_nm->size[0];
  emxEnsureCapacity_real_T(edge_indices, i);
  loop_ub = b_nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    edge_indices->data[i] = b_nm->data[i];
  }

  emxInit_real_T(&xe, 1);
  i = xe->size[0];
  xe->size[0] = edge_indices->size[0];
  emxEnsureCapacity_real_T(xe, i);
  loop_ub = edge_indices->size[0];
  for (i = 0; i < loop_ub; i++) {
    xe->data[i] = x->data[(int)edge_indices->data[i] - 1];
  }

  emxInit_boolean_T(&idx, 1);

  /*  Wherever the diff is negative, the x-coordinate should be x-1 instead of */
  /*  x. */
  i = idx->size[0];
  idx->size[0] = edge_indices->size[0];
  emxEnsureCapacity_boolean_T(idx, i);
  loop_ub = edge_indices->size[0];
  for (i = 0; i < loop_ub; i++) {
    idx->data[i] = (nm->data[(int)edge_indices->data[i] - 1] < 0.0);
  }

  eml_find(idx, b_nm);
  i = nm->size[0];
  nm->size[0] = b_nm->size[0];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_nm->data[i];
  }

  emxInit_real_T(&b_xe, 1);
  i = b_xe->size[0];
  b_xe->size[0] = nm->size[0];
  emxEnsureCapacity_real_T(b_xe, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_xe->data[i] = xe->data[(int)nm->data[i] - 1] - 1.0;
  }

  loop_ub = b_xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    xe->data[(int)nm->data[i] - 1] = b_xe->data[i];
  }

  emxFree_real_T(&b_xe);
  emxInit_real_T(&ye, 1);

  /*  In order for the result to be the same no matter which direction we are */
  /*  tracing the polynomial, the y-value for a diagonal transition has to be */
  /*  biased the same way no matter what.  We'll always chooser the smaller */
  /*  y-value associated with diagonal transitions. */
  i = ye->size[0];
  ye->size[0] = edge_indices->size[0];
  emxEnsureCapacity_real_T(ye, i);
  nx = edge_indices->size[0];
  for (k = 0; k < nx; k++) {
    lastColumn = (int)edge_indices->data[k];
    ye->data[k] = fmin(y->data[lastColumn - 1], y->data[lastColumn]);
  }

  /*  BW = edgelist2mask(M,N,xe,ye); */
  /*  Scale x values, throwing away edgelist points that aren't on a pixel's */
  /*  center column. */
  loop_ub = xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    xe->data[i] = (xe->data[i] + 2.0) / 5.0;
  }

  i = nm->size[0];
  nm->size[0] = xe->size[0];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = xe->data[i];
  }

  nx = xe->size[0];
  for (k = 0; k < nx; k++) {
    nm->data[k] = floor(nm->data[k]);
  }

  i = idx->size[0];
  idx->size[0] = xe->size[0];
  emxEnsureCapacity_boolean_T(idx, i);
  loop_ub = xe->size[0];
  for (i = 0; i < loop_ub; i++) {
    idx->data[i] = (xe->data[i] == nm->data[i]);
  }

  lastColumn = xe->size[0] - 1;
  trueCount = 0;
  for (nx = 0; nx <= lastColumn; nx++) {
    if (xe->data[nx] == nm->data[nx]) {
      trueCount++;
    }
  }

  n = 0;
  for (nx = 0; nx <= lastColumn; nx++) {
    if (xe->data[nx] == nm->data[nx]) {
      xe->data[n] = xe->data[nx];
      n++;
    }
  }

  i = xe->size[0];
  xe->size[0] = trueCount;
  emxEnsureCapacity_real_T(xe, i);
  lastColumn = idx->size[0] - 1;
  k = 0;
  n = 0;
  for (nx = 0; nx <= lastColumn; nx++) {
    if (idx->data[nx]) {
      k++;
      ye->data[n] = ye->data[nx];
      n++;
    }
  }

  i = ye->size[0];
  ye->size[0] = k;
  emxEnsureCapacity_real_T(ye, i);

  /*  Scale y values. */
  for (i = 0; i < k; i++) {
    ye->data[i] = (ye->data[i] + 2.0) / 5.0;
  }

  nx = ye->size[0];
  for (k = 0; k < nx; k++) {
    ye->data[k] = ceil(ye->data[k]);
  }

  /*  Throw away horizontal edges that are too far left, too far right, or below the image. */
  i = idx->size[0];
  idx->size[0] = trueCount;
  emxEnsureCapacity_boolean_T(idx, i);
  for (i = 0; i < trueCount; i++) {
    idx->data[i] = ((xe->data[i] < 1.0) || (xe->data[i] > N) || (ye->data[i] > M));
  }

  eml_find(idx, b_nm);
  i = nm->size[0];
  nm->size[0] = b_nm->size[0];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = b_nm->size[0];
  emxFree_boolean_T(&idx);
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = b_nm->data[i];
  }

  i = b_nm->size[0];
  b_nm->size[0] = nm->size[0];
  emxEnsureCapacity_int32_T(b_nm, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_nm->data[i] = (int)nm->data[i];
  }

  nullAssignment(xe, b_nm);
  i = edge_indices->size[0];
  edge_indices->size[0] = ye->size[0];
  emxEnsureCapacity_real_T(edge_indices, i);
  loop_ub = ye->size[0];
  for (i = 0; i < loop_ub; i++) {
    edge_indices->data[i] = ye->data[i];
  }

  i = b_nm->size[0];
  b_nm->size[0] = nm->size[0];
  emxEnsureCapacity_int32_T(b_nm, i);
  loop_ub = nm->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_nm->data[i] = (int)nm->data[i];
  }

  nullAssignment(edge_indices, b_nm);

  /*  Treat horizontal edges above the top of the image as they are along the */
  /*  upper edge. */
  i = ye->size[0];
  ye->size[0] = edge_indices->size[0];
  emxEnsureCapacity_real_T(ye, i);
  nx = edge_indices->size[0];
  for (k = 0; k < nx; k++) {
    ye->data[k] = fmax(1.0, edge_indices->data[k]);
  }

  c_emxInitStruct_coder_internal_(&expl_temp);

  /*  Insert the edge list locations into a sparse matrix, taking */
  /*  advantage of the accumulation behavior of the SPARSE function. */
  sparse(ye, xe, M, N, &expl_temp);
  i = nm->size[0];
  nm->size[0] = expl_temp.d->size[0];
  emxEnsureCapacity_real_T(nm, i);
  loop_ub = expl_temp.d->size[0];
  emxFree_real_T(&ye);
  emxFree_real_T(&xe);
  for (i = 0; i < loop_ub; i++) {
    nm->data[i] = expl_temp.d->data[i];
  }

  i = b_nm->size[0];
  b_nm->size[0] = expl_temp.colidx->size[0];
  emxEnsureCapacity_int32_T(b_nm, i);
  loop_ub = expl_temp.colidx->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_nm->data[i] = expl_temp.colidx->data[i];
  }

  emxInit_int32_T(&S_rowidx, 1);
  i = S_rowidx->size[0];
  S_rowidx->size[0] = expl_temp.rowidx->size[0];
  emxEnsureCapacity_int32_T(S_rowidx, i);
  loop_ub = expl_temp.rowidx->size[0];
  for (i = 0; i < loop_ub; i++) {
    S_rowidx->data[i] = expl_temp.rowidx->data[i];
  }

  trueCount = expl_temp.m;
  k = expl_temp.n;

  /*  We reduce the memory consumption of edgelist2mask by processing only a */
  /*  group of columns at a time (g274577); this does not compromise speed. */
  i = BW->size[0] * BW->size[1];
  BW->size[0] = expl_temp.m;
  BW->size[1] = expl_temp.n;
  emxEnsureCapacity_boolean_T(BW, i);
  loop_ub = expl_temp.m * expl_temp.n;
  for (i = 0; i < loop_ub; i++) {
    BW->data[i] = false;
  }

  /*  for m = 1:columnChunk:numCols */
  /*    firstColumn = m; */
  /*    lastColumn = min(m + columnChunk - 1, numCols); */
  /*    columns = full(S(:, firstColumn:lastColumn)); */
  /*    BW1(:, firstColumn:lastColumn) = builtin('_parityscan', columns); % not supported for matlab coder */
  /*  end */
  i = (int)(((double)expl_temp.n + 49.0) / 50.0);
  emxInit_real_T(&columns, 2);
  emxInit_int32_T(&t0_colidx, 1);
  emxInit_int32_T(&t0_rowidx, 1);
  emxInit_real_T(&r, 2);
  for (m = 0; m < i; m++) {
    b_m = m * 50U + 1U;
    lastColumn = (int)fmin(((double)b_m + 50.0) - 1.0, k);
    if ((double)lastColumn < b_m) {
      y_size[0] = 1;
      y_size[1] = 0;
    } else {
      y_size[0] = 1;
      loop_ub = lastColumn - (int)b_m;
      y_size[1] = loop_ub + 1;
      for (i1 = 0; i1 <= loop_ub; i1++) {
        y_data[i1] = b_m + i1;
      }
    }

    sparse_parenReference(nm, b_nm, S_rowidx, trueCount, y_data, y_size,
                          &expl_temp);
    i1 = edge_indices->size[0];
    edge_indices->size[0] = expl_temp.d->size[0];
    emxEnsureCapacity_real_T(edge_indices, i1);
    loop_ub = expl_temp.d->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      edge_indices->data[i1] = expl_temp.d->data[i1];
    }

    i1 = t0_colidx->size[0];
    t0_colidx->size[0] = expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(t0_colidx, i1);
    loop_ub = expl_temp.colidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      t0_colidx->data[i1] = expl_temp.colidx->data[i1];
    }

    i1 = t0_rowidx->size[0];
    t0_rowidx->size[0] = expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(t0_rowidx, i1);
    loop_ub = expl_temp.rowidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      t0_rowidx->data[i1] = expl_temp.rowidx->data[i1];
    }

    sparse_full(edge_indices, t0_colidx, t0_rowidx, expl_temp.m, expl_temp.n,
                columns);
    if ((double)b_m > lastColumn) {
      i1 = 1;
    } else {
      i1 = (int)b_m;
    }

    useConstantDim(columns);
    b_mod(columns, r);
    loop_ub = r->size[1];
    for (nx = 0; nx < loop_ub; nx++) {
      lastColumn = r->size[0];
      for (n = 0; n < lastColumn; n++) {
        BW->data[n + BW->size[0] * ((i1 + nx) - 1)] = (r->data[n + r->size[0] *
          nx] != 0.0);
      }
    }
  }

  emxFree_int32_T(&b_nm);
  c_emxFreeStruct_coder_internal_(&expl_temp);
  emxFree_real_T(&r);
  emxFree_int32_T(&t0_rowidx);
  emxFree_int32_T(&t0_colidx);
  emxFree_real_T(&columns);
  emxFree_int32_T(&S_rowidx);
  emxFree_real_T(&edge_indices);
  emxFree_real_T(&nm);
}

/* End of code generation (poly2mask.c) */
