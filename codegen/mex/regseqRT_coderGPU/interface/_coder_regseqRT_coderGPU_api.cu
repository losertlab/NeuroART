/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_regseqRT_coderGPU_api.cu
 *
 * Code generation for function '_coder_regseqRT_coderGPU_api'
 *
 */

/* Include files */
#include "_coder_regseqRT_coderGPU_api.h"
#include "regseqRT_coderGPU.h"
#include "regseqRT_coderGPU_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static const mxArray *emlrt_marshallOut(const real_T u);

/* Function Definitions */
static const mxArray *emlrt_marshallOut(const real_T u)
{
  const mxArray *y;
  const mxArray *m;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

void regseqRT_coderGPU_api(const mxArray * const prhs[2], int32_T nlhs, const
  mxArray *plhs[3])
{
  mxGPUArray *regFrame_gpu;
  static const int32_T dims[2] = { 512, 512 };

  real_T (*regFrame)[262144];
  const mxGPUArray *template_gpu;
  static const int32_T b_dims[2] = { 512, 512 };

  creal_T (*b_template)[262144];
  const mxGPUArray *currentFrame_gpu;
  static const int32_T c_dims[2] = { 512, 512 };

  real_T (*currentFrame)[262144];
  real_T tx;
  real_T ty;
  emlrtInitGPU(emlrtRootTLSGlobal);

  /* Create GpuArrays for outputs */
  regFrame_gpu = emlrtGPUCreateNumericArray("double", false, 2, dims);
  regFrame = (real_T (*)[262144])emlrtGPUGetData(regFrame_gpu);

  /* Marshall function inputs */
  template_gpu = emlrt_marshallInGPU(emlrtRootTLSGlobal, prhs[0], "template",
    "double", true, 2, b_dims, false);
  b_template = (creal_T (*)[262144])emlrtGPUGetDataReadOnly(template_gpu);
  currentFrame_gpu = emlrt_marshallInGPU(emlrtRootTLSGlobal, prhs[1],
    "currentFrame", "double", false, 2, c_dims, true);
  currentFrame = (real_T (*)[262144])emlrtGPUGetDataReadOnly(currentFrame_gpu);

  /* Invoke the target function */
  regseqRT_coderGPU(*b_template, *currentFrame, *regFrame, &tx, &ty);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOutGPU(regFrame_gpu);
  if (nlhs > 1) {
    plhs[1] = emlrt_marshallOut(tx);
  }

  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(ty);
  }

  /* Destroy GPUArrays */
  emlrtDestroyGPUArray(template_gpu);
  emlrtDestroyGPUArray(currentFrame_gpu);
  emlrtDestroyGPUArray(regFrame_gpu);
}

/* End of code generation (_coder_regseqRT_coderGPU_api.cu) */
