/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * meshgrid.c
 *
 * Code generation for function 'meshgrid'
 *
 */

/* Include files */
#include "meshgrid.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_emxutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRTEInfo oc_emlrtRTEI = { 1,/* lineNo */
  23,                                  /* colNo */
  "meshgrid",                          /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\elmat\\meshgrid.m"/* pName */
};

/* Function Definitions */
void meshgrid(const emlrtStack *sp, const real_T x_data[], const int32_T x_size
              [2], const real_T y_data[], const int32_T y_size[2],
              emxArray_real_T *xx, emxArray_real_T *yy)
{
  int32_T nx;
  int32_T ny;
  int32_T j;
  int32_T i;
  nx = x_size[1];
  ny = y_size[1];
  j = xx->size[0] * xx->size[1];
  xx->size[0] = (int16_T)y_size[1];
  xx->size[1] = (int16_T)x_size[1];
  emxEnsureCapacity_real_T(sp, xx, j, &oc_emlrtRTEI);
  j = yy->size[0] * yy->size[1];
  yy->size[0] = (int16_T)y_size[1];
  yy->size[1] = (int16_T)x_size[1];
  emxEnsureCapacity_real_T(sp, yy, j, &oc_emlrtRTEI);
  if ((x_size[1] != 0) && (y_size[1] != 0)) {
    for (j = 0; j < nx; j++) {
      for (i = 0; i < ny; i++) {
        xx->data[i + xx->size[0] * j] = x_data[j];
        yy->data[i + yy->size[0] * j] = y_data[i];
      }
    }
  }
}

/* End of code generation (meshgrid.c) */
