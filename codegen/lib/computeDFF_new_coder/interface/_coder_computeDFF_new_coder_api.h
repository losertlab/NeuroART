/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_computeDFF_new_coder_api.h
 *
 * Code generation for function '_coder_computeDFF_new_coder_api'
 *
 */

#ifndef _CODER_COMPUTEDFF_NEW_CODER_API_H
#define _CODER_COMPUTEDFF_NEW_CODER_API_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */
#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int32_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_int32_T*/

#ifndef typedef_emxArray_int32_T
#define typedef_emxArray_int32_T

typedef struct emxArray_int32_T emxArray_int32_T;

#endif                                 /*typedef_emxArray_int32_T*/

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

#ifndef typedef_coder_internal_sparse
#define typedef_coder_internal_sparse

typedef struct {
  emxArray_real_T *d;
  emxArray_int32_T *colidx;
  emxArray_int32_T *rowidx;
  int32_T m;
  int32_T n;
  int32_T maxnz;
} coder_internal_sparse;

#endif                                 /*typedef_coder_internal_sparse*/

#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

struct emxArray_boolean_T
{
  boolean_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_boolean_T*/

#ifndef typedef_emxArray_boolean_T
#define typedef_emxArray_boolean_T

typedef struct emxArray_boolean_T emxArray_boolean_T;

#endif                                 /*typedef_emxArray_boolean_T*/

#ifndef typedef_coder_internal_sparse_1
#define typedef_coder_internal_sparse_1

typedef struct {
  emxArray_boolean_T *d;
  emxArray_int32_T *colidx;
  emxArray_int32_T *rowidx;
  int32_T m;
  int32_T n;
  int32_T maxnz;
} coder_internal_sparse_1;

#endif                                 /*typedef_coder_internal_sparse_1*/

#ifndef struct_emxArray_uint16_T
#define struct_emxArray_uint16_T

struct emxArray_uint16_T
{
  uint16_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_uint16_T*/

#ifndef typedef_emxArray_uint16_T
#define typedef_emxArray_uint16_T

typedef struct emxArray_uint16_T emxArray_uint16_T;

#endif                                 /*typedef_emxArray_uint16_T*/

#ifndef struct_smqG7yl6ESdT1cDP2FVAnvC_tag
#define struct_smqG7yl6ESdT1cDP2FVAnvC_tag

struct smqG7yl6ESdT1cDP2FVAnvC_tag
{
  emxArray_real_T *smooth;
  emxArray_real_T *raw;
};

#endif                                 /*struct_smqG7yl6ESdT1cDP2FVAnvC_tag*/

#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct smqG7yl6ESdT1cDP2FVAnvC_tag struct0_T;

#endif                                 /*typedef_struct0_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void computeDFF_new_coder(emxArray_uint16_T *RegImg, real_T frameRate,
  real_T pos_lst_data[], int32_T pos_lst_size[2], real_T r_pixels, real_T
  winsize, real_T percentBaselineSub, real_T neuropilSubPercent, emxArray_real_T
  *norm_meanIMG, coder_internal_sparse roiBW2_sp_data[], int32_T roiBW2_sp_size
  [1], coder_internal_sparse_1 npBWout_sp_data[], int32_T npBWout_sp_size[1],
  struct0_T *DFF, emxArray_real_T *normF, emxArray_real_T *npSubFluoSmooth,
  real_T xcRaw_data[], int32_T xcRaw_size[1], real_T ycRaw_data[], int32_T
  ycRaw_size[1], real_T *minNpSubFluo, real_T *maxAdjF);
extern void computeDFF_new_coder_api(const mxArray * const prhs[7], int32_T nlhs,
  const mxArray *plhs[10]);
extern void computeDFF_new_coder_atexit(void);
extern void computeDFF_new_coder_initialize(void);
extern void computeDFF_new_coder_terminate(void);
extern void computeDFF_new_coder_xil_shutdown(void);
extern void computeDFF_new_coder_xil_terminate(void);

#endif

/* End of code generation (_coder_computeDFF_new_coder_api.h) */
