/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * regseqRT_coderGPU_initialize.cu
 *
 * Code generation for function 'regseqRT_coderGPU_initialize'
 *
 */

/* Include files */
#include "regseqRT_coderGPU_initialize.h"
#include "_coder_regseqRT_coderGPU_mex.h"
#include "regseqRT_coderGPU.h"
#include "regseqRT_coderGPU_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void regseqRT_coderGPU_initialize()
{
  mex_InitInfAndNan();
  mexFunctionCreateRootTLS();
  emlrtClearAllocCountR2012b(emlrtRootTLSGlobal, false, 0U, 0);
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtLicenseCheckR2012b(emlrtRootTLSGlobal, "Distrib_Computing_Toolbox", 2);
  emlrtLicenseCheckR2012b(emlrtRootTLSGlobal, "Image_Toolbox", 2);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
  cudaGetLastError();
}

/* End of code generation (regseqRT_coderGPU_initialize.cu) */
