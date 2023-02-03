/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coderGPU_emxutil.h
 *
 * Code generation for function 'dftregistration_coderGPU_emxutil'
 *
 */

#pragma once

/* Include files */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mex.h"
#include "emlrt.h"
#include "rtwtypes.h"
#include "dftregistration_coderGPU_types.h"

/* Function Declarations */
CODEGEN_EXPORT_SYM void emxEnsureCapacity_creal_T(emxArray_creal_T *emxArray,
  int32_T oldNumel);
CODEGEN_EXPORT_SYM void emxEnsureCapacity_int16_T(emxArray_int16_T *emxArray,
  int32_T oldNumel);
CODEGEN_EXPORT_SYM void emxEnsureCapacity_real_T(emxArray_real_T *emxArray,
  int32_T oldNumel);
CODEGEN_EXPORT_SYM void emxEnsureCapacity_uint16_T(emxArray_uint16_T *emxArray,
  int32_T oldNumel);
CODEGEN_EXPORT_SYM void emxFree_creal_T(emxArray_creal_T **pEmxArray);
CODEGEN_EXPORT_SYM void emxFree_int16_T(emxArray_int16_T **pEmxArray);
CODEGEN_EXPORT_SYM void emxFree_real_T(emxArray_real_T **pEmxArray);
CODEGEN_EXPORT_SYM void emxFree_uint16_T(emxArray_uint16_T **pEmxArray);
CODEGEN_EXPORT_SYM void emxInit_creal_T(emxArray_creal_T **pEmxArray, int32_T
  numDimensions, boolean_T doPush);
CODEGEN_EXPORT_SYM void emxInit_int16_T(emxArray_int16_T **pEmxArray, int32_T
  numDimensions, boolean_T doPush);
CODEGEN_EXPORT_SYM void emxInit_real_T(emxArray_real_T **pEmxArray, int32_T
  numDimensions, boolean_T doPush);
CODEGEN_EXPORT_SYM void emxInit_uint16_T(emxArray_uint16_T **pEmxArray, int32_T
  numDimensions, boolean_T doPush);

/* End of code generation (dftregistration_coderGPU_emxutil.h) */
