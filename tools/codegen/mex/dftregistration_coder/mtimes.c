/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mtimes.c
 *
 * Code generation for function 'mtimes'
 *
 */

/* Include files */
#include "mtimes.h"
#include "blas.h"
#include "dftregistration_coder.h"
#include "dftregistration_coder_emxutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo ed_emlrtRSI = { 232,/* lineNo */
  "mtimes",                            /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+blas\\mtimes.m"/* pathName */
};

static emlrtRTEInfo mc_emlrtRTEI = { 211,/* lineNo */
  5,                                   /* colNo */
  "mtimes",                            /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+blas\\mtimes.m"/* pName */
};

static emlrtRTEInfo nc_emlrtRTEI = { 232,/* lineNo */
  13,                                  /* colNo */
  "mtimes",                            /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+blas\\mtimes.m"/* pName */
};

static const creal_T dc = { 0.0,       /* re */
  0.0                                  /* im */
};

static const creal_T dc1 = { 1.0,      /* re */
  0.0                                  /* im */
};

/* Function Definitions */
void b_mtimes(const creal_T A_data[], const int32_T A_size[2], const
              emxArray_creal_T *B, creal_T C_data[], int32_T C_size[2])
{
  int32_T loop_ub;
  int32_T i;
  char_T TRANSB1;
  char_T TRANSA1;
  ptrdiff_t m_t;
  ptrdiff_t n_t;
  ptrdiff_t k_t;
  ptrdiff_t lda_t;
  ptrdiff_t ldb_t;
  ptrdiff_t ldc_t;
  if ((A_size[1] == 0) || (B->size[0] == 0) || (B->size[1] == 0)) {
    C_size[0] = 1;
    C_size[1] = B->size[1];
    loop_ub = B->size[1];
    for (i = 0; i < loop_ub; i++) {
      C_data[i] = dc;
    }
  } else {
    TRANSB1 = 'N';
    TRANSA1 = 'N';
    m_t = (ptrdiff_t)1;
    n_t = (ptrdiff_t)B->size[1];
    k_t = (ptrdiff_t)A_size[1];
    lda_t = (ptrdiff_t)1;
    ldb_t = (ptrdiff_t)B->size[0];
    ldc_t = (ptrdiff_t)1;
    C_size[0] = 1;
    C_size[1] = B->size[1];
    zgemm(&TRANSA1, &TRANSB1, &m_t, &n_t, &k_t, (real_T *)&dc1, (real_T *)
          &A_data[0], &lda_t, (real_T *)&B->data[0], &ldb_t, (real_T *)&dc,
          (real_T *)&C_data[0], &ldc_t);
  }
}

void mtimes(const emlrtStack *sp, const emxArray_creal_T *A, const
            emxArray_creal_T *B, emxArray_creal_T *C)
{
  int32_T i;
  char_T TRANSB1;
  int32_T loop_ub;
  char_T TRANSA1;
  ptrdiff_t m_t;
  ptrdiff_t n_t;
  ptrdiff_t k_t;
  ptrdiff_t lda_t;
  ptrdiff_t ldb_t;
  ptrdiff_t ldc_t;
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;
  if ((A->size[1] == 0) || (B->size[0] == 0) || (B->size[1] == 0)) {
    i = C->size[0] * C->size[1];
    C->size[0] = A->size[0];
    C->size[1] = B->size[1];
    emxEnsureCapacity_creal_T(sp, C, i, &mc_emlrtRTEI);
    loop_ub = A->size[0] * B->size[1];
    for (i = 0; i < loop_ub; i++) {
      C->data[i] = dc;
    }
  } else {
    st.site = &ed_emlrtRSI;
    TRANSB1 = 'N';
    TRANSA1 = 'N';
    m_t = (ptrdiff_t)A->size[0];
    n_t = (ptrdiff_t)B->size[1];
    k_t = (ptrdiff_t)A->size[1];
    lda_t = (ptrdiff_t)A->size[0];
    ldb_t = (ptrdiff_t)B->size[0];
    ldc_t = (ptrdiff_t)A->size[0];
    i = C->size[0] * C->size[1];
    C->size[0] = A->size[0];
    C->size[1] = B->size[1];
    emxEnsureCapacity_creal_T(&st, C, i, &nc_emlrtRTEI);
    zgemm(&TRANSA1, &TRANSB1, &m_t, &n_t, &k_t, (real_T *)&dc1, (real_T *)
          &A->data[0], &lda_t, (real_T *)&B->data[0], &ldb_t, (real_T *)&dc,
          (real_T *)&C->data[0], &ldc_t);
  }
}

/* End of code generation (mtimes.c) */
