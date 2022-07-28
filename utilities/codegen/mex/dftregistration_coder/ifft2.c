/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * ifft2.c
 *
 * Code generation for function 'ifft2'
 *
 */

/* Include files */
#include "ifft2.h"
#include "FFTWApi.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_data.h"
#include "dftregistration_coder_emxutil.h"
#include "ifft.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo nb_emlrtRSI = { 34, /* lineNo */
  "ifft2",                             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\ifft2.m"/* pathName */
};

static emlrtRTEInfo wb_emlrtRTEI = { 34,/* lineNo */
  10,                                  /* colNo */
  "ifft2",                             /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\ifft2.m"/* pName */
};

/* Function Definitions */
void ifft2(const emlrtStack *sp, const emxArray_creal_T *x, emxArray_creal_T *f)
{
  emxArray_creal_T *b_x;
  int32_T i;
  emlrtStack st;
  emlrtStack b_st;
  emlrtStack c_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);
  emxInit_creal_T(sp, &b_x, 2, &wb_emlrtRTEI, true);
  st.site = &nb_emlrtRSI;
  b_st.site = &nb_emlrtRSI;
  ifft(&b_st, x, b_x);
  b_st.site = &ob_emlrtRSI;
  if ((b_x->size[0] == 0) || (b_x->size[1] == 0)) {
    i = f->size[0] * f->size[1];
    f->size[0] = (int16_T)b_x->size[0];
    f->size[1] = (int16_T)b_x->size[1];
    emxEnsureCapacity_creal_T(&b_st, f, i, &vb_emlrtRTEI);
  } else {
    c_st.site = &qb_emlrtRSI;
    FFTWApi_fft1d(&c_st, b_x, b_x->size[0], (int16_T)b_x->size[1], f);
  }

  emxFree_creal_T(&b_x);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
}

/* End of code generation (ifft2.c) */
