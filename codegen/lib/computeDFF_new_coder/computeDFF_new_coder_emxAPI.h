/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder_emxAPI.h
 *
 * Code generation for function 'computeDFF_new_coder_emxAPI'
 *
 */

#ifndef COMPUTEDFF_NEW_CODER_EMXAPI_H
#define COMPUTEDFF_NEW_CODER_EMXAPI_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "computeDFF_new_coder_types.h"

/* Function Declarations */
extern emxArray_boolean_T *emxCreateND_boolean_T(int numDimensions, const int
  *size);
extern emxArray_int32_T *emxCreateND_int32_T(int numDimensions, const int *size);
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size);
extern emxArray_uint16_T *emxCreateND_uint16_T(int numDimensions, const int
  *size);
extern emxArray_boolean_T *emxCreateWrapperND_boolean_T(boolean_T *data, int
  numDimensions, const int *size);
extern emxArray_int32_T *emxCreateWrapperND_int32_T(int *data, int numDimensions,
  const int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, const int *size);
extern emxArray_uint16_T *emxCreateWrapperND_uint16_T(unsigned short *data, int
  numDimensions, const int *size);
extern emxArray_boolean_T *emxCreateWrapper_boolean_T(boolean_T *data, int rows,
  int cols);
extern emxArray_int32_T *emxCreateWrapper_int32_T(int *data, int rows, int cols);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_uint16_T *emxCreateWrapper_uint16_T(unsigned short *data, int
  rows, int cols);
extern emxArray_boolean_T *emxCreate_boolean_T(int rows, int cols);
extern emxArray_int32_T *emxCreate_int32_T(int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern emxArray_uint16_T *emxCreate_uint16_T(int rows, int cols);
extern void emxDestroyArray_boolean_T(emxArray_boolean_T *emxArray);
extern void emxDestroyArray_int32_T(emxArray_int32_T *emxArray);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxDestroyArray_uint16_T(emxArray_uint16_T *emxArray);
extern void emxDestroy_emxArray_coder_internal_sparse_
  (c_emxArray_coder_internal_spars emxArray);
extern void emxDestroy_emxArray_coder_internal_sparse_1
  (d_emxArray_coder_internal_spars emxArray);
extern void emxDestroy_struct0_T(struct0_T emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInitArray_uint16_T(emxArray_uint16_T **pEmxArray, int
  numDimensions);
extern void emxInit_emxArray_coder_internal_sparse_
  (c_emxArray_coder_internal_spars *pEmxArray);
extern void emxInit_emxArray_coder_internal_sparse_1
  (d_emxArray_coder_internal_spars *pEmxArray);
extern void emxInit_struct0_T(struct0_T *pStruct);

#endif

/* End of code generation (computeDFF_new_coder_emxAPI.h) */
