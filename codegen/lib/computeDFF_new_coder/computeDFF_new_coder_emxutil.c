/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder_emxutil.c
 *
 * Code generation for function 'computeDFF_new_coder_emxutil'
 *
 */

/* Include files */
#include "computeDFF_new_coder_emxutil.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
static void c_emxExpand_coder_internal_spar(coder_internal_sparse data[1000],
  int fromIndex, int toIndex);
static void c_emxTrim_coder_internal_sparse(coder_internal_sparse data[1000],
  int fromIndex, int toIndex);
static void d_emxExpand_coder_internal_spar(coder_internal_sparse_1 data[1000],
  int fromIndex, int toIndex);
static void d_emxFreeStruct_coder_internal_(coder_internal_sparse_1 *pStruct);
static void d_emxInitStruct_coder_internal_(coder_internal_sparse_1 *pStruct);
static void d_emxTrim_coder_internal_sparse(coder_internal_sparse_1 data[1000],
  int fromIndex, int toIndex);
static void emxExpand_cell_wrap_3_360(cell_wrap_3 data[360], int fromIndex, int
  toIndex);
static void emxFreeStruct_cell_wrap_3(cell_wrap_3 *pStruct);
static void emxFreeStruct_cell_wrap_5(cell_wrap_5 *pStruct);
static void emxInitStruct_cell_wrap_3(cell_wrap_3 *pStruct);
static void emxInitStruct_cell_wrap_5(cell_wrap_5 *pStruct);
static void emxTrim_cell_wrap_3_360(cell_wrap_3 data[360], int fromIndex, int
  toIndex);

/* Function Definitions */
static void c_emxExpand_coder_internal_spar(coder_internal_sparse data[1000],
  int fromIndex, int toIndex)
{
  int i;
  for (i = fromIndex; i < toIndex; i++) {
    c_emxInitStruct_coder_internal_(&data[i]);
  }
}

static void c_emxTrim_coder_internal_sparse(coder_internal_sparse data[1000],
  int fromIndex, int toIndex)
{
  int i;
  for (i = fromIndex; i < toIndex; i++) {
    c_emxFreeStruct_coder_internal_(&data[i]);
  }
}

static void d_emxExpand_coder_internal_spar(coder_internal_sparse_1 data[1000],
  int fromIndex, int toIndex)
{
  int i;
  for (i = fromIndex; i < toIndex; i++) {
    d_emxInitStruct_coder_internal_(&data[i]);
  }
}

static void d_emxFreeStruct_coder_internal_(coder_internal_sparse_1 *pStruct)
{
  emxFree_boolean_T(&pStruct->d);
  emxFree_int32_T(&pStruct->colidx);
  emxFree_int32_T(&pStruct->rowidx);
}

static void d_emxInitStruct_coder_internal_(coder_internal_sparse_1 *pStruct)
{
  emxInit_boolean_T(&pStruct->d, 1);
  emxInit_int32_T(&pStruct->colidx, 1);
  emxInit_int32_T(&pStruct->rowidx, 1);
}

static void d_emxTrim_coder_internal_sparse(coder_internal_sparse_1 data[1000],
  int fromIndex, int toIndex)
{
  int i;
  for (i = fromIndex; i < toIndex; i++) {
    d_emxFreeStruct_coder_internal_(&data[i]);
  }
}

static void emxExpand_cell_wrap_3_360(cell_wrap_3 data[360], int fromIndex, int
  toIndex)
{
  int i;
  for (i = fromIndex; i < toIndex; i++) {
    emxInitStruct_cell_wrap_3(&data[i]);
  }
}

static void emxFreeStruct_cell_wrap_3(cell_wrap_3 *pStruct)
{
  emxFree_real_T(&pStruct->f1);
}

static void emxFreeStruct_cell_wrap_5(cell_wrap_5 *pStruct)
{
  emxFree_int32_T(&pStruct->f1);
}

static void emxInitStruct_cell_wrap_3(cell_wrap_3 *pStruct)
{
  emxInit_real_T(&pStruct->f1, 1);
}

static void emxInitStruct_cell_wrap_5(cell_wrap_5 *pStruct)
{
  emxInit_int32_T(&pStruct->f1, 1);
}

static void emxTrim_cell_wrap_3_360(cell_wrap_3 data[360], int fromIndex, int
  toIndex)
{
  int i;
  for (i = fromIndex; i < toIndex; i++) {
    emxFreeStruct_cell_wrap_3(&data[i]);
  }
}

