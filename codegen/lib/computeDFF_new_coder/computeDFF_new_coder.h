/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder.h
 *
 * Code generation for function 'computeDFF_new_coder'
 *
 */

#ifndef COMPUTEDFF_NEW_CODER_H
#define COMPUTEDFF_NEW_CODER_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "computeDFF_new_coder_types.h"

/* Function Declarations */
extern void computeDFF_new_coder(const emxArray_uint16_T *RegImg, double
  frameRate, const double pos_lst_data[], const int pos_lst_size[2], double
  r_pixels, double winsize, double percentBaselineSub, double neuropilSubPercent,
  emxArray_real_T *norm_meanIMG, coder_internal_sparse roiBW2_sp_data[], int
  roiBW2_sp_size[1], coder_internal_sparse_1 npBWout_sp_data[], int
  npBWout_sp_size[1], struct0_T *DFF, emxArray_real_T *normF, emxArray_real_T
  *npSubFluoSmooth, double xcRaw_data[], int xcRaw_size[1], double ycRaw_data[],
  int ycRaw_size[1], double *minNpSubFluo, double *maxAdjF);

#endif

/* End of code generation (computeDFF_new_coder.h) */
