/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coder.h
 *
 * Code generation for function 'dftregistration_coder'
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
void dftregistration_coder(const emlrtStack *sp, const emxArray_creal_T *buf1ft,
  const emxArray_creal_T *buf2ft, real_T usfac, real_T output_data[], int32_T
  output_size[2], emxArray_creal_T *Greg);
void eml_fftshift(const emlrtStack *sp, emxArray_creal_T *x, int32_T dim);

/* End of code generation (dftregistration_coder.h) */
