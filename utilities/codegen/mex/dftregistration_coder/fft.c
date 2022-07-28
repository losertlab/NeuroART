/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * fft.c
 *
 * Code generation for function 'fft'
 *
 */

/* Include files */
#include "fft.h"
#include "FFTWApi.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_data.h"
#include "dftregistration_coder_emxutil.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void fft(const emlrtStack *sp, const emxArray_creal_T *x, int32_T n,
         emxArray_creal_T *y)
{
  int32_T i;
  int32_T loop_ub;
  int32_T b_loop_ub;
  int32_T i1;
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;
  if ((x->size[0] == 0) || (x->size[1] == 0) || (n == 0)) {
    i = y->size[0] * y->size[1];
    y->size[0] = n;
    y->size[1] = (int16_T)x->size[1];
    emxEnsureCapacity_creal_T(sp, y, i, &vb_emlrtRTEI);
    if (n > x->size[0]) {
      loop_ub = (int16_T)x->size[1];
      for (i = 0; i < loop_ub; i++) {
        b_loop_ub = y->size[0];
        for (i1 = 0; i1 < b_loop_ub; i1++) {
          y->data[i1 + y->size[0] * i].re = 0.0;
          y->data[i1 + y->size[0] * i].im = 0.0;
        }
      }
    }
  } else {
    st.site = &qb_emlrtRSI;
    FFTWApi_fft1d(&st, x, n, (int16_T)x->size[1], y);
  }
}

/* End of code generation (fft.c) */
