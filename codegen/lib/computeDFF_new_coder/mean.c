/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mean.c
 *
 * Code generation for function 'mean'
 *
 */

/* Include files */
#include "mean.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void mean(const emxArray_uint16_T *x, emxArray_real_T *y)
{
  int vlen;
  int k;
  unsigned int sz_idx_0;
  unsigned int sz_idx_1;
  int j;
  int vstride;
  int xoffset;
  vlen = x->size[2];
  if ((x->size[0] == 0) || (x->size[1] == 0) || (x->size[2] == 0)) {
    sz_idx_0 = (unsigned int)x->size[0];
    sz_idx_1 = (unsigned int)x->size[1];
    j = y->size[0] * y->size[1];
    y->size[0] = (int)sz_idx_0;
    y->size[1] = (int)sz_idx_1;
    emxEnsureCapacity_real_T(y, j);
    k = (int)sz_idx_0 * (int)sz_idx_1;
    for (j = 0; j < k; j++) {
      y->data[j] = 0.0;
    }
  } else {
    k = 3;
    if (x->size[2] == 1) {
      k = 2;
    }

    if (3 > k) {
      vstride = x->size[0] * x->size[1] * x->size[2];
    } else {
      vstride = x->size[0] * x->size[1];
    }

    j = y->size[0] * y->size[1];
    y->size[0] = x->size[0];
    y->size[1] = x->size[1];
    emxEnsureCapacity_real_T(y, j);
    for (j = 0; j < vstride; j++) {
      y->data[j] = x->data[j];
    }

    for (k = 2; k <= vlen; k++) {
      xoffset = (k - 1) * vstride;
      for (j = 0; j < vstride; j++) {
        y->data[j] += (double)x->data[xoffset + j];
      }
    }
  }

  k = y->size[0] * y->size[1];
  for (j = 0; j < k; j++) {
    y->data[j] /= (double)x->size[2];
  }
}

/* End of code generation (mean.c) */
