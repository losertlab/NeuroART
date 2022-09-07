/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_dftregistration_coderGPU_mex.cu
 *
 * Code generation for function '_coder_dftregistration_coderGPU_mex'
 *
 */

/* Include files */
#include "_coder_dftregistration_coderGPU_mex.h"
#include "_coder_dftregistration_coderGPU_api.h"
#include "dftregistration_coderGPU.h"
#include "dftregistration_coderGPU_data.h"
#include "dftregistration_coderGPU_initialize.h"
#include "dftregistration_coderGPU_terminate.h"

/* Function Declarations */
MEXFUNCTION_LINKAGE void c_dftregistration_coderGPU_mexF
  (c_dftregistration_coderGPUStack *SD, int32_T nlhs, mxArray *plhs[2], int32_T
   nrhs, const mxArray *prhs[3]);

/* Function Definitions */
void c_dftregistration_coderGPU_mexF(c_dftregistration_coderGPUStack *SD,
  int32_T nlhs, mxArray *plhs[2], int32_T nrhs, const mxArray *prhs[3])
{
  const mxArray *outputs[2];
  int32_T b_nlhs;

  /* Check for proper number of arguments. */
  if (nrhs != 3) {
    emlrtErrMsgIdAndTxt(emlrtRootTLSGlobal, "EMLRT:runTime:WrongNumberOfInputs",
                        5, 12, 3, 4, 24, "dftregistration_coderGPU");
  }

  if (nlhs > 2) {
    emlrtErrMsgIdAndTxt(emlrtRootTLSGlobal,
                        "EMLRT:runTime:TooManyOutputArguments", 3, 4, 24,
                        "dftregistration_coderGPU");
  }

  /* Call the function. */
  dftregistration_coderGPU_api(SD, prhs, nlhs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);
}

void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  c_dftregistration_coderGPUStack *d_dftregistration_coderGPUStack = NULL;
  d_dftregistration_coderGPUStack = (c_dftregistration_coderGPUStack *)
    emlrtMxCalloc(1, (size_t)1U * sizeof(c_dftregistration_coderGPUStack));
  mexAtExit(dftregistration_coderGPU_atexit);

  /* Module initialization. */
  dftregistration_coderGPU_initialize();

  /* Dispatch the entry-point. */
  c_dftregistration_coderGPU_mexF(d_dftregistration_coderGPUStack, nlhs, plhs,
    nrhs, prhs);

  /* Module termination. */
  dftregistration_coderGPU_terminate();
  emlrtMxFree(d_dftregistration_coderGPUStack);
}

emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/* End of code generation (_coder_dftregistration_coderGPU_mex.cu) */
