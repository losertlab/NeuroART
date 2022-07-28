/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sort.c
 *
 * Code generation for function 'sort'
 *
 */

/* Include files */
#include "sort.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include <string.h>

/* Function Definitions */
void sort(double x_data[], const int x_size[2])
{
  int ib;
  int idx_data[7500];
  int n;
  int b_n;
  double x4[4];
  short idx4[4];
  int iwork_data[7500];
  static double xwork_data[7500];
  int nNaNs;
  int k;
  int i4;
  int idx_tmp;
  signed char perm[4];
  int quartetOffset;
  int i1;
  int i2;
  double d;
  double d1;
  ib = x_size[1];
  if (0 <= ib - 1) {
    memset(&idx_data[0], 0, ib * sizeof(int));
  }

  if (x_size[1] != 0) {
    n = x_size[1];
    b_n = x_size[1];
    x4[0] = 0.0;
    idx4[0] = 0;
    x4[1] = 0.0;
    idx4[1] = 0;
    x4[2] = 0.0;
    idx4[2] = 0;
    x4[3] = 0.0;
    idx4[3] = 0;
    ib = x_size[1];
    if (0 <= ib - 1) {
      memset(&iwork_data[0], 0, ib * sizeof(int));
    }

    ib = x_size[1];
    if (0 <= ib - 1) {
      memset(&xwork_data[0], 0, ib * sizeof(double));
    }

    nNaNs = 0;
    ib = -1;
    for (k = 0; k < b_n; k++) {
      if (rtIsNaN(x_data[k])) {
        idx_tmp = (b_n - nNaNs) - 1;
        idx_data[idx_tmp] = k + 1;
        xwork_data[idx_tmp] = x_data[k];
        nNaNs++;
      } else {
        ib++;
        idx4[ib] = (short)(k + 1);
        x4[ib] = x_data[k];
        if (ib + 1 == 4) {
          quartetOffset = k - nNaNs;
          if (x4[0] <= x4[1]) {
            i1 = 1;
            i2 = 2;
          } else {
            i1 = 2;
            i2 = 1;
          }

          if (x4[2] <= x4[3]) {
            ib = 3;
            i4 = 4;
          } else {
            ib = 4;
            i4 = 3;
          }

          d = x4[i1 - 1];
          d1 = x4[ib - 1];
          if (d <= d1) {
            d = x4[i2 - 1];
            if (d <= d1) {
              perm[0] = (signed char)i1;
              perm[1] = (signed char)i2;
              perm[2] = (signed char)ib;
              perm[3] = (signed char)i4;
            } else if (d <= x4[i4 - 1]) {
              perm[0] = (signed char)i1;
              perm[1] = (signed char)ib;
              perm[2] = (signed char)i2;
              perm[3] = (signed char)i4;
            } else {
              perm[0] = (signed char)i1;
              perm[1] = (signed char)ib;
              perm[2] = (signed char)i4;
              perm[3] = (signed char)i2;
            }
          } else {
            d1 = x4[i4 - 1];
            if (d <= d1) {
              if (x4[i2 - 1] <= d1) {
                perm[0] = (signed char)ib;
                perm[1] = (signed char)i1;
                perm[2] = (signed char)i2;
                perm[3] = (signed char)i4;
              } else {
                perm[0] = (signed char)ib;
                perm[1] = (signed char)i1;
                perm[2] = (signed char)i4;
                perm[3] = (signed char)i2;
              }
            } else {
              perm[0] = (signed char)ib;
              perm[1] = (signed char)i4;
              perm[2] = (signed char)i1;
              perm[3] = (signed char)i2;
            }
          }

          idx_tmp = perm[0] - 1;
          idx_data[quartetOffset - 3] = idx4[idx_tmp];
          i2 = perm[1] - 1;
          idx_data[quartetOffset - 2] = idx4[i2];
          ib = perm[2] - 1;
          idx_data[quartetOffset - 1] = idx4[ib];
          i1 = perm[3] - 1;
          idx_data[quartetOffset] = idx4[i1];
          x_data[quartetOffset - 3] = x4[idx_tmp];
          x_data[quartetOffset - 2] = x4[i2];
          x_data[quartetOffset - 1] = x4[ib];
          x_data[quartetOffset] = x4[i1];
          ib = -1;
        }
      }
    }

    i4 = (b_n - nNaNs) - 1;
    if (ib + 1 > 0) {
      perm[1] = 0;
      perm[2] = 0;
      perm[3] = 0;
      if (ib + 1 == 1) {
        perm[0] = 1;
      } else if (ib + 1 == 2) {
        if (x4[0] <= x4[1]) {
          perm[0] = 1;
          perm[1] = 2;
        } else {
          perm[0] = 2;
          perm[1] = 1;
        }
      } else if (x4[0] <= x4[1]) {
        if (x4[1] <= x4[2]) {
          perm[0] = 1;
          perm[1] = 2;
          perm[2] = 3;
        } else if (x4[0] <= x4[2]) {
          perm[0] = 1;
          perm[1] = 3;
          perm[2] = 2;
        } else {
          perm[0] = 3;
          perm[1] = 1;
          perm[2] = 2;
        }
      } else if (x4[0] <= x4[2]) {
        perm[0] = 2;
        perm[1] = 1;
        perm[2] = 3;
      } else if (x4[1] <= x4[2]) {
        perm[0] = 2;
        perm[1] = 3;
        perm[2] = 1;
      } else {
        perm[0] = 3;
        perm[1] = 2;
        perm[2] = 1;
      }

      for (k = 0; k <= ib; k++) {
        idx_tmp = perm[k] - 1;
        i2 = (i4 - ib) + k;
        idx_data[i2] = idx4[idx_tmp];
        x_data[i2] = x4[idx_tmp];
      }
    }

    ib = (nNaNs >> 1) + 1;
    for (k = 0; k <= ib - 2; k++) {
      i1 = (i4 + k) + 1;
      i2 = (short)idx_data[i1];
      idx_tmp = (b_n - k) - 1;
      idx_data[i1] = (short)idx_data[idx_tmp];
      idx_data[idx_tmp] = i2;
      x_data[i1] = xwork_data[idx_tmp];
      x_data[idx_tmp] = xwork_data[i1];
    }

    if ((nNaNs & 1) != 0) {
      ib += i4;
      x_data[ib] = xwork_data[ib];
    }

    i2 = n - nNaNs;
    ib = 2;
    if (i2 > 1) {
      if (n >= 256) {
        i1 = i2 >> 8;
        if (i1 > 0) {
          for (ib = 0; ib < i1; ib++) {
            merge_pow2_block(idx_data, x_data, ib << 8);
          }

          ib = i1 << 8;
          i1 = i2 - ib;
          if (i1 > 0) {
            merge_block(idx_data, x_data, ib, i1, 2, iwork_data, xwork_data);
          }

          ib = 8;
        }
      }

      merge_block(idx_data, x_data, 0, i2, ib, iwork_data, xwork_data);
    }
  }
}

/* End of code generation (sort.c) */
