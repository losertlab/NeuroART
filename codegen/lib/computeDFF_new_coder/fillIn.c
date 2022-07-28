/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * fillIn.c
 *
 * Code generation for function 'fillIn'
 *
 */

/* Include files */
#include "fillIn.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void sparse_fillIn(coder_internal_sparse *this)
{
  int idx;
  int i;
  int c;
  int ridx;
  int exitg1;
  int i1;
  double val;
  int currRowIdx_tmp;
  idx = 1;
  i = this->colidx->size[0];
  for (c = 0; c <= i - 2; c++) {
    ridx = this->colidx->data[c];
    this->colidx->data[c] = idx;
    do {
      exitg1 = 0;
      i1 = this->colidx->data[c + 1];
      if (ridx < i1) {
        val = 0.0;
        currRowIdx_tmp = this->rowidx->data[ridx - 1];
        while ((ridx < i1) && (this->rowidx->data[ridx - 1] == currRowIdx_tmp))
        {
          val += this->d->data[ridx - 1];
          ridx++;
        }

        if (val != 0.0) {
          this->d->data[idx - 1] = val;
          this->rowidx->data[idx - 1] = currRowIdx_tmp;
          idx++;
        }
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  this->colidx->data[this->colidx->size[0] - 1] = idx;
}

/* End of code generation (fillIn.c) */