void c_emxEnsureCapacity_coder_inter(coder_internal_sparse data[1000], const int
  size[1], int oldNumel)
{
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  if (oldNumel > size[0]) {
    c_emxTrim_coder_internal_sparse(data, size[0], oldNumel);
  } else {
    if (oldNumel < size[0]) {
      c_emxExpand_coder_internal_spar(data, oldNumel, size[0]);
    }
  }
}

void c_emxFreeStruct_coder_internal_(coder_internal_sparse *pStruct)
{
  emxFree_real_T(&pStruct->d);
  emxFree_int32_T(&pStruct->colidx);
  emxFree_int32_T(&pStruct->rowidx);
}

void c_emxFree_coder_internal_sparse(c_emxArray_coder_internal_spars *pEmxArray)
{
  int numEl;
  int i;
  numEl = 1;
  for (i = 0; i < 1; i++) {
    numEl *= pEmxArray->size[0];
  }

  for (i = 0; i < numEl; i++) {
    c_emxFreeStruct_coder_internal_(&pEmxArray->data[i]);
  }
}

void c_emxInitStruct_coder_internal_(coder_internal_sparse *pStruct)
{
  emxInit_real_T(&pStruct->d, 1);
  emxInit_int32_T(&pStruct->colidx, 1);
  emxInit_int32_T(&pStruct->rowidx, 1);
}

void c_emxInit_coder_internal_sparse(c_emxArray_coder_internal_spars *pEmxArray)
{
  pEmxArray->size[0] = 0;
}

void d_emxEnsureCapacity_coder_inter(coder_internal_sparse_1 data[1000], const
  int size[1], int oldNumel)
{
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  if (oldNumel > size[0]) {
    d_emxTrim_coder_internal_sparse(data, size[0], oldNumel);
  } else {
    if (oldNumel < size[0]) {
      d_emxExpand_coder_internal_spar(data, oldNumel, size[0]);
    }
  }
}

void d_emxFree_coder_internal_sparse(d_emxArray_coder_internal_spars *pEmxArray)
{
  int numEl;
  int i;
  numEl = 1;
  for (i = 0; i < 1; i++) {
    numEl *= pEmxArray->size[0];
  }

  for (i = 0; i < numEl; i++) {
    d_emxFreeStruct_coder_internal_(&pEmxArray->data[i]);
  }
}

void d_emxInit_coder_internal_sparse(d_emxArray_coder_internal_spars *pEmxArray)
{
  pEmxArray->size[0] = 0;
}

void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int oldNumel)
{
  int newNumel;
  int i;
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

    newData = calloc((unsigned int)i, sizeof(boolean_T));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(boolean_T) * oldNumel);
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }

    emxArray->data = (boolean_T *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

void emxEnsureCapacity_cell_wrap_3(cell_wrap_3 data[360], const int size[1], int
  oldNumel)
{
  if (oldNumel < 0) {
    oldNumel = 0;
  }

  if (oldNumel > size[0]) {
    emxTrim_cell_wrap_3_360(data, size[0], oldNumel);
  } else {
    if (oldNumel < size[0]) {
      emxExpand_cell_wrap_3_360(data, oldNumel, size[0]);
    }
  }
}

void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel)
{
  int newNumel;
  int i;
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

    newData = calloc((unsigned int)i, sizeof(int));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(int) * oldNumel);
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }

    emxArray->data = (int *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

void emxEnsureCapacity_int8_T(emxArray_int8_T *emxArray, int oldNumel)
{
  int newNumel;
  int i;
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

    newData = calloc((unsigned int)i, sizeof(signed char));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(signed char) * oldNumel);
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }

    emxArray->data = (signed char *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel)
{
  int newNumel;
  int i;
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

    newData = calloc((unsigned int)i, sizeof(double));
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(double) * oldNumel);
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }

    emxArray->data = (double *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

void emxFreeMatrix_cell_wrap_3(cell_wrap_3 pMatrix[359])
{
  int i;
  for (i = 0; i < 359; i++) {
    emxFreeStruct_cell_wrap_3(&pMatrix[i]);
  }
}

void emxFreeMatrix_cell_wrap_5(cell_wrap_5 pMatrix[2])
{
  int i;
  for (i = 0; i < 2; i++) {
    emxFreeStruct_cell_wrap_5(&pMatrix[i]);
  }
}

void emxFreeStruct_struct0_T(struct0_T *pStruct)
{
  emxFree_real_T(&pStruct->smooth);
  emxFree_real_T(&pStruct->raw);
}

void emxFree_boolean_T(emxArray_boolean_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_boolean_T *)NULL) {
    if (((*pEmxArray)->data != (boolean_T *)NULL) && (*pEmxArray)->canFreeData)
    {
      free((*pEmxArray)->data);
    }

    free((*pEmxArray)->size);
    free(*pEmxArray);
    *pEmxArray = (emxArray_boolean_T *)NULL;
  }
}

