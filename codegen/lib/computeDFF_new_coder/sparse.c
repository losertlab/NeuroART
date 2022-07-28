/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse.c
 *
 * Code generation for function 'sparse'
 *
 */

/* Include files */
#include "sparse.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "fillIn.h"
#include "introsort.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void b_sparse(const emxArray_real_T *varargin_1, emxArray_real_T *y_d,
              emxArray_int32_T *y_colidx, emxArray_int32_T *y_rowidx, int *y_m,
              int *y_n, int *y_maxnz)
{
  int mInt;
  int nInt;
  int ctr;
  int row;
  int k;
  int numalloc;
  double xrc;
  mInt = varargin_1->size[0];
  nInt = varargin_1->size[1];
  ctr = 0;
  row = varargin_1->size[0] * varargin_1->size[1];
  for (k = 0; k < row; k++) {
    if (varargin_1->data[k] != 0.0) {
      ctr++;
    }
  }

  *y_m = varargin_1->size[0];
  *y_n = varargin_1->size[1];
  if (ctr >= 1) {
    numalloc = ctr;
  } else {
    numalloc = 1;
  }

  row = y_d->size[0];
  y_d->size[0] = numalloc;
  emxEnsureCapacity_real_T(y_d, row);
  for (row = 0; row < numalloc; row++) {
    y_d->data[row] = 0.0;
  }

  row = y_colidx->size[0];
  y_colidx->size[0] = varargin_1->size[1] + 1;
  emxEnsureCapacity_int32_T(y_colidx, row);
  ctr = varargin_1->size[1];
  for (row = 0; row <= ctr; row++) {
    y_colidx->data[row] = 0;
  }

  y_colidx->data[0] = 1;
  row = y_rowidx->size[0];
  y_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(y_rowidx, row);
  for (row = 0; row < numalloc; row++) {
    y_rowidx->data[row] = 0;
  }

  y_rowidx->data[0] = 1;
  ctr = 0;
  for (k = 0; k < nInt; k++) {
    for (row = 0; row < mInt; row++) {
      xrc = varargin_1->data[row + varargin_1->size[0] * k];
      if (xrc != 0.0) {
        y_rowidx->data[ctr] = row + 1;
        y_d->data[ctr] = xrc;
        ctr++;
      }
    }

    y_colidx->data[k + 1] = ctr + 1;
  }

  *y_maxnz = numalloc;
}

void c_sparse(const emxArray_boolean_T *varargin_1, emxArray_boolean_T *y_d,
              emxArray_int32_T *y_colidx, emxArray_int32_T *y_rowidx, int *y_m,
              int *y_n, int *y_maxnz)
{
  int mInt;
  int nInt;
  int ctr;
  int row;
  int k;
  int numalloc;
  mInt = varargin_1->size[0];
  nInt = varargin_1->size[1];
  ctr = 0;
  row = varargin_1->size[0] * varargin_1->size[1];
  for (k = 0; k < row; k++) {
    if (varargin_1->data[k]) {
      ctr++;
    }
  }

  *y_m = varargin_1->size[0];
  *y_n = varargin_1->size[1];
  if (ctr >= 1) {
    numalloc = ctr;
  } else {
    numalloc = 1;
  }

  row = y_d->size[0];
  y_d->size[0] = numalloc;
  emxEnsureCapacity_boolean_T(y_d, row);
  for (row = 0; row < numalloc; row++) {
    y_d->data[row] = false;
  }

  row = y_colidx->size[0];
  y_colidx->size[0] = varargin_1->size[1] + 1;
  emxEnsureCapacity_int32_T(y_colidx, row);
  ctr = varargin_1->size[1];
  for (row = 0; row <= ctr; row++) {
    y_colidx->data[row] = 0;
  }

  y_colidx->data[0] = 1;
  row = y_rowidx->size[0];
  y_rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(y_rowidx, row);
  for (row = 0; row < numalloc; row++) {
    y_rowidx->data[row] = 0;
  }

  y_rowidx->data[0] = 1;
  ctr = 0;
  for (k = 0; k < nInt; k++) {
    for (row = 0; row < mInt; row++) {
      if (varargin_1->data[row + varargin_1->size[0] * k]) {
        y_rowidx->data[ctr] = row + 1;
        y_d->data[ctr] = true;
        ctr++;
      }
    }

    y_colidx->data[k + 1] = ctr + 1;
  }

  *y_maxnz = numalloc;
}

