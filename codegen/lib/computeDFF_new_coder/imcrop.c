/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * imcrop.c
 *
 * Code generation for function 'imcrop'
 *
 */

/* Include files */
#include "imcrop.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "computeDFF_new_coder_rtwutil.h"
#include "poly2mask.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void imcrop(const emxArray_real_T *varargin_1, const double varargin_2[4],
            emxArray_real_T *varargout_1)
{
  int xmax;
  int xmin;
  int ymax;
  int ymin;
  double pixelsPerVerticalUnit;
  double pixelsPerHorizontalUnit;
  double r1;
  int colIdx2;
  int i;
  int i1;
  xmax = 1;
  xmin = 1;
  if ((short)varargin_1->size[1] > 1) {
    xmax = (short)varargin_1->size[1];
  }

  if ((short)varargin_1->size[1] < 1) {
    xmin = 0;
  }

  ymax = 1;
  ymin = 1;
  if ((short)varargin_1->size[0] > 1) {
    ymax = (short)varargin_1->size[0];
  }

  if ((short)varargin_1->size[0] < 1) {
    ymin = 0;
  }

  if (varargin_1->size[0] == 1) {
    pixelsPerVerticalUnit = 1.0;
  } else {
    pixelsPerVerticalUnit = ((double)varargin_1->size[0] - 1.0) / (double)(ymax
      - ymin);
  }

  if (varargin_1->size[1] == 1) {
    pixelsPerHorizontalUnit = 1.0;
  } else {
    pixelsPerHorizontalUnit = ((double)varargin_1->size[1] - 1.0) / (double)
      (xmax - xmin);
  }

  r1 = (varargin_2[1] - (double)ymin) * pixelsPerVerticalUnit + 1.0;
  ymin = (int)rt_roundd_snf(r1 + 31.0 * pixelsPerVerticalUnit);
  xmax = (int)rt_roundd_snf(r1);
  pixelsPerVerticalUnit = (varargin_2[0] - (double)xmin) *
    pixelsPerHorizontalUnit + 1.0;
  colIdx2 = (int)rt_roundd_snf(pixelsPerVerticalUnit + 31.0 *
    pixelsPerHorizontalUnit);
  ymax = (int)rt_roundd_snf(pixelsPerVerticalUnit);
  if ((xmax > varargin_1->size[0]) || (ymin < 1) || (ymax > varargin_1->size[1])
      || (colIdx2 < 1)) {
    varargout_1->size[0] = 0;
    varargout_1->size[1] = 0;
  } else {
    if (xmax < 1) {
      xmax = 1;
    }

    if (ymin > varargin_1->size[0]) {
      ymin = varargin_1->size[0];
    }

    if (ymax < 1) {
      ymax = 1;
    }

    if (colIdx2 > varargin_1->size[1]) {
      colIdx2 = varargin_1->size[1];
    }

    if (xmax > ymin) {
      i = 0;
      ymin = 0;
    } else {
      i = xmax - 1;
    }

    if (ymax > colIdx2) {
      i1 = 0;
      colIdx2 = 0;
    } else {
      i1 = ymax - 1;
    }

    ymin -= i;
    xmin = varargout_1->size[0] * varargout_1->size[1];
    varargout_1->size[0] = ymin;
    xmax = colIdx2 - i1;
    varargout_1->size[1] = xmax;
    emxEnsureCapacity_real_T(varargout_1, xmin);
    for (xmin = 0; xmin < xmax; xmin++) {
      for (ymax = 0; ymax < ymin; ymax++) {
        varargout_1->data[ymax + varargout_1->size[0] * xmin] = varargin_1->
          data[(i + ymax) + varargin_1->size[0] * (i1 + xmin)];
      }
    }
  }
}

/* End of code generation (imcrop.c) */
