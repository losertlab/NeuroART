/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_computeDFF_new_coder_api.c
 *
 * Code generation for function '_coder_computeDFF_new_coder_api'
 *
 */

/* Include files */
#include "_coder_computeDFF_new_coder_api.h"
#include "_coder_computeDFF_new_coder_mex.h"

/* Type Definitions */
#ifndef typedef_c_emxArray_coder_internal_spars
#define typedef_c_emxArray_coder_internal_spars

typedef struct {
  coder_internal_sparse data[1000];
  int32_T size[1];
} c_emxArray_coder_internal_spars;

#endif                                 /*typedef_c_emxArray_coder_internal_spars*/

#ifndef typedef_d_emxArray_coder_internal_spars
#define typedef_d_emxArray_coder_internal_spars

typedef struct {
  coder_internal_sparse_1 data[1000];
  int32_T size[1];
} d_emxArray_coder_internal_spars;

#endif                                 /*typedef_d_emxArray_coder_internal_spars*/

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131483U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "computeDFF_new_coder",              /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_uint16_T *y);
static const mxArray *b_emlrt_marshallOut(const emlrtStack *sp, const
  coder_internal_sparse u_data[], const int32_T u_size[1]);
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *frameRate,
  const char_T *identifier);
static const mxArray *c_emlrt_marshallOut(const emlrtStack *sp, const
  coder_internal_sparse_1 u_data[], const int32_T u_size[1]);
static void c_emxCopyStruct_coder_internal_(coder_internal_sparse *dst, const
  coder_internal_sparse *src);
static void c_emxFreeStruct_coder_internal_(coder_internal_sparse *pStruct);
static void c_emxFree_coder_internal_sparse(c_emxArray_coder_internal_spars
  *pEmxArray);
static void c_emxInitStruct_coder_internal_(const emlrtStack *sp,
  coder_internal_sparse *pStruct, boolean_T doPush);
static void c_emxInit_coder_internal_sparse(c_emxArray_coder_internal_spars
  *pEmxArray);
static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static const mxArray *d_emlrt_marshallOut(const struct0_T u);
static void d_emxCopyStruct_coder_internal_(coder_internal_sparse_1 *dst, const
  coder_internal_sparse_1 *src);
static void d_emxFreeStruct_coder_internal_(coder_internal_sparse_1 *pStruct);
static void d_emxFree_coder_internal_sparse(d_emxArray_coder_internal_spars
  *pEmxArray);
static void d_emxInitStruct_coder_internal_(const emlrtStack *sp,
  coder_internal_sparse_1 *pStruct, boolean_T doPush);
static void d_emxInit_coder_internal_sparse(d_emxArray_coder_internal_spars
  *pEmxArray);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *pos_lst,
  const char_T *identifier, real_T **y_data, int32_T y_size[2]);
static const mxArray *e_emlrt_marshallOut(const emxArray_real_T *u);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *RegImg, const
  char_T *identifier, emxArray_uint16_T *y);
static const mxArray *emlrt_marshallOut(const emxArray_real_T *u);
static void emxCopy_boolean_T(emxArray_boolean_T **dst, emxArray_boolean_T *
  const *src);
static void emxCopy_int32_T(emxArray_int32_T **dst, emxArray_int32_T * const
  *src);
static void emxCopy_real_T(emxArray_real_T **dst, emxArray_real_T * const *src);
static void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int32_T
  oldNumel);
static void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int32_T
  oldNumel);
static void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int32_T oldNumel);
static void emxEnsureCapacity_uint16_T(emxArray_uint16_T *emxArray, int32_T
  oldNumel);
static void emxFreeStruct_struct0_T(struct0_T *pStruct);
static void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
static void emxFree_int32_T(emxArray_int32_T **pEmxArray);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxFree_uint16_T(emxArray_uint16_T **pEmxArray);
static void emxInitStruct_struct0_T(const emlrtStack *sp, struct0_T *pStruct,
  boolean_T doPush);
