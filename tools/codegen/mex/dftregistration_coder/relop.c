/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * relop.c
 *
 * Code generation for function 'relop'
 *
 */

/* Include files */
#include "relop.h"
#include "dftregistration_coder.h"
#include "mwmathutil.h"
#include "rt_nonfinite.h"

/* Function Definitions */
boolean_T relop(const creal_T a, const creal_T b)
{
  boolean_T p;
  real_T ma;
  boolean_T SCALEA;
  real_T mb;
  boolean_T SCALEB;
  real_T absai;
  real_T absbi;
  real_T Ma;
  real_T Mb;
  if (muDoubleScalarIsNaN(b.re) || muDoubleScalarIsNaN(b.im)) {
    p = false;
  } else if (muDoubleScalarIsNaN(a.re) || muDoubleScalarIsNaN(a.im)) {
    p = true;
  } else {
    ma = muDoubleScalarAbs(a.re);
    if ((ma > 8.9884656743115785E+307) || (muDoubleScalarAbs(a.im) >
         8.9884656743115785E+307)) {
      SCALEA = true;
    } else {
      SCALEA = false;
    }

    mb = muDoubleScalarAbs(b.re);
    if ((mb > 8.9884656743115785E+307) || (muDoubleScalarAbs(b.im) >
         8.9884656743115785E+307)) {
      SCALEB = true;
    } else {
      SCALEB = false;
    }

    if (SCALEA || SCALEB) {
      absai = muDoubleScalarHypot(a.re / 2.0, a.im / 2.0);
      absbi = muDoubleScalarHypot(b.re / 2.0, b.im / 2.0);
    } else {
      absai = muDoubleScalarHypot(a.re, a.im);
      absbi = muDoubleScalarHypot(b.re, b.im);
    }

    if (absai == absbi) {
      absai = muDoubleScalarAbs(a.im);
      absbi = muDoubleScalarAbs(b.im);
      if (ma > absai) {
        Ma = ma;
        ma = absai;
      } else {
        Ma = absai;
      }

      if (mb > absbi) {
        Mb = mb;
        mb = absbi;
      } else {
        Mb = absbi;
      }

      if (Ma > Mb) {
        if (ma < mb) {
          absai = Ma - Mb;
          absbi = (ma / 2.0 + mb / 2.0) / (Ma / 2.0 + Mb / 2.0) * (mb - ma);
        } else {
          absai = Ma;
          absbi = Mb;
        }
      } else if (Ma < Mb) {
        if (ma > mb) {
          absbi = Mb - Ma;
          absai = (ma / 2.0 + mb / 2.0) / (Ma / 2.0 + Mb / 2.0) * (ma - mb);
        } else {
          absai = Ma;
          absbi = Mb;
        }
      } else {
        absai = ma;
        absbi = mb;
      }

      if (absai == absbi) {
        absai = muDoubleScalarAtan2(a.im, a.re);
        absbi = muDoubleScalarAtan2(b.im, b.re);
        if (absai == absbi) {
          if (absai > 0.78539816339744828) {
            if (absai > 2.3561944901923448) {
              absai = -a.im;
              absbi = -b.im;
            } else {
              absai = -a.re;
              absbi = -b.re;
            }
          } else if (absai > -0.78539816339744828) {
            absai = a.im;
            absbi = b.im;
          } else if (absai > -2.3561944901923448) {
            absai = a.re;
            absbi = b.re;
          } else {
            absai = -a.im;
            absbi = -b.im;
          }

          if (absai == absbi) {
            absai = 0.0;
            absbi = 0.0;
          }
        }
      }
    }

    p = (absai < absbi);
  }

  return p;
}

/* End of code generation (relop.c) */
