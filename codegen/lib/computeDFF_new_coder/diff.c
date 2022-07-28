/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * diff.c
 *
 * Code generation for function 'diff'
 *
 */

/* Include files */
#include "diff.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int dimSize;
  int ixLead;
  int iyLead;
  double work_data_idx_0;
  int m;
  double tmp1;
  dimSize = x->size[0];
  if (x->size[0] == 0) {
    y->size[0] = 0;
  } else {
    ixLead = x->size[0] - 1;
    if (ixLead >= 1) {
      ixLead = 1;
    }

    if (ixLead < 1) {
      y->size[0] = 0;
    } else {
      ixLead = y->size[0];
      y->size[0] = x->size[0] - 1;
      emxEnsureCapacity_real_T(y, ixLead);
      if (x->size[0] - 1 != 0) {
        ixLead = 1;
        iyLead = 0;
        work_data_idx_0 = x->data[0];
        for (m = 2; m <= dimSize; m++) {
          tmp1 = x->data[ixLead] - work_data_idx_0;
          work_data_idx_0 = x->data[ixLead];
          ixLead++;
          y->data[iyLead] = tmp1;
          iyLead++;
        }
      }
    }
  }
}

/* End of code generation (diff.c) */
