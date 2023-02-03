/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * ifftshift.c
 *
 * Code generation for function 'ifftshift'
 *
 */

/* Include files */
#include "ifftshift.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_data.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void ifftshift(const emlrtStack *sp, emxArray_creal_T *x)
{
  int32_T vlend2;
  int32_T npages;
  int32_T k;
  int32_T vspread;
  int32_T midoffset;
  int32_T i2;
  int32_T i;
  int32_T i1;
  int32_T j;
  int32_T ia;
  int32_T ib;
  real_T xtmp_re;
  real_T xtmp_im;
  int32_T ic;
  emlrtStack st;
  emlrtStack b_st;
  emlrtStack c_st;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = &pc_emlrtRSI;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  if (x->size[0] > 1) {
    vlend2 = x->size[0] / 2;
    if (vlend2 << 1 == x->size[0]) {
      b_st.site = &uc_emlrtRSI;
      eml_fftshift(&b_st, x, 1);
    } else {
      b_st.site = &tc_emlrtRSI;
      npages = 1;
      for (k = 2; k < 3; k++) {
        npages *= x->size[1];
      }

      vspread = x->size[0] - 1;
      midoffset = vlend2 - 1;
      i2 = -1;
      b_st.site = &sc_emlrtRSI;
      if ((1 <= npages) && (npages > 2147483646)) {
        c_st.site = &hb_emlrtRSI;
        check_forloop_overflow_error(&c_st);
      }

      for (i = 0; i < npages; i++) {
        i1 = i2 + 1;
        i2 += vspread;
        b_st.site = &rc_emlrtRSI;
        for (j = 0; j < 1; j++) {
          i1++;
          i2++;
          ia = i1 + midoffset;
          ib = i2;
          xtmp_re = x->data[i2].re;
          xtmp_im = x->data[i2].im;
          b_st.site = &qc_emlrtRSI;
          for (k = 0; k < vlend2; k++) {
            ia--;
            ic = ib;
            ib--;
            x->data[ic] = x->data[ia];
            x->data[ia] = x->data[ib];
          }

          x->data[ib].re = xtmp_re;
          x->data[ib].im = xtmp_im;
        }
      }
    }
  }

  st.site = &pc_emlrtRSI;
  if (x->size[1] > 1) {
    vlend2 = x->size[1] / 2;
    if (vlend2 << 1 == x->size[1]) {
      b_st.site = &uc_emlrtRSI;
      eml_fftshift(&b_st, x, 2);
    } else {
      b_st.site = &tc_emlrtRSI;
      npages = 1;
      for (k = 0; k < 1; k++) {
        npages *= x->size[0];
      }

      vspread = (x->size[1] - 1) * npages;
      midoffset = vlend2 * npages - 1;
      i2 = -1;
      b_st.site = &sc_emlrtRSI;
      for (i = 0; i < 1; i++) {
        i1 = i2 + 1;
        i2 += vspread;
        b_st.site = &rc_emlrtRSI;
        if ((1 <= npages) && (npages > 2147483646)) {
          c_st.site = &hb_emlrtRSI;
          check_forloop_overflow_error(&c_st);
        }

        for (j = 0; j < npages; j++) {
          i1++;
          i2++;
          ia = i1 + midoffset;
          ib = i2;
          xtmp_re = x->data[i2].re;
          xtmp_im = x->data[i2].im;
          b_st.site = &qc_emlrtRSI;
          for (k = 0; k < vlend2; k++) {
            ia -= npages;
            ic = ib;
            ib -= npages;
            x->data[ic] = x->data[ia];
            x->data[ia] = x->data[ib];
          }

          x->data[ib].re = xtmp_re;
          x->data[ib].im = xtmp_im;
        }
      }
    }
  }
}

/* End of code generation (ifftshift.c) */
