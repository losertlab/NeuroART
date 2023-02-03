/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coder_data.c
 *
 * Code generation for function 'dftregistration_coder_data'
 *
 */

/* Include files */
#include "dftregistration_coder_data.h"
#include "dftregistration_coder.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
omp_lock_t emlrtLockGlobal;
omp_nest_lock_t emlrtNestLockGlobal;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131483U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "dftregistration_coder",             /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

emlrtRSInfo gb_emlrtRSI = { 75,        /* lineNo */
  "applyScalarFunction",               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\applyScalarFunction.m"/* pathName */
};

emlrtRSInfo hb_emlrtRSI = { 21,        /* lineNo */
  "eml_int_forloop_overflow_check",    /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\eml\\eml_int_forloop_overflow_check.m"/* pathName */
};

emlrtRSInfo jb_emlrtRSI = { 96,        /* lineNo */
  "sumprod",                           /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\sumprod.m"/* pathName */
};

emlrtRSInfo kb_emlrtRSI = { 124,       /* lineNo */
  "combineVectorElements",             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\combineVectorElements.m"/* pathName */
};

emlrtRSInfo lb_emlrtRSI = { 163,       /* lineNo */
  "colMajorFlatIter",                  /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\combineVectorElements.m"/* pathName */
};

emlrtRSInfo mb_emlrtRSI = { 184,       /* lineNo */
  "colMajorFlatIter",                  /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\combineVectorElements.m"/* pathName */
};

emlrtRSInfo ob_emlrtRSI = { 19,        /* lineNo */
  "ifft",                              /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\ifft.m"/* pathName */
};

emlrtRSInfo qb_emlrtRSI = { 102,       /* lineNo */
  "fft",                               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\fft.m"/* pathName */
};

emlrtRSInfo rb_emlrtRSI = { 18,        /* lineNo */
  "fftw",                              /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\fftw.m"/* pathName */
};

emlrtRSInfo sb_emlrtRSI = { 28,        /* lineNo */
  "FFTWApi/fft1d",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+fftw\\FFTWApi.m"/* pathName */
};

emlrtRSInfo ub_emlrtRSI = { 17,        /* lineNo */
  "MATLABFFTWCallback/fft1d",          /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+fftw\\MATLABFFTWCallback.m"/* pathName */
};

emlrtRSInfo pc_emlrtRSI = { 11,        /* lineNo */
  "ifftshift",                         /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\ifftshift.m"/* pathName */
};

emlrtRSInfo qc_emlrtRSI = { 40,        /* lineNo */
  "eml_ifftshift",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_ifftshift.m"/* pathName */
};

emlrtRSInfo rc_emlrtRSI = { 33,        /* lineNo */
  "eml_ifftshift",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_ifftshift.m"/* pathName */
};

emlrtRSInfo sc_emlrtRSI = { 30,        /* lineNo */
  "eml_ifftshift",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_ifftshift.m"/* pathName */
};

emlrtRSInfo tc_emlrtRSI = { 25,        /* lineNo */
  "eml_ifftshift",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_ifftshift.m"/* pathName */
};

emlrtRSInfo uc_emlrtRSI = { 22,        /* lineNo */
  "eml_ifftshift",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_ifftshift.m"/* pathName */
};

emlrtRSInfo fd_emlrtRSI = { 228,       /* lineNo */
  "mtimes",                            /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+blas\\mtimes.m"/* pathName */
};

emlrtRSInfo gd_emlrtRSI = { 183,       /* lineNo */
  "mtimes",                            /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+blas\\mtimes.m"/* pathName */
};

emlrtRSInfo hd_emlrtRSI = { 40,        /* lineNo */
  "xdotu",                             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+blas\\xdotu.m"/* pathName */
};

emlrtRSInfo id_emlrtRSI = { 15,        /* lineNo */
  "xdotu",                             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xdotu.m"/* pathName */
};

emlrtRSInfo jd_emlrtRSI = { 32,        /* lineNo */
  "xdotx",                             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xdotx.m"/* pathName */
};

emlrtRSInfo ld_emlrtRSI = { 306,       /* lineNo */
  "eml_float_colon",                   /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\colon.m"/* pathName */
};

emlrtRSInfo md_emlrtRSI = { 31,        /* lineNo */
  "meshgrid",                          /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\elmat\\meshgrid.m"/* pathName */
};

emlrtRSInfo nd_emlrtRSI = { 32,        /* lineNo */
  "meshgrid",                          /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\elmat\\meshgrid.m"/* pathName */
};

emlrtRTEInfo vb_emlrtRTEI = { 86,      /* lineNo */
  9,                                   /* colNo */
  "fft",                               /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\fft.m"/* pName */
};

/* End of code generation (dftregistration_coder_data.c) */