static void emxInit_boolean_T(const emlrtStack *sp, emxArray_boolean_T
  **pEmxArray, int32_T numDimensions, boolean_T doPush);
static void emxInit_int32_T(const emlrtStack *sp, emxArray_int32_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxInit_uint16_T(const emlrtStack *sp, emxArray_uint16_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T **y_data, int32_T y_size[2]);
static const mxArray *f_emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[1]);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_uint16_T *ret);
static const mxArray *g_emlrt_marshallOut(const real_T u);
static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T **ret_data, int32_T ret_size[2]);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_uint16_T *y)
{
  g_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *b_emlrt_marshallOut(const emlrtStack *sp, const
  coder_internal_sparse u_data[], const int32_T u_size[1])
{
  const mxArray *y;
  int32_T i;
  emxArray_real_T *u_d;
  emxArray_int32_T *u_colidx;
  emxArray_int32_T *u_rowidx;
  coder_internal_sparse expl_temp;
  int32_T i1;
  int32_T loop_ub;
  const mxArray *b_y;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);
  y = NULL;
  emlrtAssign(&y, emlrtCreateCellArrayR2014a(1, u_size));
  i = 0;
  emxInit_real_T(sp, &u_d, 1, true);
  emxInit_int32_T(sp, &u_colidx, 1, true);
  emxInit_int32_T(sp, &u_rowidx, 1, true);
  c_emxInitStruct_coder_internal_(sp, &expl_temp, true);
  while (i < u_size[0]) {
    c_emxCopyStruct_coder_internal_(&expl_temp, &u_data[i]);
    i1 = u_d->size[0];
    u_d->size[0] = expl_temp.d->size[0];
    emxEnsureCapacity_real_T(u_d, i1);
    loop_ub = expl_temp.d->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      u_d->data[i1] = expl_temp.d->data[i1];
    }

    i1 = u_colidx->size[0];
    u_colidx->size[0] = expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(u_colidx, i1);
    loop_ub = expl_temp.colidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      u_colidx->data[i1] = expl_temp.colidx->data[i1];
    }

    i1 = u_rowidx->size[0];
    u_rowidx->size[0] = expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(u_rowidx, i1);
    loop_ub = expl_temp.rowidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      u_rowidx->data[i1] = expl_temp.rowidx->data[i1];
    }

    b_y = NULL;
    emlrtAssign(&b_y, emlrtCreateSparse(&u_d->data[0], &u_colidx->data[0],
      &u_rowidx->data[0], expl_temp.m, expl_temp.n, expl_temp.maxnz,
      mxDOUBLE_CLASS, mxREAL));
    emlrtSetCell(y, i, b_y);
    i++;
  }

  c_emxFreeStruct_coder_internal_(&expl_temp);
  emxFree_int32_T(&u_rowidx);
  emxFree_int32_T(&u_colidx);
  emxFree_real_T(&u_d);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
  return y;
}

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *frameRate,
  const char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(frameRate), &thisId);
  emlrtDestroyArray(&frameRate);
  return y;
}

