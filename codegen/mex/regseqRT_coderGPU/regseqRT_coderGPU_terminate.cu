/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * regseqRT_coderGPU_terminate.cu
 *
 * Code generation for function 'regseqRT_coderGPU_terminate'
 *
 */

/* Include files */
#include "regseqRT_coderGPU_terminate.h"
#include "_coder_regseqRT_coderGPU_mex.h"
#include "regseqRT_coderGPU.h"
#include "regseqRT_coderGPU_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void regseqRT_coderGPU_atexit()
{
  mexFunctionCreateRootTLS();
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void regseqRT_coderGPU_terminate()
{
  cudaError_t errCode;
  errCode = cudaGetLastError();
  if (errCode != cudaSuccess) {
    emlrtThinCUDAError(false, emlrtRootTLSGlobal);
  }

  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (regseqRT_coderGPU_terminate.cu) */
