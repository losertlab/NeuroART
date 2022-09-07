/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coderGPU_initialize.cu
 *
 * Code generation for function 'dftregistration_coderGPU_initialize'
 *
 */

/* Include files */
#include "dftregistration_coderGPU_initialize.h"
#include "_coder_dftregistration_coderGPU_mex.h"
#include "dftregistration_coderGPU.h"
#include "dftregistration_coderGPU_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void cublasEnsureInitialization();

/* Function Definitions */
static void cublasEnsureInitialization()
{
  if (cublasGlobalHandle == NULL) {
    cublasCreate(&cublasGlobalHandle);
    cublasSetPointerMode(cublasGlobalHandle, CUBLAS_POINTER_MODE_DEVICE);
  }
}

void dftregistration_coderGPU_initialize()
{
  mex_InitInfAndNan();
  mexFunctionCreateRootTLS();
  emlrtClearAllocCountR2012b(emlrtRootTLSGlobal, false, 0U, 0);
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtLicenseCheckR2012b(emlrtRootTLSGlobal, "Distrib_Computing_Toolbox", 2);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
  cublasEnsureInitialization();
  cudaGetLastError();
}

/* End of code generation (dftregistration_coderGPU_initialize.cu) */