static const mxArray *c_emlrt_marshallOut(const emlrtStack *sp, const
  coder_internal_sparse_1 u_data[], const int32_T u_size[1])
{
  const mxArray *y;
  int32_T i;
  emxArray_boolean_T *u_d;
  emxArray_int32_T *u_colidx;
  emxArray_int32_T *u_rowidx;
  coder_internal_sparse_1 expl_temp;
  int32_T i1;
  int32_T loop_ub;
  const mxArray *b_y;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);
  y = NULL;
  emlrtAssign(&y, emlrtCreateCellArrayR2014a(1, u_size));
  i = 0;
  emxInit_boolean_T(sp, &u_d, 1, true);
  emxInit_int32_T(sp, &u_colidx, 1, true);
  emxInit_int32_T(sp, &u_rowidx, 1, true);
  d_emxInitStruct_coder_internal_(sp, &expl_temp, true);
  while (i < u_size[0]) {
    d_emxCopyStruct_coder_internal_(&expl_temp, &u_data[i]);
    i1 = u_d->size[0];
    u_d->size[0] = expl_temp.d->size[0];
    emxEnsureCapacity_boolean_T(u_d, i1);
    loop_ub = expl_temp.d->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      u_d->data[i1] = expl_temp.d->data[i1];
    }

    i1 = u_colidx->size[0];
    u_colidx->size[0] = expl_temp.colidx->size[0];
    emxEnsureCapacity_int32_T(u_colidx, i1);
    loop_ub = expl_temp.colidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      u_colidx->data[i1] = expl_temp.colidx->data[i1];
    }

    i1 = u_rowidx->size[0];
    u_rowidx->size[0] = expl_temp.rowidx->size[0];
    emxEnsureCapacity_int32_T(u_rowidx, i1);
    loop_ub = expl_temp.rowidx->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      u_rowidx->data[i1] = expl_temp.rowidx->data[i1];
    }

    b_y = NULL;
    emlrtAssign(&b_y, emlrtCreateSparse(&u_d->data[0], &u_colidx->data[0],
      &u_rowidx->data[0], expl_temp.m, expl_temp.n, expl_temp.maxnz,
      mxLOGICAL_CLASS, mxREAL));
    emlrtSetCell(y, i, b_y);
    i++;
  }

  d_emxFreeStruct_coder_internal_(&expl_temp);
  emxFree_int32_T(&u_rowidx);
  emxFree_int32_T(&u_colidx);
  emxFree_boolean_T(&u_d);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
  return y;
}

static void c_emxCopyStruct_coder_internal_(coder_internal_sparse *dst, const
  coder_internal_sparse *src)
{
  emxCopy_real_T(&dst->d, &src->d);
  emxCopy_int32_T(&dst->colidx, &src->colidx);
  emxCopy_int32_T(&dst->rowidx, &src->rowidx);
  dst->m = src->m;
  dst->n = src->n;
  dst->maxnz = src->maxnz;
}

static void c_emxFreeStruct_coder_internal_(coder_internal_sparse *pStruct)
{
  emxFree_real_T(&pStruct->d);
  emxFree_int32_T(&pStruct->colidx);
  emxFree_int32_T(&pStruct->rowidx);
}

static void c_emxFree_coder_internal_sparse(c_emxArray_coder_internal_spars
  *pEmxArray)
{
  int32_T numEl;
  int32_T i;
  numEl = 1;
  for (i = 0; i < 1; i++) {
    numEl *= pEmxArray->size[0];
  }

  for (i = 0; i < numEl; i++) {
    c_emxFreeStruct_coder_internal_(&pEmxArray->data[i]);
  }
}

static void c_emxInitStruct_coder_internal_(const emlrtStack *sp,
  coder_internal_sparse *pStruct, boolean_T doPush)
{
  emxInit_real_T(sp, &pStruct->d, 1, doPush);
  emxInit_int32_T(sp, &pStruct->colidx, 1, doPush);
  emxInit_int32_T(sp, &pStruct->rowidx, 1, doPush);
}

static void c_emxInit_coder_internal_sparse(c_emxArray_coder_internal_spars
  *pEmxArray)
{
  int32_T i;
  for (i = 0; i < 1; i++) {
    pEmxArray->size[0] = 0;
  }
}

static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = h_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static const mxArray *d_emlrt_marshallOut(const struct0_T u)
{
  const mxArray *y;
  static const char * sv[2] = { "smooth", "raw" };

  y = NULL;
  emlrtAssign(&y, emlrtCreateStructMatrix(1, 1, 2, sv));
  emlrtSetFieldR2017b(y, 0, "smooth", e_emlrt_marshallOut(u.smooth), 0);
  emlrtSetFieldR2017b(y, 0, "raw", e_emlrt_marshallOut(u.raw), 1);
  return y;
}

