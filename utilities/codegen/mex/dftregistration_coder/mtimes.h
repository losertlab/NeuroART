/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mtimes.h
 *
 * Code generation for function 'mtimes'
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
void b_mtimes(const creal_T A_data[], const int32_T A_size[2], const
              emxArray_creal_T *B, creal_T C_data[], int32_T C_size[2]);
void mtimes(const emlrtStack *sp, const emxArray_creal_T *A, const
            emxArray_creal_T *B, emxArray_creal_T *C);

/* End of code generation (mtimes.h) */
