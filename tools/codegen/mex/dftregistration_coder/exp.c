/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * exp.c
 *
 * Code generation for function 'exp'
 *
 */

/* Include files */
#include "exp.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_data.h"
#include "eml_int_forloop_overflow_check.h"
#include "mwmathutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo ad_emlrtRSI = { 10, /* lineNo */
  "exp",                               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\elfun\\exp.m"/* pathName */
};

static emlrtRSInfo bd_emlrtRSI = { 33, /* lineNo */
  "applyScalarFunctionInPlace",        /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\applyScalarFunctionInPlace.m"/* pathName */
};

/* Function Definitions */
void b_exp(creal_T *x)
{
  real_T r;
  real_T d;
  if (x->im == 0.0) {
    x->re = muDoubleScalarExp(x->re);
    x->im = 0.0;
  } else if (muDoubleScalarIsInf(x->im) && muDoubleScalarIsInf(x->re) && (x->re <
              0.0)) {
    x->re = 0.0;
    x->im = 0.0;
  } else {
    r = muDoubleScalarExp(x->re / 2.0);
    d = x->im;
    x->re = r * (r * muDoubleScalarCos(x->im));
    x->im = r * (r * muDoubleScalarSin(d));
  }
}

void c_exp(const emlrtStack *sp, emxArray_creal_T *x)
{
  int32_T nx;
  int32_T k;
  real_T r;
  real_T d;
  emlrtStack st;
  emlrtStack b_st;
  emlrtStack c_st;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = &ad_emlrtRSI;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  nx = x->size[0] * x->size[1];
  b_st.site = &bd_emlrtRSI;
  if ((1 <= nx) && (nx > 2147483646)) {
    c_st.site = &hb_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }

  for (k = 0; k < nx; k++) {
    if (x->data[k].im == 0.0) {
      x->data[k].re = muDoubleScalarExp(x->data[k].re);
      x->data[k].im = 0.0;
    } else if (muDoubleScalarIsInf(x->data[k].im) && muDoubleScalarIsInf(x->
                data[k].re) && (x->data[k].re < 0.0)) {
      x->data[k].re = 0.0;
      x->data[k].im = 0.0;
    } else {
      r = muDoubleScalarExp(x->data[k].re / 2.0);
      d = x->data[k].im;
      x->data[k].re = r * (r * muDoubleScalarCos(x->data[k].im));
      x->data[k].im = r * (r * muDoubleScalarSin(d));
    }
  }
}

/* End of code generation (exp.c) */
