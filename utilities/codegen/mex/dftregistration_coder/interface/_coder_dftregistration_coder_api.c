/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_dftregistration_coder_api.c
 *
 * Code generation for function '_coder_dftregistration_coder_api'
 *
 */

/* Include files */
#include "_coder_dftregistration_coder_api.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_data.h"
#include "dftregistration_coder_emxutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRTEInfo pc_emlrtRTEI = { 1,/* lineNo */
  1,                                   /* colNo */
  "_coder_dftregistration_coder_api",  /* fName */
  ""                                   /* pName */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_creal_T *y);
static const mxArray *b_emlrt_marshallOut(const emlrtStack *sp, const
  emxArray_creal_T *u);
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *usfac,
  const char_T *identifier);
static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_creal_T *ret);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *buf1ft, const
  char_T *identifier, emxArray_creal_T *y);
static const mxArray *emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2]);
static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_creal_T *y)
{
  e_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *b_emlrt_marshallOut(const emlrtStack *sp, const
  emxArray_creal_T *u)
{
  const mxArray *y;
  int32_T iv[2];
  const mxArray *m;
  y = NULL;
  iv[0] = u->size[0];
  iv[1] = u->size[1];
  m = emlrtCreateNumericArray(2, &iv[0], mxDOUBLE_CLASS, mxCOMPLEX);
  emlrtExportNumericArrayR2013b(sp, m, &u->data[0], 8);
  emlrtAssign(&y, m);
  return y;
}

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *usfac,
  const char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(usfac), &thisId);
  emlrtDestroyArray(&usfac);
  return y;
}

static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = f_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_creal_T *ret)
{
  static const int32_T dims[2] = { 512, 512 };

  const boolean_T bv[2] = { true, true };

  int32_T iv[2];
  int32_T i;
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", true, 2U, dims, &bv[0], iv);
  i = ret->size[0] * ret->size[1];
  ret->size[0] = iv[0];
  ret->size[1] = iv[1];
  emxEnsureCapacity_creal_T(sp, ret, i, (emlrtRTEInfo *)NULL);
  emlrtImportArrayR2015b(sp, src, ret->data, 8, true);
  emlrtDestroyArray(&src);
}

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *buf1ft, const
  char_T *identifier, emxArray_creal_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(buf1ft), &thisId, y);
  emlrtDestroyArray(&buf1ft);
}

static const mxArray *emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2])
{
  const mxArray *y;
  const mxArray *m;
  static const int32_T iv[2] = { 0, 0 };

  y = NULL;
  m = emlrtCreateNumericArray(2, iv, mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, (void *)&u_data[0]);
  emlrtSetDimensions((mxArray *)m, u_size, 2);
  emlrtAssign(&y, m);
  return y;
}

static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId)
{
  real_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 0U, &dims);
  ret = *(real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

void dftregistration_coder_api(const mxArray * const prhs[3], int32_T nlhs,
  const mxArray *plhs[2])
{
  real_T (*output_data)[4];
  emxArray_creal_T *buf1ft;
  emxArray_creal_T *buf2ft;
  emxArray_creal_T *Greg;
  real_T usfac;
  int32_T output_size[2];
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  output_data = (real_T (*)[4])mxMalloc(sizeof(real_T [4]));
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_creal_T(&st, &buf1ft, 2, &pc_emlrtRTEI, true);
  emxInit_creal_T(&st, &buf2ft, 2, &pc_emlrtRTEI, true);
  emxInit_creal_T(&st, &Greg, 2, &pc_emlrtRTEI, true);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "buf1ft", buf1ft);
  emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "buf2ft", buf2ft);
  usfac = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "usfac");

  /* Invoke the target function */
  dftregistration_coder(&st, buf1ft, buf2ft, usfac, *output_data, output_size,
                        Greg);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(*output_data, output_size);
  emxFree_creal_T(&buf2ft);
  emxFree_creal_T(&buf1ft);
  if (nlhs > 1) {
    plhs[1] = b_emlrt_marshallOut(&st, Greg);
  }

  emxFree_creal_T(&Greg);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/* End of code generation (_coder_dftregistration_coder_api.c) */
