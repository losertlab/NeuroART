/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * nanmean.c
 *
 * Code generation for function 'nanmean'
 *
 */

/* Include files */
#include "nanmean.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void nanmean(const emxArray_real_T *varargin_1, double y_data[], int y_size[2])
{
  unsigned int sz_idx_1;
  int vlen;
  int npages;
  int ixstart;
  int ix;
  int n;
  int iy;
  int i;
  double s;
  sz_idx_1 = (unsigned int)varargin_1->size[1];
  y_size[0] = 1;
  y_size[1] = (int)sz_idx_1;
  if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
    y_size[0] = 1;
    ixstart = (int)sz_idx_1;
    for (n = 0; n < ixstart; n++) {
      y_data[n] = rtNaN;
    }
  } else {
    vlen = varargin_1->size[0];
    npages = varargin_1->size[1];
    ix = -1;
    iy = -1;
    for (i = 0; i < npages; i++) {
      ixstart = ix + 1;
      ix++;
      if (!rtIsNaN(varargin_1->data[ixstart])) {
        s = varargin_1->data[ixstart];
        n = 1;
      } else {
        s = 0.0;
        n = 0;
      }

      for (ixstart = 2; ixstart <= vlen; ixstart++) {
        ix++;
        if (!rtIsNaN(varargin_1->data[ix])) {
          s += varargin_1->data[ix];
          n++;
        }
      }

      if (n == 0) {
        s = rtNaN;
      } else {
        s /= (double)n;
      }

      iy++;
      y_data[iy] = s;
    }
  }
}

/* End of code generation (nanmean.c) */
