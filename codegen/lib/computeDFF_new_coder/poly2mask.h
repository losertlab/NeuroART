/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * poly2mask.h
 *
 * Code generation for function 'poly2mask'
 *
 */

#ifndef POLY2MASK_H
#define POLY2MASK_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "computeDFF_new_coder_types.h"

/* Function Declarations */
extern void b_poly2mask(double x[360], double y[360], double M, double N,
  emxArray_boolean_T *BW);
extern void poly2mask(emxArray_real_T *x, emxArray_real_T *y, double M, double N,
                      emxArray_boolean_T *BW);

#endif

/* End of code generation (poly2mask.h) */
