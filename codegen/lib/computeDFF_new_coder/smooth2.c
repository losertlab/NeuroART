/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * smooth2.c
 *
 * Code generation for function 'smooth2'
 *
 */

/* Include files */
#include "smooth2.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static int div_s32_floor(int numerator, int denominator);

/* Function Definitions */
static int div_s32_floor(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  unsigned int tempAbsQuotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      absNumerator = ~(unsigned int)numerator + 1U;
    } else {
      absNumerator = (unsigned int)numerator;
    }

    if (denominator < 0) {
      absDenominator = ~(unsigned int)denominator + 1U;
    } else {
      absDenominator = (unsigned int)denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0U) {
        tempAbsQuotient++;
      }

      quotient = -(int)tempAbsQuotient;
    } else {
      quotient = (int)tempAbsQuotient;
    }
  }

  return quotient;
}

void smooth2(double tmp_data[], const int tmp_size[2], double win, double T,
             double F1_data[], int F1_size[2])
{
  double r;
  int loop_ub;
  int i;
  double wf_data[10];
  int j;
  double b_j;
  int i1;
  int k;
  int nsamps;
  double b_wf_data[5];
  double b_tmp_data[7511];
  double x_data[10];

  /* cenflag ->  1 == symmetric with peak in middle, 0 asym with peak in 1st pos */
  /* win size should be odd */
  if (rtIsNaN(win) || rtIsInf(win)) {
    r = rtNaN;
  } else if (win == 0.0) {
    r = 0.0;
  } else {
    r = fmod(win, 2.0);
    if (r == 0.0) {
      r = 0.0;
    }
  }

  if (r == 0.0) {
    /* window is even so add 1 */
    win++;
  }

  /* create window */
  r = floor(win / 2.0);
  loop_ub = (int)win;
  for (i = 0; i < loop_ub; i++) {
    wf_data[i] = 1.0;
  }

  i = (int)(win + (1.0 - (r + 1.0)));
  for (j = 0; j < i; j++) {
    b_j = (r + 1.0) + (double)j;
    wf_data[(int)b_j - 1] = wf_data[(int)(b_j - 1.0) - 1] * exp(-1.5 * T);
  }

  if ((r + 1.0) + 1.0 > loop_ub) {
    i = 0;
    i1 = 1;
    k = -1;
  } else {
    i = loop_ub - 1;
    i1 = -1;
    k = (int)((r + 1.0) + 1.0) - 1;
  }

  j = div_s32_floor(k - i, i1);
  nsamps = j + 1;
  for (k = 0; k <= j; k++) {
    b_wf_data[k] = wf_data[i + i1 * k];
  }

  if (0 <= nsamps - 1) {
    memcpy(&wf_data[0], &b_wf_data[0], nsamps * sizeof(double));
  }

  if (loop_ub == 0) {
    r = 0.0;
  } else {
    r = wf_data[0];
    for (k = 2; k <= loop_ub; k++) {
      r += wf_data[k - 1];
    }
  }

  loop_ub--;
  for (i = 0; i <= loop_ub; i++) {
    wf_data[i] /= r;
  }

  /* figure; plot(wf);    */
  /* multiply raw data at each frame by window */
  nsamps = tmp_size[1];
  k = (int)(win + 1.0);
  j = tmp_size[1] + k;
  loop_ub = tmp_size[1];
  if (0 <= loop_ub - 1) {
    memcpy(&b_tmp_data[0], &tmp_data[0], loop_ub * sizeof(double));
  }

  for (i = 0; i < k; i++) {
    b_tmp_data[i + tmp_size[1]] = tmp_data[i];
  }

  if (0 <= j - 1) {
    memcpy(&tmp_data[0], &b_tmp_data[0], j * sizeof(double));
  }

  F1_size[0] = 1;
  F1_size[1] = tmp_size[1];

  /*  added to allow mex compilation */
  for (j = 0; j < nsamps; j++) {
    r = (((double)j + 1.0) + win) - 1.0;
    if ((double)j + 1.0 > r) {
      i = 0;
      i1 = 0;
    } else {
      i = j;
      i1 = (int)r;
    }

    loop_ub = i1 - i;
    for (i1 = 0; i1 < loop_ub; i1++) {
      x_data[i1] = tmp_data[i + i1] * wf_data[i1];
    }

    if (loop_ub == 0) {
      r = 0.0;
    } else {
      r = x_data[0];
      for (k = 2; k <= loop_ub; k++) {
        r += x_data[k - 1];
      }
    }

    F1_data[j] = r;
  }

  /*  F1 = F1(1:nsamps); % commented on 07/08/2020 */
  /* figure; plot(tmp); hold on; plot(F1,'k'); */
}

/* End of code generation (smooth2.c) */
