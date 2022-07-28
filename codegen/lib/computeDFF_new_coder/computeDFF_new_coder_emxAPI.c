/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder_emxAPI.c
 *
 * Code generation for function 'computeDFF_new_coder_emxAPI'
 *
 */

/* Include files */
#include "computeDFF_new_coder_emxAPI.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "rt_nonfinite.h"
#include <stdlib.h>
#include <string.h>

/* Function Definitions */
emxArray_boolean_T *emxCreateND_boolean_T(int numDimensions, const int *size)
{
  emxArray_boolean_T *emx;
  int numEl;
  int i;
  emxInit_boolean_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (boolean_T *)calloc((unsigned int)numEl, sizeof(boolean_T));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_int32_T *emxCreateND_int32_T(int numDimensions, const int *size)
{
  emxArray_int32_T *emx;
  int numEl;
  int i;
  emxInit_int32_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (int *)calloc((unsigned int)numEl, sizeof(int));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size)
{
  emxArray_real_T *emx;
  int numEl;
  int i;
  emxInit_real_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (double *)calloc((unsigned int)numEl, sizeof(double));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_uint16_T *emxCreateND_uint16_T(int numDimensions, const int *size)
{
  emxArray_uint16_T *emx;
  int numEl;
  int i;
  emxInit_uint16_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (unsigned short *)calloc((unsigned int)numEl, sizeof(unsigned
    short));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_boolean_T *emxCreateWrapperND_boolean_T(boolean_T *data, int
  numDimensions, const int *size)
{
  emxArray_boolean_T *emx;
  int numEl;
  int i;
  emxInit_boolean_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

emxArray_int32_T *emxCreateWrapperND_int32_T(int *data, int numDimensions, const
  int *size)
{
  emxArray_int32_T *emx;
  int numEl;
  int i;
  emxInit_int32_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

emxArray_real_T *emxCreateWrapperND_real_T(double *data, int numDimensions,
  const int *size)
{
  emxArray_real_T *emx;
  int numEl;
  int i;
  emxInit_real_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

emxArray_uint16_T *emxCreateWrapperND_uint16_T(unsigned short *data, int
  numDimensions, const int *size)
{
  emxArray_uint16_T *emx;
  int numEl;
  int i;
  emxInit_uint16_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

emxArray_boolean_T *emxCreateWrapper_boolean_T(boolean_T *data, int rows, int
  cols)
{
  emxArray_boolean_T *emx;
  emxInit_boolean_T(&emx, 2);
  emx->size[0] = rows;
  emx->size[1] = cols;
  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = rows * cols;
  emx->canFreeData = false;
  return emx;
}

emxArray_int32_T *emxCreateWrapper_int32_T(int *data, int rows, int cols)
{
  emxArray_int32_T *emx;
  emxInit_int32_T(&emx, 2);
  emx->size[0] = rows;
  emx->size[1] = cols;
  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = rows * cols;
  emx->canFreeData = false;
  return emx;
}

emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols)
{
  emxArray_real_T *emx;
  emxInit_real_T(&emx, 2);
  emx->size[0] = rows;
  emx->size[1] = cols;
  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = rows * cols;
  emx->canFreeData = false;
  return emx;
}

emxArray_uint16_T *emxCreateWrapper_uint16_T(unsigned short *data, int rows, int
  cols)
{
  emxArray_uint16_T *emx;
  emxInit_uint16_T(&emx, 2);
  emx->size[0] = rows;
  emx->size[1] = cols;
  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = rows * cols;
  emx->canFreeData = false;
  return emx;
}

emxArray_boolean_T *emxCreate_boolean_T(int rows, int cols)
{
  emxArray_boolean_T *emx;
  int numEl;
  emxInit_boolean_T(&emx, 2);
  emx->size[0] = rows;
  numEl = rows * cols;
  emx->size[1] = cols;
  emx->data = (boolean_T *)calloc((unsigned int)numEl, sizeof(boolean_T));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_int32_T *emxCreate_int32_T(int rows, int cols)
{
  emxArray_int32_T *emx;
  int numEl;
  emxInit_int32_T(&emx, 2);
  emx->size[0] = rows;
  numEl = rows * cols;
  emx->size[1] = cols;
  emx->data = (int *)calloc((unsigned int)numEl, sizeof(int));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_real_T *emxCreate_real_T(int rows, int cols)
{
  emxArray_real_T *emx;
  int numEl;
  emxInit_real_T(&emx, 2);
  emx->size[0] = rows;
  numEl = rows * cols;
  emx->size[1] = cols;
  emx->data = (double *)calloc((unsigned int)numEl, sizeof(double));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_uint16_T *emxCreate_uint16_T(int rows, int cols)
{
  emxArray_uint16_T *emx;
  int numEl;
  emxInit_uint16_T(&emx, 2);
  emx->size[0] = rows;
  numEl = rows * cols;
  emx->size[1] = cols;
  emx->data = (unsigned short *)calloc((unsigned int)numEl, sizeof(unsigned
    short));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

void emxDestroyArray_boolean_T(emxArray_boolean_T *emxArray)
{
  emxFree_boolean_T(&emxArray);
}

void emxDestroyArray_int32_T(emxArray_int32_T *emxArray)
{
  emxFree_int32_T(&emxArray);
}

void emxDestroyArray_real_T(emxArray_real_T *emxArray)
{
  emxFree_real_T(&emxArray);
}

void emxDestroyArray_uint16_T(emxArray_uint16_T *emxArray)
{
  emxFree_uint16_T(&emxArray);
}

void emxDestroy_emxArray_coder_internal_sparse_(c_emxArray_coder_internal_spars
  emxArray)
{
  c_emxFree_coder_internal_sparse(&emxArray);
}

void emxDestroy_emxArray_coder_internal_sparse_1(d_emxArray_coder_internal_spars
  emxArray)
{
  d_emxFree_coder_internal_sparse(&emxArray);
}

void emxDestroy_struct0_T(struct0_T emxArray)
{
  emxFreeStruct_struct0_T(&emxArray);
}

void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxInit_real_T(pEmxArray, numDimensions);
}

void emxInitArray_uint16_T(emxArray_uint16_T **pEmxArray, int numDimensions)
{
  emxInit_uint16_T(pEmxArray, numDimensions);
}

void emxInit_emxArray_coder_internal_sparse_(c_emxArray_coder_internal_spars
  *pEmxArray)
{
  c_emxInit_coder_internal_sparse(pEmxArray);
}

void emxInit_emxArray_coder_internal_sparse_1(d_emxArray_coder_internal_spars
  *pEmxArray)
{
  d_emxInit_coder_internal_sparse(pEmxArray);
}

void emxInit_struct0_T(struct0_T *pStruct)
{
  emxInitStruct_struct0_T(pStruct);
}

/* End of code generation (computeDFF_new_coder_emxAPI.c) */