static void d_emxCopyStruct_coder_internal_(coder_internal_sparse_1 *dst, const
  coder_internal_sparse_1 *src)
{
  emxCopy_boolean_T(&dst->d, &src->d);
  emxCopy_int32_T(&dst->colidx, &src->colidx);
  emxCopy_int32_T(&dst->rowidx, &src->rowidx);
  dst->m = src->m;
  dst->n = src->n;
  dst->maxnz = src->maxnz;
}

static void d_emxFreeStruct_coder_internal_(coder_internal_sparse_1 *pStruct)
{
  emxFree_boolean_T(&pStruct->d);
  emxFree_int32_T(&pStruct->colidx);
  emxFree_int32_T(&pStruct->rowidx);
}

static void d_emxFree_coder_internal_sparse(d_emxArray_coder_internal_spars
  *pEmxArray)
{
  int32_T numEl;
  int32_T i;
  numEl = 1;
  for (i = 0; i < 1; i++) {
    numEl *= pEmxArray->size[0];
  }

  for (i = 0; i < numEl; i++) {
    d_emxFreeStruct_coder_internal_(&pEmxArray->data[i]);
  }
}

static void d_emxInitStruct_coder_internal_(const emlrtStack *sp,
  coder_internal_sparse_1 *pStruct, boolean_T doPush)
{
  emxInit_boolean_T(sp, &pStruct->d, 1, doPush);
  emxInit_int32_T(sp, &pStruct->colidx, 1, doPush);
  emxInit_int32_T(sp, &pStruct->rowidx, 1, doPush);
}

static void d_emxInit_coder_internal_sparse(d_emxArray_coder_internal_spars
  *pEmxArray)
{
  int32_T i;
  for (i = 0; i < 1; i++) {
    pEmxArray->size[0] = 0;
  }
}

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *pos_lst,
  const char_T *identifier, real_T **y_data, int32_T y_size[2])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  f_emlrt_marshallIn(sp, emlrtAlias(pos_lst), &thisId, y_data, y_size);
  emlrtDestroyArray(&pos_lst);
}

static const mxArray *e_emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  int32_T iv[2];
  const mxArray *m;
  real_T *pData;
  int32_T i;
  int32_T b_i;
  int32_T c_i;
  y = NULL;
  iv[0] = u->size[0];
  iv[1] = u->size[1];
  m = emlrtCreateNumericArray(2, &iv[0], mxDOUBLE_CLASS, mxREAL);
  pData = emlrtMxGetPr(m);
  i = 0;
  for (b_i = 0; b_i < u->size[1]; b_i++) {
    for (c_i = 0; c_i < u->size[0]; c_i++) {
      pData[i] = u->data[c_i + u->size[0] * b_i];
      i++;
    }
  }

  emlrtAssign(&y, m);
  return y;
}

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *RegImg, const
  char_T *identifier, emxArray_uint16_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(RegImg), &thisId, y);
  emlrtDestroyArray(&RegImg);
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

static void emxCopy_boolean_T(emxArray_boolean_T **dst, emxArray_boolean_T *
  const *src)
{
  int32_T numElDst;
  int32_T numElSrc;
  int32_T i;
  numElDst = 1;
  numElSrc = 1;
  for (i = 0; i < (*dst)->numDimensions; i++) {
    numElDst *= (*dst)->size[i];
    numElSrc *= (*src)->size[i];
  }

  for (i = 0; i < (*dst)->numDimensions; i++) {
    (*dst)->size[i] = (*src)->size[i];
  }

  emxEnsureCapacity_boolean_T(*dst, numElDst);
  for (i = 0; i < numElSrc; i++) {
    (*dst)->data[i] = (*src)->data[i];
  }
}

