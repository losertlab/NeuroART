/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse.h
 *
 * Code generation for function 'sparse'
 *
 */

#ifndef SPARSE_H
#define SPARSE_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "computeDFF_new_coder_types.h"

/* Function Declarations */
extern void b_sparse(const emxArray_real_T *varargin_1, emxArray_real_T *y_d,
                     emxArray_int32_T *y_colidx, emxArray_int32_T *y_rowidx, int
                     *y_m, int *y_n, int *y_maxnz);
extern void c_sparse(const emxArray_boolean_T *varargin_1, emxArray_boolean_T
                     *y_d, emxArray_int32_T *y_colidx, emxArray_int32_T
                     *y_rowidx, int *y_m, int *y_n, int *y_maxnz);
extern void sparse(const emxArray_real_T *varargin_1, const emxArray_real_T
                   *varargin_2, double varargin_4, double varargin_5,
                   coder_internal_sparse *y);

#endif

/* End of code generation (sparse.h) */
