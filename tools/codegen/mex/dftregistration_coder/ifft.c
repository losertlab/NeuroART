/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * ifft.c
 *
 * Code generation for function 'ifft'
 *
 */

/* Include files */
#include "ifft.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_data.h"
#include "dftregistration_coder_emxutil.h"
#include "fft.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo pb_emlrtRSI = { 69, /* lineNo */
  "fft",                               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\fft.m"/* pathName */
};

static emlrtRTEInfo xb_emlrtRTEI = { 29,/* lineNo */
  9,                                   /* colNo */
  "permute",                           /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\elmat\\permute.m"/* pName */
};

static emlrtRTEInfo yb_emlrtRTEI = { 19,/* lineNo */
  1,                                   /* colNo */
  "ifft",                              /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\ifft.m"/* pName */
};

static emlrtRTEInfo ac_emlrtRTEI = { 64,/* lineNo */
  9,                                   /* colNo */
  "fft",                               /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\fft.m"/* pName */
};

/* Function Definitions */
void ifft(const emlrtStack *sp, const emxArray_creal_T *x, emxArray_creal_T *y)
{
  emxArray_creal_T *b_x;
  int32_T i;
  int32_T loop_ub;
  emxArray_creal_T *b_y1;
  int32_T b_loop_ub;
  int32_T i1;
  emlrtStack st;
  emlrtStack b_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);
  emxInit_creal_T(sp, &b_x, 2, &xb_emlrtRTEI, true);
  st.site = &ob_emlrtRSI;
  i = b_x->size[0] * b_x->size[1];
  b_x->size[0] = x->size[1];
  b_x->size[1] = x->size[0];
  emxEnsureCapacity_creal_T(&st, b_x, i, &xb_emlrtRTEI);
  loop_ub = x->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_loop_ub = x->size[1];
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      b_x->data[i1 + b_x->size[0] * i] = x->data[i + x->size[0] * i1];
    }
  }

  emxInit_creal_T(&st, &b_y1, 2, &ac_emlrtRTEI, true);
  b_st.site = &pb_emlrtRSI;
  fft(&b_st, b_x, x->size[1], b_y1);
  i = y->size[0] * y->size[1];
  y->size[0] = b_y1->size[1];
  y->size[1] = b_y1->size[0];
  emxEnsureCapacity_creal_T(&st, y, i, &yb_emlrtRTEI);
  loop_ub = b_y1->size[0];
  emxFree_creal_T(&b_x);
  for (i = 0; i < loop_ub; i++) {
    b_loop_ub = b_y1->size[1];
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      y->data[i1 + y->size[0] * i] = b_y1->data[i + b_y1->size[0] * i1];
    }
  }

  emxFree_creal_T(&b_y1);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
}

/* End of code generation (ifft.c) */
