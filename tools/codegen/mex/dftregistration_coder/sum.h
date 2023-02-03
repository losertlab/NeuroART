/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sum.h
 *
 * Code generation for function 'sum'
 *
 */

#pragma once

/* Include files */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mex.h"
#include "emlrt.h"
#include "rtwtypes.h"
#include "omp.h"
#include "dftregistration_coder_types.h"

/* Function Declarations */
creal_T b_sum(const creal_T x_data[], const int32_T x_size[2]);
real_T c_sum(const emxArray_real_T *x);
void sum(const emlrtStack *sp, const emxArray_creal_T *x, creal_T y_data[],
         int32_T y_size[2]);

/* End of code generation (sum.h) */