void sparse(const emxArray_real_T *varargin_1, const emxArray_real_T *varargin_2,
            double varargin_4, double varargin_5, coder_internal_sparse *y)
{
  emxArray_int32_T *ridxInt;
  int nc;
  int ns;
  int k;
  emxArray_int32_T *cidxInt;
  emxArray_int32_T *sortedIndices;
  cell_wrap_5 this_tunableEnvironment[2];
  emxArray_int32_T *t;
  int ny;
  int y_tmp;
  emxInit_int32_T(&ridxInt, 1);
  nc = varargin_2->size[0];
  ns = varargin_1->size[0];
  k = ridxInt->size[0];
  ridxInt->size[0] = varargin_1->size[0];
  emxEnsureCapacity_int32_T(ridxInt, k);
  for (k = 0; k < ns; k++) {
    ridxInt->data[k] = (int)varargin_1->data[k];
  }

  emxInit_int32_T(&cidxInt, 1);
  ns = varargin_2->size[0];
  k = cidxInt->size[0];
  cidxInt->size[0] = varargin_2->size[0];
  emxEnsureCapacity_int32_T(cidxInt, k);
  for (k = 0; k < ns; k++) {
    cidxInt->data[k] = (int)varargin_2->data[k];
  }

  emxInit_int32_T(&sortedIndices, 1);
  k = sortedIndices->size[0];
  sortedIndices->size[0] = varargin_2->size[0];
  emxEnsureCapacity_int32_T(sortedIndices, k);
  for (k = 0; k < nc; k++) {
    sortedIndices->data[k] = k + 1;
  }

  emxInitMatrix_cell_wrap_5(this_tunableEnvironment);
  k = this_tunableEnvironment[0].f1->size[0];
  this_tunableEnvironment[0].f1->size[0] = cidxInt->size[0];
  emxEnsureCapacity_int32_T(this_tunableEnvironment[0].f1, k);
  ns = cidxInt->size[0];
  for (k = 0; k < ns; k++) {
    this_tunableEnvironment[0].f1->data[k] = cidxInt->data[k];
  }

  k = this_tunableEnvironment[1].f1->size[0];
  this_tunableEnvironment[1].f1->size[0] = ridxInt->size[0];
  emxEnsureCapacity_int32_T(this_tunableEnvironment[1].f1, k);
  ns = ridxInt->size[0];
  for (k = 0; k < ns; k++) {
    this_tunableEnvironment[1].f1->data[k] = ridxInt->data[k];
  }

  emxInit_int32_T(&t, 1);
  introsort(sortedIndices, cidxInt->size[0], this_tunableEnvironment);
  ny = cidxInt->size[0];
  k = t->size[0];
  t->size[0] = cidxInt->size[0];
  emxEnsureCapacity_int32_T(t, k);
  ns = cidxInt->size[0];
  emxFreeMatrix_cell_wrap_5(this_tunableEnvironment);
  for (k = 0; k < ns; k++) {
    t->data[k] = cidxInt->data[k];
  }

  for (k = 0; k < ny; k++) {
    cidxInt->data[k] = t->data[sortedIndices->data[k] - 1];
  }

  ny = ridxInt->size[0];
  k = t->size[0];
  t->size[0] = ridxInt->size[0];
  emxEnsureCapacity_int32_T(t, k);
  ns = ridxInt->size[0];
  for (k = 0; k < ns; k++) {
    t->data[k] = ridxInt->data[k];
  }

  for (k = 0; k < ny; k++) {
    ridxInt->data[k] = t->data[sortedIndices->data[k] - 1];
  }

  emxFree_int32_T(&t);
  emxFree_int32_T(&sortedIndices);
  y->m = (int)varargin_4;
  y_tmp = (int)varargin_5;
  y->n = y_tmp;
  if (varargin_2->size[0] >= 1) {
    ns = varargin_2->size[0];
  } else {
    ns = 1;
  }

  k = y->d->size[0];
  y->d->size[0] = ns;
  emxEnsureCapacity_real_T(y->d, k);
  for (k = 0; k < ns; k++) {
    y->d->data[k] = 0.0;
  }

  y->maxnz = ns;
  k = y->colidx->size[0];
  y->colidx->size[0] = y_tmp + 1;
  emxEnsureCapacity_int32_T(y->colidx, k);
  y->colidx->data[0] = 1;
  k = y->rowidx->size[0];
  y->rowidx->size[0] = ns;
  emxEnsureCapacity_int32_T(y->rowidx, k);
  for (k = 0; k < ns; k++) {
    y->rowidx->data[k] = 0;
  }

  ns = 0;
  for (k = 0; k < y_tmp; k++) {
    ny = k + 1;
    while ((ns + 1 <= nc) && (cidxInt->data[ns] == ny)) {
      y->rowidx->data[ns] = ridxInt->data[ns];
      ns++;
    }

    y->colidx->data[ny] = ns + 1;
  }

  emxFree_int32_T(&cidxInt);
  emxFree_int32_T(&ridxInt);
  for (k = 0; k < nc; k++) {
    y->d->data[k] = 1.0;
  }

  sparse_fillIn(y);
}

/* End of code generation (sparse.c) */
