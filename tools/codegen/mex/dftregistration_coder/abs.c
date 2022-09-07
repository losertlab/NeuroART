/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * abs.c
 *
 * Code generation for function 'abs'
 *
 */

/* Include files */
#include "abs.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_emxutil.h"
#include "mwmathutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo fb_emlrtRSI = { 18, /* lineNo */
  "abs",                               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\elfun\\abs.m"/* pathName */
};

static emlrtRTEInfo ub_emlrtRTEI = { 18,/* lineNo */
  5,                                   /* colNo */
  "abs",                               /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\elfun\\abs.m"/* pName */
};

/* Function Definitions */
void b_abs(const emlrtStack *sp, const emxArray_creal_T *x, emxArray_real_T *y)
{
  int32_T nx;
  int32_T k;
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = &fb_emlrtRSI;
  nx = x->size[0];
  k = y->size[0];
  y->size[0] = x->size[0];
  emxEnsureCapacity_real_T(&st, y, k, &ub_emlrtRTEI);
  for (k = 0; k < nx; k++) {
    y->data[k] = muDoubleScalarHypot(x->data[k].re, x->data[k].im);
  }
}

/* End of code generation (abs.c) */