static void emxCopy_int32_T(emxArray_int32_T **dst, emxArray_int32_T * const
  *src)
{
  int32_T numElDst;
  int32_T numElSrc;
  int32_T i;
  numElDst = 1;
  numElSrc = 1;
  for (i = 0; i < (*dst)->numDimensions; i++) {
    numElDst *= (*dst)->size[i];
    numElSrc *= (*src)->size[i];
  }

  for (i = 0; i < (*dst)->numDimensions; i++) {
    (*dst)->size[i] = (*src)->size[i];
  }

  emxEnsureCapacity_int32_T(*dst, numElDst);
  for (i = 0; i < numElSrc; i++) {
    (*dst)->data[i] = (*src)->data[i];
  }
}

static void emxCopy_real_T(emxArray_real_T **dst, emxArray_real_T * const *src)
{
  int32_T numElDst;
  int32_T numElSrc;
  int32_T i;
  numElDst = 1;
  numElSrc = 1;
  for (i = 0; i < (*dst)->numDimensions; i++) {
    numElDst *= (*dst)->size[i];
    numElSrc *= (*src)->size[i];
  }

  for (i = 0; i < (*dst)->numDimensions; i++) {
    (*dst)->size[i] = (*src)->size[i];
  }

  emxEnsureCapacity_real_T(*dst, numElDst);
  for (i = 0; i < numElSrc; i++) {
    (*dst)->data[i] = (*src)->data[i];
  }
}

static void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int32_T
  oldNumel)
{
  int32_T newNumel;
  int32_T i;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }

    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }

    newData = emlrtCallocMex((uint32_T)i, sizeof(boolean_T));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(boolean_T) * oldNumel);
      if (emxArray->canFreeData) {
        emlrtFreeMex(emxArray->data);
      }
    }

    emxArray->data = (boolean_T *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

static void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int32_T
  oldNumel)
{
  int32_T newNumel;
  int32_T i;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }

    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }

    newData = emlrtCallocMex((uint32_T)i, sizeof(int32_T));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(int32_T) * oldNumel);
      if (emxArray->canFreeData) {
        emlrtFreeMex(emxArray->data);
      }
    }

    emxArray->data = (int32_T *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

static void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int32_T oldNumel)
{
  int32_T newNumel;
  int32_T i;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }

    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }

    newData = emlrtCallocMex((uint32_T)i, sizeof(real_T));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(real_T) * oldNumel);
      if (emxArray->canFreeData) {
        emlrtFreeMex(emxArray->data);
      }
    }

    emxArray->data = (real_T *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

static void emxEnsureCapacity_uint16_T(emxArray_uint16_T *emxArray, int32_T
  oldNumel)
{
  int32_T newNumel;
  int32_T i;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }

    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }

    newData = emlrtCallocMex((uint32_T)i, sizeof(uint16_T));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(uint16_T) * oldNumel);
      if (emxArray->canFreeData) {
        emlrtFreeMex(emxArray->data);
      }
    }

    emxArray->data = (uint16_T *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

static void emxFreeStruct_struct0_T(struct0_T *pStruct)
{
  emxFree_real_T(&pStruct->smooth);
  emxFree_real_T(&pStruct->raw);
}

static void emxFree_boolean_T(emxArray_boolean_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_boolean_T *)NULL) {
    if (((*pEmxArray)->data != (boolean_T *)NULL) && (*pEmxArray)->canFreeData)
    {
      emlrtFreeMex((*pEmxArray)->data);
    }

    emlrtFreeMex((*pEmxArray)->size);
    emlrtFreeMex(*pEmxArray);
    *pEmxArray = (emxArray_boolean_T *)NULL;
  }
}

