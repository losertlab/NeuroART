/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_computeDFF_new_coder_mex.c
 *
 * Code generation for function '_coder_computeDFF_new_coder_mex'
 *
 */

/* Include files */
#include "_coder_computeDFF_new_coder_mex.h"
#include "_coder_computeDFF_new_coder_api.h"

/* Function Declarations */
MEXFUNCTION_LINKAGE void c_computeDFF_new_coder_mexFunct(int32_T nlhs, mxArray
  *plhs[10], int32_T nrhs, const mxArray *prhs[7]);

/* Function Definitions */
void c_computeDFF_new_coder_mexFunct(int32_T nlhs, mxArray *plhs[10], int32_T
  nrhs, const mxArray *prhs[7])
{
  const mxArray *outputs[10];
  int32_T b_nlhs;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 7) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 7, 4,
                        20, "computeDFF_new_coder");
  }

  if (nlhs > 10) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 20,
                        "computeDFF_new_coder");
  }

  /* Call the function. */
  computeDFF_new_coder_api(prhs, nlhs, outputs);

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
  mexAtExit(computeDFF_new_coder_atexit);

  /* Module initialization. */
  computeDFF_new_coder_initialize();

  /* Dispatch the entry-point. */
  c_computeDFF_new_coder_mexFunct(nlhs, plhs, nrhs, prhs);

  /* Module termination. */
  computeDFF_new_coder_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/* End of code generation (_coder_computeDFF_new_coder_mex.c) */
