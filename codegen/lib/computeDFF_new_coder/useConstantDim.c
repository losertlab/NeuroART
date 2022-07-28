/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * useConstantDim.c
 *
 * Code generation for function 'useConstantDim'
 *
 */

/* Include files */
#include "useConstantDim.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void useConstantDim(emxArray_real_T *varargin_2)
{
  int i;
  int k;
  int i1;
  short subsb_idx_1;
  int b_k;
  int i2;
  int i3;
  if ((varargin_2->size[0] != 0) && (varargin_2->size[1] != 0)) {
    i = varargin_2->size[1];
    for (k = 0; k < i; k++) {
      i1 = varargin_2->size[0];
      if (0 <= i1 - 2) {
        subsb_idx_1 = (short)(k + 1);
      }

      for (b_k = 0; b_k <= i1 - 2; b_k++) {
        i2 = (short)((short)(b_k + 1) + 1) - 1;
        i3 = subsb_idx_1 - 1;
        varargin_2->data[i2 + varargin_2->size[0] * i3] += varargin_2->data[b_k
          + varargin_2->size[0] * k];
      }
    }
  }
}

/* End of code generation (useConstantDim.c) */
