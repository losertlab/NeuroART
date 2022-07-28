/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sortIdx.h
 *
 * Code generation for function 'sortIdx'
 *
 */

#ifndef SORTIDX_H
#define SORTIDX_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "computeDFF_new_coder_types.h"

/* Function Declarations */
extern void merge_block(int idx_data[], double x_data[], int offset, int n, int
  preSortLevel, int iwork_data[], double xwork_data[]);
extern void merge_pow2_block(int idx_data[], double x_data[], int offset);

#endif

/* End of code generation (sortIdx.h) */
