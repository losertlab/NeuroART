/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coderGPU_data.cu
 *
 * Code generation for function 'dftregistration_coderGPU_data'
 *
 */

/* Include files */
#include "dftregistration_coderGPU_data.h"
#include "dftregistration_coderGPU.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131483U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "dftregistration_coderGPU",          /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 3194014219U, 53452778U, 2865749887U, 3590888462U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

cublasHandle_t cublasGlobalHandle = NULL;

/* End of code generation (dftregistration_coderGPU_data.cu) */
