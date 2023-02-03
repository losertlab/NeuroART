/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * meshgrid.h
 *
 * Code generation for function 'meshgrid'
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
void meshgrid(const emlrtStack *sp, const real_T x_data[], const int32_T x_size
              [2], const real_T y_data[], const int32_T y_size[2],
              emxArray_real_T *xx, emxArray_real_T *yy);

/* End of code generation (meshgrid.h) */
