/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * slideWinSub.c
 *
 * Code generation for function 'slideWinSub'
 *
 */

/* Include files */
#include "slideWinSub.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void slideWinSub(const emxArray_real_T *F, double winsize, double percent,
                 emxArray_real_T *dFF)
{
  int nFrames;
  short unnamed_idx_0;
  short unnamed_idx_1;
  int i;
  int vlen;
  int j;
  int k;
  double lWin;
  double rWin;
  double percentWin;
  int i1;
  int b_k;
  int tWin_size[2];
  static double tWin_data[7500];

  /*  Sliding window dF/F calculation */
  /*  Inputs: F       - raw fluorecense */
  /*          winsize - baseline window size */
  /*          percent - lower percent of baseline values to average */
  /*  Output: dFF     - relative change in fluorescense in percent */
  nFrames = F->size[1];
  unnamed_idx_0 = (short)F->size[0];
  unnamed_idx_1 = (short)F->size[1];
  i = dFF->size[0] * dFF->size[1];
  dFF->size[0] = unnamed_idx_0;
  dFF->size[1] = unnamed_idx_1;
  emxEnsureCapacity_real_T(dFF, i);
  vlen = unnamed_idx_0 * unnamed_idx_1;
  for (i = 0; i < vlen; i++) {
    dFF->data[i] = 0.0;
  }

  /* M = zeros(1, nrois); */
  /* SD = zeros(1, nrois); */
  i = F->size[0];
  for (j = 0; j < i; j++) {
    for (k = 0; k < nFrames; k++) {
      lWin = 1.0;
      rWin = fmin(((double)k + 1.0) + winsize, nFrames);
      if ((double)k + 1.0 > winsize) {
        lWin = fmax(1.0, ((double)k + 1.0) - 2.0 * winsize);
        rWin = (double)k + 1.0;
      }

      percentWin = floor(percent / 100.0 * (rWin - lWin));
      if (lWin > rWin) {
        i1 = 0;
        b_k = 0;
      } else {
        i1 = (int)lWin - 1;
        b_k = (int)rWin;
      }

      tWin_size[0] = 1;
      vlen = b_k - i1;
      tWin_size[1] = vlen;
      for (b_k = 0; b_k < vlen; b_k++) {
        tWin_data[b_k] = F->data[j + F->size[0] * (i1 + b_k)];
      }

      sort(tWin_data, tWin_size);
      if (1.0 > percentWin) {
        i1 = 0;
      } else {
        i1 = (int)percentWin;
      }

      vlen = (short)i1;
      if ((short)i1 == 0) {
        lWin = 0.0;
      } else {
        lWin = tWin_data[0];
        for (b_k = 2; b_k <= vlen; b_k++) {
          lWin += tWin_data[b_k - 1];
        }
      }

      lWin /= (double)(short)i1;
      dFF->data[j + dFF->size[0] * k] = 100.0 * (F->data[j + F->size[0] * k] -
        lWin) / lWin;
    }

    /*      for k = 1 : nFrames */
    /*              lWin = max(1,k-winsize); */
    /*              rWin = min(k+winsize, nFrames); */
    /*              percentWin = floor(percent/100*(rWin-lWin)); */
    /*              tWin = sort(F(j,(lWin:rWin)),'ascend'); */
    /*              F0 = mean(tWin(1:percentWin)); */
    /*              dFF(j,k) = 100 * (F(j,k)- F0) / F0;    */
    /*      end */
    /* twin = sort(nDFF(j,:)); */
    /* pcentwin = floor(percent/100*(nframes)); */
    /* M(j) = mean(twin(1:pcentwin)); */
    /* SD(j) = std(twin(1:pcentwin)); */
  }
}

/* End of code generation (slideWinSub.c) */
