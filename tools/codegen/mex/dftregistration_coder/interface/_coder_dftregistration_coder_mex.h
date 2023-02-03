/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_dftregistration_coder_mex.h
 *
 * Code generation for function '_coder_dftregistration_coder_mex'
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
MEXFUNCTION_LINKAGE void c_dftregistration_coder_mexFunc(int32_T nlhs, mxArray
  *plhs[2], int32_T nrhs, const mxArray *prhs[3]);
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
  const mxArray *prhs[]);
emlrtCTX mexFunctionCreateRootTLS(void);

/* End of code generation (_coder_dftregistration_coder_mex.h) */
