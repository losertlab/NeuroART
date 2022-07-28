/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coderGPU.h
 *
 * Code generation for function 'dftregistration_coderGPU'
 *
 */

#pragma once

/* Include files */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mex.h"
#include "emlrt.h"
#include "rtwtypes.h"
#include "dftregistration_coderGPU_types.h"

/* Type Definitions */
#include "cublas_v2.h"
#include "cufft.h"

/* Function Declarations */
CODEGEN_EXPORT_SYM void dftregistration_coderGPU(c_dftregistration_coderGPUStack
  *SD, const creal_T buf1ft[262144], const creal_T buf2ft[262144], int16_T usfac,
  emxArray_real_T *output, creal_T Greg[262144]);

/* End of code generation (dftregistration_coderGPU.h) */
