/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coderGPU_terminate.cu
 *
 * Code generation for function 'dftregistration_coderGPU_terminate'
 *
 */

/* Include files */
#include "dftregistration_coderGPU_terminate.h"
#include "_coder_dftregistration_coderGPU_mex.h"
#include "dftregistration_coderGPU.h"
#include "dftregistration_coderGPU_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void cublasEnsureDestruction();

/* Function Definitions */
static void cublasEnsureDestruction()
{
  if (cublasGlobalHandle != NULL) {
    cublasDestroy(cublasGlobalHandle);
    cublasGlobalHandle = NULL;
  }
}

void dftregistration_coderGPU_atexit()
{
  mexFunctionCreateRootTLS();
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void dftregistration_coderGPU_terminate()
{
  cudaError_t errCode;
  errCode = cudaGetLastError();
  if (errCode != cudaSuccess) {
    emlrtThinCUDAError(false, emlrtRootTLSGlobal);
  }

  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  cublasEnsureDestruction();
}

/* End of code generation (dftregistration_coderGPU_terminate.cu) */
