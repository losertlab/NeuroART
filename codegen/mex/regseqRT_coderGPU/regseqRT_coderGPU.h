/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * regseqRT_coderGPU.h
 *
 * Code generation for function 'regseqRT_coderGPU'
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
#include "regseqRT_coderGPU_types.h"

/* Type Definitions */
#include "cufft.h"

/* Function Declarations */
CODEGEN_EXPORT_SYM void regseqRT_coderGPU(const creal_T b_template[262144],
  const real_T currentFrame[262144], real_T regFrame[262144], real_T *tx, real_T
  *ty);

/* End of code generation (regseqRT_coderGPU.h) */
