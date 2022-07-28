/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_dftregistration_coderGPU_api.cu
 *
 * Code generation for function '_coder_dftregistration_coderGPU_api'
 *
 */

/* Include files */
#include "_coder_dftregistration_coderGPU_api.h"
#include "dftregistration_coderGPU.h"
#include "dftregistration_coderGPU_data.h"
#include "dftregistration_coderGPU_emxutil.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static int16_T b_emlrt_marshallIn(const mxArray *u, const emlrtMsgIdentifier
  *parentId);
static int16_T c_emlrt_marshallIn(const mxArray *src, const emlrtMsgIdentifier
  *msgId);
static int16_T emlrt_marshallIn(const mxArray *usfac, const char_T *identifier);
static const mxArray *emlrt_marshallOut(const emxArray_real_T *u);

/* Function Definitions */
static int16_T b_emlrt_marshallIn(const mxArray *u, const emlrtMsgIdentifier
  *parentId)
{
  int16_T y;
  y = c_emlrt_marshallIn(emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static int16_T c_emlrt_marshallIn(const mxArray *src, const emlrtMsgIdentifier
  *msgId)
{
  int16_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(emlrtRootTLSGlobal, msgId, src, "int16", false, 0U,
    &dims);
  ret = *(int16_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static int16_T emlrt_marshallIn(const mxArray *usfac, const char_T *identifier)
{
  int16_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = const_cast<const char *>(identifier);
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(emlrtAlias(usfac), &thisId);
  emlrtDestroyArray(&usfac);
  return y;
}

static const mxArray *emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  const mxArray *m;
  static const int32_T iv[2] = { 0, 0 };

  y = NULL;
  m = emlrtCreateNumericArray(2, iv, mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, &u->data[0]);
  emlrtSetDimensions((mxArray *)m, u->size, 2);
  emlrtAssign(&y, m);
  return y;
}

void dftregistration_coderGPU_api(c_dftregistration_coderGPUStack *SD, const
  mxArray * const prhs[3], int32_T nlhs, const mxArray *plhs[2])
{
  emxArray_real_T *output;
  mxGPUArray *Greg_gpu;
  static const int32_T dims[2] = { 512, 512 };

  creal_T (*Greg)[262144];
  const mxGPUArray *buf1ft_gpu;
  static const int32_T b_dims[2] = { 512, 512 };

  creal_T (*buf1ft)[262144];
  const mxGPUArray *buf2ft_gpu;
  static const int32_T c_dims[2] = { 512, 512 };

  creal_T (*buf2ft)[262144];
  int16_T usfac;
  emlrtHeapReferenceStackEnterFcnR2012b(emlrtRootTLSGlobal);
  emxInit_real_T(&output, 2, true);
  emlrtInitGPU(emlrtRootTLSGlobal);

  /* Create GpuArrays for outputs */
  Greg_gpu = emlrtGPUCreateNumericArray("double", true, 2, dims);
  Greg = (creal_T (*)[262144])emlrtGPUGetData(Greg_gpu);

  /* Marshall function inputs */
  buf1ft_gpu = emlrt_marshallInGPU(emlrtRootTLSGlobal, prhs[0], "buf1ft",
    "double", true, 2, b_dims, false);
  buf1ft = (creal_T (*)[262144])emlrtGPUGetDataReadOnly(buf1ft_gpu);
  buf2ft_gpu = emlrt_marshallInGPU(emlrtRootTLSGlobal, prhs[1], "buf2ft",
    "double", true, 2, c_dims, false);
  buf2ft = (creal_T (*)[262144])emlrtGPUGetDataReadOnly(buf2ft_gpu);
  usfac = emlrt_marshallIn(emlrtAliasP(prhs[2]), "usfac");

  /* Invoke the target function */
  dftregistration_coderGPU(SD, *buf1ft, *buf2ft, usfac, output, *Greg);

  /* Marshall function outputs */
  output->canFreeData = false;
  plhs[0] = emlrt_marshallOut(output);
  emxFree_real_T(&output);
  if (nlhs > 1) {
    plhs[1] = emlrt_marshallOutGPU(Greg_gpu);
  }

  /* Destroy GPUArrays */
  emlrtDestroyGPUArray(buf1ft_gpu);
  emlrtDestroyGPUArray(buf2ft_gpu);
  emlrtHeapReferenceStackLeaveFcnR2012b(emlrtRootTLSGlobal);
  emlrtDestroyGPUArray(Greg_gpu);
}

/* End of code generation (_coder_dftregistration_coderGPU_api.cu) */
