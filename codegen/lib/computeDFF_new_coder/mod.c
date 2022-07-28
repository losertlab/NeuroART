/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mod.c
 *
 * Code generation for function 'mod'
 *
 */

/* Include files */
#include "mod.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void b_mod(const emxArray_real_T *x, emxArray_real_T *r)
{
  short unnamed_idx_0;
  short unnamed_idx_1;
  int nx;
  int k;
  double b_r;
  unnamed_idx_0 = (short)x->size[0];
  unnamed_idx_1 = (short)x->size[1];
  nx = r->size[0] * r->size[1];
  r->size[0] = unnamed_idx_0;
  r->size[1] = unnamed_idx_1;
  emxEnsureCapacity_real_T(r, nx);
  nx = unnamed_idx_0 * unnamed_idx_1;
  for (k = 0; k < nx; k++) {
    if (rtIsNaN(x->data[k]) || rtIsInf(x->data[k])) {
      b_r = rtNaN;
    } else if (x->data[k] == 0.0) {
      b_r = 0.0;
    } else {
      b_r = fmod(x->data[k], 2.0);
      if (b_r == 0.0) {
        b_r = 0.0;
      }
    }

    r->data[k] = b_r;
  }
}

/* End of code generation (mod.c) */
