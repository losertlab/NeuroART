/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse1.h
 *
 * Code generation for function 'sparse1'
 *
 */

#ifndef SPARSE1_H
#define SPARSE1_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "computeDFF_new_coder_types.h"

/* Function Declarations */
extern void sparse_full(const emxArray_real_T *this_d, const emxArray_int32_T
  *this_colidx, const emxArray_int32_T *this_rowidx, int this_m, int this_n,
  emxArray_real_T *y);
extern void sparse_parenReference(const emxArray_real_T *this_d, const
  emxArray_int32_T *this_colidx, const emxArray_int32_T *this_rowidx, int this_m,
  const double varargin_2_data[], const int varargin_2_size[2],
  coder_internal_sparse *s);

#endif

/* End of code generation (sparse1.h) */
