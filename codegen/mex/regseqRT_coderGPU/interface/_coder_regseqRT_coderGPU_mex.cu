/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_regseqRT_coderGPU_mex.cu
 *
 * Code generation for function '_coder_regseqRT_coderGPU_mex'
 *
 */

/* Include files */
#include "_coder_regseqRT_coderGPU_mex.h"
#include "_coder_regseqRT_coderGPU_api.h"
#include "regseqRT_coderGPU.h"
#include "regseqRT_coderGPU_data.h"
#include "regseqRT_coderGPU_initialize.h"
#include "regseqRT_coderGPU_terminate.h"

/* Function Declarations */
MEXFUNCTION_LINKAGE void regseqRT_coderGPU_mexFunction(int32_T nlhs, mxArray
  *plhs[3], int32_T nrhs, const mxArray *prhs[2]);

/* Function Definitions */
void regseqRT_coderGPU_mexFunction(int32_T nlhs, mxArray *plhs[3], int32_T nrhs,
  const mxArray *prhs[2])
{
  const mxArray *outputs[3];
  int32_T b_nlhs;

  /* Check for proper number of arguments. */
  if (nrhs != 2) {
    emlrtErrMsgIdAndTxt(emlrtRootTLSGlobal, "EMLRT:runTime:WrongNumberOfInputs",
                        5, 12, 2, 4, 17, "regseqRT_coderGPU");
  }

  if (nlhs > 3) {
    emlrtErrMsgIdAndTxt(emlrtRootTLSGlobal,
                        "EMLRT:runTime:TooManyOutputArguments", 3, 4, 17,
                        "regseqRT_coderGPU");
  }

  /* Call the function. */
  regseqRT_coderGPU_api(prhs, nlhs, outputs);

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
  mexAtExit(regseqRT_coderGPU_atexit);

  /* Module initialization. */
  regseqRT_coderGPU_initialize();

  /* Dispatch the entry-point. */
  regseqRT_coderGPU_mexFunction(nlhs, plhs, nrhs, prhs);

  /* Module termination. */
  regseqRT_coderGPU_terminate();
}

emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/* End of code generation (_coder_regseqRT_coderGPU_mex.cu) */
