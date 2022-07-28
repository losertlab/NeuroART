/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sparse1.c
 *
 * Code generation for function 'sparse1'
 *
 */

/* Include files */
#include "sparse1.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "fillIn.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void sparse_full(const emxArray_real_T *this_d, const emxArray_int32_T
                 *this_colidx, const emxArray_int32_T *this_rowidx, int this_m,
                 int this_n, emxArray_real_T *y)
{
  int i;
  int loop_ub;
  int cend;
  int idx;
  i = y->size[0] * y->size[1];
  y->size[0] = this_m;
  y->size[1] = this_n;
  emxEnsureCapacity_real_T(y, i);
  loop_ub = this_m * this_n;
  for (i = 0; i < loop_ub; i++) {
    y->data[i] = 0.0;
  }

  for (loop_ub = 0; loop_ub < this_n; loop_ub++) {
    cend = this_colidx->data[loop_ub + 1] - 1;
    i = this_colidx->data[loop_ub];
    for (idx = i; idx <= cend; idx++) {
      y->data[(this_rowidx->data[idx - 1] + y->size[0] * loop_ub) - 1] =
        this_d->data[idx - 1];
    }
  }
}

void sparse_parenReference(const emxArray_real_T *this_d, const emxArray_int32_T
  *this_colidx, const emxArray_int32_T *this_rowidx, int this_m, const double
  varargin_2_data[], const int varargin_2_size[2], coder_internal_sparse *s)
{
  int sn;
  int nd;
  int cidx;
  int numalloc;
  int i;
  int colNnz;
  int k;
  sn = varargin_2_size[1] - 1;
  nd = 0;
  for (cidx = 0; cidx <= sn; cidx++) {
    numalloc = (int)varargin_2_data[cidx];
    nd = (nd + this_colidx->data[numalloc]) - this_colidx->data[numalloc - 1];
  }

  s->m = this_m;
  s->n = varargin_2_size[1];
  if (nd >= 1) {
    numalloc = nd;
  } else {
    numalloc = 1;
  }

  i = s->d->size[0];
  s->d->size[0] = numalloc;
  emxEnsureCapacity_real_T(s->d, i);
  for (i = 0; i < numalloc; i++) {
    s->d->data[i] = 0.0;
  }

  s->maxnz = numalloc;
  i = s->colidx->size[0];
  s->colidx->size[0] = varargin_2_size[1] + 1;
  emxEnsureCapacity_int32_T(s->colidx, i);
  s->colidx->data[0] = 1;
  i = s->rowidx->size[0];
  s->rowidx->size[0] = numalloc;
  emxEnsureCapacity_int32_T(s->rowidx, i);
  for (i = 0; i < numalloc; i++) {
    s->rowidx->data[i] = 0;
  }

  i = varargin_2_size[1];
  for (numalloc = 0; numalloc < i; numalloc++) {
    s->colidx->data[numalloc + 1] = 1;
  }

  sparse_fillIn(s);
  if (nd != 0) {
    numalloc = 0;
    for (cidx = 0; cidx <= sn; cidx++) {
      nd = this_colidx->data[(int)varargin_2_data[cidx] - 1] - 2;
      colNnz = this_colidx->data[(int)varargin_2_data[cidx]] - this_colidx->
        data[(int)varargin_2_data[cidx] - 1];
      for (k = 0; k < colNnz; k++) {
        i = (nd + k) + 1;
        s->d->data[numalloc] = this_d->data[i];
        s->rowidx->data[numalloc] = this_rowidx->data[i];
        numalloc++;
      }

      s->colidx->data[cidx + 1] = s->colidx->data[cidx] + colNnz;
    }
  }
}

/* End of code generation (sparse1.c) */
