/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sum.c
 *
 * Code generation for function 'sum'
 *
 */

/* Include files */
#include "sum.h"
#include "dftregistration_coder.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Variable Definitions */
static emlrtRSInfo ib_emlrtRSI = { 20, /* lineNo */
  "sum",                               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\sum.m"/* pathName */
};

static emlrtRTEInfo d_emlrtRTEI = { 73,/* lineNo */
  9,                                   /* colNo */
  "sumprod",                           /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\sumprod.m"/* pName */
};

static emlrtRTEInfo e_emlrtRTEI = { 43,/* lineNo */
  23,                                  /* colNo */
  "sumprod",                           /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\sumprod.m"/* pName */
};

/* Function Definitions */
creal_T b_sum(const creal_T x_data[], const int32_T x_size[2])
{
  creal_T y;
  int32_T vlen;
  int32_T k;
  vlen = x_size[1];
  if (x_size[1] == 0) {
    y.re = 0.0;
    y.im = 0.0;
  } else {
    y = x_data[0];
    for (k = 2; k <= vlen; k++) {
      y.re += x_data[k - 1].re;
      y.im += x_data[k - 1].im;
    }
  }

  return y;
}

real_T c_sum(const emxArray_real_T *x)
{
  real_T y;
  int32_T vlen;
  int32_T k;
  vlen = x->size[0];
  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= vlen; k++) {
      y += x->data[k - 1];
    }
  }

  return y;
}

void sum(const emlrtStack *sp, const emxArray_creal_T *x, creal_T y_data[],
         int32_T y_size[2])
{
  boolean_T p;
  int32_T vlen;
  int16_T sz_idx_1;
  int32_T npages;
  int32_T i;
  int32_T xpageoffset;
  int32_T k;
  int32_T b_i;
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = &ib_emlrtRSI;
  if (((x->size[0] != 1) || (x->size[1] != 1)) && (x->size[0] == 1)) {
    emlrtErrorWithMessageIdR2018a(&st, &e_emlrtRTEI,
      "Coder:toolbox:autoDimIncompatibility",
      "Coder:toolbox:autoDimIncompatibility", 0);
  }

  p = false;
  if ((x->size[0] == 0) && (x->size[1] == 0)) {
    p = true;
  }

  if (p) {
    emlrtErrorWithMessageIdR2018a(&st, &d_emlrtRTEI,
      "Coder:toolbox:UnsupportedSpecialEmpty",
      "Coder:toolbox:UnsupportedSpecialEmpty", 0);
  }

  vlen = x->size[0];
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    sz_idx_1 = (int16_T)x->size[1];
    y_size[0] = 1;
    y_size[1] = sz_idx_1;
    vlen = sz_idx_1;
    if (0 <= vlen - 1) {
      memset(&y_data[0], 0, vlen * sizeof(creal_T));
    }
  } else {
    npages = x->size[1];
    y_size[0] = 1;
    y_size[1] = (int16_T)x->size[1];
    for (i = 0; i < npages; i++) {
      xpageoffset = i * x->size[0];
      y_data[i] = x->data[xpageoffset];
      for (k = 2; k <= vlen; k++) {
        b_i = (xpageoffset + k) - 1;
        y_data[i].re += x->data[b_i].re;
        y_data[i].im += x->data[b_i].im;
      }
    }
  }
}

/* End of code generation (sum.c) */
