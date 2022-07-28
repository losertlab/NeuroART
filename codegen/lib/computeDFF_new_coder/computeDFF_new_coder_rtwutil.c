/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder_rtwutil.c
 *
 * Code generation for function 'computeDFF_new_coder_rtwutil'
 *
 */

/* Include files */
#include "computeDFF_new_coder_rtwutil.h"
#include "computeDFF_new_coder.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* End of code generation (computeDFF_new_coder_rtwutil.c) */
