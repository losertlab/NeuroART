/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coder.c
 *
 * Code generation for function 'dftregistration_coder'
 *
 */

/* Include files */
#include "dftregistration_coder.h"
#include "abs.h"
#include "dftregistration_coder_data.h"
#include "dftregistration_coder_emxutil.h"
#include "eml_int_forloop_overflow_check.h"
#include "exp.h"
#include "ifft2.h"
#include "ifftshift.h"
#include "meshgrid.h"
#include "mtimes.h"
#include "mwmathutil.h"
#include "relop.h"
#include "rt_nonfinite.h"
#include "sum.h"

/* Variable Definitions */
static emlrtRSInfo emlrtRSI = { 52,    /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo b_emlrtRSI = { 53,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo c_emlrtRSI = { 54,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo d_emlrtRSI = { 55,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo e_emlrtRSI = { 56,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo f_emlrtRSI = { 58,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo g_emlrtRSI = { 66,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo h_emlrtRSI = { 67,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo i_emlrtRSI = { 68,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo j_emlrtRSI = { 72,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo k_emlrtRSI = { 73,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo l_emlrtRSI = { 75,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo m_emlrtRSI = { 76,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo n_emlrtRSI = { 80,  /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo o_emlrtRSI = { 107, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo p_emlrtRSI = { 110, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo q_emlrtRSI = { 111, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo r_emlrtRSI = { 112, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo s_emlrtRSI = { 141, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo t_emlrtRSI = { 144, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo u_emlrtRSI = { 145, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo v_emlrtRSI = { 148, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo w_emlrtRSI = { 149, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo x_emlrtRSI = { 157, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo y_emlrtRSI = { 158, /* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo ab_emlrtRSI = { 161,/* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo bb_emlrtRSI = { 178,/* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo cb_emlrtRSI = { 179,/* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo db_emlrtRSI = { 180,/* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo eb_emlrtRSI = { 181,/* lineNo */
  "dftregistration_coder",             /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo vb_emlrtRSI = { 16, /* lineNo */
  "max",                               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\max.m"/* pathName */
};

static emlrtRSInfo wb_emlrtRSI = { 17, /* lineNo */
  "minOrMax",                          /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\minOrMax.m"/* pathName */
};

static emlrtRSInfo xb_emlrtRSI = { 43, /* lineNo */
  "unaryOrBinaryDispatch",             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\minOrMax.m"/* pathName */
};

static emlrtRSInfo yb_emlrtRSI = { 165,/* lineNo */
  "unaryMinOrMax",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pathName */
};

static emlrtRSInfo ac_emlrtRSI = { 324,/* lineNo */
  "unaryMinOrMaxDispatch",             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pathName */
};

static emlrtRSInfo bc_emlrtRSI = { 392,/* lineNo */
  "minOrMax2D",                        /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pathName */
};

static emlrtRSInfo cc_emlrtRSI = { 476,/* lineNo */
  "minOrMax2DColumnMajorDim1",         /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pathName */
};

static emlrtRSInfo dc_emlrtRSI = { 474,/* lineNo */
  "minOrMax2DColumnMajorDim1",         /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pathName */
};

static emlrtRSInfo ec_emlrtRSI = { 314,/* lineNo */
  "unaryMinOrMaxDispatch",             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pathName */
};

static emlrtRSInfo fc_emlrtRSI = { 361,/* lineNo */
  "minOrMax1D",                        /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pathName */
};

static emlrtRSInfo gc_emlrtRSI = { 11, /* lineNo */
  "fftshift",                          /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\fftshift.m"/* pathName */
};

static emlrtRSInfo hc_emlrtRSI = { 58, /* lineNo */
  "eml_fftshift",                      /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_fftshift.m"/* pathName */
};

static emlrtRSInfo ic_emlrtRSI = { 51, /* lineNo */
  "eml_fftshift",                      /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_fftshift.m"/* pathName */
};

static emlrtRSInfo jc_emlrtRSI = { 48, /* lineNo */
  "eml_fftshift",                      /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_fftshift.m"/* pathName */
};

static emlrtRSInfo kc_emlrtRSI = { 36, /* lineNo */
  "eml_fftshift",                      /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_fftshift.m"/* pathName */
};

static emlrtRSInfo lc_emlrtRSI = { 30, /* lineNo */
  "eml_fftshift",                      /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_fftshift.m"/* pathName */
};

static emlrtRSInfo mc_emlrtRSI = { 27, /* lineNo */
  "eml_fftshift",                      /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_fftshift.m"/* pathName */
};

static emlrtRSInfo nc_emlrtRSI = { 20, /* lineNo */
  "eml_fftshift",                      /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\private\\eml_fftshift.m"/* pathName */
};

static emlrtRSInfo oc_emlrtRSI = { 50, /* lineNo */
  "prodsize",                          /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\prodsize.m"/* pathName */
};

static emlrtRSInfo vc_emlrtRSI = { 224,/* lineNo */
  "dftups",                            /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo wc_emlrtRSI = { 225,/* lineNo */
  "dftups",                            /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo xc_emlrtRSI = { 226,/* lineNo */
  "dftups",                            /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo yc_emlrtRSI = { 227,/* lineNo */
  "dftups",                            /* fcnName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pathName */
};

static emlrtRSInfo cd_emlrtRSI = { 102,/* lineNo */
  "eml_mtimes_helper",                 /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"/* pathName */
};

static emlrtRSInfo dd_emlrtRSI = { 56, /* lineNo */
  "eml_mtimes_helper",                 /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"/* pathName */
};

static emlrtRSInfo kd_emlrtRSI = { 28, /* lineNo */
  "colon",                             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\colon.m"/* pathName */
};

static emlrtBCInfo emlrtBCI = { -1,    /* iFirst */
  -1,                                  /* iLast */
  147,                                 /* lineNo */
  17,                                  /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo b_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  114,                                 /* lineNo */
  11,                                  /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtRTEInfo emlrtRTEI = { 95,  /* lineNo */
  27,                                  /* colNo */
  "unaryMinOrMax",                     /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pName */
};

static emlrtRTEInfo b_emlrtRTEI = { 26,/* lineNo */
  27,                                  /* colNo */
  "unaryMinOrMax",                     /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pName */
};

static emlrtECInfo emlrtECI = { 2,     /* nDims */
  181,                                 /* lineNo */
  12,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtECInfo b_emlrtECI = { 2,   /* nDims */
  181,                                 /* lineNo */
  33,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtECInfo c_emlrtECI = { 2,   /* nDims */
  158,                                 /* lineNo */
  25,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtECInfo d_emlrtECI = { 2,   /* nDims */
  157,                                 /* lineNo */
  25,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtECInfo e_emlrtECI = { 2,   /* nDims */
  149,                                 /* lineNo */
  23,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtECInfo f_emlrtECI = { 2,   /* nDims */
  148,                                 /* lineNo */
  23,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtBCInfo c_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  146,                                 /* lineNo */
  21,                                  /* colNo */
  "loc1",                              /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtECInfo g_emlrtECI = { 2,   /* nDims */
  141,                                 /* lineNo */
  27,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtBCInfo d_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  113,                                 /* lineNo */
  15,                                  /* colNo */
  "loc1",                              /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtECInfo h_emlrtECI = { -1,  /* nDims */
  106,                                 /* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtBCInfo e_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  106,                                 /* lineNo */
  51,                                  /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo f_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  106,                                 /* lineNo */
  38,                                  /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo g_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  106,                                 /* lineNo */
  21,                                  /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo h_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  106,                                 /* lineNo */
  8,                                   /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtECInfo i_emlrtECI = { 2,   /* nDims */
  107,                                 /* lineNo */
  9,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtBCInfo i_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  71,                                  /* lineNo */
  19,                                  /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo j_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  71,                                  /* lineNo */
  14,                                  /* colNo */
  "CC",                                /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo k_emlrtBCI = { -1,  /* iFirst */
  -1,                                  /* iLast */
  69,                                  /* lineNo */
  15,                                  /* colNo */
  "loc1",                              /* aName */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m",/* pName */
  0                                    /* checkKind */
};

static emlrtECInfo j_emlrtECI = { 2,   /* nDims */
  66,                                  /* lineNo */
  16,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtECInfo k_emlrtECI = { 2,   /* nDims */
  54,                                  /* lineNo */
  21,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo f_emlrtRTEI = { 158,/* lineNo */
  23,                                  /* colNo */
  "dynamic_size_checks",               /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"/* pName */
};

static emlrtRTEInfo g_emlrtRTEI = { 153,/* lineNo */
  23,                                  /* colNo */
  "dynamic_size_checks",               /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"/* pName */
};

static emlrtRTEInfo h_emlrtRTEI = { 66,/* lineNo */
  24,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo i_emlrtRTEI = { 105,/* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo j_emlrtRTEI = { 54,/* lineNo */
  29,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo k_emlrtRTEI = { 66,/* lineNo */
  16,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo l_emlrtRTEI = { 107,/* lineNo */
  9,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo m_emlrtRTEI = { 54,/* lineNo */
  21,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo n_emlrtRTEI = { 66,/* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo o_emlrtRTEI = { 11,/* lineNo */
  13,                                  /* colNo */
  "fftshift",                          /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\fftshift.m"/* pName */
};

static emlrtRTEInfo p_emlrtRTEI = { 392,/* lineNo */
  9,                                   /* colNo */
  "unaryMinOrMax",                     /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pName */
};

static emlrtRTEInfo q_emlrtRTEI = { 468,/* lineNo */
  5,                                   /* colNo */
  "unaryMinOrMax",                     /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\unaryMinOrMax.m"/* pName */
};

static emlrtRTEInfo r_emlrtRTEI = { 1, /* lineNo */
  23,                                  /* colNo */
  "fftshift",                          /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\fftshift.m"/* pName */
};

static emlrtRTEInfo s_emlrtRTEI = { 18,/* lineNo */
  5,                                   /* colNo */
  "minOrMax",                          /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\eml\\+coder\\+internal\\minOrMax.m"/* pName */
};

static emlrtRTEInfo t_emlrtRTEI = { 11,/* lineNo */
  9,                                   /* colNo */
  "fftshift",                          /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\datafun\\fftshift.m"/* pName */
};

static emlrtRTEInfo u_emlrtRTEI = { 67,/* lineNo */
  11,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo v_emlrtRTEI = { 174,/* lineNo */
  1,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo w_emlrtRTEI = { 28,/* lineNo */
  9,                                   /* colNo */
  "colon",                             /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\colon.m"/* pName */
};

static emlrtRTEInfo x_emlrtRTEI = { 110,/* lineNo */
  16,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo y_emlrtRTEI = { 110,/* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo ab_emlrtRTEI = { 181,/* lineNo */
  20,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo bb_emlrtRTEI = { 182,/* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo cb_emlrtRTEI = { 157,/* lineNo */
  33,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo db_emlrtRTEI = { 157,/* lineNo */
  25,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo eb_emlrtRTEI = { 141,/* lineNo */
  35,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo fb_emlrtRTEI = { 141,/* lineNo */
  27,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo gb_emlrtRTEI = { 158,/* lineNo */
  33,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo hb_emlrtRTEI = { 158,/* lineNo */
  25,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo ib_emlrtRTEI = { 148,/* lineNo */
  31,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo jb_emlrtRTEI = { 148,/* lineNo */
  23,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo kb_emlrtRTEI = { 149,/* lineNo */
  31,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo lb_emlrtRTEI = { 149,/* lineNo */
  23,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo mb_emlrtRTEI = { 52,/* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo nb_emlrtRTEI = { 53,/* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo ob_emlrtRTEI = { 180,/* lineNo */
  6,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo pb_emlrtRTEI = { 180,/* lineNo */
  9,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo qb_emlrtRTEI = { 1,/* lineNo */
  26,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo rb_emlrtRTEI = { 144,/* lineNo */
  10,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo sb_emlrtRTEI = { 107,/* lineNo */
  32,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo tb_emlrtRTEI = { 110,/* lineNo */
  10,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo cc_emlrtRTEI = { 224,/* lineNo */
  51,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo dc_emlrtRTEI = { 99,/* lineNo */
  13,                                  /* colNo */
  "eml_mtimes_helper",                 /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"/* pName */
};

static emlrtRTEInfo ec_emlrtRTEI = { 225,/* lineNo */
  7,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo fc_emlrtRTEI = { 225,/* lineNo */
  1,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo gc_emlrtRTEI = { 226,/* lineNo */
  80,                                  /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo hc_emlrtRTEI = { 226,/* lineNo */
  7,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo ic_emlrtRTEI = { 226,/* lineNo */
  1,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo jc_emlrtRTEI = { 99,/* lineNo */
  9,                                   /* colNo */
  "eml_mtimes_helper",                 /* fName */
  "C:\\Program Files\\MATLAB\\R2019b\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"/* pName */
};

static emlrtRTEInfo kc_emlrtRTEI = { 227,/* lineNo */
  1,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

static emlrtRTEInfo lc_emlrtRTEI = { 227,/* lineNo */
  5,                                   /* colNo */
  "dftregistration_coder",             /* fName */
  "C:\\Users\\Losert Lab\\Desktop\\RTapp_v12\\utilities\\dftregistration_coder.m"/* pName */
};

/* Function Declarations */
static creal_T b_dftups(const emlrtStack *sp, const emxArray_creal_T *in, real_T
  usfac);
static void dftups(const emlrtStack *sp, const emxArray_creal_T *in, real_T nor,
                   real_T noc, real_T usfac, real_T roff, real_T coff,
                   emxArray_creal_T *out);
static void eml_ifftshift(const emlrtStack *sp, real_T x_data[], const int32_T
  x_size[2], int32_T dim);

/* Function Definitions */
static creal_T b_dftups(const emlrtStack *sp, const emxArray_creal_T *in, real_T
  usfac)
{
  creal_T out;
  real_T r;
  real_T z_im;
  emxArray_real_T *y;
  int32_T i;
  int32_T loop_ub;
  int32_T x_size[2];
  real_T x_data[512];
  int32_T nx;
  int32_T kernc_size_idx_0;
  creal_T kernc_data[512];
  int32_T k;
  int32_T kernr_size[2];
  creal_T kernr_data[512];
  creal_T y_data[512];
  int32_T y_size[2];
  emlrtStack st;
  emlrtStack b_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);

  /*   this _coder version has calls to exist() removed to enable mex compilation */
  /*  function out=dftups(in,nor,noc,usfac,roff,coff); */
  /*  Upsampled DFT by matrix multiplies, can compute an upsampled DFT in just */
  /*  a small region. */
  /*  usfac         Upsampling factor (default usfac = 1) */
  /*  [nor,noc]     Number of pixels in the output upsampled DFT, in */
  /*                units of upsampled pixels (default = size(in)) */
  /*  roff, coff    Row and column offsets, allow to shift the output array to */
  /*                a region of interest on the DFT (default = 0) */
  /*  Recieves DC in upper left corner, image center must be in (1,1)  */
  /*  Manuel Guizar - Dec 13, 2007 */
  /*  Modified from dftus, by J.R. Fienup 7/31/06 */
  /*  This code is intended to provide the same result as if the following */
  /*  operations were performed */
  /*    - Embed the array "in" in an array that is usfac times larger in each */
  /*      dimension. ifftshift to bring the center of the image to (1,1). */
  /*    - Take the FFT of the larger array */
  /*    - Extract an [nor, noc] region of the result. Starting with the  */
  /*      [roff+1 coff+1] element. */
  /*  It achieves this result by computing the DFT in the output array without */
  /*  the need to zeropad. Much faster and memory efficient than the */
  /*  zero-padded FFT approach if [nor noc] are much smaller than [nr*usfac nc*usfac] */
  /*  Set defaults */
  /*  % % if exist('roff', 'var')~=1, roff=0; end */
  /*  % % if exist('coff', 'var')~=1, coff=0; end */
  /*  % % if exist('usfac', 'var')~=1, usfac=1; end */
  /*  % % if exist('noc', 'var')~=1, noc=nc; end */
  /*  % % if exist('nor', 'var')~=1, nor=nr; end */
  /*  Compute kernels and obtain DFT by matrix products */
  r = (real_T)in->size[1] * usfac;
  z_im = -6.2831853071795862 / r;
  emxInit_real_T(sp, &y, 2, &cc_emlrtRTEI, true);
  if (in->size[1] - 1 < 0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = (int32_T)((real_T)in->size[1] - 1.0) + 1;
    emxEnsureCapacity_real_T(sp, y, i, &w_emlrtRTEI);
    loop_ub = (int32_T)((real_T)in->size[1] - 1.0);
    for (i = 0; i <= loop_ub; i++) {
      y->data[i] = i;
    }
  }

  st.site = &vc_emlrtRSI;
  x_size[0] = 1;
  x_size[1] = y->size[1];
  loop_ub = y->size[0] * y->size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i] = y->data[i];
  }

  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 1);
  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 2);
  nx = (int32_T)muDoubleScalarFloor((real_T)in->size[1] / 2.0);
  kernc_size_idx_0 = x_size[1];
  loop_ub = x_size[1];
  for (i = 0; i < loop_ub; i++) {
    r = x_data[i] - (real_T)nx;
    kernc_data[i].re = 0.0 * (r * 0.0);
    kernc_data[i].im = 0.0 * (r * z_im);
  }

  st.site = &wc_emlrtRSI;
  for (k = 0; k < kernc_size_idx_0; k++) {
    if (kernc_data[k].im == 0.0) {
      kernc_data[k].re = muDoubleScalarExp(kernc_data[k].re);
      kernc_data[k].im = 0.0;
    } else if (muDoubleScalarIsInf(kernc_data[k].im) && muDoubleScalarIsInf
               (kernc_data[k].re) && (kernc_data[k].re < 0.0)) {
      kernc_data[k].re = 0.0;
      kernc_data[k].im = 0.0;
    } else {
      r = muDoubleScalarExp(kernc_data[k].re / 2.0);
      kernc_data[k].re = r * (r * muDoubleScalarCos(kernc_data[k].im));
      kernc_data[k].im = r * (r * muDoubleScalarSin(kernc_data[k].im));
    }
  }

  r = (real_T)in->size[0] * usfac;
  z_im = 0.0 * (-6.2831853071795862 / r);
  if (in->size[0] - 1 < 0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = (int32_T)((real_T)in->size[0] - 1.0) + 1;
    emxEnsureCapacity_real_T(sp, y, i, &w_emlrtRTEI);
    loop_ub = (int32_T)((real_T)in->size[0] - 1.0);
    for (i = 0; i <= loop_ub; i++) {
      y->data[i] = i;
    }
  }

  st.site = &xc_emlrtRSI;
  x_size[0] = 1;
  x_size[1] = y->size[1];
  loop_ub = y->size[0] * y->size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i] = y->data[i];
  }

  emxFree_real_T(&y);
  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 1);
  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 2);
  nx = (int32_T)muDoubleScalarFloor((real_T)in->size[0] / 2.0);
  kernr_size[0] = 1;
  kernr_size[1] = x_size[1];
  loop_ub = x_size[0] * x_size[1];
  for (i = 0; i < loop_ub; i++) {
    r = x_data[i] - (real_T)nx;
    kernr_data[i].re = r * 0.0;
    kernr_data[i].im = r * z_im;
  }

  st.site = &xc_emlrtRSI;
  nx = x_size[1];
  for (k = 0; k < nx; k++) {
    if (kernr_data[k].im == 0.0) {
      kernr_data[k].re = muDoubleScalarExp(kernr_data[k].re);
      kernr_data[k].im = 0.0;
    } else if (muDoubleScalarIsInf(kernr_data[k].im) && muDoubleScalarIsInf
               (kernr_data[k].re) && (kernr_data[k].re < 0.0)) {
      kernr_data[k].re = 0.0;
      kernr_data[k].im = 0.0;
    } else {
      r = muDoubleScalarExp(kernr_data[k].re / 2.0);
      kernr_data[k].re = r * (r * muDoubleScalarCos(kernr_data[k].im));
      kernr_data[k].im = r * (r * muDoubleScalarSin(kernr_data[k].im));
    }
  }

  st.site = &yc_emlrtRSI;
  b_st.site = &dd_emlrtRSI;
  if (x_size[1] != in->size[0]) {
    if ((x_size[1] == 1) || ((in->size[0] == 1) && (in->size[1] == 1))) {
      emlrtErrorWithMessageIdR2018a(&b_st, &g_emlrtRTEI,
        "Coder:toolbox:mtimes_noDynamicScalarExpansion",
        "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
    } else {
      emlrtErrorWithMessageIdR2018a(&b_st, &f_emlrtRTEI, "MATLAB:innerdim",
        "MATLAB:innerdim", 0);
    }
  }

  if ((x_size[1] == 1) || (in->size[0] == 1)) {
    y_size[1] = in->size[1];
    loop_ub = in->size[1];
    for (i = 0; i < loop_ub; i++) {
      y_data[i].re = 0.0;
      y_data[i].im = 0.0;
      nx = kernr_size[1];
      for (k = 0; k < nx; k++) {
        y_data[i].re += kernr_data[k].re * in->data[k + in->size[0] * i].re -
          kernr_data[k].im * in->data[k + in->size[0] * i].im;
        y_data[i].im += kernr_data[k].re * in->data[k + in->size[0] * i].im +
          kernr_data[k].im * in->data[k + in->size[0] * i].re;
      }
    }
  } else {
    b_mtimes(kernr_data, kernr_size, in, y_data, y_size);
  }

  st.site = &yc_emlrtRSI;
  b_st.site = &dd_emlrtRSI;
  if (y_size[1] != kernc_size_idx_0) {
    if ((y_size[1] == 1) || (kernc_size_idx_0 == 1)) {
      emlrtErrorWithMessageIdR2018a(&b_st, &g_emlrtRTEI,
        "Coder:toolbox:mtimes_noDynamicScalarExpansion",
        "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
    } else {
      emlrtErrorWithMessageIdR2018a(&b_st, &f_emlrtRTEI, "MATLAB:innerdim",
        "MATLAB:innerdim", 0);
    }
  }

  if ((y_size[1] == 1) || (kernc_size_idx_0 == 1)) {
    r = 0.0;
    z_im = 0.0;
    loop_ub = y_size[1];
    for (i = 0; i < loop_ub; i++) {
      r += y_data[i].re * kernc_data[i].re - y_data[i].im * kernc_data[i].im;
      z_im += y_data[i].re * kernc_data[i].im + y_data[i].im * kernc_data[i].re;
    }

    out.re = r;
    out.im = z_im;
  } else {
    out.re = 0.0;
    out.im = 0.0;
    if (y_size[1] >= 1) {
      nx = y_size[1];
      for (k = 0; k < nx; k++) {
        out.re += y_data[k].re * kernc_data[k].re - y_data[k].im * kernc_data[k]
          .im;
        out.im += y_data[k].re * kernc_data[k].im + y_data[k].im * kernc_data[k]
          .re;
      }
    }
  }

  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
  return out;
}

static void dftups(const emlrtStack *sp, const emxArray_creal_T *in, real_T nor,
                   real_T noc, real_T usfac, real_T roff, real_T coff,
                   emxArray_creal_T *out)
{
  real_T z_im;
  emxArray_real_T *y;
  int32_T i;
  int32_T loop_ub;
  int32_T x_size[2];
  real_T x_data[512];
  int32_T x;
  emxArray_creal_T *z;
  emxArray_creal_T *b_y;
  real_T d;
  emxArray_creal_T *c_y;
  emxArray_creal_T *kernc;
  int32_T i1;
  emxArray_real_T *d_y;
  emxArray_creal_T *kernr;
  int32_T b_loop_ub;
  int32_T i2;
  emlrtStack st;
  emlrtStack b_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);

  /*   this _coder version has calls to exist() removed to enable mex compilation */
  /*  function out=dftups(in,nor,noc,usfac,roff,coff); */
  /*  Upsampled DFT by matrix multiplies, can compute an upsampled DFT in just */
  /*  a small region. */
  /*  usfac         Upsampling factor (default usfac = 1) */
  /*  [nor,noc]     Number of pixels in the output upsampled DFT, in */
  /*                units of upsampled pixels (default = size(in)) */
  /*  roff, coff    Row and column offsets, allow to shift the output array to */
  /*                a region of interest on the DFT (default = 0) */
  /*  Recieves DC in upper left corner, image center must be in (1,1)  */
  /*  Manuel Guizar - Dec 13, 2007 */
  /*  Modified from dftus, by J.R. Fienup 7/31/06 */
  /*  This code is intended to provide the same result as if the following */
  /*  operations were performed */
  /*    - Embed the array "in" in an array that is usfac times larger in each */
  /*      dimension. ifftshift to bring the center of the image to (1,1). */
  /*    - Take the FFT of the larger array */
  /*    - Extract an [nor, noc] region of the result. Starting with the  */
  /*      [roff+1 coff+1] element. */
  /*  It achieves this result by computing the DFT in the output array without */
  /*  the need to zeropad. Much faster and memory efficient than the */
  /*  zero-padded FFT approach if [nor noc] are much smaller than [nr*usfac nc*usfac] */
  /*  Set defaults */
  /*  % % if exist('roff', 'var')~=1, roff=0; end */
  /*  % % if exist('coff', 'var')~=1, coff=0; end */
  /*  % % if exist('usfac', 'var')~=1, usfac=1; end */
  /*  % % if exist('noc', 'var')~=1, noc=nc; end */
  /*  % % if exist('nor', 'var')~=1, nor=nr; end */
  /*  Compute kernels and obtain DFT by matrix products */
  z_im = (real_T)in->size[1] * usfac;
  z_im = -6.2831853071795862 / z_im;
  emxInit_real_T(sp, &y, 2, &cc_emlrtRTEI, true);
  if (in->size[1] - 1 < 0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = (int32_T)((real_T)in->size[1] - 1.0) + 1;
    emxEnsureCapacity_real_T(sp, y, i, &w_emlrtRTEI);
    loop_ub = (int32_T)((real_T)in->size[1] - 1.0);
    for (i = 0; i <= loop_ub; i++) {
      y->data[i] = i;
    }
  }

  st.site = &vc_emlrtRSI;
  x_size[0] = 1;
  x_size[1] = y->size[1];
  loop_ub = y->size[0] * y->size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i] = y->data[i];
  }

  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 1);
  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 2);
  x = (int32_T)muDoubleScalarFloor((real_T)in->size[1] / 2.0);
  if (noc - 1.0 < 0.0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else if (muDoubleScalarIsInf(noc - 1.0) && (0.0 == noc - 1.0)) {
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 1;
    emxEnsureCapacity_real_T(sp, y, i, &w_emlrtRTEI);
    y->data[0] = rtNaN;
  } else {
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    loop_ub = (int32_T)muDoubleScalarFloor(noc - 1.0);
    y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(sp, y, i, &w_emlrtRTEI);
    for (i = 0; i <= loop_ub; i++) {
      y->data[i] = i;
    }
  }

  emxInit_creal_T(sp, &z, 1, &dc_emlrtRTEI, true);
  i = z->size[0];
  z->size[0] = x_size[1];
  emxEnsureCapacity_creal_T(sp, z, i, &dc_emlrtRTEI);
  loop_ub = x_size[1];
  for (i = 0; i < loop_ub; i++) {
    d = x_data[i] - (real_T)x;
    z->data[i].re = d * 0.0;
    z->data[i].im = d * z_im;
  }

  emxInit_creal_T(sp, &b_y, 2, &dc_emlrtRTEI, true);
  i = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = y->size[1];
  emxEnsureCapacity_creal_T(sp, b_y, i, &dc_emlrtRTEI);
  loop_ub = y->size[0] * y->size[1];
  for (i = 0; i < loop_ub; i++) {
    b_y->data[i].re = y->data[i] - coff;
    b_y->data[i].im = 0.0;
  }

  emxInit_creal_T(sp, &c_y, 2, &lc_emlrtRTEI, true);
  i = c_y->size[0] * c_y->size[1];
  c_y->size[0] = z->size[0];
  c_y->size[1] = b_y->size[1];
  emxEnsureCapacity_creal_T(sp, c_y, i, &ec_emlrtRTEI);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    x = z->size[0];
    for (i1 = 0; i1 < x; i1++) {
      c_y->data[i1 + c_y->size[0] * i].re = z->data[i1].re * b_y->data[i].re -
        z->data[i1].im * b_y->data[i].im;
      c_y->data[i1 + c_y->size[0] * i].im = z->data[i1].re * b_y->data[i].im +
        z->data[i1].im * b_y->data[i].re;
    }
  }

  emxInit_creal_T(sp, &kernc, 2, &fc_emlrtRTEI, true);
  st.site = &wc_emlrtRSI;
  c_exp(&st, c_y);
  i = kernc->size[0] * kernc->size[1];
  kernc->size[0] = c_y->size[0];
  kernc->size[1] = c_y->size[1];
  emxEnsureCapacity_creal_T(sp, kernc, i, &fc_emlrtRTEI);
  loop_ub = c_y->size[0] * c_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    kernc->data[i] = c_y->data[i];
  }

  z_im = (real_T)in->size[0] * usfac;
  z_im = -6.2831853071795862 / z_im;
  if (nor - 1.0 < 0.0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else if (muDoubleScalarIsInf(nor - 1.0) && (0.0 == nor - 1.0)) {
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 1;
    emxEnsureCapacity_real_T(sp, y, i, &w_emlrtRTEI);
    y->data[0] = rtNaN;
  } else {
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    loop_ub = (int32_T)muDoubleScalarFloor(nor - 1.0);
    y->size[1] = loop_ub + 1;
    emxEnsureCapacity_real_T(sp, y, i, &w_emlrtRTEI);
    for (i = 0; i <= loop_ub; i++) {
      y->data[i] = i;
    }
  }

  emxInit_real_T(sp, &d_y, 2, &gc_emlrtRTEI, true);
  if (in->size[0] - 1 < 0) {
    d_y->size[0] = 1;
    d_y->size[1] = 0;
  } else {
    i = d_y->size[0] * d_y->size[1];
    d_y->size[0] = 1;
    d_y->size[1] = (int32_T)((real_T)in->size[0] - 1.0) + 1;
    emxEnsureCapacity_real_T(sp, d_y, i, &w_emlrtRTEI);
    loop_ub = (int32_T)((real_T)in->size[0] - 1.0);
    for (i = 0; i <= loop_ub; i++) {
      d_y->data[i] = i;
    }
  }

  st.site = &xc_emlrtRSI;
  x_size[0] = 1;
  x_size[1] = d_y->size[1];
  loop_ub = d_y->size[0] * d_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i] = d_y->data[i];
  }

  emxFree_real_T(&d_y);
  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 1);
  b_st.site = &pc_emlrtRSI;
  eml_ifftshift(&b_st, x_data, x_size, 2);
  x = (int32_T)muDoubleScalarFloor((real_T)in->size[0] / 2.0);
  i = z->size[0];
  z->size[0] = y->size[1];
  emxEnsureCapacity_creal_T(sp, z, i, &dc_emlrtRTEI);
  loop_ub = y->size[1];
  for (i = 0; i < loop_ub; i++) {
    d = y->data[i] - roff;
    z->data[i].re = d * 0.0;
    z->data[i].im = d * z_im;
  }

  emxFree_real_T(&y);
  i = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = x_size[1];
  emxEnsureCapacity_creal_T(sp, b_y, i, &dc_emlrtRTEI);
  loop_ub = x_size[0] * x_size[1];
  for (i = 0; i < loop_ub; i++) {
    b_y->data[i].re = x_data[i] - (real_T)x;
    b_y->data[i].im = 0.0;
  }

  i = c_y->size[0] * c_y->size[1];
  c_y->size[0] = z->size[0];
  c_y->size[1] = b_y->size[1];
  emxEnsureCapacity_creal_T(sp, c_y, i, &hc_emlrtRTEI);
  loop_ub = b_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    x = z->size[0];
    for (i1 = 0; i1 < x; i1++) {
      c_y->data[i1 + c_y->size[0] * i].re = z->data[i1].re * b_y->data[i].re -
        z->data[i1].im * b_y->data[i].im;
      c_y->data[i1 + c_y->size[0] * i].im = z->data[i1].re * b_y->data[i].im +
        z->data[i1].im * b_y->data[i].re;
    }
  }

  emxFree_creal_T(&b_y);
  emxFree_creal_T(&z);
  emxInit_creal_T(sp, &kernr, 2, &ic_emlrtRTEI, true);
  st.site = &xc_emlrtRSI;
  c_exp(&st, c_y);
  i = kernr->size[0] * kernr->size[1];
  kernr->size[0] = c_y->size[0];
  kernr->size[1] = c_y->size[1];
  emxEnsureCapacity_creal_T(sp, kernr, i, &ic_emlrtRTEI);
  loop_ub = c_y->size[0] * c_y->size[1];
  for (i = 0; i < loop_ub; i++) {
    kernr->data[i] = c_y->data[i];
  }

  st.site = &yc_emlrtRSI;
  b_st.site = &dd_emlrtRSI;
  if (kernr->size[1] != in->size[0]) {
    if (((kernr->size[0] == 1) && (kernr->size[1] == 1)) || ((in->size[0] == 1) &&
         (in->size[1] == 1))) {
      emlrtErrorWithMessageIdR2018a(&b_st, &g_emlrtRTEI,
        "Coder:toolbox:mtimes_noDynamicScalarExpansion",
        "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
    } else {
      emlrtErrorWithMessageIdR2018a(&b_st, &f_emlrtRTEI, "MATLAB:innerdim",
        "MATLAB:innerdim", 0);
    }
  }

  if ((kernr->size[1] == 1) || (in->size[0] == 1)) {
    i = c_y->size[0] * c_y->size[1];
    c_y->size[0] = kernr->size[0];
    c_y->size[1] = in->size[1];
    emxEnsureCapacity_creal_T(&st, c_y, i, &jc_emlrtRTEI);
    loop_ub = kernr->size[0];
    for (i = 0; i < loop_ub; i++) {
      x = in->size[1];
      for (i1 = 0; i1 < x; i1++) {
        c_y->data[i + c_y->size[0] * i1].re = 0.0;
        c_y->data[i + c_y->size[0] * i1].im = 0.0;
        b_loop_ub = kernr->size[1];
        for (i2 = 0; i2 < b_loop_ub; i2++) {
          c_y->data[i + c_y->size[0] * i1].re += kernr->data[i + kernr->size[0] *
            i2].re * in->data[i2 + in->size[0] * i1].re - kernr->data[i +
            kernr->size[0] * i2].im * in->data[i2 + in->size[0] * i1].im;
          c_y->data[i + c_y->size[0] * i1].im += kernr->data[i + kernr->size[0] *
            i2].re * in->data[i2 + in->size[0] * i1].im + kernr->data[i +
            kernr->size[0] * i2].im * in->data[i2 + in->size[0] * i1].re;
        }
      }
    }
  } else {
    b_st.site = &cd_emlrtRSI;
    mtimes(&b_st, kernr, in, c_y);
  }

  emxFree_creal_T(&kernr);
  st.site = &yc_emlrtRSI;
  b_st.site = &dd_emlrtRSI;
  if (c_y->size[1] != kernc->size[0]) {
    if (((c_y->size[0] == 1) && (c_y->size[1] == 1)) || ((kernc->size[0] == 1) &&
         (kernc->size[1] == 1))) {
      emlrtErrorWithMessageIdR2018a(&b_st, &g_emlrtRTEI,
        "Coder:toolbox:mtimes_noDynamicScalarExpansion",
        "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
    } else {
      emlrtErrorWithMessageIdR2018a(&b_st, &f_emlrtRTEI, "MATLAB:innerdim",
        "MATLAB:innerdim", 0);
    }
  }

  if ((c_y->size[1] == 1) || (kernc->size[0] == 1)) {
    i = out->size[0] * out->size[1];
    out->size[0] = c_y->size[0];
    out->size[1] = kernc->size[1];
    emxEnsureCapacity_creal_T(&st, out, i, &kc_emlrtRTEI);
    loop_ub = c_y->size[0];
    for (i = 0; i < loop_ub; i++) {
      x = kernc->size[1];
      for (i1 = 0; i1 < x; i1++) {
        out->data[i + out->size[0] * i1].re = 0.0;
        out->data[i + out->size[0] * i1].im = 0.0;
        b_loop_ub = c_y->size[1];
        for (i2 = 0; i2 < b_loop_ub; i2++) {
          out->data[i + out->size[0] * i1].re += c_y->data[i + c_y->size[0] * i2]
            .re * kernc->data[i2 + kernc->size[0] * i1].re - c_y->data[i +
            c_y->size[0] * i2].im * kernc->data[i2 + kernc->size[0] * i1].im;
          out->data[i + out->size[0] * i1].im += c_y->data[i + c_y->size[0] * i2]
            .re * kernc->data[i2 + kernc->size[0] * i1].im + c_y->data[i +
            c_y->size[0] * i2].im * kernc->data[i2 + kernc->size[0] * i1].re;
        }
      }
    }
  } else {
    b_st.site = &cd_emlrtRSI;
    mtimes(&b_st, c_y, kernc, out);
  }

  emxFree_creal_T(&c_y);
  emxFree_creal_T(&kernc);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
}

static void eml_ifftshift(const emlrtStack *sp, real_T x_data[], const int32_T
  x_size[2], int32_T dim)
{
  int32_T a_tmp;
  int32_T vlend2_tmp;
  int32_T a;
  int32_T npages;
  int32_T i1;
  int32_T k;
  int32_T i2;
  int32_T i;
  int32_T ia;
  int32_T ib;
  real_T xtmp;
  emlrtStack st;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  a_tmp = x_size[dim - 1];
  if (a_tmp > 1) {
    vlend2_tmp = a_tmp / 2 - 1;
    if ((vlend2_tmp + 1) << 1 == a_tmp) {
      st.site = &uc_emlrtRSI;
      a = a_tmp - 1;
      if (a_tmp > 1) {
        b_st.site = &nc_emlrtRSI;
        c_st.site = &oc_emlrtRSI;
        if ((1 <= dim - 1) && (dim - 1 > 2147483646)) {
          d_st.site = &hb_emlrtRSI;
          check_forloop_overflow_error(&d_st);
        }

        npages = 1;
        i1 = dim + 1;
        for (k = i1; k < 3; k++) {
          npages *= x_size[1];
        }

        if ((vlend2_tmp + 1) << 1 == a_tmp) {
          i2 = 0;
          b_st.site = &mc_emlrtRSI;
          if ((1 <= npages) && (npages > 2147483646)) {
            c_st.site = &hb_emlrtRSI;
            check_forloop_overflow_error(&c_st);
          }

          for (i = 0; i < npages; i++) {
            i1 = i2;
            i2 += a;
            b_st.site = &lc_emlrtRSI;
            i1++;
            i2++;
            ia = i1 - 1;
            ib = i1 + vlend2_tmp;
            b_st.site = &kc_emlrtRSI;
            for (k = 0; k <= vlend2_tmp; k++) {
              xtmp = x_data[ia];
              x_data[ia] = x_data[ib];
              x_data[ib] = xtmp;
              ia++;
              ib++;
            }
          }
        } else {
          i2 = 0;
          b_st.site = &jc_emlrtRSI;
          if ((1 <= npages) && (npages > 2147483646)) {
            c_st.site = &hb_emlrtRSI;
            check_forloop_overflow_error(&c_st);
          }

          for (i = 0; i < npages; i++) {
            i1 = i2;
            i2 += a;
            b_st.site = &ic_emlrtRSI;
            i1++;
            i2++;
            ia = i1 - 1;
            ib = i1 + vlend2_tmp;
            xtmp = x_data[ib];
            b_st.site = &hc_emlrtRSI;
            for (k = 0; k <= vlend2_tmp; k++) {
              x_data[ib] = x_data[ia];
              x_data[ia] = x_data[ib + 1];
              ia++;
              ib++;
            }

            x_data[ib] = xtmp;
          }
        }
      }
    } else {
      st.site = &tc_emlrtRSI;
      b_st.site = &oc_emlrtRSI;
      if ((1 <= dim - 1) && (dim - 1 > 2147483646)) {
        c_st.site = &hb_emlrtRSI;
        check_forloop_overflow_error(&c_st);
      }

      npages = 1;
      i1 = dim + 1;
      for (k = i1; k < 3; k++) {
        npages *= x_size[1];
      }

      i2 = -1;
      st.site = &sc_emlrtRSI;
      if ((1 <= npages) && (npages > 2147483646)) {
        b_st.site = &hb_emlrtRSI;
        check_forloop_overflow_error(&b_st);
      }

      for (i = 0; i < npages; i++) {
        i1 = i2 + 1;
        i2 = (i2 + a_tmp) - 1;
        st.site = &rc_emlrtRSI;
        i1++;
        i2++;
        ia = i1 + vlend2_tmp;
        ib = i2;
        xtmp = x_data[i2];
        st.site = &qc_emlrtRSI;
        for (k = 0; k <= vlend2_tmp; k++) {
          ia--;
          i1 = ib;
          ib--;
          x_data[i1] = x_data[ia];
          x_data[ia] = x_data[ib];
        }

        x_data[ib] = xtmp;
      }
    }
  }
}

void dftregistration_coder(const emlrtStack *sp, const emxArray_creal_T *buf1ft,
  const emxArray_creal_T *buf2ft, real_T usfac, real_T output_data[], int32_T
  output_size[2], emxArray_creal_T *Greg)
{
  real_T row_shift;
  real_T col_shift;
  emxArray_real_T *absb1;
  emxArray_real_T *absb2;
  emxArray_creal_T *CC;
  emxArray_creal_T *r;
  emxArray_creal_T *x;
  emxArray_uint32_T *indx;
  int32_T mlarge;
  emxArray_creal_T *ex;
  emxArray_creal_T b_buf1ft;
  emxArray_int32_T *idx;
  int32_T c_buf1ft[1];
  emxArray_creal_T *r1;
  int32_T i;
  int32_T nlarge;
  int32_T d_buf1ft[1];
  int32_T loop_ub;
  creal_T tmp_data[512];
  int32_T tmp_size[2];
  creal_T CCmax;
  real_T dftshift;
  real_T rgzero;
  real_T CCmax_re;
  real_T CCmax_im;
  int32_T m;
  real_T diffphase;
  int32_T n;
  creal_T rf00;
  int32_T Nr_size[2];
  emxArray_int16_T *loc1;
  real_T Nr_data[512];
  int32_T Nc_size[2];
  emxArray_real_T *Nc;
  emxArray_real_T *Nr;
  real_T Nc_data[512];
  int32_T e_buf1ft[1];
  int32_T f_buf1ft[1];
  emxArray_creal_T *r2;
  real_T re;
  real_T rg00_re;
  int32_T md2;
  int32_T nd2;
  real_T rg00_im;
  emlrtStack st;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack i_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  e_st.prev = &d_st;
  e_st.tls = d_st.tls;
  f_st.prev = &e_st;
  f_st.tls = e_st.tls;
  g_st.prev = &f_st;
  g_st.tls = f_st.tls;
  h_st.prev = &g_st;
  h_st.tls = g_st.tls;
  i_st.prev = &h_st;
  i_st.tls = h_st.tls;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);

  /*   this _coder version has calls to exist() removed to enable mex compilation */
  /*  function [output Greg] = dftregistration(buf1ft,buf2ft,usfac); */
  /*  Efficient subpixel image registration by crosscorrelation. This code */
  /*  gives the same precision as the FFT upsampled cross correlation in a */
  /*  small fraction of the computation time and with reduced memory  */
  /*  requirements. It obtains an initial estimate of the crosscorrelation peak */
  /*  by an FFT and then refines the shift estimation by upsampling the DFT */
  /*  only in a small neighborhood of that estimate by means of a  */
  /*  matrix-multiply DFT. With this procedure all the image points are used to */
  /*  compute the upsampled crosscorrelation. */
  /*  Manuel Guizar - Dec 13, 2007 */
  /*  Portions of this code were taken from code written by Ann M. Kowalczyk  */
  /*  and James R. Fienup.  */
  /*  J.R. Fienup and A.M. Kowalczyk, "Phase retrieval for a complex-valued  */
  /*  object by using a low-resolution image," J. Opt. Soc. Am. A 7, 450-458  */
  /*  (1990). */
  /*  Citation for this algorithm: */
  /*  Manuel Guizar-Sicairos, Samuel T. Thurman, and James R. Fienup,  */
  /*  "Efficient subpixel image registration algorithms," Opt. Lett. 33,  */
  /*  156-158 (2008). */
  /*  Inputs */
  /*  buf1ft    Fourier transform of reference image,  */
  /*            DC in (1,1)   [DO NOT FFTSHIFT] */
  /*  buf2ft    Fourier transform of image to register,  */
  /*            DC in (1,1) [DO NOT FFTSHIFT] */
  /*  usfac     Upsampling factor (integer). Images will be registered to  */
  /*            within 1/usfac of a pixel. For example usfac = 20 means the */
  /*            images will be registered within 1/20 of a pixel. (default = 1) */
  /*  Outputs */
  /*  output =  [error,diffphase,net_row_shift,net_col_shift] */
  /*  error     Translation invariant normalized RMS error between f and g */
  /*  diffphase     Global phase difference between the two images (should be */
  /*                zero if images are non-negative). */
  /*  net_row_shift net_col_shift   Pixel shifts between images */
  /*  Greg      (Optional) Fourier transform of registered version of buf2ft, */
  /*            the global phase difference is compensated for. */
  /*  Default usfac to 1 */
  /* %%if exist('usfac', 'var')~=1, usfac=1; end */
  row_shift = 0.0;
  col_shift = 0.0;

  /*  Compute error for no pixel shift */
  emxInit_real_T(sp, &absb1, 1, &mb_emlrtRTEI, true);
  emxInit_real_T(sp, &absb2, 1, &nb_emlrtRTEI, true);
  emxInit_creal_T(sp, &CC, 2, &n_emlrtRTEI, true);
  emxInit_creal_T(sp, &r, 2, &qb_emlrtRTEI, true);
  emxInit_creal_T(sp, &x, 2, &sb_emlrtRTEI, true);
  if (usfac == 0.0) {
    mlarge = buf1ft->size[0] * buf1ft->size[1];
    b_buf1ft = *buf1ft;
    c_buf1ft[0] = mlarge;
    b_buf1ft.size = &c_buf1ft[0];
    b_buf1ft.numDimensions = 1;
    st.site = &emlrtRSI;
    b_abs(&st, &b_buf1ft, absb1);
    mlarge = buf2ft->size[0] * buf2ft->size[1];
    b_buf1ft = *buf2ft;
    d_buf1ft[0] = mlarge;
    b_buf1ft.size = &d_buf1ft[0];
    b_buf1ft.numDimensions = 1;
    st.site = &b_emlrtRSI;
    b_abs(&st, &b_buf1ft, absb2);
    i = r->size[0] * r->size[1];
    r->size[0] = buf2ft->size[0];
    r->size[1] = buf2ft->size[1];
    emxEnsureCapacity_creal_T(sp, r, i, &j_emlrtRTEI);
    loop_ub = buf2ft->size[0] * buf2ft->size[1];
    for (i = 0; i < loop_ub; i++) {
      r->data[i].re = buf2ft->data[i].re;
      r->data[i].im = -buf2ft->data[i].im;
    }

    emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf1ft->size, *(int32_T (*)[2])
      r->size, &k_emlrtECI, sp);
    i = x->size[0] * x->size[1];
    x->size[0] = buf1ft->size[0];
    x->size[1] = buf1ft->size[1];
    emxEnsureCapacity_creal_T(sp, x, i, &m_emlrtRTEI);
    loop_ub = buf1ft->size[0] * buf1ft->size[1];
    for (i = 0; i < loop_ub; i++) {
      x->data[i].re = buf1ft->data[i].re * r->data[i].re - buf1ft->data[i].im *
        r->data[i].im;
      x->data[i].im = buf1ft->data[i].re * r->data[i].im + buf1ft->data[i].im *
        r->data[i].re;
    }

    st.site = &c_emlrtRSI;
    sum(&st, x, tmp_data, tmp_size);
    st.site = &c_emlrtRSI;
    CCmax = b_sum(tmp_data, tmp_size);
    loop_ub = absb1->size[0];
    for (i = 0; i < loop_ub; i++) {
      absb1->data[i] *= absb1->data[i];
    }

    st.site = &d_emlrtRSI;
    dftshift = c_sum(absb1);
    loop_ub = absb2->size[0];
    for (i = 0; i < loop_ub; i++) {
      absb2->data[i] *= absb2->data[i];
    }

    st.site = &e_emlrtRSI;
    rgzero = c_sum(absb2);
    CCmax_re = CCmax.re * CCmax.re - CCmax.im * -CCmax.im;
    CCmax_im = CCmax.re * -CCmax.im + CCmax.im * CCmax.re;
    diffphase = rgzero * dftshift;
    if (CCmax_im == 0.0) {
      CCmax_re /= diffphase;
      CCmax_im = 0.0;
    } else if (CCmax_re == 0.0) {
      CCmax_re = 0.0;
      CCmax_im /= diffphase;
    } else {
      CCmax_re /= diffphase;
      CCmax_im /= diffphase;
    }

    st.site = &f_emlrtRSI;
    diffphase = muDoubleScalarAtan2(CCmax.im, CCmax.re);
    output_size[0] = 1;
    output_size[1] = 2;
    output_data[0] = muDoubleScalarSqrt(muDoubleScalarHypot(1.0 - CCmax_re, 0.0
      - CCmax_im));
    output_data[1] = diffphase;

    /*  Whole-pixel shift - Compute crosscorrelation by an IFFT and locate the */
    /*  peak */
  } else {
    emxInit_uint32_T(sp, &indx, 2, &qb_emlrtRTEI, true);
    emxInit_creal_T(sp, &ex, 2, &rb_emlrtRTEI, true);
    emxInit_int32_T(sp, &idx, 2, &qb_emlrtRTEI, true);
    emxInit_creal_T(sp, &r1, 2, &o_emlrtRTEI, true);
    if (usfac == 1.0) {
      i = r->size[0] * r->size[1];
      r->size[0] = buf2ft->size[0];
      r->size[1] = buf2ft->size[1];
      emxEnsureCapacity_creal_T(sp, r, i, &h_emlrtRTEI);
      loop_ub = buf2ft->size[0] * buf2ft->size[1];
      for (i = 0; i < loop_ub; i++) {
        r->data[i].re = buf2ft->data[i].re;
        r->data[i].im = -buf2ft->data[i].im;
      }

      emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf1ft->size, *(int32_T (*)[2])
        r->size, &j_emlrtECI, sp);
      i = x->size[0] * x->size[1];
      x->size[0] = buf1ft->size[0];
      x->size[1] = buf1ft->size[1];
      emxEnsureCapacity_creal_T(sp, x, i, &k_emlrtRTEI);
      loop_ub = buf1ft->size[0] * buf1ft->size[1];
      for (i = 0; i < loop_ub; i++) {
        x->data[i].re = buf1ft->data[i].re * r->data[i].re - buf1ft->data[i].im *
          r->data[i].im;
        x->data[i].im = buf1ft->data[i].re * r->data[i].im + buf1ft->data[i].im *
          r->data[i].re;
      }

      st.site = &g_emlrtRSI;
      ifft2(&st, x, r1);
      i = CC->size[0] * CC->size[1];
      CC->size[0] = r1->size[0];
      CC->size[1] = r1->size[1];
      emxEnsureCapacity_creal_T(sp, CC, i, &n_emlrtRTEI);
      loop_ub = r1->size[0] * r1->size[1];
      for (i = 0; i < loop_ub; i++) {
        CC->data[i] = r1->data[i];
      }

      st.site = &h_emlrtRSI;
      b_st.site = &vb_emlrtRSI;
      c_st.site = &wb_emlrtRSI;
      d_st.site = &xb_emlrtRSI;
      if (((CC->size[0] != 1) || (CC->size[1] != 1)) && (CC->size[0] == 1)) {
        emlrtErrorWithMessageIdR2018a(&d_st, &b_emlrtRTEI,
          "Coder:toolbox:autoDimIncompatibility",
          "Coder:toolbox:autoDimIncompatibility", 0);
      }

      if (CC->size[0] < 1) {
        emlrtErrorWithMessageIdR2018a(&d_st, &emlrtRTEI,
          "Coder:toolbox:eml_min_or_max_varDimZero",
          "Coder:toolbox:eml_min_or_max_varDimZero", 0);
      }

      e_st.site = &yb_emlrtRSI;
      f_st.site = &ac_emlrtRSI;
      g_st.site = &bc_emlrtRSI;
      m = CC->size[0];
      n = CC->size[1];
      i = ex->size[0] * ex->size[1];
      ex->size[0] = 1;
      ex->size[1] = CC->size[1];
      emxEnsureCapacity_creal_T(&g_st, ex, i, &p_emlrtRTEI);
      i = idx->size[0] * idx->size[1];
      idx->size[0] = 1;
      idx->size[1] = CC->size[1];
      emxEnsureCapacity_int32_T(&g_st, idx, i, &q_emlrtRTEI);
      loop_ub = CC->size[1];
      for (i = 0; i < loop_ub; i++) {
        idx->data[i] = 1;
      }

      if (CC->size[1] >= 1) {
        h_st.site = &dc_emlrtRSI;
        for (mlarge = 0; mlarge < n; mlarge++) {
          ex->data[mlarge] = CC->data[CC->size[0] * mlarge];
          h_st.site = &cc_emlrtRSI;
          for (nlarge = 2; nlarge <= m; nlarge++) {
            CCmax = CC->data[(nlarge + CC->size[0] * mlarge) - 1];
            if (relop(ex->data[mlarge], CCmax)) {
              ex->data[mlarge] = CCmax;
              idx->data[mlarge] = nlarge;
            }
          }
        }
      }

      i = indx->size[0] * indx->size[1];
      indx->size[0] = 1;
      indx->size[1] = idx->size[1];
      emxEnsureCapacity_uint32_T(&b_st, indx, i, &s_emlrtRTEI);
      loop_ub = idx->size[0] * idx->size[1];
      for (i = 0; i < loop_ub; i++) {
        indx->data[i] = (uint32_T)idx->data[i];
      }

      Nr_size[1] = idx->size[1];
      loop_ub = idx->size[0] * idx->size[1];
      for (i = 0; i < loop_ub; i++) {
        Nr_data[i] = idx->data[i];
      }

      emxInit_int16_T(sp, &loc1, 2, &u_emlrtRTEI, true);
      i = loc1->size[0] * loc1->size[1];
      loc1->size[0] = 1;
      loc1->size[1] = idx->size[1];
      emxEnsureCapacity_int16_T(sp, loc1, i, &u_emlrtRTEI);
      loop_ub = idx->size[0] * idx->size[1];
      for (i = 0; i < loop_ub; i++) {
        loc1->data[i] = (int16_T)idx->data[i];
      }

      st.site = &i_emlrtRSI;
      b_st.site = &vb_emlrtRSI;
      c_st.site = &wb_emlrtRSI;
      d_st.site = &xb_emlrtRSI;
      if (ex->size[1] < 1) {
        emlrtErrorWithMessageIdR2018a(&d_st, &emlrtRTEI,
          "Coder:toolbox:eml_min_or_max_varDimZero",
          "Coder:toolbox:eml_min_or_max_varDimZero", 0);
      }

      e_st.site = &yb_emlrtRSI;
      mlarge = ex->size[1];
      f_st.site = &ec_emlrtRSI;
      m = 0;
      rf00 = ex->data[0];
      g_st.site = &fc_emlrtRSI;
      for (nlarge = 2; nlarge <= mlarge; nlarge++) {
        CCmax = ex->data[nlarge - 1];
        if (relop(rf00, CCmax)) {
          rf00 = CCmax;
          m = nlarge - 1;
        }
      }

      i = m + 1;
      if (i > Nr_size[1]) {
        emlrtDynamicBoundsCheckR2012b(i, 1, Nr_size[1], &k_emlrtBCI, sp);
      }

      i = (int16_T)indx->data[m];
      if ((i < 1) || (i > CC->size[0])) {
        emlrtDynamicBoundsCheckR2012b(i, 1, CC->size[0], &j_emlrtBCI, sp);
      }

      i = m + 1;
      if (i > CC->size[1]) {
        emlrtDynamicBoundsCheckR2012b(i, 1, CC->size[1], &i_emlrtBCI, sp);
      }

      mlarge = buf1ft->size[0] * buf1ft->size[1];
      b_buf1ft = *buf1ft;
      e_buf1ft[0] = mlarge;
      b_buf1ft.size = &e_buf1ft[0];
      b_buf1ft.numDimensions = 1;
      st.site = &j_emlrtRSI;
      b_abs(&st, &b_buf1ft, absb1);
      mlarge = buf2ft->size[0] * buf2ft->size[1];
      b_buf1ft = *buf2ft;
      f_buf1ft[0] = mlarge;
      b_buf1ft.size = &f_buf1ft[0];
      b_buf1ft.numDimensions = 1;
      st.site = &k_emlrtRSI;
      b_abs(&st, &b_buf1ft, absb2);
      mlarge = buf1ft->size[0] * buf1ft->size[1];
      loop_ub = absb1->size[0];
      for (i = 0; i < loop_ub; i++) {
        absb1->data[i] *= absb1->data[i];
      }

      st.site = &l_emlrtRSI;
      dftshift = c_sum(absb1) / (real_T)mlarge;
      loop_ub = absb2->size[0];
      for (i = 0; i < loop_ub; i++) {
        absb2->data[i] *= absb2->data[i];
      }

      st.site = &m_emlrtRSI;
      rgzero = c_sum(absb2) / (real_T)mlarge;

      /*  slow code */
      /* rgzero = sum(abs(buf2ft(:)).^2)/(m*n);  */
      mlarge = (int16_T)Nr_data[m] - 1;
      re = CC->data[mlarge + CC->size[0] * m].re * CC->data[mlarge + CC->size[0]
        * m].re - CC->data[mlarge + CC->size[0] * m].im * -CC->data[mlarge +
        CC->size[0] * m].im;
      rg00_re = CC->data[mlarge + CC->size[0] * m].re * -CC->data[mlarge +
        CC->size[0] * m].im + CC->data[mlarge + CC->size[0] * m].im * CC->
        data[mlarge + CC->size[0] * m].re;
      diffphase = rgzero * dftshift;
      if (rg00_re == 0.0) {
        re /= diffphase;
        rg00_re = 0.0;
      } else if (re == 0.0) {
        re = 0.0;
        rg00_re /= diffphase;
      } else {
        re /= diffphase;
        rg00_re /= diffphase;
      }

      st.site = &n_emlrtRSI;
      diffphase = muDoubleScalarAtan2(CC->data[(loc1->data[m] + CC->size[0] * m)
        - 1].im, CC->data[(loc1->data[m] + CC->size[0] * m) - 1].re);
      emxFree_int16_T(&loc1);
      if ((int16_T)indx->data[m] > (int32_T)muDoubleScalarFloor((real_T)
           buf1ft->size[0] / 2.0)) {
        row_shift = (real_T)((int16_T)indx->data[m] - buf1ft->size[0]) - 1.0;
      } else {
        row_shift = (real_T)(int16_T)indx->data[m] - 1.0;
      }

      if (m + 1 > (int32_T)muDoubleScalarFloor((real_T)buf1ft->size[1] / 2.0)) {
        col_shift = (real_T)((m - buf1ft->size[1]) + 1) - 1.0;
      } else {
        col_shift = ((real_T)m + 1.0) - 1.0;
      }

      output_size[0] = 1;
      output_size[1] = 4;
      output_data[0] = muDoubleScalarSqrt(muDoubleScalarHypot(1.0 - re, 0.0 -
        rg00_re));
      output_data[1] = diffphase;
      output_data[2] = row_shift;
      output_data[3] = col_shift;

      /*  Partial-pixel shift */
    } else {
      /*  First upsample by a factor of 2 to obtain initial estimate */
      /*  Embed Fourier data in a 2x larger array */
      mlarge = buf1ft->size[0] << 1;
      nlarge = buf1ft->size[1] << 1;
      i = CC->size[0] * CC->size[1];
      CC->size[0] = mlarge;
      CC->size[1] = nlarge;
      emxEnsureCapacity_creal_T(sp, CC, i, &i_emlrtRTEI);
      loop_ub = mlarge * nlarge;
      for (i = 0; i < loop_ub; i++) {
        CC->data[i].re = 0.0;
        CC->data[i].im = 0.0;
      }

      st.site = &o_emlrtRSI;
      i = r->size[0] * r->size[1];
      r->size[0] = buf1ft->size[0];
      r->size[1] = buf1ft->size[1];
      emxEnsureCapacity_creal_T(&st, r, i, &l_emlrtRTEI);
      loop_ub = buf1ft->size[0] * buf1ft->size[1];
      for (i = 0; i < loop_ub; i++) {
        r->data[i] = buf1ft->data[i];
      }

      i = r1->size[0] * r1->size[1];
      r1->size[0] = r->size[0];
      r1->size[1] = r->size[1];
      emxEnsureCapacity_creal_T(&st, r1, i, &o_emlrtRTEI);
      loop_ub = r->size[0] * r->size[1];
      for (i = 0; i < loop_ub; i++) {
        r1->data[i] = r->data[i];
      }

      b_st.site = &gc_emlrtRSI;
      eml_fftshift(&b_st, r1, 1);
      i = r->size[0] * r->size[1];
      r->size[0] = r1->size[0];
      r->size[1] = r1->size[1];
      emxEnsureCapacity_creal_T(&st, r, i, &l_emlrtRTEI);
      loop_ub = r1->size[0] * r1->size[1];
      for (i = 0; i < loop_ub; i++) {
        r->data[i] = r1->data[i];
      }

      i = r1->size[0] * r1->size[1];
      r1->size[0] = r->size[0];
      r1->size[1] = r->size[1];
      emxEnsureCapacity_creal_T(&st, r1, i, &o_emlrtRTEI);
      loop_ub = r->size[0] * r->size[1];
      for (i = 0; i < loop_ub; i++) {
        r1->data[i] = r->data[i];
      }

      b_st.site = &gc_emlrtRSI;
      eml_fftshift(&b_st, r1, 2);
      i = r->size[0] * r->size[1];
      r->size[0] = r1->size[0];
      r->size[1] = r1->size[1];
      emxEnsureCapacity_creal_T(&st, r, i, &l_emlrtRTEI);
      loop_ub = r1->size[0] * r1->size[1];
      for (i = 0; i < loop_ub; i++) {
        r->data[i] = r1->data[i];
      }

      st.site = &o_emlrtRSI;
      i = x->size[0] * x->size[1];
      x->size[0] = buf2ft->size[0];
      x->size[1] = buf2ft->size[1];
      emxEnsureCapacity_creal_T(&st, x, i, &r_emlrtRTEI);
      loop_ub = buf2ft->size[0] * buf2ft->size[1];
      for (i = 0; i < loop_ub; i++) {
        x->data[i] = buf2ft->data[i];
      }

      i = r1->size[0] * r1->size[1];
      r1->size[0] = x->size[0];
      r1->size[1] = x->size[1];
      emxEnsureCapacity_creal_T(&st, r1, i, &o_emlrtRTEI);
      loop_ub = x->size[0] * x->size[1];
      for (i = 0; i < loop_ub; i++) {
        r1->data[i] = x->data[i];
      }

      b_st.site = &gc_emlrtRSI;
      eml_fftshift(&b_st, r1, 1);
      i = x->size[0] * x->size[1];
      x->size[0] = r1->size[0];
      x->size[1] = r1->size[1];
      emxEnsureCapacity_creal_T(&st, x, i, &t_emlrtRTEI);
      loop_ub = r1->size[0] * r1->size[1];
      for (i = 0; i < loop_ub; i++) {
        x->data[i] = r1->data[i];
      }

      i = r1->size[0] * r1->size[1];
      r1->size[0] = x->size[0];
      r1->size[1] = x->size[1];
      emxEnsureCapacity_creal_T(&st, r1, i, &o_emlrtRTEI);
      loop_ub = x->size[0] * x->size[1];
      for (i = 0; i < loop_ub; i++) {
        r1->data[i] = x->data[i];
      }

      b_st.site = &gc_emlrtRSI;
      eml_fftshift(&b_st, r1, 2);
      i = x->size[0] * x->size[1];
      x->size[0] = r1->size[0];
      x->size[1] = r1->size[1];
      emxEnsureCapacity_creal_T(&st, x, i, &t_emlrtRTEI);
      loop_ub = r1->size[0] * r1->size[1];
      for (i = 0; i < loop_ub; i++) {
        x->data[i] = r1->data[i];
      }

      loop_ub = x->size[0] * x->size[1];
      for (i = 0; i < loop_ub; i++) {
        x->data[i].im = -x->data[i].im;
      }

      emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])r->size, *(int32_T (*)[2])
        x->size, &i_emlrtECI, sp);
      i = (buf1ft->size[0] - (int32_T)muDoubleScalarFloor((real_T)buf1ft->size[0]
            / 2.0)) + 1;
      diffphase = ((real_T)buf1ft->size[0] - 1.0) / 2.0;
      if (diffphase < 0.0) {
        diffphase = -0.0;
      } else {
        diffphase = muDoubleScalarFloor(diffphase);
      }

      nlarge = (buf1ft->size[0] + (int32_T)diffphase) + 1;
      if (i > nlarge) {
        i = 0;
        nlarge = 0;
      } else {
        if ((i < 1) || (i > CC->size[0])) {
          emlrtDynamicBoundsCheckR2012b(i, 1, CC->size[0], &h_emlrtBCI, sp);
        }

        i--;
        if ((nlarge < 1) || (nlarge > CC->size[0])) {
          emlrtDynamicBoundsCheckR2012b(nlarge, 1, CC->size[0], &g_emlrtBCI, sp);
        }
      }

      m = (buf1ft->size[1] - (int32_T)muDoubleScalarFloor((real_T)buf1ft->size[1]
            / 2.0)) + 1;
      diffphase = ((real_T)buf1ft->size[1] - 1.0) / 2.0;
      if (diffphase < 0.0) {
        diffphase = -0.0;
      } else {
        diffphase = muDoubleScalarFloor(diffphase);
      }

      n = (buf1ft->size[1] + (int32_T)diffphase) + 1;
      if (m > n) {
        m = 0;
        n = 0;
      } else {
        if ((m < 1) || (m > CC->size[1])) {
          emlrtDynamicBoundsCheckR2012b(m, 1, CC->size[1], &f_emlrtBCI, sp);
        }

        m--;
        if ((n < 1) || (n > CC->size[1])) {
          emlrtDynamicBoundsCheckR2012b(n, 1, CC->size[1], &e_emlrtBCI, sp);
        }
      }

      loop_ub = r->size[0] * r->size[1];
      for (mlarge = 0; mlarge < loop_ub; mlarge++) {
        diffphase = r->data[mlarge].re * x->data[mlarge].im + r->data[mlarge].im
          * x->data[mlarge].re;
        r->data[mlarge].re = r->data[mlarge].re * x->data[mlarge].re - r->
          data[mlarge].im * x->data[mlarge].im;
        r->data[mlarge].im = diffphase;
      }

      tmp_size[0] = nlarge - i;
      tmp_size[1] = n - m;
      emlrtSubAssignSizeCheckR2012b(&tmp_size[0], 2, &r->size[0], 2, &h_emlrtECI,
        sp);
      loop_ub = r->size[1];
      for (nlarge = 0; nlarge < loop_ub; nlarge++) {
        mlarge = r->size[0];
        for (n = 0; n < mlarge; n++) {
          CC->data[(i + n) + CC->size[0] * (m + nlarge)] = r->data[n + r->size[0]
            * nlarge];
        }
      }

      /*  Compute crosscorrelation and locate the peak  */
      i = r1->size[0] * r1->size[1];
      r1->size[0] = CC->size[0];
      r1->size[1] = CC->size[1];
      emxEnsureCapacity_creal_T(sp, r1, i, &x_emlrtRTEI);
      loop_ub = CC->size[0] * CC->size[1];
      for (i = 0; i < loop_ub; i++) {
        r1->data[i] = CC->data[i];
      }

      emxInit_creal_T(sp, &r2, 2, &tb_emlrtRTEI, true);
      st.site = &p_emlrtRSI;
      ifftshift(&st, r1);
      st.site = &p_emlrtRSI;
      ifft2(&st, r1, r2);
      i = CC->size[0] * CC->size[1];
      CC->size[0] = r2->size[0];
      CC->size[1] = r2->size[1];
      emxEnsureCapacity_creal_T(sp, CC, i, &y_emlrtRTEI);
      loop_ub = r2->size[0] * r2->size[1];
      for (i = 0; i < loop_ub; i++) {
        CC->data[i] = r2->data[i];
      }

      emxFree_creal_T(&r2);

      /*  Calculate cross-correlation */
      st.site = &q_emlrtRSI;
      b_st.site = &vb_emlrtRSI;
      c_st.site = &wb_emlrtRSI;
      d_st.site = &xb_emlrtRSI;
      if (((CC->size[0] != 1) || (CC->size[1] != 1)) && (CC->size[0] == 1)) {
        emlrtErrorWithMessageIdR2018a(&d_st, &b_emlrtRTEI,
          "Coder:toolbox:autoDimIncompatibility",
          "Coder:toolbox:autoDimIncompatibility", 0);
      }

      if (CC->size[0] < 1) {
        emlrtErrorWithMessageIdR2018a(&d_st, &emlrtRTEI,
          "Coder:toolbox:eml_min_or_max_varDimZero",
          "Coder:toolbox:eml_min_or_max_varDimZero", 0);
      }

      e_st.site = &yb_emlrtRSI;
      f_st.site = &ac_emlrtRSI;
      g_st.site = &bc_emlrtRSI;
      m = CC->size[0];
      n = CC->size[1];
      i = ex->size[0] * ex->size[1];
      ex->size[0] = 1;
      ex->size[1] = CC->size[1];
      emxEnsureCapacity_creal_T(&g_st, ex, i, &p_emlrtRTEI);
      i = idx->size[0] * idx->size[1];
      idx->size[0] = 1;
      idx->size[1] = CC->size[1];
      emxEnsureCapacity_int32_T(&g_st, idx, i, &q_emlrtRTEI);
      loop_ub = CC->size[1];
      for (i = 0; i < loop_ub; i++) {
        idx->data[i] = 1;
      }

      if (CC->size[1] >= 1) {
        h_st.site = &dc_emlrtRSI;
        for (mlarge = 0; mlarge < n; mlarge++) {
          ex->data[mlarge] = CC->data[CC->size[0] * mlarge];
          h_st.site = &cc_emlrtRSI;
          for (nlarge = 2; nlarge <= m; nlarge++) {
            CCmax = CC->data[(nlarge + CC->size[0] * mlarge) - 1];
            if (relop(ex->data[mlarge], CCmax)) {
              ex->data[mlarge] = CCmax;
              idx->data[mlarge] = nlarge;
            }
          }
        }
      }

      i = indx->size[0] * indx->size[1];
      indx->size[0] = 1;
      indx->size[1] = idx->size[1];
      emxEnsureCapacity_uint32_T(&b_st, indx, i, &s_emlrtRTEI);
      loop_ub = idx->size[0] * idx->size[1];
      for (i = 0; i < loop_ub; i++) {
        indx->data[i] = (uint32_T)idx->data[i];
      }

      st.site = &r_emlrtRSI;
      b_st.site = &vb_emlrtRSI;
      c_st.site = &wb_emlrtRSI;
      d_st.site = &xb_emlrtRSI;
      if (ex->size[1] < 1) {
        emlrtErrorWithMessageIdR2018a(&d_st, &emlrtRTEI,
          "Coder:toolbox:eml_min_or_max_varDimZero",
          "Coder:toolbox:eml_min_or_max_varDimZero", 0);
      }

      e_st.site = &yb_emlrtRSI;
      mlarge = ex->size[1];
      f_st.site = &ec_emlrtRSI;
      m = 1;
      rf00 = ex->data[0];
      g_st.site = &fc_emlrtRSI;
      for (nlarge = 2; nlarge <= mlarge; nlarge++) {
        CCmax = ex->data[nlarge - 1];
        if (relop(rf00, CCmax)) {
          rf00 = CCmax;
          m = nlarge;
        }
      }

      if (m > idx->size[1]) {
        emlrtDynamicBoundsCheckR2012b(m, 1, idx->size[1], &d_emlrtBCI, sp);
      }

      i = (int16_T)indx->data[m - 1];
      if ((i < 1) || (i > CC->size[0])) {
        emlrtDynamicBoundsCheckR2012b(i, 1, CC->size[0], &b_emlrtBCI, sp);
      }

      if (m > CC->size[1]) {
        emlrtDynamicBoundsCheckR2012b(m, 1, CC->size[1], &b_emlrtBCI, sp);
      }

      CCmax.re = CC->data[(i + CC->size[0] * (m - 1)) - 1].re;
      if (m > CC->size[1]) {
        emlrtDynamicBoundsCheckR2012b(m, 1, CC->size[1], &b_emlrtBCI, sp);
      }

      i = (int16_T)indx->data[m - 1];
      if ((i < 1) || (i > CC->size[0])) {
        emlrtDynamicBoundsCheckR2012b(i, 1, CC->size[0], &b_emlrtBCI, sp);
      }

      CCmax.im = CC->data[(i + CC->size[0] * (m - 1)) - 1].im;

      /*  Obtain shift in original pixel grid from the position of the */
      /*  crosscorrelation peak  */
      md2 = (int32_T)muDoubleScalarFloor((real_T)CC->size[0] / 2.0);
      nd2 = (int32_T)muDoubleScalarFloor((real_T)CC->size[1] / 2.0);
      if ((int16_T)indx->data[m - 1] > md2) {
        row_shift = (real_T)((int16_T)indx->data[m - 1] - CC->size[0]) - 1.0;
      } else {
        row_shift = (real_T)(int16_T)indx->data[m - 1] - 1.0;
      }

      row_shift /= 2.0;
      if (m > nd2) {
        col_shift = (real_T)(m - CC->size[1]) - 1.0;
      } else {
        col_shift = (real_T)m - 1.0;
      }

      col_shift /= 2.0;

      /*  If upsampling > 2, then refine estimate with matrix multiply DFT */
      if (usfac > 2.0) {
        /* %% DFT computation %%% */
        /*  Initial shift estimate in upsampled grid */
        row_shift = muDoubleScalarRound(row_shift * usfac) / usfac;
        col_shift = muDoubleScalarRound(col_shift * usfac) / usfac;
        dftshift = muDoubleScalarFloor(muDoubleScalarCeil(usfac * 1.5) / 2.0);

        /*  Center of output array at dftshift+1 */
        /*  Matrix multiply DFT around the current shift estimate */
        i = r->size[0] * r->size[1];
        r->size[0] = buf1ft->size[0];
        r->size[1] = buf1ft->size[1];
        emxEnsureCapacity_creal_T(sp, r, i, &eb_emlrtRTEI);
        loop_ub = buf1ft->size[0] * buf1ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          r->data[i].re = buf1ft->data[i].re;
          r->data[i].im = -buf1ft->data[i].im;
        }

        emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf2ft->size, *(int32_T (*)[2])
          r->size, &g_emlrtECI, sp);
        i = x->size[0] * x->size[1];
        x->size[0] = buf2ft->size[0];
        x->size[1] = buf2ft->size[1];
        emxEnsureCapacity_creal_T(sp, x, i, &fb_emlrtRTEI);
        loop_ub = buf2ft->size[0] * buf2ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          x->data[i].re = buf2ft->data[i].re * r->data[i].re - buf2ft->data[i].
            im * r->data[i].im;
          x->data[i].im = buf2ft->data[i].re * r->data[i].im + buf2ft->data[i].
            im * r->data[i].re;
        }

        st.site = &s_emlrtRSI;
        dftups(&st, x, muDoubleScalarCeil(usfac * 1.5), muDoubleScalarCeil(usfac
                * 1.5), usfac, dftshift - row_shift * usfac, dftshift -
               col_shift * usfac, CC);
        loop_ub = CC->size[0] * CC->size[1];
        for (i = 0; i < loop_ub; i++) {
          CC->data[i].im = -CC->data[i].im;
        }

        rgzero = (real_T)md2 * (real_T)nd2 * (usfac * usfac);
        loop_ub = CC->size[0] * CC->size[1];
        for (i = 0; i < loop_ub; i++) {
          if (CC->data[i].im == 0.0) {
            re = CC->data[i].re / rgzero;
            diffphase = 0.0;
          } else if (CC->data[i].re == 0.0) {
            re = 0.0;
            diffphase = CC->data[i].im / rgzero;
          } else {
            re = CC->data[i].re / rgzero;
            diffphase = CC->data[i].im / rgzero;
          }

          CC->data[i].re = re;
          CC->data[i].im = diffphase;
        }

        /*  Locate maximum and map back to original pixel grid  */
        st.site = &t_emlrtRSI;
        b_st.site = &vb_emlrtRSI;
        c_st.site = &wb_emlrtRSI;
        d_st.site = &xb_emlrtRSI;
        if (((CC->size[0] != 1) || (CC->size[1] != 1)) && (CC->size[0] == 1)) {
          emlrtErrorWithMessageIdR2018a(&d_st, &b_emlrtRTEI,
            "Coder:toolbox:autoDimIncompatibility",
            "Coder:toolbox:autoDimIncompatibility", 0);
        }

        if (CC->size[0] < 1) {
          emlrtErrorWithMessageIdR2018a(&d_st, &emlrtRTEI,
            "Coder:toolbox:eml_min_or_max_varDimZero",
            "Coder:toolbox:eml_min_or_max_varDimZero", 0);
        }

        e_st.site = &yb_emlrtRSI;
        f_st.site = &ac_emlrtRSI;
        g_st.site = &bc_emlrtRSI;
        m = CC->size[0];
        n = CC->size[1];
        i = ex->size[0] * ex->size[1];
        ex->size[0] = 1;
        ex->size[1] = CC->size[1];
        emxEnsureCapacity_creal_T(&g_st, ex, i, &p_emlrtRTEI);
        i = idx->size[0] * idx->size[1];
        idx->size[0] = 1;
        idx->size[1] = CC->size[1];
        emxEnsureCapacity_int32_T(&g_st, idx, i, &q_emlrtRTEI);
        loop_ub = CC->size[1];
        for (i = 0; i < loop_ub; i++) {
          idx->data[i] = 1;
        }

        if (CC->size[1] >= 1) {
          h_st.site = &dc_emlrtRSI;
          if (CC->size[1] > 2147483646) {
            i_st.site = &hb_emlrtRSI;
            check_forloop_overflow_error(&i_st);
          }

          for (mlarge = 0; mlarge < n; mlarge++) {
            ex->data[mlarge] = CC->data[CC->size[0] * mlarge];
            h_st.site = &cc_emlrtRSI;
            if ((2 <= m) && (m > 2147483646)) {
              i_st.site = &hb_emlrtRSI;
              check_forloop_overflow_error(&i_st);
            }

            for (nlarge = 2; nlarge <= m; nlarge++) {
              CCmax = CC->data[(nlarge + CC->size[0] * mlarge) - 1];
              if (relop(ex->data[mlarge], CCmax)) {
                ex->data[mlarge] = CCmax;
                idx->data[mlarge] = nlarge;
              }
            }
          }
        }

        i = indx->size[0] * indx->size[1];
        indx->size[0] = 1;
        indx->size[1] = idx->size[1];
        emxEnsureCapacity_uint32_T(&b_st, indx, i, &s_emlrtRTEI);
        loop_ub = idx->size[0] * idx->size[1];
        for (i = 0; i < loop_ub; i++) {
          indx->data[i] = (uint32_T)idx->data[i];
        }

        st.site = &u_emlrtRSI;
        b_st.site = &vb_emlrtRSI;
        c_st.site = &wb_emlrtRSI;
        d_st.site = &xb_emlrtRSI;
        if (ex->size[1] < 1) {
          emlrtErrorWithMessageIdR2018a(&d_st, &emlrtRTEI,
            "Coder:toolbox:eml_min_or_max_varDimZero",
            "Coder:toolbox:eml_min_or_max_varDimZero", 0);
        }

        e_st.site = &yb_emlrtRSI;
        mlarge = ex->size[1];
        f_st.site = &ec_emlrtRSI;
        m = 1;
        rf00 = ex->data[0];
        g_st.site = &fc_emlrtRSI;
        if ((2 <= ex->size[1]) && (ex->size[1] > 2147483646)) {
          h_st.site = &hb_emlrtRSI;
          check_forloop_overflow_error(&h_st);
        }

        for (nlarge = 2; nlarge <= mlarge; nlarge++) {
          CCmax = ex->data[nlarge - 1];
          if (relop(rf00, CCmax)) {
            rf00 = CCmax;
            m = nlarge;
          }
        }

        if (m > indx->size[1]) {
          emlrtDynamicBoundsCheckR2012b(m, 1, indx->size[1], &c_emlrtBCI, sp);
        }

        i = (int32_T)indx->data[m - 1];
        if ((i < 1) || (i > CC->size[0])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)indx->data[m - 1], 1, CC->size
            [0], &emlrtBCI, sp);
        }

        if (m > CC->size[1]) {
          emlrtDynamicBoundsCheckR2012b(m, 1, CC->size[1], &emlrtBCI, sp);
        }

        mlarge = i - 1;
        CCmax.re = CC->data[mlarge + CC->size[0] * (m - 1)].re;
        if (m > CC->size[1]) {
          emlrtDynamicBoundsCheckR2012b(m, 1, CC->size[1], &emlrtBCI, sp);
        }

        if (i > CC->size[0]) {
          emlrtDynamicBoundsCheckR2012b((int32_T)indx->data[m - 1], 1, CC->size
            [0], &emlrtBCI, sp);
        }

        CCmax.im = CC->data[mlarge + CC->size[0] * (m - 1)].im;
        i = r->size[0] * r->size[1];
        r->size[0] = buf1ft->size[0];
        r->size[1] = buf1ft->size[1];
        emxEnsureCapacity_creal_T(sp, r, i, &ib_emlrtRTEI);
        loop_ub = buf1ft->size[0] * buf1ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          r->data[i].re = buf1ft->data[i].re;
          r->data[i].im = -buf1ft->data[i].im;
        }

        emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf1ft->size, *(int32_T (*)[2])
          r->size, &f_emlrtECI, sp);
        i = x->size[0] * x->size[1];
        x->size[0] = buf1ft->size[0];
        x->size[1] = buf1ft->size[1];
        emxEnsureCapacity_creal_T(sp, x, i, &jb_emlrtRTEI);
        loop_ub = buf1ft->size[0] * buf1ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          x->data[i].re = buf1ft->data[i].re * r->data[i].re - buf1ft->data[i].
            im * r->data[i].im;
          x->data[i].im = buf1ft->data[i].re * r->data[i].im + buf1ft->data[i].
            im * r->data[i].re;
        }

        st.site = &v_emlrtRSI;
        rf00 = b_dftups(&st, x, usfac);
        if (rf00.im == 0.0) {
          rg00_re = rf00.re / rgzero;
          rg00_im = 0.0;
        } else if (rf00.re == 0.0) {
          rg00_re = 0.0;
          rg00_im = rf00.im / rgzero;
        } else {
          rg00_re = rf00.re / rgzero;
          rg00_im = rf00.im / rgzero;
        }

        i = r->size[0] * r->size[1];
        r->size[0] = buf2ft->size[0];
        r->size[1] = buf2ft->size[1];
        emxEnsureCapacity_creal_T(sp, r, i, &kb_emlrtRTEI);
        loop_ub = buf2ft->size[0] * buf2ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          r->data[i].re = buf2ft->data[i].re;
          r->data[i].im = -buf2ft->data[i].im;
        }

        emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf2ft->size, *(int32_T (*)[2])
          r->size, &e_emlrtECI, sp);
        i = x->size[0] * x->size[1];
        x->size[0] = buf2ft->size[0];
        x->size[1] = buf2ft->size[1];
        emxEnsureCapacity_creal_T(sp, x, i, &lb_emlrtRTEI);
        loop_ub = buf2ft->size[0] * buf2ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          x->data[i].re = buf2ft->data[i].re * r->data[i].re - buf2ft->data[i].
            im * r->data[i].im;
          x->data[i].im = buf2ft->data[i].re * r->data[i].im + buf2ft->data[i].
            im * r->data[i].re;
        }

        st.site = &w_emlrtRSI;
        rf00 = b_dftups(&st, x, usfac);
        if (rf00.im == 0.0) {
          re = rf00.re / rgzero;
          diffphase = 0.0;
        } else if (rf00.re == 0.0) {
          re = 0.0;
          diffphase = rf00.im / rgzero;
        } else {
          re = rf00.re / rgzero;
          diffphase = rf00.im / rgzero;
        }

        rf00.re = re;
        rf00.im = diffphase;
        row_shift += (((real_T)indx->data[m - 1] - dftshift) - 1.0) / usfac;
        col_shift += (((real_T)m - dftshift) - 1.0) / usfac;

        /*  If upsampling = 2, no additional pixel shift refinement */
      } else {
        i = r->size[0] * r->size[1];
        r->size[0] = buf1ft->size[0];
        r->size[1] = buf1ft->size[1];
        emxEnsureCapacity_creal_T(sp, r, i, &cb_emlrtRTEI);
        loop_ub = buf1ft->size[0] * buf1ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          r->data[i].re = buf1ft->data[i].re;
          r->data[i].im = -buf1ft->data[i].im;
        }

        emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf1ft->size, *(int32_T (*)[2])
          r->size, &d_emlrtECI, sp);
        i = x->size[0] * x->size[1];
        x->size[0] = buf1ft->size[0];
        x->size[1] = buf1ft->size[1];
        emxEnsureCapacity_creal_T(sp, x, i, &db_emlrtRTEI);
        loop_ub = buf1ft->size[0] * buf1ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          x->data[i].re = buf1ft->data[i].re * r->data[i].re - buf1ft->data[i].
            im * r->data[i].im;
          x->data[i].im = buf1ft->data[i].re * r->data[i].im + buf1ft->data[i].
            im * r->data[i].re;
        }

        st.site = &x_emlrtRSI;
        sum(&st, x, tmp_data, tmp_size);
        st.site = &x_emlrtRSI;
        rf00 = b_sum(tmp_data, tmp_size);
        if (rf00.im == 0.0) {
          diffphase = rf00.re / (real_T)CC->size[0];
          dftshift = 0.0;
        } else if (rf00.re == 0.0) {
          diffphase = 0.0;
          dftshift = rf00.im / (real_T)CC->size[0];
        } else {
          diffphase = rf00.re / (real_T)CC->size[0];
          dftshift = rf00.im / (real_T)CC->size[0];
        }

        if (dftshift == 0.0) {
          rg00_re = diffphase / (real_T)CC->size[1];
          rg00_im = 0.0;
        } else if (diffphase == 0.0) {
          rg00_re = 0.0;
          rg00_im = dftshift / (real_T)CC->size[1];
        } else {
          rg00_re = diffphase / (real_T)CC->size[1];
          rg00_im = dftshift / (real_T)CC->size[1];
        }

        i = r->size[0] * r->size[1];
        r->size[0] = buf2ft->size[0];
        r->size[1] = buf2ft->size[1];
        emxEnsureCapacity_creal_T(sp, r, i, &gb_emlrtRTEI);
        loop_ub = buf2ft->size[0] * buf2ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          r->data[i].re = buf2ft->data[i].re;
          r->data[i].im = -buf2ft->data[i].im;
        }

        emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf2ft->size, *(int32_T (*)[2])
          r->size, &c_emlrtECI, sp);
        i = x->size[0] * x->size[1];
        x->size[0] = buf2ft->size[0];
        x->size[1] = buf2ft->size[1];
        emxEnsureCapacity_creal_T(sp, x, i, &hb_emlrtRTEI);
        loop_ub = buf2ft->size[0] * buf2ft->size[1];
        for (i = 0; i < loop_ub; i++) {
          x->data[i].re = buf2ft->data[i].re * r->data[i].re - buf2ft->data[i].
            im * r->data[i].im;
          x->data[i].im = buf2ft->data[i].re * r->data[i].im + buf2ft->data[i].
            im * r->data[i].re;
        }

        st.site = &y_emlrtRSI;
        sum(&st, x, tmp_data, tmp_size);
        st.site = &y_emlrtRSI;
        rf00 = b_sum(tmp_data, tmp_size);
        if (rf00.im == 0.0) {
          diffphase = rf00.re / (real_T)CC->size[0];
          dftshift = 0.0;
        } else if (rf00.re == 0.0) {
          diffphase = 0.0;
          dftshift = rf00.im / (real_T)CC->size[0];
        } else {
          diffphase = rf00.re / (real_T)CC->size[0];
          dftshift = rf00.im / (real_T)CC->size[0];
        }

        if (dftshift == 0.0) {
          rf00.re = diffphase / (real_T)CC->size[1];
          rf00.im = 0.0;
        } else if (diffphase == 0.0) {
          rf00.re = 0.0;
          rf00.im = dftshift / (real_T)CC->size[1];
        } else {
          rf00.re = diffphase / (real_T)CC->size[1];
          rf00.im = dftshift / (real_T)CC->size[1];
        }
      }

      CCmax_re = CCmax.re * CCmax.re - CCmax.im * -CCmax.im;
      CCmax_im = CCmax.re * -CCmax.im + CCmax.im * CCmax.re;
      re = rg00_re * rf00.re - rg00_im * rf00.im;
      rg00_im = rg00_re * rf00.im + rg00_im * rf00.re;
      if (rg00_im == 0.0) {
        if (CCmax_im == 0.0) {
          rg00_re = CCmax_re / re;
          CCmax_im = 0.0;
        } else if (CCmax_re == 0.0) {
          rg00_re = 0.0;
          CCmax_im /= re;
        } else {
          rg00_re = CCmax_re / re;
          CCmax_im /= re;
        }
      } else if (re == 0.0) {
        if (CCmax_re == 0.0) {
          rg00_re = CCmax_im / rg00_im;
          CCmax_im = 0.0;
        } else if (CCmax_im == 0.0) {
          rg00_re = 0.0;
          CCmax_im = -(CCmax_re / rg00_im);
        } else {
          rg00_re = CCmax_im / rg00_im;
          CCmax_im = -(CCmax_re / rg00_im);
        }
      } else {
        rgzero = muDoubleScalarAbs(re);
        diffphase = muDoubleScalarAbs(rg00_im);
        if (rgzero > diffphase) {
          dftshift = rg00_im / re;
          diffphase = re + dftshift * rg00_im;
          rg00_re = (CCmax_re + dftshift * CCmax_im) / diffphase;
          CCmax_im = (CCmax_im - dftshift * CCmax_re) / diffphase;
        } else if (diffphase == rgzero) {
          if (re > 0.0) {
            dftshift = 0.5;
          } else {
            dftshift = -0.5;
          }

          if (rg00_im > 0.0) {
            diffphase = 0.5;
          } else {
            diffphase = -0.5;
          }

          rg00_re = (CCmax_re * dftshift + CCmax_im * diffphase) / rgzero;
          CCmax_im = (CCmax_im * dftshift - CCmax_re * diffphase) / rgzero;
        } else {
          dftshift = re / rg00_im;
          diffphase = rg00_im + dftshift * re;
          rg00_re = (dftshift * CCmax_re + CCmax_im) / diffphase;
          CCmax_im = (dftshift * CCmax_im - CCmax_re) / diffphase;
        }
      }

      st.site = &ab_emlrtRSI;
      diffphase = muDoubleScalarAtan2(CCmax.im, CCmax.re);

      /*  If its only one row or column the shift along that dimension has no */
      /*  effect. We set to zero. */
      if (md2 == 1) {
        row_shift = 0.0;
      }

      if (nd2 == 1) {
        col_shift = 0.0;
      }

      output_size[0] = 1;
      output_size[1] = 4;
      output_data[0] = muDoubleScalarSqrt(muDoubleScalarHypot(1.0 - rg00_re, 0.0
        - CCmax_im));
      output_data[1] = diffphase;
      output_data[2] = row_shift;
      output_data[3] = col_shift;
    }

    emxFree_creal_T(&r1);
    emxFree_int32_T(&idx);
    emxFree_creal_T(&ex);
    emxFree_uint32_T(&indx);
  }

  emxFree_creal_T(&x);
  emxFree_creal_T(&r);
  emxFree_real_T(&absb2);
  emxFree_real_T(&absb1);
  rf00.re = diffphase * 0.0;
  rf00.im = diffphase;
  b_exp(&rf00);
  i = Greg->size[0] * Greg->size[1];
  Greg->size[0] = buf2ft->size[0];
  Greg->size[1] = buf2ft->size[1];
  emxEnsureCapacity_creal_T(sp, Greg, i, &v_emlrtRTEI);
  loop_ub = buf2ft->size[0] * buf2ft->size[1];
  for (i = 0; i < loop_ub; i++) {
    Greg->data[i].re = buf2ft->data[i].re * rf00.re - buf2ft->data[i].im *
      rf00.im;
    Greg->data[i].im = buf2ft->data[i].re * rf00.im + buf2ft->data[i].im *
      rf00.re;
  }

  /*  Compute registered version of buf2ft */
  if (usfac > 0.0) {
    st.site = &bb_emlrtRSI;
    nlarge = (int32_T)muDoubleScalarFloor((real_T)buf2ft->size[0] / 2.0);
    mlarge = (int32_T)muDoubleScalarCeil((real_T)buf2ft->size[0] / 2.0);
    b_st.site = &kd_emlrtRSI;
    if (mlarge - 1 < -nlarge) {
      Nr_size[0] = 1;
      Nr_size[1] = 0;
    } else {
      Nr_size[0] = 1;
      loop_ub = (int32_T)(((real_T)mlarge - 1.0) - (-(real_T)nlarge));
      Nr_size[1] = loop_ub + 1;
      for (i = 0; i <= loop_ub; i++) {
        Nr_data[i] = -(real_T)nlarge + (real_T)i;
      }
    }

    st.site = &bb_emlrtRSI;
    b_st.site = &pc_emlrtRSI;
    eml_ifftshift(&b_st, Nr_data, Nr_size, 1);
    b_st.site = &pc_emlrtRSI;
    eml_ifftshift(&b_st, Nr_data, Nr_size, 2);
    st.site = &cb_emlrtRSI;
    nlarge = (int32_T)muDoubleScalarFloor((real_T)buf2ft->size[1] / 2.0);
    mlarge = (int32_T)muDoubleScalarCeil((real_T)buf2ft->size[1] / 2.0);
    b_st.site = &kd_emlrtRSI;
    if (mlarge - 1 < -nlarge) {
      Nc_size[0] = 1;
      Nc_size[1] = 0;
    } else {
      Nc_size[0] = 1;
      loop_ub = (int32_T)(((real_T)mlarge - 1.0) - (-(real_T)nlarge));
      Nc_size[1] = loop_ub + 1;
      for (i = 0; i <= loop_ub; i++) {
        Nc_data[i] = -(real_T)nlarge + (real_T)i;
      }
    }

    emxInit_real_T(&b_st, &Nc, 2, &ob_emlrtRTEI, true);
    emxInit_real_T(&b_st, &Nr, 2, &pb_emlrtRTEI, true);
    st.site = &cb_emlrtRSI;
    b_st.site = &pc_emlrtRSI;
    eml_ifftshift(&b_st, Nc_data, Nc_size, 1);
    b_st.site = &pc_emlrtRSI;
    eml_ifftshift(&b_st, Nc_data, Nc_size, 2);
    st.site = &db_emlrtRSI;
    meshgrid(&st, Nc_data, Nc_size, Nr_data, Nr_size, Nc, Nr);
    loop_ub = Nr->size[0] * Nr->size[1];
    for (i = 0; i < loop_ub; i++) {
      Nr->data[i] = -row_shift * Nr->data[i] / (real_T)buf2ft->size[0];
    }

    loop_ub = Nc->size[0] * Nc->size[1];
    for (i = 0; i < loop_ub; i++) {
      Nc->data[i] = col_shift * Nc->data[i] / (real_T)buf2ft->size[1];
    }

    emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])Nr->size, *(int32_T (*)[2])
      Nc->size, &b_emlrtECI, sp);
    i = CC->size[0] * CC->size[1];
    CC->size[0] = Nr->size[0];
    CC->size[1] = Nr->size[1];
    emxEnsureCapacity_creal_T(sp, CC, i, &ab_emlrtRTEI);
    loop_ub = Nr->size[0] * Nr->size[1];
    for (i = 0; i < loop_ub; i++) {
      CC->data[i].re = (Nr->data[i] - Nc->data[i]) * 0.0;
      CC->data[i].im = (Nr->data[i] - Nc->data[i]) * 6.2831853071795862;
    }

    emxFree_real_T(&Nr);
    emxFree_real_T(&Nc);
    st.site = &eb_emlrtRSI;
    c_exp(&st, CC);
    i = Greg->size[0] * Greg->size[1];
    Greg->size[0] = CC->size[0];
    Greg->size[1] = CC->size[1];
    emxEnsureCapacity_creal_T(sp, Greg, i, &ab_emlrtRTEI);
    loop_ub = CC->size[0] * CC->size[1];
    for (i = 0; i < loop_ub; i++) {
      Greg->data[i] = CC->data[i];
    }

    emlrtSizeEqCheckNDR2012b(*(int32_T (*)[2])buf2ft->size, *(int32_T (*)[2])
      Greg->size, &emlrtECI, sp);
    loop_ub = buf2ft->size[0] * buf2ft->size[1];
    i = Greg->size[0] * Greg->size[1];
    Greg->size[0] = buf2ft->size[0];
    Greg->size[1] = buf2ft->size[1];
    emxEnsureCapacity_creal_T(sp, Greg, i, &bb_emlrtRTEI);
    for (i = 0; i < loop_ub; i++) {
      diffphase = buf2ft->data[i].re * Greg->data[i].re - buf2ft->data[i].im *
        Greg->data[i].im;
      dftshift = buf2ft->data[i].re * Greg->data[i].im + buf2ft->data[i].im *
        Greg->data[i].re;
      Greg->data[i].re = diffphase * rf00.re - dftshift * rf00.im;
      Greg->data[i].im = diffphase * rf00.im + dftshift * rf00.re;
    }
  }

  emxFree_creal_T(&CC);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
}

void eml_fftshift(const emlrtStack *sp, emxArray_creal_T *x, int32_T dim)
{
  int32_T vlend2;
  int32_T vstride;
  int32_T k;
  int32_T npages;
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
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  if (x->size[dim - 1] > 1) {
    vlend2 = x->size[dim - 1] / 2;
    st.site = &nc_emlrtRSI;
    vstride = 1;
    b_st.site = &oc_emlrtRSI;
    if ((1 <= dim - 1) && (dim - 1 > 2147483646)) {
      c_st.site = &hb_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }

    for (k = 0; k <= dim - 2; k++) {
      vstride *= x->size[0];
    }

    npages = 1;
    vspread = dim + 1;
    for (k = vspread; k < 3; k++) {
      npages *= x->size[1];
    }

    vspread = (x->size[dim - 1] - 1) * vstride;
    midoffset = vlend2 * vstride - 1;
    if (vlend2 << 1 == x->size[dim - 1]) {
      i2 = 0;
      st.site = &mc_emlrtRSI;
      if ((1 <= npages) && (npages > 2147483646)) {
        b_st.site = &hb_emlrtRSI;
        check_forloop_overflow_error(&b_st);
      }

      for (i = 0; i < npages; i++) {
        i1 = i2;
        i2 += vspread;
        st.site = &lc_emlrtRSI;
        if ((1 <= vstride) && (vstride > 2147483646)) {
          b_st.site = &hb_emlrtRSI;
          check_forloop_overflow_error(&b_st);
        }

        for (j = 0; j < vstride; j++) {
          i1++;
          i2++;
          ia = i1 - 1;
          ib = i1 + midoffset;
          st.site = &kc_emlrtRSI;
          for (k = 0; k < vlend2; k++) {
            xtmp_re = x->data[ia].re;
            xtmp_im = x->data[ia].im;
            x->data[ia] = x->data[ib];
            x->data[ib].re = xtmp_re;
            x->data[ib].im = xtmp_im;
            ia += vstride;
            ib += vstride;
          }
        }
      }
    } else {
      i2 = 0;
      st.site = &jc_emlrtRSI;
      if ((1 <= npages) && (npages > 2147483646)) {
        b_st.site = &hb_emlrtRSI;
        check_forloop_overflow_error(&b_st);
      }

      for (i = 0; i < npages; i++) {
        i1 = i2;
        i2 += vspread;
        st.site = &ic_emlrtRSI;
        if ((1 <= vstride) && (vstride > 2147483646)) {
          b_st.site = &hb_emlrtRSI;
          check_forloop_overflow_error(&b_st);
        }

        for (j = 0; j < vstride; j++) {
          i1++;
          i2++;
          ia = i1 - 1;
          ib = i1 + midoffset;
          xtmp_re = x->data[ib].re;
          xtmp_im = x->data[ib].im;
          st.site = &hc_emlrtRSI;
          for (k = 0; k < vlend2; k++) {
            ic = ib + vstride;
            x->data[ib] = x->data[ia];
            x->data[ia] = x->data[ic];
            ia += vstride;
            ib = ic;
          }

          x->data[ib].re = xtmp_re;
          x->data[ib].im = xtmp_im;
        }
      }
    }
  }
}

/* End of code generation (dftregistration_coder.c) */
