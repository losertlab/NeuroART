/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder_types.h
 *
 * Code generation for function 'computeDFF_new_coder_types'
 *
 */

#ifndef COMPUTEDFF_NEW_CODER_TYPES_H
#define COMPUTEDFF_NEW_CODER_TYPES_H

/* Include files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
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
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
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
  int m;
  int n;
  int maxnz;
} coder_internal_sparse;

#endif                                 /*typedef_coder_internal_sparse*/

#ifndef typedef_c_emxArray_coder_internal_spars
#define typedef_c_emxArray_coder_internal_spars

typedef struct {
  coder_internal_sparse data[1000];
  int size[1];
} c_emxArray_coder_internal_spars;

#endif                                 /*typedef_c_emxArray_coder_internal_spars*/

#ifndef typedef_cell_wrap_3
#define typedef_cell_wrap_3

typedef struct {
  emxArray_real_T *f1;
} cell_wrap_3;

#endif                                 /*typedef_cell_wrap_3*/

#ifndef typedef_cell_wrap_5
#define typedef_cell_wrap_5

typedef struct {
  emxArray_int32_T *f1;
} cell_wrap_5;

#endif                                 /*typedef_cell_wrap_5*/

#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

struct emxArray_boolean_T
{
  boolean_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
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
  int m;
  int n;
  int maxnz;
} coder_internal_sparse_1;

#endif                                 /*typedef_coder_internal_sparse_1*/

#ifndef typedef_d_emxArray_coder_internal_spars
#define typedef_d_emxArray_coder_internal_spars

typedef struct {
  coder_internal_sparse_1 data[1000];
  int size[1];
} d_emxArray_coder_internal_spars;

#endif                                 /*typedef_d_emxArray_coder_internal_spars*/

#ifndef typedef_emxArray_cell_wrap_3_360
#define typedef_emxArray_cell_wrap_3_360

typedef struct {
  cell_wrap_3 data[360];
  int size[1];
} emxArray_cell_wrap_3_360;

#endif                                 /*typedef_emxArray_cell_wrap_3_360*/

#ifndef struct_emxArray_int8_T
#define struct_emxArray_int8_T

struct emxArray_int8_T
{
  signed char *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_int8_T*/

#ifndef typedef_emxArray_int8_T
#define typedef_emxArray_int8_T

typedef struct emxArray_int8_T emxArray_int8_T;

#endif                                 /*typedef_emxArray_int8_T*/

#ifndef struct_emxArray_uint16_T
#define struct_emxArray_uint16_T

struct emxArray_uint16_T
{
  unsigned short *data;
  int *size;
  int allocatedSize;
  int numDimensions;
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
#endif

/* End of code generation (computeDFF_new_coder_types.h) */