static void emxFree_int32_T(emxArray_int32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_int32_T *)NULL) {
    if (((*pEmxArray)->data != (int32_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((*pEmxArray)->data);
    }

    emlrtFreeMex((*pEmxArray)->size);
    emlrtFreeMex(*pEmxArray);
    *pEmxArray = (emxArray_int32_T *)NULL;
  }
}

static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (real_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((*pEmxArray)->data);
    }

    emlrtFreeMex((*pEmxArray)->size);
    emlrtFreeMex(*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

static void emxFree_uint16_T(emxArray_uint16_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_uint16_T *)NULL) {
    if (((*pEmxArray)->data != (uint16_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((*pEmxArray)->data);
    }

    emlrtFreeMex((*pEmxArray)->size);
    emlrtFreeMex(*pEmxArray);
    *pEmxArray = (emxArray_uint16_T *)NULL;
  }
}

static void emxInitStruct_struct0_T(const emlrtStack *sp, struct0_T *pStruct,
  boolean_T doPush)
{
  emxInit_real_T(sp, &pStruct->smooth, 2, doPush);
  emxInit_real_T(sp, &pStruct->raw, 2, doPush);
}

static void emxInit_boolean_T(const emlrtStack *sp, emxArray_boolean_T
  **pEmxArray, int32_T numDimensions, boolean_T doPush)
{
  emxArray_boolean_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_boolean_T *)emlrtMallocMex(sizeof(emxArray_boolean_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_boolean_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (boolean_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex(sizeof(int32_T) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

static void emxInit_int32_T(const emlrtStack *sp, emxArray_int32_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_int32_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_int32_T *)emlrtMallocMex(sizeof(emxArray_int32_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_int32_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (int32_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex(sizeof(int32_T) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex(sizeof(int32_T) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

static void emxInit_uint16_T(const emlrtStack *sp, emxArray_uint16_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_uint16_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_uint16_T *)emlrtMallocMex(sizeof(emxArray_uint16_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_uint16_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (uint16_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex(sizeof(int32_T) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T **y_data, int32_T y_size[2])
{
  real_T *r;
  i_emlrt_marshallIn(sp, emlrtAlias(u), parentId, &r, y_size);
  *y_data = r;
  emlrtDestroyArray(&u);
}

static const mxArray *f_emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[1])
{
  const mxArray *y;
  const mxArray *m;
  static const int32_T iv[1] = { 0 };

  y = NULL;
  m = emlrtCreateNumericArray(1, iv, mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, (void *)&u_data[0]);
  emlrtSetDimensions((mxArray *)m, u_size, 1);
  emlrtAssign(&y, m);
  return y;
}

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_uint16_T *ret)
{
  static const int32_T dims[3] = { -1, -1, -1 };

  const boolean_T bv[3] = { true, true, true };

  int32_T iv[3];
  int32_T i;
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "uint16", false, 3U, dims, &bv[0],
    iv);
  ret->allocatedSize = iv[0] * iv[1] * iv[2];
  i = ret->size[0] * ret->size[1] * ret->size[2];
  ret->size[0] = iv[0];
  ret->size[1] = iv[1];
  ret->size[2] = iv[2];
  emxEnsureCapacity_uint16_T(ret, i);
  ret->data = (uint16_T *)emlrtMxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static const mxArray *g_emlrt_marshallOut(const real_T u)
{
  const mxArray *y;
  const mxArray *m;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId)
{
  real_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 0U, &dims);
  ret = *(real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T **ret_data, int32_T ret_size[2])
{
  static const int32_T dims[2] = { 1000, 2 };

  const boolean_T bv[2] = { true, false };

  int32_T iv[2];
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims, &bv[0],
    iv);
  ret_size[0] = iv[0];
  ret_size[1] = iv[1];
  *ret_data = (real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
}

void computeDFF_new_coder_api(const mxArray * const prhs[7], int32_T nlhs, const
  mxArray *plhs[10])
{
  real_T (*xcRaw_data)[1000];
  real_T (*ycRaw_data)[1000];
  emxArray_uint16_T *RegImg;
  emxArray_real_T *norm_meanIMG;
  c_emxArray_coder_internal_spars roiBW2_sp;
  d_emxArray_coder_internal_spars npBWout_sp;
  struct0_T DFF;
  emxArray_real_T *normF;
  emxArray_real_T *npSubFluoSmooth;
  real_T frameRate;
  real_T (*pos_lst_data)[2000];
  int32_T pos_lst_size[2];
  real_T r_pixels;
  real_T winsize;
  real_T percentBaselineSub;
  real_T neuropilSubPercent;
  int32_T xcRaw_size[1];
  int32_T ycRaw_size[1];
  real_T minNpSubFluo;
  real_T maxAdjF;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  xcRaw_data = (real_T (*)[1000])mxMalloc(sizeof(real_T [1000]));
  ycRaw_data = (real_T (*)[1000])mxMalloc(sizeof(real_T [1000]));
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_uint16_T(&st, &RegImg, 3, true);
  emxInit_real_T(&st, &norm_meanIMG, 2, true);
  c_emxInit_coder_internal_sparse(&roiBW2_sp);
  d_emxInit_coder_internal_sparse(&npBWout_sp);
  emxInitStruct_struct0_T(&st, &DFF, true);
  emxInit_real_T(&st, &normF, 2, true);
  emxInit_real_T(&st, &npSubFluoSmooth, 2, true);

  /* Marshall function inputs */
  RegImg->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "RegImg", RegImg);
  frameRate = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "frameRate");
  e_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "pos_lst", (real_T **)
                     &pos_lst_data, pos_lst_size);
  r_pixels = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "r_pixels");
  winsize = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "winsize");
  percentBaselineSub = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]),
    "percentBaselineSub");
  neuropilSubPercent = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]),
    "neuropilSubPercent");

  /* Invoke the target function */
  computeDFF_new_coder(RegImg, frameRate, *pos_lst_data, pos_lst_size, r_pixels,
                       winsize, percentBaselineSub, neuropilSubPercent,
                       norm_meanIMG, roiBW2_sp.data, roiBW2_sp.size,
                       npBWout_sp.data, npBWout_sp.size, &DFF, normF,
                       npSubFluoSmooth, *xcRaw_data, xcRaw_size, *ycRaw_data,
                       ycRaw_size, &minNpSubFluo, &maxAdjF);

  /* Marshall function outputs */
  norm_meanIMG->canFreeData = false;
  plhs[0] = emlrt_marshallOut(norm_meanIMG);
  emxFree_real_T(&norm_meanIMG);
  emxFree_uint16_T(&RegImg);
  if (nlhs > 1) {
    plhs[1] = b_emlrt_marshallOut(&st, roiBW2_sp.data, roiBW2_sp.size);
  }

  c_emxFree_coder_internal_sparse(&roiBW2_sp);
  if (nlhs > 2) {
    plhs[2] = c_emlrt_marshallOut(&st, npBWout_sp.data, npBWout_sp.size);
  }

  d_emxFree_coder_internal_sparse(&npBWout_sp);
  if (nlhs > 3) {
    plhs[3] = d_emlrt_marshallOut(DFF);
  }

  emxFreeStruct_struct0_T(&DFF);
  if (nlhs > 4) {
    normF->canFreeData = false;
    plhs[4] = emlrt_marshallOut(normF);
  }

  emxFree_real_T(&normF);
  if (nlhs > 5) {
    npSubFluoSmooth->canFreeData = false;
    plhs[5] = emlrt_marshallOut(npSubFluoSmooth);
  }

  emxFree_real_T(&npSubFluoSmooth);
  if (nlhs > 6) {
    plhs[6] = f_emlrt_marshallOut(*xcRaw_data, xcRaw_size);
  }

  if (nlhs > 7) {
    plhs[7] = f_emlrt_marshallOut(*ycRaw_data, ycRaw_size);
  }

  if (nlhs > 8) {
    plhs[8] = g_emlrt_marshallOut(minNpSubFluo);
  }

  if (nlhs > 9) {
    plhs[9] = g_emlrt_marshallOut(maxAdjF);
  }

  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

void computeDFF_new_coder_atexit(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  computeDFF_new_coder_xil_terminate();
  computeDFF_new_coder_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void computeDFF_new_coder_initialize(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void computeDFF_new_coder_terminate(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (_coder_computeDFF_new_coder_api.c) */
