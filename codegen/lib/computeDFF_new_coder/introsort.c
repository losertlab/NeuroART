/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * introsort.c
 *
 * Code generation for function 'introsort'
 *
 */

/* Include files */
#include "introsort.h"
#include "computeDFF_new_coder.h"
#include "heapsort.h"
#include "insertionsort.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Type Definitions */
#ifndef typedef_struct_T
#define typedef_struct_T

typedef struct {
  int xstart;
  int xend;
  int depth;
} struct_T;

#endif                                 /*typedef_struct_T*/

/* Function Definitions */
void introsort(emxArray_int32_T *x, int xend, const cell_wrap_5
               cmp_tunableEnvironment[2])
{
  int pmax;
  int pmin;
  boolean_T exitg1;
  int MAXDEPTH;
  int p;
  struct_T frame;
  int pow2p;
  int i;
  struct_T st_d_data[120];
  int b_i;
  boolean_T varargout_1;
  int exitg2;
  int exitg3;
  if (1 < xend) {
    if (xend <= 32) {
      insertionsort(x, 1, xend, cmp_tunableEnvironment);
    } else {
      pmax = 31;
      pmin = 0;
      exitg1 = false;
      while ((!exitg1) && (pmax - pmin > 1)) {
        p = (pmin + pmax) >> 1;
        pow2p = 1 << p;
        if (pow2p == xend) {
          pmax = p;
          exitg1 = true;
        } else if (pow2p > xend) {
          pmax = p;
        } else {
          pmin = p;
        }
      }

      MAXDEPTH = (pmax - 1) << 1;
      frame.xstart = 1;
      frame.xend = xend;
      frame.depth = 0;
      pmax = MAXDEPTH << 1;
      for (i = 0; i < pmax; i++) {
        st_d_data[i] = frame;
      }

      st_d_data[0] = frame;
      p = 1;
      while (p > 0) {
        frame = st_d_data[p - 1];
        p--;
        i = frame.xend - frame.xstart;
        if (i + 1 <= 32) {
          insertionsort(x, frame.xstart, frame.xend, cmp_tunableEnvironment);
        } else if (frame.depth == MAXDEPTH) {
          b_heapsort(x, frame.xstart, frame.xend, cmp_tunableEnvironment);
        } else {
          pmax = (frame.xstart + i / 2) - 1;
          b_i = frame.xstart - 1;
          i = x->data[b_i] - 1;
          pmin = cmp_tunableEnvironment[0].f1->data[x->data[pmax] - 1];
          varargout_1 = ((pmin < cmp_tunableEnvironment[0].f1->data[i]) ||
                         ((pmin == cmp_tunableEnvironment[0].f1->data[i]) &&
                          (cmp_tunableEnvironment[1].f1->data[x->data[pmax] - 1]
                           < cmp_tunableEnvironment[1].f1->data[i])));
          if (varargout_1) {
            pmin = x->data[b_i];
            x->data[b_i] = x->data[pmax];
            x->data[pmax] = pmin;
          }

          i = frame.xend - 1;
          varargout_1 = ((cmp_tunableEnvironment[0].f1->data[x->data[i] - 1] <
                          cmp_tunableEnvironment[0].f1->data[x->data[b_i] - 1]) ||
                         ((cmp_tunableEnvironment[0].f1->data[x->data[i] - 1] ==
                           cmp_tunableEnvironment[0].f1->data[x->data[b_i] - 1])
                          && (cmp_tunableEnvironment[1].f1->data[x->data[i] - 1]
                              < cmp_tunableEnvironment[1].f1->data[x->data[b_i]
                              - 1])));
          if (varargout_1) {
            pmin = x->data[b_i];
            x->data[b_i] = x->data[i];
            x->data[i] = pmin;
          }

          varargout_1 = ((cmp_tunableEnvironment[0].f1->data[x->data[i] - 1] <
                          cmp_tunableEnvironment[0].f1->data[x->data[pmax] - 1])
                         || ((cmp_tunableEnvironment[0].f1->data[x->data[i] - 1]
                              == cmp_tunableEnvironment[0].f1->data[x->data[pmax]
                              - 1]) && (cmp_tunableEnvironment[1].f1->data
            [x->data[i] - 1] < cmp_tunableEnvironment[1].f1->data[x->data[pmax]
            - 1])));
          if (varargout_1) {
            pmin = x->data[pmax];
            x->data[pmax] = x->data[i];
            x->data[i] = pmin;
          }

          pow2p = x->data[pmax] - 1;
          i = frame.xend - 2;
          x->data[pmax] = x->data[i];
          x->data[i] = pow2p + 1;
          pmax = i;
          do {
            exitg2 = 0;
            b_i++;
            do {
              exitg3 = 0;
              pmin = cmp_tunableEnvironment[0].f1->data[x->data[b_i] - 1];
              varargout_1 = ((pmin < cmp_tunableEnvironment[0].f1->data[pow2p]) ||
                             ((pmin == cmp_tunableEnvironment[0].f1->data[pow2p])
                              && (cmp_tunableEnvironment[1].f1->data[x->data[b_i]
                                  - 1] < cmp_tunableEnvironment[1].f1->
                                  data[pow2p])));
              if (varargout_1) {
                b_i++;
              } else {
                exitg3 = 1;
              }
            } while (exitg3 == 0);

            pmax--;
            do {
              exitg3 = 0;
              pmin = cmp_tunableEnvironment[0].f1->data[x->data[pmax] - 1];
              varargout_1 = ((cmp_tunableEnvironment[0].f1->data[pow2p] < pmin) ||
                             ((cmp_tunableEnvironment[0].f1->data[pow2p] == pmin)
                              && (cmp_tunableEnvironment[1].f1->data[pow2p] <
                                  cmp_tunableEnvironment[1].f1->data[x->
                                  data[pmax] - 1])));
              if (varargout_1) {
                pmax--;
              } else {
                exitg3 = 1;
              }
            } while (exitg3 == 0);

            if (b_i + 1 >= pmax + 1) {
              exitg2 = 1;
            } else {
              pmin = x->data[b_i];
              x->data[b_i] = x->data[pmax];
              x->data[pmax] = pmin;
            }
          } while (exitg2 == 0);

          x->data[i] = x->data[b_i];
          x->data[b_i] = pow2p + 1;
          if (b_i + 2 < frame.xend) {
            st_d_data[p].xstart = b_i + 2;
            st_d_data[p].xend = frame.xend;
            st_d_data[p].depth = frame.depth + 1;
            p++;
          }

          if (frame.xstart < b_i + 1) {
            st_d_data[p].xstart = frame.xstart;
            st_d_data[p].xend = b_i + 1;
            st_d_data[p].depth = frame.depth + 1;
            p++;
          }
        }
      }
    }
  }
}

/* End of code generation (introsort.c) */