void emxFree_cell_wrap_3_360(emxArray_cell_wrap_3_360 *pEmxArray)
{
  int numEl;
  int i;
  numEl = 1;
  for (i = 0; i < 1; i++) {
    numEl *= pEmxArray->size[0];
  }

  for (i = 0; i < numEl; i++) {
    emxFreeStruct_cell_wrap_3(&pEmxArray->data[i]);
  }
}

void emxFree_int32_T(emxArray_int32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_int32_T *)NULL) {
    if (((*pEmxArray)->data != (int *)NULL) && (*pEmxArray)->canFreeData) {
      free((*pEmxArray)->data);
    }

    free((*pEmxArray)->size);
    free(*pEmxArray);
    *pEmxArray = (emxArray_int32_T *)NULL;
  }
}

void emxFree_int8_T(emxArray_int8_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_int8_T *)NULL) {
    if (((*pEmxArray)->data != (signed char *)NULL) && (*pEmxArray)->canFreeData)
    {
      free((*pEmxArray)->data);
    }

    free((*pEmxArray)->size);
    free(*pEmxArray);
    *pEmxArray = (emxArray_int8_T *)NULL;
  }
}

void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (double *)NULL) && (*pEmxArray)->canFreeData) {
      free((*pEmxArray)->data);
    }

    free((*pEmxArray)->size);
    free(*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

void emxFree_uint16_T(emxArray_uint16_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_uint16_T *)NULL) {
    if (((*pEmxArray)->data != (unsigned short *)NULL) && (*pEmxArray)
        ->canFreeData) {
      free((*pEmxArray)->data);
    }

    free((*pEmxArray)->size);
    free(*pEmxArray);
    *pEmxArray = (emxArray_uint16_T *)NULL;
  }
}

void emxInitMatrix_cell_wrap_3(cell_wrap_3 pMatrix[359])
{
  int i;
  for (i = 0; i < 359; i++) {
    emxInitStruct_cell_wrap_3(&pMatrix[i]);
  }
}

void emxInitMatrix_cell_wrap_5(cell_wrap_5 pMatrix[2])
{
  int i;
  for (i = 0; i < 2; i++) {
    emxInitStruct_cell_wrap_5(&pMatrix[i]);
  }
}

void emxInitStruct_struct0_T(struct0_T *pStruct)
{
  emxInit_real_T(&pStruct->smooth, 2);
  emxInit_real_T(&pStruct->raw, 2);
}

void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions)
{
  emxArray_boolean_T *emxArray;
  int i;
  *pEmxArray = (emxArray_boolean_T *)malloc(sizeof(emxArray_boolean_T));
  emxArray = *pEmxArray;
  emxArray->data = (boolean_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

void emxInit_cell_wrap_3_360(emxArray_cell_wrap_3_360 *pEmxArray)
{
  pEmxArray->size[0] = 0;
}

void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions)
{
  emxArray_int32_T *emxArray;
  int i;
  *pEmxArray = (emxArray_int32_T *)malloc(sizeof(emxArray_int32_T));
  emxArray = *pEmxArray;
  emxArray->data = (int *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

void emxInit_int8_T(emxArray_int8_T **pEmxArray, int numDimensions)
{
  emxArray_int8_T *emxArray;
  int i;
  *pEmxArray = (emxArray_int8_T *)malloc(sizeof(emxArray_int8_T));
  emxArray = *pEmxArray;
  emxArray->data = (signed char *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxArray_real_T *emxArray;
  int i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (double *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

void emxInit_uint16_T(emxArray_uint16_T **pEmxArray, int numDimensions)
{
  emxArray_uint16_T *emxArray;
  int i;
  *pEmxArray = (emxArray_uint16_T *)malloc(sizeof(emxArray_uint16_T));
  emxArray = *pEmxArray;
  emxArray->data = (unsigned short *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/* End of code generation (computeDFF_new_coder_emxutil.c) */
