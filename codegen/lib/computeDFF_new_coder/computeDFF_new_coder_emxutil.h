/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder_emxutil.h
 *
 * Code generation for function 'computeDFF_new_coder_emxutil'
 *
 */

#ifndef COMPUTEDFF_NEW_CODER_EMXUTIL_H
#define COMPUTEDFF_NEW_CODER_EMXUTIL_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "computeDFF_new_coder_types.h"

/* Function Declarations */
extern void c_emxEnsureCapacity_coder_inter(coder_internal_sparse data[1000],
  const int size[1], int oldNumel);
extern void c_emxFreeStruct_coder_internal_(coder_internal_sparse *pStruct);
extern void c_emxFree_coder_internal_sparse(c_emxArray_coder_internal_spars
  *pEmxArray);
extern void c_emxInitStruct_coder_internal_(coder_internal_sparse *pStruct);
extern void c_emxInit_coder_internal_sparse(c_emxArray_coder_internal_spars
  *pEmxArray);
extern void d_emxEnsureCapacity_coder_inter(coder_internal_sparse_1 data[1000],
  const int size[1], int oldNumel);
extern void d_emxFree_coder_internal_sparse(d_emxArray_coder_internal_spars
  *pEmxArray);
extern void d_emxInit_coder_internal_sparse(d_emxArray_coder_internal_spars
  *pEmxArray);
extern void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int
  oldNumel);
extern void emxEnsureCapacity_cell_wrap_3(cell_wrap_3 data[360], const int size
  [1], int oldNumel);
extern void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_int8_T(emxArray_int8_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
extern void emxFreeMatrix_cell_wrap_3(cell_wrap_3 pMatrix[359]);
extern void emxFreeMatrix_cell_wrap_5(cell_wrap_5 pMatrix[2]);
extern void emxFreeStruct_struct0_T(struct0_T *pStruct);
extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
extern void emxFree_cell_wrap_3_360(emxArray_cell_wrap_3_360 *pEmxArray);
extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);
extern void emxFree_int8_T(emxArray_int8_T **pEmxArray);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxFree_uint16_T(emxArray_uint16_T **pEmxArray);
extern void emxInitMatrix_cell_wrap_3(cell_wrap_3 pMatrix[359]);
extern void emxInitMatrix_cell_wrap_5(cell_wrap_5 pMatrix[2]);
extern void emxInitStruct_struct0_T(struct0_T *pStruct);
extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions);
extern void emxInit_cell_wrap_3_360(emxArray_cell_wrap_3_360 *pEmxArray);
extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
extern void emxInit_int8_T(emxArray_int8_T **pEmxArray, int numDimensions);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_uint16_T(emxArray_uint16_T **pEmxArray, int numDimensions);

#endif

/* End of code generation (computeDFF_new_coder_emxutil.h) */
