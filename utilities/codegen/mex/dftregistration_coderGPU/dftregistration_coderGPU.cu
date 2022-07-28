/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * dftregistration_coderGPU.cu
 *
 * Code generation for function 'dftregistration_coderGPU'
 *
 */

/* Include files */
#include "dftregistration_coderGPU.h"
#include "MWCudaDimUtility.h"
#include "MWLaunchParametersUtilities.h"
#include "dftregistration_coderGPU_data.h"
#include "dftregistration_coderGPU_emxutil.h"
#include "math_constants.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Type Definitions */
#include "cublas_v2.h"
#include "cufft.h"

/* Variable Definitions */
static cufftHandle cufftGlobalHandle;
static cufftHandle *cufftGlobalHandlePtr = NULL;
static int32_T cufftGlobalSize = 0U;
static int32_T cufftGlobalBatch = 0U;
static cufftType_t cufftGlobalType = CUFFT_R2C;
static int32_T cufftGlobalDist = 0U;

/* Function Declarations */
static __global__ void ab_dftregistration_coderGPU_ker(const int16_T yk, creal_T
  *y);
static __global__ void ac_dftregistration_coderGPU_ker(int16_T idx[512]);
static __global__ void ad_dftregistration_coderGPU_ker(const real_T *tmp_re,
  creal_T *y);
static __inline__ __device__ real_T atomicOpreal_T(real_T *address, real_T value);
static __device__ real_T b_rt_atan2d_snf(real_T u0, real_T u1);
static __device__ real_T b_rt_hypotd_snf(real_T u0, real_T u1);
static __global__ void bb_dftregistration_coderGPU_ker(const creal_T *y, const
  int16_T cloc, const emxArray_int16_T *b_y, emxArray_creal_T *z);
static __global__ void bc_dftregistration_coderGPU_ker(int16_T idx[512], creal_T
  CC[262144], creal_T y[512]);
static __global__ void bd_dftregistration_coderGPU_ker(const creal_T *y, const
  creal_T buf2ft[262144], creal_T Greg[262144]);
static __global__ void c_dftregistration_coderGPU_kern(creal_T CC[1048576]);
static __global__ void cb_dftregistration_coderGPU_ker(const int16_T x[512],
  creal_T y[512]);
static __global__ void cc_dftregistration_coderGPU_ker(const creal_T buf2ft
  [262144], const creal_T buf1ft[262144], real_T absb1[262144], real_T absb2
  [262144]);
static __global__ void cd_dftregistration_coderGPU_ker(const int16_T b[512],
  int16_T Nr[262144], int16_T Nc[262144]);
static void cufftEnsureDestruction();
static void cufftEnsureInitialization(int32_T nelem, cufftType_t type, int32_T
  batch, int32_T idist);
static __global__ void d_dftregistration_coderGPU_kern(const creal_T buf1ft
  [262144], creal_T CC[262144]);
static __global__ void db_dftregistration_coderGPU_ker(const emxArray_creal_T *z,
  emxArray_creal_T *kernr, creal_T y[512]);
static __global__ void dc_dftregistration_coderGPU_ker(const real_T absb1[262144],
  real_T *absar);
static __global__ void dd_dftregistration_coderGPU_ker(const int16_T Nc[262144],
  const real_T col_shift, const int16_T Nr[262144], const real_T *row_shift,
  creal_T Greg[262144]);
static __global__ void e_dftregistration_coderGPU_kern(const creal_T buf2ft
  [262144], creal_T x[262144]);
static __global__ void eb_dftregistration_coderGPU_ker(const emxArray_creal_T
  *kernr, int32_T *nx);
static __global__ void ec_dftregistration_coderGPU_ker(const real_T absb1[262144],
  real_T *absar);
static __global__ void ed_dftregistration_coderGPU_ker(creal_T Greg[262144]);
static __global__ void f_dftregistration_coderGPU_kern(creal_T CC[262144],
  creal_T b_CC[1048576], creal_T x[262144]);
static __global__ void fb_dftregistration_coderGPU_ker(const int32_T *nx,
  emxArray_creal_T *kernr);
static __global__ void fc_dftregistration_coderGPU_ker(real_T absb2[262144]);
static __global__ void fd_dftregistration_coderGPU_ker(const real_T *tmp_im,
  creal_T *y);
static __global__ void g_dftregistration_coderGPU_kern(creal_T CC[1048576],
  creal_T y[1048576]);
static __global__ void gb_dftregistration_coderGPU_ker(const emxArray_creal_T
  *kernr, emxArray_creal_T *y);
static __global__ void gc_dftregistration_coderGPU_ker(const real_T absb2[262144],
  real_T *tmp_re);
static __global__ void gd_dftregistration_coderGPU_ker(const creal_T *y, real_T *
  tmp_re);
static void gpuEmxFree_creal_T(emxArray_creal_T *inter);
static void gpuEmxFree_int16_T(emxArray_int16_T *inter);
static void gpuEmxFree_real_T(emxArray_real_T *inter);
static void gpuEmxFree_uint16_T(emxArray_uint16_T *inter);
static void gpuEmxMemcpyCpuToGpu_creal_T(const emxArray_creal_T *cpu,
  emxArray_creal_T *inter, emxArray_creal_T *gpu);
static void gpuEmxMemcpyCpuToGpu_int16_T(const emxArray_int16_T *cpu,
  emxArray_int16_T *inter, emxArray_int16_T *gpu);
static void gpuEmxMemcpyCpuToGpu_real_T(const emxArray_real_T *cpu,
  emxArray_real_T *inter, emxArray_real_T *gpu);
static void gpuEmxMemcpyCpuToGpu_uint16_T(const emxArray_uint16_T *cpu,
  emxArray_uint16_T *inter, emxArray_uint16_T *gpu);
static void gpuEmxMemcpyGpuToCpu_creal_T(emxArray_creal_T *cpu, emxArray_creal_T
  *inter);
static void gpuEmxMemcpyGpuToCpu_real_T(emxArray_real_T *cpu, emxArray_real_T
  *inter);
static void gpuEmxMemcpyGpuToCpu_uint16_T(emxArray_uint16_T *cpu,
  emxArray_uint16_T *inter);
static void gpuEmxReset_creal_T(emxArray_creal_T *inter);
static void gpuEmxReset_int16_T(emxArray_int16_T *inter);
static void gpuEmxReset_real_T(emxArray_real_T *inter);
static void gpuEmxReset_uint16_T(emxArray_uint16_T *inter);
static __global__ void h_dftregistration_coderGPU_kern(creal_T CC[1048576]);
static __global__ void hb_dftregistration_coderGPU_ker(const emxArray_creal_T
  *kernc, const emxArray_creal_T *y, emxArray_creal_T *out);
static __global__ void hc_dftregistration_coderGPU_ker(const real_T absb2[262144],
  real_T *tmp_re);
static __global__ void hd_dftregistration_coderGPU_ker(const real_T *tmp_im,
  const real_T *tmp_re, creal_T *y);
static __global__ void i_dftregistration_coderGPU_kern(creal_T CC[1048576],
  creal_T y[1048576]);
static __global__ void ib_dftregistration_coderGPU_ker(const real_T *absar,
  const emxArray_creal_T *out, emxArray_creal_T *CC);
static __global__ void ic_dftregistration_coderGPU_ker(const real_T col_shift,
  const real_T *row_shift, const real_T *tmp_im, const real_T Ma, const real_T
  absbr, real_T dv1[4]);
static __global__ void id_dftregistration_coderGPU_ker(const creal_T *y, real_T *
  tmp_re);
static __global__ void j_dftregistration_coderGPU_kern(creal_T CC[1048576]);
static __global__ void jb_dftregistration_coderGPU_ker(const emxArray_creal_T
  *ex, uint16_T uv[2]);
static __global__ void jc_dftregistration_coderGPU_ker(const real_T dv1[4],
  emxArray_real_T *output);
static __global__ void jd_dftregistration_coderGPU_ker(const real_T *tmp_re,
  creal_T *y);
static __global__ void k_dftregistration_coderGPU_kern(int16_T idx[1024]);
static __global__ void kb_dftregistration_coderGPU_ker(const uint16_T uv[2],
  emxArray_uint16_T *idx);
static __global__ void kc_dftregistration_coderGPU_ker(const creal_T buf2ft
  [262144], const creal_T buf1ft[262144], creal_T CC[262144], real_T absb2
  [262144], real_T absb1[262144]);
static __global__ void kd_dftregistration_coderGPU_ker(const creal_T *y, const
  creal_T buf2ft[262144], creal_T Greg[262144]);
static __global__ void l_dftregistration_coderGPU_kern(int16_T idx[1024],
  creal_T CC[1048576], creal_T ex[1024]);
static __global__ void lb_dftregistration_coderGPU_ker(const emxArray_creal_T
  *CC, const int32_T vstride, const int32_T npages, emxArray_uint16_T *idx,
  emxArray_creal_T *ex);
static __global__ void lc_dftregistration_coderGPU_ker(creal_T y[512], creal_T
  CC[262144]);
static __global__ void m_dftregistration_coderGPU_kern(const int16_T idx[1024],
  const int32_T *nx, real_T *row_shift, creal_T CC[1048576], creal_T *CCmax);
static __global__ void mb_dftregistration_coderGPU_ker(const emxArray_creal_T
  *CC, const int32_T *nx, const emxArray_uint16_T *idx, creal_T *CCmax);
static __global__ void mc_dftregistration_coderGPU_ker(creal_T *CCmax);
static __global__ void n_dftregistration_coderGPU_kern(const creal_T buf1ft
  [262144], creal_T CC[262144]);
static __global__ void nb_dftregistration_coderGPU_ker(const creal_T buf1ft
  [262144], creal_T CC[262144]);
static __global__ void nc_dftregistration_coderGPU_ker(real_T absb1[262144]);
static __global__ void o_dftregistration_coderGPU_kern(creal_T y[512], creal_T
  CC[262144]);
static __global__ void ob_dftregistration_coderGPU_ker(creal_T y[512], creal_T
  kernr[512]);
static __global__ void oc_dftregistration_coderGPU_ker(const real_T absb1[262144],
  real_T *tmp_re);
static __global__ void p_dftregistration_coderGPU_kern(creal_T *y);
static __global__ void pb_dftregistration_coderGPU_ker(creal_T *y);
static __global__ void pc_dftregistration_coderGPU_ker(const real_T absb1[262144],
  real_T *tmp_re);
static __global__ void q_dftregistration_coderGPU_kern(const creal_T buf2ft
  [262144], creal_T CC[262144]);
static __global__ void qb_dftregistration_coderGPU_ker(const creal_T buf2ft
  [262144], creal_T CC[262144]);
static __global__ void qc_dftregistration_coderGPU_ker(real_T absb2[262144]);
static __global__ void r_dftregistration_coderGPU_kern(creal_T y[512], creal_T
  CC[262144]);
static __global__ void rb_dftregistration_coderGPU_ker(creal_T y[512], creal_T
  kernr[512]);
static __global__ void rc_dftregistration_coderGPU_ker(const real_T absb2[262144],
  real_T *tmp_im);
static real_T rt_atan2d_snf(real_T u0, real_T u1);
static real_T rt_hypotd_snf(real_T u0, real_T u1);
static real_T rt_roundd_snf(real_T u);
static __global__ void s_dftregistration_coderGPU_kern(creal_T *y);
static __global__ void sb_dftregistration_coderGPU_ker(creal_T *y);
static __global__ void sc_dftregistration_coderGPU_ker(const real_T absb2[262144],
  real_T *tmp_im);
static __inline__ __device__ real_T shflDown2(real_T in1, uint32_T offset,
  uint32_T mask);
static __global__ void t_dftregistration_coderGPU_kern(const creal_T buf1ft
  [262144], const creal_T buf2ft[262144], creal_T CC[262144]);
static __global__ void tb_dftregistration_coderGPU_ker(const real_T col_shift,
  const real_T *row_shift, const real_T *tmp_im, const real_T CCmax_im, const
  real_T Ma, real_T dv1[4]);
static __global__ void tc_dftregistration_coderGPU_ker(const real_T *tmp_im,
  const real_T CCmax_im, const real_T ex_im, real_T dv[2]);
static __device__ real_T threadGroupReduction(real_T val, uint32_T lane,
  uint32_T mask);
static __global__ void u_dftregistration_coderGPU_kern(const int16_T yk, creal_T
  *y);
static __global__ void ub_dftregistration_coderGPU_ker(const real_T dv1[4],
  emxArray_real_T *output);
static __global__ void uc_dftregistration_coderGPU_ker(const real_T dv[2],
  emxArray_real_T *output);
static __global__ void v_dftregistration_coderGPU_kern(const int16_T coff, const
  emxArray_int16_T *y, emxArray_creal_T *b_y);
static __global__ void vb_dftregistration_coderGPU_ker(const creal_T buf1ft
  [262144], const creal_T buf2ft[262144], creal_T CC[262144]);
static __global__ void vc_dftregistration_coderGPU_ker(const real_T *tmp_im,
  creal_T *y);
static __global__ void w_dftregistration_coderGPU_kern(const emxArray_creal_T *y,
  const creal_T *b_y, const int16_T b[512], int32_T *nx, emxArray_creal_T *kernc);
static __global__ void wb_dftregistration_coderGPU_ker(creal_T b_y1[262144]);
static __global__ void wc_dftregistration_coderGPU_ker(const creal_T *y, real_T *
  tmp_re);
static __device__ real_T workGroupReduction(real_T val, uint32_T mask, uint32_T
  numActiveWarps);
static __global__ void x_dftregistration_coderGPU_kern(const emxArray_creal_T
  *kernc, int32_T *nx);
static __global__ void xb_dftregistration_coderGPU_ker(creal_T b_y1[262144],
  creal_T x[262144]);
static __global__ void xc_dftregistration_coderGPU_ker(const real_T *tmp_im,
  const real_T *tmp_re, creal_T *y);
static __global__ void y_dftregistration_coderGPU_kern(const int32_T *nx,
  emxArray_creal_T *kernc);
static __global__ void yb_dftregistration_coderGPU_ker(creal_T CC[262144]);
static __global__ void yc_dftregistration_coderGPU_ker(const creal_T *y, real_T *
  tmp_re);

/* Function Definitions */
static __global__ __launch_bounds__(32, 1) void ab_dftregistration_coderGPU_ker(
  const int16_T yk, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->im = -6.2831853071795862 / static_cast<real_T>(yk);
  }
}

static __global__ __launch_bounds__(512, 1) void ac_dftregistration_coderGPU_ker
  (int16_T idx[512])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 512) {
    idx[i] = static_cast<int16_T>(1);
  }
}

static __global__ __launch_bounds__(32, 1) void ad_dftregistration_coderGPU_ker(
  const real_T *tmp_re, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = exp(*tmp_re);
    y->im = 0.0;
  }
}

static __inline__ __device__ real_T atomicOpreal_T(real_T *address, real_T value)
{
  unsigned long long int *address_as_up;
  unsigned long long int old;
  unsigned long long int assumed;
  address_as_up = (unsigned long long int *)address;
  old = *address_as_up;
  do {
    assumed = old;
    old = atomicCAS(address_as_up, old, __double_as_longlong(value +
      __longlong_as_double(old)));
  } while (assumed != old);

  return __longlong_as_double(old);
}

static __device__ real_T b_rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T b_u0;
  int32_T b_u1;
  if ((static_cast<int32_T>(isnan(u0))) || (static_cast<int32_T>(isnan(u1)))) {
    y = CUDART_NAN;
  } else if ((static_cast<int32_T>(isinf(u0))) && (static_cast<int32_T>(isinf(u1))))
  {
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = atan2(static_cast<real_T>(b_u0), static_cast<real_T>(b_u1));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

static __device__ real_T b_rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T a;
  real_T b;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (isnan(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

static __global__ __launch_bounds__(1024, 1) void
  bb_dftregistration_coderGPU_ker(const creal_T *y, const int16_T cloc, const
  emxArray_int16_T *b_y, emxArray_creal_T *z)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(b_y->size[1] - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    i = static_cast<int32_T>(idx);
    iv0 = static_cast<int32_T>(b_y->data[i]) - static_cast<int32_T>(cloc);
    if (iv0 > 32767) {
      iv0 = 32767;
    } else {
      if (iv0 < -32768) {
        iv0 = -32768;
      }
    }

    z->data[i].re = static_cast<real_T>(iv0) * 0.0;
    iv0 = static_cast<int32_T>(b_y->data[i]) - static_cast<int32_T>(cloc);
    if (iv0 > 32767) {
      iv0 = 32767;
    } else {
      if (iv0 < -32768) {
        iv0 = -32768;
      }
    }

    z->data[i].im = static_cast<real_T>(iv0) * y->im;
  }
}

static __global__ __launch_bounds__(512, 1) void bc_dftregistration_coderGPU_ker
  (int16_T idx[512], creal_T CC[262144], creal_T y[512])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T v;
  int32_T i;
  int32_T xpageoffset;
  boolean_T p;
  boolean_T SCALEA;
  real_T x;
  real_T br;
  real_T absar;
  real_T r;
  real_T absbr;
  real_T bi;
  real_T ai;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  v = static_cast<int32_T>(threadId);
  if (v < 512) {
    iv0 = (v * 511 + v) + 1;
    y[v] = CC[iv0 - 1];
    for (i = 0; i < 511; i++) {
      xpageoffset = (iv0 + i) + 1;
      if ((static_cast<int32_T>(isnan(CC[xpageoffset - 1].re))) ||
          (static_cast<int32_T>(isnan(CC[xpageoffset - 1].im)))) {
        p = false;
      } else if ((static_cast<int32_T>(isnan(y[v].re))) || (static_cast<int32_T>
                  (isnan(y[v].im)))) {
        p = true;
      } else {
        if ((static_cast<int32_T>(fabs(y[v].re) > 8.9884656743115785E+307)) || (
             static_cast<int32_T>(fabs(y[v].im) > 8.9884656743115785E+307))) {
          SCALEA = true;
        } else {
          SCALEA = false;
        }

        if ((static_cast<int32_T>(fabs(CC[xpageoffset - 1].re) >
              8.9884656743115785E+307)) || (static_cast<int32_T>(fabs
              (CC[xpageoffset - 1].im) > 8.9884656743115785E+307))) {
          p = true;
        } else {
          p = false;
        }

        if ((static_cast<int32_T>(SCALEA)) || (static_cast<int32_T>(p))) {
          x = b_rt_hypotd_snf(y[v].re / 2.0, y[v].im / 2.0);
          br = b_rt_hypotd_snf(CC[xpageoffset - 1].re / 2.0, CC[xpageoffset - 1]
                               .im / 2.0);
        } else {
          x = b_rt_hypotd_snf(y[v].re, y[v].im);
          br = b_rt_hypotd_snf(CC[xpageoffset - 1].re, CC[xpageoffset - 1].im);
        }

        if (x == br) {
          absar = fabs(y[v].re);
          r = fabs(y[v].im);
          absbr = fabs(CC[xpageoffset - 1].re);
          bi = fabs(CC[xpageoffset - 1].im);
          if (absar > r) {
            ai = absar;
            absar = r;
          } else {
            ai = r;
          }

          if (absbr > bi) {
            r = absbr;
            absbr = bi;
          } else {
            r = bi;
          }

          if (ai > r) {
            if (absar < absbr) {
              x = ai - r;
              br = (absar / 2.0 + absbr / 2.0) / (ai / 2.0 + r / 2.0) * (absbr -
                absar);
            } else {
              x = ai;
              br = r;
            }
          } else if (ai < r) {
            if (absar > absbr) {
              br = r - ai;
              x = (absar / 2.0 + absbr / 2.0) / (ai / 2.0 + r / 2.0) * (absar -
                absbr);
            } else {
              x = ai;
              br = r;
            }
          } else {
            x = absar;
            br = absbr;
          }

          if (x == br) {
            x = b_rt_atan2d_snf(y[v].im, y[v].re);
            br = b_rt_atan2d_snf(CC[xpageoffset - 1].im, CC[xpageoffset - 1].re);
            if (x == br) {
              br = CC[xpageoffset - 1].re;
              bi = CC[xpageoffset - 1].im;
              if (x > 0.78539816339744828) {
                if (x > 2.3561944901923448) {
                  x = -y[v].im;
                  br = -bi;
                } else {
                  x = -y[v].re;
                  br = -br;
                }
              } else if (x > -0.78539816339744828) {
                x = y[v].im;
                br = bi;
              } else if (x > -2.3561944901923448) {
                x = y[v].re;
              } else {
                x = -y[v].im;
                br = -bi;
              }

              if (x == br) {
                x = 0.0;
                br = 0.0;
              }
            }
          }
        }

        p = (x < br);
      }

      if (p) {
        y[v] = CC[xpageoffset - 1];
        idx[v] = static_cast<int16_T>(i + 2);
      }
    }
  }
}

static __global__ __launch_bounds__(512, 1) void bd_dftregistration_coderGPU_ker
  (const creal_T *y, const creal_T buf2ft[262144], creal_T Greg[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    Greg[i].re = buf2ft[i].re * y->re - buf2ft[i].im * y->im;
    Greg[i].im = buf2ft[i].re * y->im + buf2ft[i].im * y->re;
  }
}

static __global__ __launch_bounds__(512, 1) void c_dftregistration_coderGPU_kern
  (creal_T CC[1048576])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 1048576) {
    /*  First upsample by a factor of 2 to obtain initial estimate */
    /*  Embed Fourier data in a 2x larger array */
    CC[i].re = 0.0;
    CC[i].im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void cb_dftregistration_coderGPU_ker
  (const int16_T x[512], creal_T y[512])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 512) {
    y[i].re = static_cast<real_T>(x[i]);
    y[i].im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void cc_dftregistration_coderGPU_ker
  (const creal_T buf2ft[262144], const creal_T buf1ft[262144], real_T absb1
   [262144], real_T absb2[262144])
{
  uint32_T threadId;
  real_T r;
  int32_T iv0;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId);
  if (iv0 < 262144) {
    r = b_rt_hypotd_snf(buf1ft[iv0].re, buf1ft[iv0].im);
    absb2[iv0] = b_rt_hypotd_snf(buf2ft[iv0].re, buf2ft[iv0].im);
    r *= r;
    absb1[iv0] = r;
  }
}

static __global__ __launch_bounds__(512, 1) void cd_dftregistration_coderGPU_ker
  (const int16_T b[512], int16_T Nr[262144], int16_T Nc[262144])
{
  uint32_T threadId;
  int32_T i;
  int32_T xpageoffset;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId % 512U);
  xpageoffset = static_cast<int32_T>((threadId - static_cast<uint32_T>(i)) /
    512U);
  if (xpageoffset < 512) {
    Nc[i + (xpageoffset << 9)] = b[xpageoffset];
    Nr[i + (xpageoffset << 9)] = b[i];
  }
}

static void cufftEnsureDestruction()
{
  if (cufftGlobalHandlePtr != NULL) {
    cufftDestroy(*cufftGlobalHandlePtr);
    cufftGlobalHandlePtr = NULL;
    cufftGlobalSize = 0;
    cufftGlobalBatch = 0;
    cufftGlobalType = CUFFT_R2C;
    cufftGlobalDist = 0;
  }
}

static void cufftEnsureInitialization(int32_T nelem, cufftType_t type, int32_T
  batch, int32_T idist)
{
  if ((cufftGlobalDist != idist) || (cufftGlobalType != type) ||
      (cufftGlobalSize != nelem) || (cufftGlobalBatch != batch)) {
    if (cufftGlobalHandlePtr != NULL) {
      cufftEnsureDestruction();
    }

    cufftGlobalHandlePtr = &cufftGlobalHandle;
    cufftPlanMany(cufftGlobalHandlePtr, 1U, &nelem, &idist, 1, idist, &nelem, 1,
                  nelem, type, batch);
    cufftGlobalSize = nelem;
    cufftGlobalBatch = batch;
    cufftGlobalType = type;
    cufftGlobalDist = idist;
  }
}

static __global__ __launch_bounds__(512, 1) void d_dftregistration_coderGPU_kern
  (const creal_T buf1ft[262144], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    CC[i] = buf1ft[i];
  }
}

static __global__ __launch_bounds__(512, 1) void db_dftregistration_coderGPU_ker
  (const emxArray_creal_T *z, emxArray_creal_T *kernr, creal_T y[512])
{
  uint32_T threadId;
  int32_T xpageoffset;
  int32_T iv0;
  real_T bi;
  int32_T i;
  real_T r;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId % 32767U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iv0)) / 32767U);
  if (i < 512) {
    xpageoffset = z->size[0] - 1;
    if (iv0 <= xpageoffset) {
      bi = z->data[iv0].re * y[i].re - z->data[iv0].im * y[i].im;
      r = z->data[iv0].re * y[i].im + z->data[iv0].im * y[i].re;
      kernr->data[iv0 + kernr->size[0] * i].re = bi;
      kernr->data[iv0 + kernr->size[0] * i].im = r;
    }
  }
}

static __global__ __launch_bounds__(32, 1) void dc_dftregistration_coderGPU_ker(
  const real_T absb1[262144], real_T *absar)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *absar = absb1[0];
  }
}

static __global__ __launch_bounds__(512, 1) void dd_dftregistration_coderGPU_ker
  (const int16_T Nc[262144], const real_T col_shift, const int16_T Nr[262144],
   const real_T *row_shift, creal_T Greg[262144])
{
  uint32_T threadId;
  real_T r;
  int32_T i;
  real_T bi;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    r = *row_shift * static_cast<real_T>(Nr[i]) / 512.0;
    bi = col_shift * static_cast<real_T>(Nc[i]) / 512.0;
    Greg[i].re = (r - bi) * 0.0;
    Greg[i].im = (r - bi) * 6.2831853071795862;
  }
}

static __global__ __launch_bounds__(512, 1) void e_dftregistration_coderGPU_kern
  (const creal_T buf2ft[262144], creal_T x[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    x[i] = buf2ft[i];
  }
}

static __global__ __launch_bounds__(32, 1) void eb_dftregistration_coderGPU_ker(
  const emxArray_creal_T *kernr, int32_T *nx)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *nx = kernr->size[0] << 9;
  }
}

static __global__ __launch_bounds__(512, 1) void ec_dftregistration_coderGPU_ker
  (const real_T absb1[262144], real_T *absar)
{
  uint32_T idx;
  real_T tmpRed0;
  uint32_T threadStride;
  uint32_T threadId;
  uint32_T thBlkId;
  uint32_T mask;
  uint32_T numActiveThreads;
  uint32_T numActiveWarps;
  uint32_T blockStride;
  uint32_T m;
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  thBlkId = static_cast<uint32_T>(mwGetThreadIndexWithinBlock());
  blockStride = static_cast<uint32_T>(mwGetThreadsPerBlock());
  tmpRed0 = 0.0;
  numActiveThreads = blockStride;
  if (mwIsLastBlock()) {
    m = 262143U % blockStride;
    if (m > 0U) {
      numActiveThreads = m;
    }
  }

  numActiveWarps = ((numActiveThreads + warpSize) - 1U) / warpSize;
  if (threadId <= 262142U) {
    tmpRed0 = absb1[static_cast<int32_T>(threadId) + 1];
  }

  mask = __ballot_sync(MAX_uint32_T, threadId <= 262142U);
  for (idx = threadId + threadStride; idx <= 262142U; idx += threadStride) {
    tmpRed0 += absb1[static_cast<int32_T>(idx) + 1];
  }

  tmpRed0 = workGroupReduction(tmpRed0, mask, numActiveWarps);
  if (thBlkId == 0U) {
    atomicOpreal_T(&absar[0], tmpRed0);
  }
}

static __global__ __launch_bounds__(512, 1) void ed_dftregistration_coderGPU_ker
  (creal_T Greg[262144])
{
  uint32_T threadId;
  int32_T iv0;
  creal_T x;
  real_T r;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId);
  if (iv0 < 262144) {
    if (Greg[iv0].im == 0.0) {
      x.re = exp(Greg[iv0].re);
      x.im = 0.0;
    } else if ((static_cast<int32_T>((static_cast<int32_T>(isinf(Greg[iv0].im)))
      && (static_cast<int32_T>(isinf(Greg[iv0].re))))) && (static_cast<int32_T>
                (Greg[iv0].re < 0.0))) {
      x.re = 0.0;
      x.im = 0.0;
    } else {
      r = exp(Greg[iv0].re / 2.0);
      x.re = r * (r * cos(Greg[iv0].im));
      x.im = r * (r * sin(Greg[iv0].im));
    }

    Greg[iv0] = x;
  }
}

static __global__ __launch_bounds__(512, 1) void f_dftregistration_coderGPU_kern
  (creal_T CC[262144], creal_T b_CC[1048576], creal_T x[262144])
{
  uint32_T threadId;
  real_T r;
  int32_T iv0;
  int32_T i;
  real_T bi;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId % 512U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iv0)) / 512U);
  if (i < 512) {
    r = x[iv0 + (i << 9)].re;
    bi = -x[iv0 + (i << 9)].im;
    b_CC[(iv0 + ((i + 256) << 10)) + 256].re = CC[iv0 + (i << 9)].re * r -
      CC[iv0 + (i << 9)].im * bi;
    b_CC[(iv0 + ((i + 256) << 10)) + 256].im = CC[iv0 + (i << 9)].re * bi +
      CC[iv0 + (i << 9)].im * r;
  }
}

static __global__ __launch_bounds__(1024, 1) void
  fb_dftregistration_coderGPU_ker(const int32_T *nx, emxArray_creal_T *kernr)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  creal_T x;
  real_T r;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(*nx - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iv0 = static_cast<int32_T>(idx);
    if (kernr->data[iv0].im == 0.0) {
      x.re = exp(kernr->data[iv0].re);
      x.im = 0.0;
    } else if ((static_cast<int32_T>((static_cast<int32_T>(isinf(kernr->data[iv0]
        .im))) && (static_cast<int32_T>(isinf(kernr->data[iv0].re))))) && (
                static_cast<int32_T>(kernr->data[iv0].re < 0.0))) {
      x.re = 0.0;
      x.im = 0.0;
    } else {
      r = exp(kernr->data[iv0].re / 2.0);
      x.re = r * (r * cos(kernr->data[iv0].im));
      x.im = r * (r * sin(kernr->data[iv0].im));
    }

    kernr->data[iv0] = x;
  }
}

static __global__ __launch_bounds__(512, 1) void fc_dftregistration_coderGPU_ker
  (real_T absb2[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    absb2[i] *= absb2[i];
  }
}

static __global__ __launch_bounds__(32, 1) void fd_dftregistration_coderGPU_ker(
  const real_T *tmp_im, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = *tmp_im * 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void g_dftregistration_coderGPU_kern
  (creal_T CC[1048576], creal_T y[1048576])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId % 1024U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iv0)) / 1024U);
  if (i < 1024) {
    y[iv0 + (i << 10)] = CC[i + (iv0 << 10)];
  }
}

static __global__ __launch_bounds__(1024, 1) void
  gb_dftregistration_coderGPU_ker(const emxArray_creal_T *kernr,
  emxArray_creal_T *y)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(kernr->size[0] * 512 - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    i = static_cast<int32_T>(idx);
    y->data[i].re = 0.0;
    y->data[i].im = 0.0;
  }
}

static __global__ __launch_bounds__(32, 1) void gc_dftregistration_coderGPU_ker(
  const real_T absb2[262144], real_T *tmp_re)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *tmp_re = absb2[0];
  }
}

static __global__ __launch_bounds__(32, 1) void gd_dftregistration_coderGPU_ker(
  const creal_T *y, real_T *tmp_re)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *tmp_re = exp(y->re / 2.0);
  }
}

static void gpuEmxFree_creal_T(emxArray_creal_T *inter)
{
  cudaFree(inter->data);
  cudaFree(inter->size);
}

static void gpuEmxFree_int16_T(emxArray_int16_T *inter)
{
  cudaFree(inter->data);
  cudaFree(inter->size);
}

static void gpuEmxFree_real_T(emxArray_real_T *inter)
{
  cudaFree(inter->data);
  cudaFree(inter->size);
}

static void gpuEmxFree_uint16_T(emxArray_uint16_T *inter)
{
  cudaFree(inter->data);
  cudaFree(inter->size);
}

static void gpuEmxMemcpyCpuToGpu_creal_T(const emxArray_creal_T *cpu,
  emxArray_creal_T *inter, emxArray_creal_T *gpu)
{
  int32_T actualSize;
  int32_T i;
  int32_T allocatingSize;
  if (inter->numDimensions < cpu->numDimensions) {
    inter->numDimensions = cpu->numDimensions;
    cudaFree(inter->size);
    cudaMalloc(&inter->size, inter->numDimensions * sizeof(int32_T));
  } else {
    inter->numDimensions = cpu->numDimensions;
  }

  actualSize = 1;
  for (i = 0; i < cpu->numDimensions; i++) {
    actualSize *= cpu->size[i];
  }

  if (inter->allocatedSize < actualSize) {
    if (inter->canFreeData) {
      cudaFree(inter->data);
    }

    allocatingSize = cpu->allocatedSize;
    if (allocatingSize < actualSize) {
      allocatingSize = actualSize;
    }

    inter->allocatedSize = allocatingSize;
    inter->canFreeData = true;
    cudaMalloc(&inter->data, inter->allocatedSize * sizeof(creal_T));
  }

  cudaMemcpy(inter->data, cpu->data, actualSize * sizeof(creal_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(inter->size, cpu->size, cpu->numDimensions * sizeof(int32_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(gpu, inter, 32ULL, cudaMemcpyHostToDevice);
}

static void gpuEmxMemcpyCpuToGpu_int16_T(const emxArray_int16_T *cpu,
  emxArray_int16_T *inter, emxArray_int16_T *gpu)
{
  int32_T actualSize;
  int32_T i;
  int32_T allocatingSize;
  if (inter->numDimensions < cpu->numDimensions) {
    inter->numDimensions = cpu->numDimensions;
    cudaFree(inter->size);
    cudaMalloc(&inter->size, inter->numDimensions * sizeof(int32_T));
  } else {
    inter->numDimensions = cpu->numDimensions;
  }

  actualSize = 1;
  for (i = 0; i < cpu->numDimensions; i++) {
    actualSize *= cpu->size[i];
  }

  if (inter->allocatedSize < actualSize) {
    if (inter->canFreeData) {
      cudaFree(inter->data);
    }

    allocatingSize = cpu->allocatedSize;
    if (allocatingSize < actualSize) {
      allocatingSize = actualSize;
    }

    inter->allocatedSize = allocatingSize;
    inter->canFreeData = true;
    cudaMalloc(&inter->data, inter->allocatedSize * sizeof(int16_T));
  }

  cudaMemcpy(inter->data, cpu->data, actualSize * sizeof(int16_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(inter->size, cpu->size, cpu->numDimensions * sizeof(int32_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(gpu, inter, 32ULL, cudaMemcpyHostToDevice);
}

static void gpuEmxMemcpyCpuToGpu_real_T(const emxArray_real_T *cpu,
  emxArray_real_T *inter, emxArray_real_T *gpu)
{
  int32_T actualSize;
  int32_T i;
  int32_T allocatingSize;
  if (inter->numDimensions < cpu->numDimensions) {
    inter->numDimensions = cpu->numDimensions;
    cudaFree(inter->size);
    cudaMalloc(&inter->size, inter->numDimensions * sizeof(int32_T));
  } else {
    inter->numDimensions = cpu->numDimensions;
  }

  actualSize = 1;
  for (i = 0; i < cpu->numDimensions; i++) {
    actualSize *= cpu->size[i];
  }

  if (inter->allocatedSize < actualSize) {
    if (inter->canFreeData) {
      cudaFree(inter->data);
    }

    allocatingSize = cpu->allocatedSize;
    if (allocatingSize < actualSize) {
      allocatingSize = actualSize;
    }

    inter->allocatedSize = allocatingSize;
    inter->canFreeData = true;
    cudaMalloc(&inter->data, inter->allocatedSize * sizeof(real_T));
  }

  cudaMemcpy(inter->data, cpu->data, actualSize * sizeof(real_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(inter->size, cpu->size, cpu->numDimensions * sizeof(int32_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(gpu, inter, 32ULL, cudaMemcpyHostToDevice);
}

static void gpuEmxMemcpyCpuToGpu_uint16_T(const emxArray_uint16_T *cpu,
  emxArray_uint16_T *inter, emxArray_uint16_T *gpu)
{
  int32_T actualSize;
  int32_T i;
  int32_T allocatingSize;
  if (inter->numDimensions < cpu->numDimensions) {
    inter->numDimensions = cpu->numDimensions;
    cudaFree(inter->size);
    cudaMalloc(&inter->size, inter->numDimensions * sizeof(int32_T));
  } else {
    inter->numDimensions = cpu->numDimensions;
  }

  actualSize = 1;
  for (i = 0; i < cpu->numDimensions; i++) {
    actualSize *= cpu->size[i];
  }

  if (inter->allocatedSize < actualSize) {
    if (inter->canFreeData) {
      cudaFree(inter->data);
    }

    allocatingSize = cpu->allocatedSize;
    if (allocatingSize < actualSize) {
      allocatingSize = actualSize;
    }

    inter->allocatedSize = allocatingSize;
    inter->canFreeData = true;
    cudaMalloc(&inter->data, inter->allocatedSize * sizeof(uint16_T));
  }

  cudaMemcpy(inter->data, cpu->data, actualSize * sizeof(uint16_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(inter->size, cpu->size, cpu->numDimensions * sizeof(int32_T),
             cudaMemcpyHostToDevice);
  cudaMemcpy(gpu, inter, 32ULL, cudaMemcpyHostToDevice);
}

static void gpuEmxMemcpyGpuToCpu_creal_T(emxArray_creal_T *cpu, emxArray_creal_T
  *inter)
{
  int32_T actualSize;
  int32_T i;
  actualSize = 1;
  for (i = 0; i < cpu->numDimensions; i++) {
    actualSize *= cpu->size[i];
  }

  cudaMemcpy(cpu->data, inter->data, actualSize * sizeof(creal_T),
             cudaMemcpyDeviceToHost);
  cudaMemcpy(cpu->size, inter->size, inter->numDimensions * sizeof(int32_T),
             cudaMemcpyDeviceToHost);
}

static void gpuEmxMemcpyGpuToCpu_real_T(emxArray_real_T *cpu, emxArray_real_T
  *inter)
{
  int32_T actualSize;
  int32_T i;
  actualSize = 1;
  for (i = 0; i < cpu->numDimensions; i++) {
    actualSize *= cpu->size[i];
  }

  cudaMemcpy(cpu->data, inter->data, actualSize * sizeof(real_T),
             cudaMemcpyDeviceToHost);
  cudaMemcpy(cpu->size, inter->size, inter->numDimensions * sizeof(int32_T),
             cudaMemcpyDeviceToHost);
}

static void gpuEmxMemcpyGpuToCpu_uint16_T(emxArray_uint16_T *cpu,
  emxArray_uint16_T *inter)
{
  int32_T actualSize;
  int32_T i;
  actualSize = 1;
  for (i = 0; i < cpu->numDimensions; i++) {
    actualSize *= cpu->size[i];
  }

  cudaMemcpy(cpu->data, inter->data, actualSize * sizeof(uint16_T),
             cudaMemcpyDeviceToHost);
  cudaMemcpy(cpu->size, inter->size, inter->numDimensions * sizeof(int32_T),
             cudaMemcpyDeviceToHost);
}

static void gpuEmxReset_creal_T(emxArray_creal_T *inter)
{
  memset(inter, 0, sizeof(emxArray_creal_T));
}

static void gpuEmxReset_int16_T(emxArray_int16_T *inter)
{
  memset(inter, 0, sizeof(emxArray_int16_T));
}

static void gpuEmxReset_real_T(emxArray_real_T *inter)
{
  memset(inter, 0, sizeof(emxArray_real_T));
}

static void gpuEmxReset_uint16_T(emxArray_uint16_T *inter)
{
  memset(inter, 0, sizeof(emxArray_uint16_T));
}

static __global__ __launch_bounds__(512, 1) void h_dftregistration_coderGPU_kern
  (creal_T CC[1048576])
{
  uint32_T threadId;
  int32_T i;
  real_T bi;
  real_T r;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 1048576) {
    if (CC[i].im == 0.0) {
      bi = CC[i].re / 1024.0;
      r = 0.0;
    } else if (CC[i].re == 0.0) {
      bi = 0.0;
      r = CC[i].im / 1024.0;
    } else {
      bi = CC[i].re / 1024.0;
      r = CC[i].im / 1024.0;
    }

    CC[i].re = bi;
    CC[i].im = r;
  }
}

static __global__ __launch_bounds__(1024, 1) void
  hb_dftregistration_coderGPU_ker(const emxArray_creal_T *kernc, const
  emxArray_creal_T *y, emxArray_creal_T *out)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(y->size[0] * kernc->size[1] - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    i = static_cast<int32_T>(idx);
    out->data[i].re = 0.0;
    out->data[i].im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void hc_dftregistration_coderGPU_ker
  (const real_T absb2[262144], real_T *tmp_re)
{
  uint32_T idx;
  real_T tmpRed0;
  uint32_T threadStride;
  uint32_T threadId;
  uint32_T thBlkId;
  uint32_T mask;
  uint32_T numActiveThreads;
  uint32_T numActiveWarps;
  uint32_T blockStride;
  uint32_T m;
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  thBlkId = static_cast<uint32_T>(mwGetThreadIndexWithinBlock());
  blockStride = static_cast<uint32_T>(mwGetThreadsPerBlock());
  tmpRed0 = 0.0;
  numActiveThreads = blockStride;
  if (mwIsLastBlock()) {
    m = 262143U % blockStride;
    if (m > 0U) {
      numActiveThreads = m;
    }
  }

  numActiveWarps = ((numActiveThreads + warpSize) - 1U) / warpSize;
  if (threadId <= 262142U) {
    tmpRed0 = absb2[static_cast<int32_T>(threadId) + 1];
  }

  mask = __ballot_sync(MAX_uint32_T, threadId <= 262142U);
  for (idx = threadId + threadStride; idx <= 262142U; idx += threadStride) {
    tmpRed0 += absb2[static_cast<int32_T>(idx) + 1];
  }

  tmpRed0 = workGroupReduction(tmpRed0, mask, numActiveWarps);
  if (thBlkId == 0U) {
    atomicOpreal_T(&tmp_re[0], tmpRed0);
  }
}

static __global__ __launch_bounds__(32, 1) void hd_dftregistration_coderGPU_ker(
  const real_T *tmp_im, const real_T *tmp_re, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = *tmp_re * (*tmp_re * cos(*tmp_im));
    y->im = *tmp_re * (*tmp_re * sin(*tmp_im));
  }
}

static __global__ __launch_bounds__(512, 1) void i_dftregistration_coderGPU_kern
  (creal_T CC[1048576], creal_T y[1048576])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId % 1024U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iv0)) / 1024U);
  if (i < 1024) {
    y[iv0 + (i << 10)] = CC[i + (iv0 << 10)];
  }
}

static __global__ __launch_bounds__(1024, 1) void
  ib_dftregistration_coderGPU_ker(const real_T *absar, const emxArray_creal_T
  *out, emxArray_creal_T *CC)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  real_T r;
  int32_T i;
  real_T ai;
  real_T bi;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(out->size[0] * out->size[1] - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    i = static_cast<int32_T>(idx);
    r = out->data[i].re;
    ai = -out->data[i].im;
    if (ai == 0.0) {
      bi = r / *absar;
      r = 0.0;
    } else if (r == 0.0) {
      bi = 0.0;
      r = ai / *absar;
    } else {
      bi = r / *absar;
      r = ai / *absar;
    }

    CC->data[i].re = bi;
    CC->data[i].im = r;
  }
}

static __global__ __launch_bounds__(32, 1) void ic_dftregistration_coderGPU_ker(
  const real_T col_shift, const real_T *row_shift, const real_T *tmp_im, const
  real_T Ma, const real_T absbr, real_T dv1[4])
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    dv1[0] = sqrt(b_rt_hypotd_snf(1.0 - absbr, 0.0 - Ma));
    dv1[1] = *tmp_im;
    dv1[2] = *row_shift;
    dv1[3] = col_shift;
  }
}

static __global__ __launch_bounds__(32, 1) void id_dftregistration_coderGPU_ker(
  const creal_T *y, real_T *tmp_re)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *tmp_re = y->re;
  }
}

static __global__ __launch_bounds__(512, 1) void j_dftregistration_coderGPU_kern
  (creal_T CC[1048576])
{
  uint32_T threadId;
  int32_T i;
  real_T bi;
  real_T r;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 1048576) {
    if (CC[i].im == 0.0) {
      bi = CC[i].re / 1024.0;
      r = 0.0;
    } else if (CC[i].re == 0.0) {
      bi = 0.0;
      r = CC[i].im / 1024.0;
    } else {
      bi = CC[i].re / 1024.0;
      r = CC[i].im / 1024.0;
    }

    CC[i].re = bi;
    CC[i].im = r;
  }
}

static __global__ __launch_bounds__(32, 1) void jb_dftregistration_coderGPU_ker(
  const emxArray_creal_T *ex, uint16_T uv[2])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 2) {
    uv[i] = static_cast<uint16_T>(ex->size[i]);
  }
}

static __global__ __launch_bounds__(32, 1) void jc_dftregistration_coderGPU_ker(
  const real_T dv1[4], emxArray_real_T *output)
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 4) {
    output->data[i] = dv1[i];
  }
}

static __global__ __launch_bounds__(32, 1) void jd_dftregistration_coderGPU_ker(
  const real_T *tmp_re, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = exp(*tmp_re);
    y->im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void k_dftregistration_coderGPU_kern
  (int16_T idx[1024])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 1024) {
    /*  Calculate cross-correlation */
    idx[i] = static_cast<int16_T>(1);
  }
}

static __global__ __launch_bounds__(1024, 1) void
  kb_dftregistration_coderGPU_ker(const uint16_T uv[2], emxArray_uint16_T *idx)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T b_idx;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(static_cast<int32_T>(uv[1]) - 1);
  for (b_idx = threadId; b_idx <= static_cast<uint32_T>(loopEnd); b_idx +=
       threadStride) {
    i = static_cast<int32_T>(b_idx);
    idx->data[i] = static_cast<uint16_T>(1U);
  }
}

static __global__ __launch_bounds__(512, 1) void kc_dftregistration_coderGPU_ker
  (const creal_T buf2ft[262144], const creal_T buf1ft[262144], creal_T CC[262144],
   real_T absb2[262144], real_T absb1[262144])
{
  uint32_T threadId;
  int32_T iv0;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId);
  if (iv0 < 262144) {
    absb1[iv0] = b_rt_hypotd_snf(buf1ft[iv0].re, buf1ft[iv0].im);
    absb2[iv0] = b_rt_hypotd_snf(buf2ft[iv0].re, buf2ft[iv0].im);
    CC[iv0].re = buf1ft[iv0].re * buf2ft[iv0].re - buf1ft[iv0].im * -buf2ft[iv0]
      .im;
    CC[iv0].im = buf1ft[iv0].re * -buf2ft[iv0].im + buf1ft[iv0].im * buf2ft[iv0]
      .re;
  }
}

static __global__ __launch_bounds__(512, 1) void kd_dftregistration_coderGPU_ker
  (const creal_T *y, const creal_T buf2ft[262144], creal_T Greg[262144])
{
  uint32_T threadId;
  real_T r;
  int32_T i;
  real_T bi;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    r = buf2ft[i].re * Greg[i].re - buf2ft[i].im * Greg[i].im;
    bi = buf2ft[i].re * Greg[i].im + buf2ft[i].im * Greg[i].re;
    Greg[i].re = r * y->re - bi * y->im;
    Greg[i].im = r * y->im + bi * y->re;
  }
}

static __global__ __launch_bounds__(512, 1) void l_dftregistration_coderGPU_kern
  (int16_T idx[1024], creal_T CC[1048576], creal_T ex[1024])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T v;
  int32_T i;
  int32_T xpageoffset;
  boolean_T p;
  boolean_T SCALEA;
  real_T x;
  real_T br;
  real_T absar;
  real_T r;
  real_T absbr;
  real_T bi;
  real_T ai;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  v = static_cast<int32_T>(threadId);
  if (v < 1024) {
    iv0 = (v * 1023 + v) + 1;
    ex[v] = CC[iv0 - 1];
    for (i = 0; i < 1023; i++) {
      xpageoffset = (iv0 + i) + 1;
      if ((static_cast<int32_T>(isnan(CC[xpageoffset - 1].re))) ||
          (static_cast<int32_T>(isnan(CC[xpageoffset - 1].im)))) {
        p = false;
      } else if ((static_cast<int32_T>(isnan(ex[v].re))) || (static_cast<int32_T>
                  (isnan(ex[v].im)))) {
        p = true;
      } else {
        if ((static_cast<int32_T>(fabs(ex[v].re) > 8.9884656743115785E+307)) ||
            (static_cast<int32_T>(fabs(ex[v].im) > 8.9884656743115785E+307))) {
          SCALEA = true;
        } else {
          SCALEA = false;
        }

        if ((static_cast<int32_T>(fabs(CC[xpageoffset - 1].re) >
              8.9884656743115785E+307)) || (static_cast<int32_T>(fabs
              (CC[xpageoffset - 1].im) > 8.9884656743115785E+307))) {
          p = true;
        } else {
          p = false;
        }

        if ((static_cast<int32_T>(SCALEA)) || (static_cast<int32_T>(p))) {
          x = b_rt_hypotd_snf(ex[v].re / 2.0, ex[v].im / 2.0);
          br = b_rt_hypotd_snf(CC[xpageoffset - 1].re / 2.0, CC[xpageoffset - 1]
                               .im / 2.0);
        } else {
          x = b_rt_hypotd_snf(ex[v].re, ex[v].im);
          br = b_rt_hypotd_snf(CC[xpageoffset - 1].re, CC[xpageoffset - 1].im);
        }

        if (x == br) {
          absar = fabs(ex[v].re);
          r = fabs(ex[v].im);
          absbr = fabs(CC[xpageoffset - 1].re);
          bi = fabs(CC[xpageoffset - 1].im);
          if (absar > r) {
            ai = absar;
            absar = r;
          } else {
            ai = r;
          }

          if (absbr > bi) {
            r = absbr;
            absbr = bi;
          } else {
            r = bi;
          }

          if (ai > r) {
            if (absar < absbr) {
              x = ai - r;
              br = (absar / 2.0 + absbr / 2.0) / (ai / 2.0 + r / 2.0) * (absbr -
                absar);
            } else {
              x = ai;
              br = r;
            }
          } else if (ai < r) {
            if (absar > absbr) {
              br = r - ai;
              x = (absar / 2.0 + absbr / 2.0) / (ai / 2.0 + r / 2.0) * (absar -
                absbr);
            } else {
              x = ai;
              br = r;
            }
          } else {
            x = absar;
            br = absbr;
          }

          if (x == br) {
            x = b_rt_atan2d_snf(ex[v].im, ex[v].re);
            br = b_rt_atan2d_snf(CC[xpageoffset - 1].im, CC[xpageoffset - 1].re);
            if (x == br) {
              br = CC[xpageoffset - 1].re;
              bi = CC[xpageoffset - 1].im;
              if (x > 0.78539816339744828) {
                if (x > 2.3561944901923448) {
                  x = -ex[v].im;
                  br = -bi;
                } else {
                  x = -ex[v].re;
                  br = -br;
                }
              } else if (x > -0.78539816339744828) {
                x = ex[v].im;
                br = bi;
              } else if (x > -2.3561944901923448) {
                x = ex[v].re;
              } else {
                x = -ex[v].im;
                br = -bi;
              }

              if (x == br) {
                x = 0.0;
                br = 0.0;
              }
            }
          }
        }

        p = (x < br);
      }

      if (p) {
        ex[v] = CC[xpageoffset - 1];
        idx[v] = static_cast<int16_T>(i + 2);
      }
    }
  }
}

static __global__ __launch_bounds__(1024, 1) void
  lb_dftregistration_coderGPU_ker(const emxArray_creal_T *CC, const int32_T
  vstride, const int32_T npages, emxArray_uint16_T *idx, emxArray_creal_T *ex)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T b_idx;
  int32_T iv0;
  int32_T v;
  int32_T i;
  int32_T xpageoffset;
  boolean_T p;
  boolean_T SCALEA;
  real_T x;
  real_T br;
  real_T absar;
  real_T r;
  real_T absbr;
  real_T bi;
  real_T ai;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(npages - 1);
  for (b_idx = threadId; b_idx <= static_cast<uint32_T>(loopEnd); b_idx +=
       threadStride) {
    v = static_cast<int32_T>(b_idx);
    iv0 = (v * (vstride - 1) + v) + 1;
    ex->data[v] = CC->data[iv0 - 1];
    for (i = 0; i <= vstride - 2; i++) {
      xpageoffset = (iv0 + i) + 1;
      if ((static_cast<int32_T>(isnan(CC->data[xpageoffset - 1].re))) || (
           static_cast<int32_T>(isnan(CC->data[xpageoffset - 1].im)))) {
        p = false;
      } else if ((static_cast<int32_T>(isnan(ex->data[v].re))) ||
                 (static_cast<int32_T>(isnan(ex->data[v].im)))) {
        p = true;
      } else {
        if ((static_cast<int32_T>(fabs(ex->data[v].re) > 8.9884656743115785E+307))
            || (static_cast<int32_T>(fabs(ex->data[v].im) >
              8.9884656743115785E+307))) {
          SCALEA = true;
        } else {
          SCALEA = false;
        }

        if ((static_cast<int32_T>(fabs(CC->data[xpageoffset - 1].re) >
              8.9884656743115785E+307)) || (static_cast<int32_T>(fabs(CC->
               data[xpageoffset - 1].im) > 8.9884656743115785E+307))) {
          p = true;
        } else {
          p = false;
        }

        if ((static_cast<int32_T>(SCALEA)) || (static_cast<int32_T>(p))) {
          x = b_rt_hypotd_snf(ex->data[v].re / 2.0, ex->data[v].im / 2.0);
          br = b_rt_hypotd_snf(CC->data[xpageoffset - 1].re / 2.0, CC->
                               data[xpageoffset - 1].im / 2.0);
        } else {
          x = b_rt_hypotd_snf(ex->data[v].re, ex->data[v].im);
          br = b_rt_hypotd_snf(CC->data[xpageoffset - 1].re, CC->
                               data[xpageoffset - 1].im);
        }

        if (x == br) {
          absar = fabs(ex->data[v].re);
          r = fabs(ex->data[v].im);
          absbr = fabs(CC->data[xpageoffset - 1].re);
          bi = fabs(CC->data[xpageoffset - 1].im);
          if (absar > r) {
            ai = absar;
            absar = r;
          } else {
            ai = r;
          }

          if (absbr > bi) {
            r = absbr;
            absbr = bi;
          } else {
            r = bi;
          }

          if (ai > r) {
            if (absar < absbr) {
              x = ai - r;
              br = (absar / 2.0 + absbr / 2.0) / (ai / 2.0 + r / 2.0) * (absbr -
                absar);
            } else {
              x = ai;
              br = r;
            }
          } else if (ai < r) {
            if (absar > absbr) {
              br = r - ai;
              x = (absar / 2.0 + absbr / 2.0) / (ai / 2.0 + r / 2.0) * (absar -
                absbr);
            } else {
              x = ai;
              br = r;
            }
          } else {
            x = absar;
            br = absbr;
          }

          if (x == br) {
            x = b_rt_atan2d_snf(ex->data[v].im, ex->data[v].re);
            br = b_rt_atan2d_snf(CC->data[xpageoffset - 1].im, CC->
                                 data[xpageoffset - 1].re);
            if (x == br) {
              r = ex->data[v].re;
              ai = ex->data[v].im;
              br = CC->data[xpageoffset - 1].re;
              bi = CC->data[xpageoffset - 1].im;
              if (x > 0.78539816339744828) {
                if (x > 2.3561944901923448) {
                  r = -ai;
                  br = -bi;
                } else {
                  r = -r;
                  br = -br;
                }
              } else if (x > -0.78539816339744828) {
                r = ai;
                br = bi;
              } else {
                if (!static_cast<int32_T>(x > -2.3561944901923448)) {
                  r = -ai;
                  br = -bi;
                }
              }

              x = r;
              if (r == br) {
                x = 0.0;
                br = 0.0;
              }
            }
          }
        }

        p = (x < br);
      }

      if (p) {
        ex->data[v] = CC->data[xpageoffset - 1];
        idx->data[v] = static_cast<uint16_T>(i + 2);
      }
    }
  }
}

static __global__ __launch_bounds__(512, 1) void lc_dftregistration_coderGPU_ker
  (creal_T y[512], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T xpageoffset;
  int32_T i;
  real_T bi;
  real_T r;
  int32_T iv0;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 512) {
    xpageoffset = i << 9;
    bi = CC[xpageoffset].re;
    r = CC[xpageoffset].im;
    for (iv0 = 0; iv0 < 511; iv0++) {
      bi += CC[(xpageoffset + iv0) + 1].re;
      r += CC[(xpageoffset + iv0) + 1].im;
    }

    y[i].re = bi;
    y[i].im = r;
  }
}

static __global__ __launch_bounds__(32, 1) void m_dftregistration_coderGPU_kern(
  const int16_T idx[1024], const int32_T *nx, real_T *row_shift, creal_T CC
  [1048576], creal_T *CCmax)
{
  uint32_T threadId;
  int32_T tmpIdx;
  int32_T b_idx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    /*  Obtain shift in original pixel grid from the position of the */
    /*  crosscorrelation peak  */
    *CCmax = CC[(static_cast<int32_T>(idx[*nx]) + (*nx << 10)) - 1];
    if (static_cast<int32_T>(idx[*nx]) > 512) {
      b_idx = static_cast<int32_T>(idx[*nx]) - 1025;
    } else {
      b_idx = static_cast<int32_T>(idx[*nx]) - 1;
    }

    *row_shift = static_cast<real_T>(b_idx) / 2.0;
  }
}

static __global__ __launch_bounds__(32, 1) void mb_dftregistration_coderGPU_ker(
  const emxArray_creal_T *CC, const int32_T *nx, const emxArray_uint16_T *idx,
  creal_T *CCmax)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *CCmax = CC->data[(static_cast<int32_T>(idx->data[*nx]) + CC->size[0] * *nx)
      - 1];
  }
}

static __global__ __launch_bounds__(32, 1) void mc_dftregistration_coderGPU_ker
  (creal_T *CCmax)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    CCmax->re = 0.0;
    CCmax->im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void n_dftregistration_coderGPU_kern
  (const creal_T buf1ft[262144], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    CC[i].re = buf1ft[i].re * buf1ft[i].re - buf1ft[i].im * -buf1ft[i].im;
    CC[i].im = buf1ft[i].re * -buf1ft[i].im + buf1ft[i].im * buf1ft[i].re;
  }
}

static __global__ __launch_bounds__(512, 1) void nb_dftregistration_coderGPU_ker
  (const creal_T buf1ft[262144], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    CC[i].re = buf1ft[i].re * buf1ft[i].re - buf1ft[i].im * -buf1ft[i].im;
    CC[i].im = buf1ft[i].re * -buf1ft[i].im + buf1ft[i].im * buf1ft[i].re;
  }
}

static __global__ __launch_bounds__(512, 1) void nc_dftregistration_coderGPU_ker
  (real_T absb1[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    absb1[i] *= absb1[i];
  }
}

static __global__ __launch_bounds__(512, 1) void o_dftregistration_coderGPU_kern
  (creal_T y[512], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T xpageoffset;
  int32_T i;
  real_T bi;
  real_T r;
  int32_T iv0;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 512) {
    xpageoffset = i << 9;
    bi = CC[xpageoffset].re;
    r = CC[xpageoffset].im;
    for (iv0 = 0; iv0 < 511; iv0++) {
      bi += CC[(xpageoffset + iv0) + 1].re;
      r += CC[(xpageoffset + iv0) + 1].im;
    }

    y[i].re = bi;
    y[i].im = r;
  }
}

static __global__ __launch_bounds__(512, 1) void ob_dftregistration_coderGPU_ker
  (creal_T y[512], creal_T kernr[512])
{
  uint32_T threadId;
  creal_T x;
  int32_T iv0;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId);
  if (iv0 < 512) {
    x.re = 1.0;
    x.im = 0.0;
    kernr[iv0] = x;
    y[iv0].re = 0.0;
    y[iv0].im = 0.0;
  }
}

static __global__ __launch_bounds__(32, 1) void oc_dftregistration_coderGPU_ker(
  const real_T absb1[262144], real_T *tmp_re)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *tmp_re = absb1[0];
  }
}

static __global__ __launch_bounds__(32, 1) void p_dftregistration_coderGPU_kern
  (creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = 0.0;
    y->im = 0.0;
  }
}

static __global__ __launch_bounds__(32, 1) void pb_dftregistration_coderGPU_ker
  (creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = 0.0;
    y->im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void pc_dftregistration_coderGPU_ker
  (const real_T absb1[262144], real_T *tmp_re)
{
  uint32_T idx;
  real_T tmpRed0;
  uint32_T threadStride;
  uint32_T threadId;
  uint32_T thBlkId;
  uint32_T mask;
  uint32_T numActiveThreads;
  uint32_T numActiveWarps;
  uint32_T blockStride;
  uint32_T m;
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  thBlkId = static_cast<uint32_T>(mwGetThreadIndexWithinBlock());
  blockStride = static_cast<uint32_T>(mwGetThreadsPerBlock());
  tmpRed0 = 0.0;
  numActiveThreads = blockStride;
  if (mwIsLastBlock()) {
    m = 262143U % blockStride;
    if (m > 0U) {
      numActiveThreads = m;
    }
  }

  numActiveWarps = ((numActiveThreads + warpSize) - 1U) / warpSize;
  if (threadId <= 262142U) {
    tmpRed0 = absb1[static_cast<int32_T>(threadId) + 1];
  }

  mask = __ballot_sync(MAX_uint32_T, threadId <= 262142U);
  for (idx = threadId + threadStride; idx <= 262142U; idx += threadStride) {
    tmpRed0 += absb1[static_cast<int32_T>(idx) + 1];
  }

  tmpRed0 = workGroupReduction(tmpRed0, mask, numActiveWarps);
  if (thBlkId == 0U) {
    atomicOpreal_T(&tmp_re[0], tmpRed0);
  }
}

static __global__ __launch_bounds__(512, 1) void q_dftregistration_coderGPU_kern
  (const creal_T buf2ft[262144], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    CC[i].re = buf2ft[i].re * buf2ft[i].re - buf2ft[i].im * -buf2ft[i].im;
    CC[i].im = buf2ft[i].re * -buf2ft[i].im + buf2ft[i].im * buf2ft[i].re;
  }
}

static __global__ __launch_bounds__(512, 1) void qb_dftregistration_coderGPU_ker
  (const creal_T buf2ft[262144], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    CC[i].re = buf2ft[i].re * buf2ft[i].re - buf2ft[i].im * -buf2ft[i].im;
    CC[i].im = buf2ft[i].re * -buf2ft[i].im + buf2ft[i].im * buf2ft[i].re;
  }
}

static __global__ __launch_bounds__(512, 1) void qc_dftregistration_coderGPU_ker
  (real_T absb2[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    absb2[i] *= absb2[i];
  }
}

static __global__ __launch_bounds__(512, 1) void r_dftregistration_coderGPU_kern
  (creal_T y[512], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T xpageoffset;
  int32_T i;
  real_T bi;
  real_T r;
  int32_T iv0;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 512) {
    xpageoffset = i << 9;
    bi = CC[xpageoffset].re;
    r = CC[xpageoffset].im;
    for (iv0 = 0; iv0 < 511; iv0++) {
      bi += CC[(xpageoffset + iv0) + 1].re;
      r += CC[(xpageoffset + iv0) + 1].im;
    }

    y[i].re = bi;
    y[i].im = r;
  }
}

static __global__ __launch_bounds__(512, 1) void rb_dftregistration_coderGPU_ker
  (creal_T y[512], creal_T kernr[512])
{
  uint32_T threadId;
  creal_T x;
  int32_T iv0;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId);
  if (iv0 < 512) {
    x.re = 1.0;
    x.im = 0.0;
    kernr[iv0] = x;
    y[iv0].re = 0.0;
    y[iv0].im = 0.0;
  }
}

static __global__ __launch_bounds__(32, 1) void rc_dftregistration_coderGPU_ker(
  const real_T absb2[262144], real_T *tmp_im)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *tmp_im = absb2[0];
  }
}

static real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T b_u0;
  int32_T b_u1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = atan2(static_cast<real_T>(b_u0), static_cast<real_T>(b_u1));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

static real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T a;
  real_T b;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

static real_T rt_roundd_snf(real_T u)
{
  real_T y;
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

static __global__ __launch_bounds__(32, 1) void s_dftregistration_coderGPU_kern
  (creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = 0.0;
    y->im = 0.0;
  }
}

static __global__ __launch_bounds__(32, 1) void sb_dftregistration_coderGPU_ker
  (creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = 0.0;
    y->im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void sc_dftregistration_coderGPU_ker
  (const real_T absb2[262144], real_T *tmp_im)
{
  uint32_T idx;
  real_T tmpRed0;
  uint32_T threadStride;
  uint32_T threadId;
  uint32_T thBlkId;
  uint32_T mask;
  uint32_T numActiveThreads;
  uint32_T numActiveWarps;
  uint32_T blockStride;
  uint32_T m;
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  thBlkId = static_cast<uint32_T>(mwGetThreadIndexWithinBlock());
  blockStride = static_cast<uint32_T>(mwGetThreadsPerBlock());
  tmpRed0 = 0.0;
  numActiveThreads = blockStride;
  if (mwIsLastBlock()) {
    m = 262143U % blockStride;
    if (m > 0U) {
      numActiveThreads = m;
    }
  }

  numActiveWarps = ((numActiveThreads + warpSize) - 1U) / warpSize;
  if (threadId <= 262142U) {
    tmpRed0 = absb2[static_cast<int32_T>(threadId) + 1];
  }

  mask = __ballot_sync(MAX_uint32_T, threadId <= 262142U);
  for (idx = threadId + threadStride; idx <= 262142U; idx += threadStride) {
    tmpRed0 += absb2[static_cast<int32_T>(idx) + 1];
  }

  tmpRed0 = workGroupReduction(tmpRed0, mask, numActiveWarps);
  if (thBlkId == 0U) {
    atomicOpreal_T(&tmp_im[0], tmpRed0);
  }
}

static __inline__ __device__ real_T shflDown2(real_T in1, uint32_T offset,
  uint32_T mask)
{
  int2 tmp;
  tmp = *(int2 *)&in1;
  tmp.x = __shfl_down_sync(mask, tmp.x, offset);
  tmp.y = __shfl_down_sync(mask, tmp.y, offset);
  return *(real_T *)&tmp;
}

static __global__ __launch_bounds__(512, 1) void t_dftregistration_coderGPU_kern
  (const creal_T buf1ft[262144], const creal_T buf2ft[262144], creal_T CC[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    CC[i].re = buf2ft[i].re * buf1ft[i].re - buf2ft[i].im * -buf1ft[i].im;
    CC[i].im = buf2ft[i].re * -buf1ft[i].im + buf2ft[i].im * buf1ft[i].re;
  }
}

static __global__ __launch_bounds__(32, 1) void tb_dftregistration_coderGPU_ker(
  const real_T col_shift, const real_T *row_shift, const real_T *tmp_im, const
  real_T CCmax_im, const real_T Ma, real_T dv1[4])
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    /*  If its only one row or column the shift along that dimension has no */
    /*  effect. We set to zero. */
    dv1[0] = sqrt(b_rt_hypotd_snf(1.0 - Ma, 0.0 - CCmax_im));
    dv1[1] = *tmp_im;
    dv1[2] = *row_shift;
    dv1[3] = col_shift;
  }
}

static __global__ __launch_bounds__(32, 1) void tc_dftregistration_coderGPU_ker(
  const real_T *tmp_im, const real_T CCmax_im, const real_T ex_im, real_T dv[2])
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    dv[0] = sqrt(b_rt_hypotd_snf(1.0 - ex_im, 0.0 - CCmax_im));
    dv[1] = *tmp_im;
  }
}

static __device__ real_T threadGroupReduction(real_T val, uint32_T lane,
  uint32_T mask)
{
  real_T other;
  uint32_T offset;
  uint32_T activeSize;
  activeSize = __popc(mask);
  offset = (activeSize + 1U) / 2U;
  while (activeSize > 1U) {
    other = shflDown2(val, offset, mask);
    if (lane + offset < activeSize) {
      val += other;
    }

    activeSize = offset;
    offset = (offset + 1U) / 2U;
  }

  return val;
}

static __global__ __launch_bounds__(32, 1) void u_dftregistration_coderGPU_kern(
  const int16_T yk, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->im = -6.2831853071795862 / static_cast<real_T>(yk);
  }
}

static __global__ __launch_bounds__(32, 1) void ub_dftregistration_coderGPU_ker(
  const real_T dv1[4], emxArray_real_T *output)
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 4) {
    output->data[i] = dv1[i];
  }
}

static __global__ __launch_bounds__(32, 1) void uc_dftregistration_coderGPU_ker(
  const real_T dv[2], emxArray_real_T *output)
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 2) {
    output->data[i] = dv[i];
  }
}

static __global__ __launch_bounds__(1024, 1) void
  v_dftregistration_coderGPU_kern(const int16_T coff, const emxArray_int16_T *y,
  emxArray_creal_T *b_y)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(y->size[0] * y->size[1] - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    i = static_cast<int32_T>(idx);
    iv0 = static_cast<int32_T>(y->data[i]) - static_cast<int32_T>(coff);
    if (iv0 > 32767) {
      iv0 = 32767;
    } else {
      if (iv0 < -32768) {
        iv0 = -32768;
      }
    }

    b_y->data[i].re = static_cast<real_T>(iv0);
    b_y->data[i].im = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void vb_dftregistration_coderGPU_ker
  (const creal_T buf1ft[262144], const creal_T buf2ft[262144], creal_T CC[262144])
{
  uint32_T threadId;
  real_T r;
  int32_T i;
  int32_T iv0;
  real_T bi;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId % 512U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iv0)) / 512U);
  if (i < 512) {
    r = buf2ft[i + (iv0 << 9)].re;
    bi = -buf2ft[i + (iv0 << 9)].im;
    CC[iv0 + (i << 9)].re = buf1ft[i + (iv0 << 9)].re * r - buf1ft[i + (iv0 << 9)]
      .im * bi;
    CC[iv0 + (i << 9)].im = buf1ft[i + (iv0 << 9)].re * bi + buf1ft[i + (iv0 <<
      9)].im * r;
  }
}

static __global__ __launch_bounds__(32, 1) void vc_dftregistration_coderGPU_ker(
  const real_T *tmp_im, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = *tmp_im * 0.0;
  }
}

static __global__ __launch_bounds__(1024, 1) void
  w_dftregistration_coderGPU_kern(const emxArray_creal_T *y, const creal_T *b_y,
  const int16_T b[512], int32_T *nx, emxArray_creal_T *kernc)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  real_T r;
  int32_T iv0;
  real_T bi;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = 512LL * (static_cast<int64_T>(*nx) + 1LL) - 1LL;
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iv0 = static_cast<int32_T>(idx % 512U);
    i = static_cast<int32_T>((idx - static_cast<uint32_T>(iv0)) / 512U);
    r = static_cast<real_T>(b[iv0]) * b_y->im;
    bi = 0.0 * y->data[i].re - r * y->data[i].im;
    r = 0.0 * y->data[i].im + r * y->data[i].re;
    kernc->data[iv0 + (i << 9)].re = bi;
    kernc->data[iv0 + (i << 9)].im = r;
  }
}

static __global__ __launch_bounds__(512, 1) void wb_dftregistration_coderGPU_ker
  (creal_T b_y1[262144])
{
  uint32_T threadId;
  int32_T i;
  real_T bi;
  real_T r;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    if (b_y1[i].im == 0.0) {
      bi = b_y1[i].re / 512.0;
      r = 0.0;
    } else if (b_y1[i].re == 0.0) {
      bi = 0.0;
      r = b_y1[i].im / 512.0;
    } else {
      bi = b_y1[i].re / 512.0;
      r = b_y1[i].im / 512.0;
    }

    b_y1[i].re = bi;
    b_y1[i].im = r;
  }
}

static __global__ __launch_bounds__(32, 1) void wc_dftregistration_coderGPU_ker(
  const creal_T *y, real_T *tmp_re)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *tmp_re = exp(y->re / 2.0);
  }
}

static __device__ real_T workGroupReduction(real_T val, uint32_T mask, uint32_T
  numActiveWarps)
{
  __shared__ real_T shared[32];
  uint32_T lane;
  uint32_T widx;
  uint32_T thBlkId;
  thBlkId = static_cast<uint32_T>(mwGetThreadIndexWithinBlock());
  lane = thBlkId % warpSize;
  widx = thBlkId / warpSize;
  val = threadGroupReduction(val, lane, mask);
  if (lane == 0U) {
    shared[widx] = val;
  }

  __syncthreads();
  mask = __ballot_sync(MAX_uint32_T, lane < numActiveWarps);
  val = shared[lane];
  if (widx == 0U) {
    val = threadGroupReduction(val, lane, mask);
  }

  return val;
}

static __global__ __launch_bounds__(32, 1) void x_dftregistration_coderGPU_kern(
  const emxArray_creal_T *kernc, int32_T *nx)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *nx = kernc->size[1] << 9;
  }
}

static __global__ __launch_bounds__(512, 1) void xb_dftregistration_coderGPU_ker
  (creal_T b_y1[262144], creal_T x[262144])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId % 512U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iv0)) / 512U);
  if (i < 512) {
    x[iv0 + (i << 9)] = b_y1[i + (iv0 << 9)];
  }
}

static __global__ __launch_bounds__(32, 1) void xc_dftregistration_coderGPU_ker(
  const real_T *tmp_im, const real_T *tmp_re, creal_T *y)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    y->re = *tmp_re * (*tmp_re * cos(*tmp_im));
    y->im = *tmp_re * (*tmp_re * sin(*tmp_im));
  }
}

static __global__ __launch_bounds__(1024, 1) void
  y_dftregistration_coderGPU_kern(const int32_T *nx, emxArray_creal_T *kernc)
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  creal_T x;
  real_T r;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(*nx - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iv0 = static_cast<int32_T>(idx);
    if (kernc->data[iv0].im == 0.0) {
      x.re = exp(kernc->data[iv0].re);
      x.im = 0.0;
    } else if ((static_cast<int32_T>((static_cast<int32_T>(isinf(kernc->data[iv0]
        .im))) && (static_cast<int32_T>(isinf(kernc->data[iv0].re))))) && (
                static_cast<int32_T>(kernc->data[iv0].re < 0.0))) {
      x.re = 0.0;
      x.im = 0.0;
    } else {
      r = exp(kernc->data[iv0].re / 2.0);
      x.re = r * (r * cos(kernc->data[iv0].im));
      x.im = r * (r * sin(kernc->data[iv0].im));
    }

    kernc->data[iv0] = x;
  }
}

static __global__ __launch_bounds__(512, 1) void yb_dftregistration_coderGPU_ker
  (creal_T CC[262144])
{
  uint32_T threadId;
  int32_T i;
  real_T bi;
  real_T r;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    if (CC[i].im == 0.0) {
      bi = CC[i].re / 512.0;
      r = 0.0;
    } else if (CC[i].re == 0.0) {
      bi = 0.0;
      r = CC[i].im / 512.0;
    } else {
      bi = CC[i].re / 512.0;
      r = CC[i].im / 512.0;
    }

    CC[i].re = bi;
    CC[i].im = r;
  }
}

static __global__ __launch_bounds__(32, 1) void yc_dftregistration_coderGPU_ker(
  const creal_T *y, real_T *tmp_re)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *tmp_re = y->re;
  }
}

void dftregistration_coderGPU(c_dftregistration_coderGPUStack *SD, const creal_T
  buf1ft[262144], const creal_T buf2ft[262144], int16_T usfac, emxArray_real_T
  *output, creal_T Greg[262144])
{
  real_T row_shift;
  real_T col_shift;
  int32_T iv0;
  creal_T y[512];
  int32_T nx;
  creal_T CCmax;
  int32_T vstride;
  int32_T k;
  int32_T npages;
  int32_T vspread;
  int32_T midoffset;
  int32_T i2;
  real_T tmp_re;
  int32_T i;
  int16_T idx[512];
  int32_T i1;
  int32_T j;
  real_T tmp_im;
  real_T rg00_im;
  real_T ex_im;
  int32_T ia;
  real_T CCmax_im;
  int32_T ib;
  real_T br;
  creal_T ex[1024];
  boolean_T SCALEA;
  real_T absar;
  boolean_T SCALEB;
  real_T absbr;
  creal_T b_y;
  real_T Ma;
  int16_T yk;
  static const int16_T b[512] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
    137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166,
    167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
    182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
    197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
    212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
    227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, -256,
    -255, -254, -253, -252, -251, -250, -249, -248, -247, -246, -245, -244, -243,
    -242, -241, -240, -239, -238, -237, -236, -235, -234, -233, -232, -231, -230,
    -229, -228, -227, -226, -225, -224, -223, -222, -221, -220, -219, -218, -217,
    -216, -215, -214, -213, -212, -211, -210, -209, -208, -207, -206, -205, -204,
    -203, -202, -201, -200, -199, -198, -197, -196, -195, -194, -193, -192, -191,
    -190, -189, -188, -187, -186, -185, -184, -183, -182, -181, -180, -179, -178,
    -177, -176, -175, -174, -173, -172, -171, -170, -169, -168, -167, -166, -165,
    -164, -163, -162, -161, -160, -159, -158, -157, -156, -155, -154, -153, -152,
    -151, -150, -149, -148, -147, -146, -145, -144, -143, -142, -141, -140, -139,
    -138, -137, -136, -135, -134, -133, -132, -131, -130, -129, -128, -127, -126,
    -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113,
    -112, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -100,
    -99, -98, -97, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -85,
    -84, -83, -82, -81, -80, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70,
    -69, -68, -67, -66, -65, -64, -63, -62, -61, -60, -59, -58, -57, -56, -55,
    -54, -53, -52, -51, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40,
    -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25,
    -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,
    -9, -8, -7, -6, -5, -4, -3, -2, -1 };

  int16_T z;
  uint16_T c_y;
  uint16_T q;
  int16_T b_z;
  emxArray_creal_T *out;
  int16_T c_z;
  emxArray_int16_T *d_y;
  int16_T cloc;
  int16_T coff;
  emxArray_creal_T *e_y;
  emxArray_creal_T *kernc;
  emxArray_int16_T *f_y;
  emxArray_creal_T *d_z;
  emxArray_creal_T *kernr;
  static const int16_T x[512] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
    137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166,
    167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
    182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
    197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
    212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
    227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, -256,
    -255, -254, -253, -252, -251, -250, -249, -248, -247, -246, -245, -244, -243,
    -242, -241, -240, -239, -238, -237, -236, -235, -234, -233, -232, -231, -230,
    -229, -228, -227, -226, -225, -224, -223, -222, -221, -220, -219, -218, -217,
    -216, -215, -214, -213, -212, -211, -210, -209, -208, -207, -206, -205, -204,
    -203, -202, -201, -200, -199, -198, -197, -196, -195, -194, -193, -192, -191,
    -190, -189, -188, -187, -186, -185, -184, -183, -182, -181, -180, -179, -178,
    -177, -176, -175, -174, -173, -172, -171, -170, -169, -168, -167, -166, -165,
    -164, -163, -162, -161, -160, -159, -158, -157, -156, -155, -154, -153, -152,
    -151, -150, -149, -148, -147, -146, -145, -144, -143, -142, -141, -140, -139,
    -138, -137, -136, -135, -134, -133, -132, -131, -130, -129, -128, -127, -126,
    -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113,
    -112, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -100,
    -99, -98, -97, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -85,
    -84, -83, -82, -81, -80, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70,
    -69, -68, -67, -66, -65, -64, -63, -62, -61, -60, -59, -58, -57, -56, -55,
    -54, -53, -52, -51, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40,
    -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25,
    -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,
    -9, -8, -7, -6, -5, -4, -3, -2, -1 };

  emxArray_creal_T *g_y;
  creal_T alpha1;
  creal_T beta1;
  emxArray_creal_T *CC;
  emxArray_creal_T *b_ex;
  uint16_T uv[2];
  emxArray_uint16_T *b_idx;
  creal_T *gpu_alpha1;
  emxArray_creal_T *gpu_kernr;
  creal_T (*gpu_CC)[262144];
  creal_T *gpu_beta1;
  emxArray_creal_T *gpu_y;
  emxArray_creal_T *gpu_kernc;
  emxArray_creal_T *gpu_out;
  creal_T (*b_gpu_kernr)[512];
  creal_T (*b_gpu_y)[512];
  creal_T (*gpu_x)[262144];
  creal_T (*c_gpu_y)[1048576];
  creal_T (*b_gpu_CC)[1048576];
  creal_T (*gpu_y1)[262144];
  int16_T (*gpu_idx)[1024];
  creal_T (*gpu_ex)[1024];
  int32_T *gpu_nx;
  real_T *gpu_row_shift;
  creal_T *gpu_CCmax;
  creal_T *d_gpu_y;
  emxArray_int16_T *e_gpu_y;
  dim3 grid;
  dim3 block;
  boolean_T validLaunchParams;
  emxArray_creal_T *f_gpu_y;
  int16_T (*gpu_b)[512];
  dim3 b_grid;
  dim3 b_block;
  boolean_T b_validLaunchParams;
  dim3 c_grid;
  dim3 c_block;
  boolean_T c_validLaunchParams;
  emxArray_int16_T *g_gpu_y;
  dim3 d_grid;
  dim3 d_block;
  boolean_T d_validLaunchParams;
  emxArray_creal_T *gpu_z;
  int16_T (*b_gpu_x)[512];
  dim3 e_grid;
  dim3 e_block;
  boolean_T e_validLaunchParams;
  dim3 f_grid;
  dim3 f_block;
  boolean_T f_validLaunchParams;
  dim3 g_grid;
  dim3 g_block;
  boolean_T g_validLaunchParams;
  real_T *gpu_absar;
  dim3 h_grid;
  dim3 h_block;
  boolean_T h_validLaunchParams;
  emxArray_creal_T *c_gpu_CC;
  emxArray_creal_T *b_gpu_ex;
  uint16_T (*gpu_uv)[2];
  dim3 i_grid;
  dim3 i_block;
  boolean_T i_validLaunchParams;
  emxArray_uint16_T *b_gpu_idx;
  dim3 j_grid;
  dim3 j_block;
  boolean_T j_validLaunchParams;
  real_T *gpu_tmp_im;
  real_T (*gpu_dv1)[4];
  emxArray_real_T *gpu_output;
  int16_T (*c_gpu_idx)[512];
  real_T (*gpu_absb1)[262144];
  real_T (*gpu_absb2)[262144];
  real_T *gpu_tmp_re;
  real_T (*gpu_dv)[2];
  int16_T (*gpu_Nr)[262144];
  int16_T (*gpu_Nc)[262144];
  boolean_T CC_dirtyOnGpu;
  boolean_T kernc_dirtyOnGpu;
  boolean_T y_dirtyOnGpu;
  boolean_T x_dirtyOnGpu;
  boolean_T b_CC_dirtyOnGpu;
  boolean_T row_shift_dirtyOnGpu;
  boolean_T CCmax_dirtyOnGpu;
  boolean_T b_y_dirtyOnGpu;
  boolean_T c_y_dirtyOnGpu;
  boolean_T z_dirtyOnGpu;
  boolean_T c_CC_dirtyOnGpu;
  boolean_T ex_dirtyOnGpu;
  boolean_T idx_dirtyOnGpu;
  boolean_T CC_dirtyOnCpu;
  boolean_T y_dirtyOnCpu;
  boolean_T kernc_dirtyOnCpu;
  boolean_T out_dirtyOnCpu;
  boolean_T x_dirtyOnCpu;
  boolean_T b_CC_dirtyOnCpu;
  boolean_T nx_dirtyOnCpu;
  boolean_T row_shift_dirtyOnCpu;
  boolean_T b_y_dirtyOnCpu;
  boolean_T c_y_dirtyOnCpu;
  boolean_T b_dirtyOnCpu;
  boolean_T z_dirtyOnCpu;
  boolean_T absar_dirtyOnCpu;
  boolean_T c_CC_dirtyOnCpu;
  boolean_T idx_dirtyOnCpu;
  boolean_T tmp_im_dirtyOnCpu;
  boolean_T tmp_re_dirtyOnCpu;
  emxArray_int16_T inter_y;
  emxArray_creal_T b_inter_y;
  emxArray_creal_T inter_kernc;
  emxArray_int16_T c_inter_y;
  emxArray_creal_T inter_z;
  emxArray_creal_T inter_kernr;
  emxArray_creal_T d_inter_y;
  emxArray_creal_T inter_out;
  emxArray_creal_T inter_CC;
  emxArray_creal_T inter_ex;
  emxArray_uint16_T inter_idx;
  emxArray_real_T inter_output;
  int32_T b_nx;
  cudaMalloc(&gpu_Nc, 524288ULL);
  cudaMalloc(&gpu_Nr, 524288ULL);
  cudaMalloc(&gpu_dv, 16ULL);
  cudaMalloc(&gpu_absb2, 2097152ULL);
  cudaMalloc(&gpu_absb1, 2097152ULL);
  cudaMalloc(&c_gpu_idx, 1024ULL);
  cudaMalloc(&gpu_y1, 4194304ULL);
  cudaMalloc(&gpu_dv1, 32ULL);
  cudaMalloc(&gpu_output, 32ULL);
  gpuEmxReset_real_T(&inter_output);
  cudaMalloc(&b_gpu_kernr, 8192ULL);
  cudaMalloc(&gpu_CCmax, 16ULL);
  cudaMalloc(&gpu_uv, 4ULL);
  cudaMalloc(&b_gpu_idx, 32ULL);
  gpuEmxReset_uint16_T(&inter_idx);
  cudaMalloc(&b_gpu_ex, 32ULL);
  gpuEmxReset_creal_T(&inter_ex);
  cudaMalloc(&c_gpu_CC, 32ULL);
  gpuEmxReset_creal_T(&inter_CC);
  cudaMalloc(&gpu_out, 32ULL);
  gpuEmxReset_creal_T(&inter_out);
  cudaMalloc(&gpu_beta1, 16ULL);
  cudaMalloc(&gpu_alpha1, 16ULL);
  cudaMalloc(&gpu_y, 32ULL);
  gpuEmxReset_creal_T(&d_inter_y);
  cudaMalloc(&gpu_kernr, 32ULL);
  gpuEmxReset_creal_T(&inter_kernr);
  cudaMalloc(&b_gpu_x, 1024ULL);
  cudaMalloc(&gpu_z, 32ULL);
  gpuEmxReset_creal_T(&inter_z);
  cudaMalloc(&g_gpu_y, 32ULL);
  gpuEmxReset_int16_T(&c_inter_y);
  cudaMalloc(&gpu_b, 1024ULL);
  cudaMalloc(&gpu_kernc, 32ULL);
  gpuEmxReset_creal_T(&inter_kernc);
  cudaMalloc(&f_gpu_y, 32ULL);
  gpuEmxReset_creal_T(&b_inter_y);
  cudaMalloc(&e_gpu_y, 32ULL);
  gpuEmxReset_int16_T(&inter_y);
  cudaMalloc(&d_gpu_y, 16ULL);
  cudaMalloc(&b_gpu_y, 8192ULL);
  cudaMalloc(&gpu_row_shift, 8ULL);
  cudaMalloc(&gpu_absar, 8ULL);
  cudaMalloc(&gpu_ex, 16384ULL);
  cudaMalloc(&gpu_idx, 2048ULL);
  cudaMalloc(&c_gpu_y, 16777216ULL);
  cudaMalloc(&b_gpu_CC, 16777216ULL);
  cudaMalloc(&gpu_x, 4194304ULL);
  cudaMalloc(&gpu_tmp_im, 8ULL);
  cudaMalloc(&gpu_tmp_re, 8ULL);
  cudaMalloc(&gpu_CC, 4194304ULL);
  cudaMalloc(&gpu_nx, 4ULL);
  tmp_re_dirtyOnCpu = false;
  absar_dirtyOnCpu = false;
  b_y_dirtyOnCpu = false;
  b_dirtyOnCpu = true;
  emlrtHeapReferenceStackEnterFcnR2012b(emlrtRootTLSGlobal);

  /*   this _coder version has calls to exist() removed to enable mex compilation */
  /*  Add kernelfun pragma to trigger GPU kernel creation */
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
  row_shift_dirtyOnGpu = false;
  col_shift = 0.0;

  /*  Compute error for no pixel shift */
  if (usfac == 0) {
    kc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (buf2ft, buf1ft, *gpu_CC, *gpu_absb2, *gpu_absb1);
    lc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*b_gpu_y, *gpu_CC);
    y_dirtyOnGpu = true;
    mc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (gpu_CCmax);
    CCmax_dirtyOnGpu = true;
    for (iv0 = 0; iv0 < 512; iv0++) {
      if (y_dirtyOnGpu) {
        cudaMemcpy(&y[0], b_gpu_y, 8192ULL, cudaMemcpyDeviceToHost);
        y_dirtyOnGpu = false;
      }

      if (CCmax_dirtyOnGpu) {
        cudaMemcpy(&CCmax, gpu_CCmax, 16ULL, cudaMemcpyDeviceToHost);
        CCmax_dirtyOnGpu = false;
      }

      CCmax.re += y[iv0].re;
      CCmax.im += y[iv0].im;
    }

    nc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_absb1);
    oc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_absb1, gpu_tmp_re);
    pc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_absb1, gpu_tmp_re);
    qc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_absb2);
    rc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_absb2, gpu_tmp_im);
    sc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_absb2, gpu_tmp_im);
    if (CCmax_dirtyOnGpu) {
      cudaMemcpy(&CCmax, gpu_CCmax, 16ULL, cudaMemcpyDeviceToHost);
    }

    ex_im = CCmax.re * CCmax.re - CCmax.im * -CCmax.im;
    CCmax_im = CCmax.re * -CCmax.im + CCmax.im * CCmax.re;
    cudaMemcpy(&tmp_im, gpu_tmp_im, 8ULL, cudaMemcpyDeviceToHost);
    cudaMemcpy(&tmp_re, gpu_tmp_re, 8ULL, cudaMemcpyDeviceToHost);
    br = tmp_im * tmp_re;
    if (CCmax_im == 0.0) {
      ex_im /= br;
      CCmax_im = 0.0;
    } else if (ex_im == 0.0) {
      ex_im = 0.0;
      CCmax_im /= br;
    } else {
      ex_im /= br;
      CCmax_im /= br;
    }

    tmp_im = rt_atan2d_snf(CCmax.im, CCmax.re);
    cudaMemcpy(gpu_tmp_im, &tmp_im, 8ULL, cudaMemcpyHostToDevice);
    tmp_im_dirtyOnCpu = false;
    tc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (gpu_tmp_im, CCmax_im, ex_im, *gpu_dv);
    nx = output->size[0] * output->size[1];
    output->size[0] = 1;
    output->size[1] = 2;
    emxEnsureCapacity_real_T(output, nx);
    gpuEmxMemcpyCpuToGpu_real_T(output, &inter_output, gpu_output);
    uc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_dv, gpu_output);

    /*  Whole-pixel shift - Compute crosscorrelation by an IFFT and locate the */
    /*  peak */
  } else if (usfac == 1) {
    vb_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (buf1ft, buf2ft, *gpu_CC);
    cufftEnsureInitialization(512, CUFFT_Z2Z, 512, 512);
    cufftExecZ2Z(*cufftGlobalHandlePtr, (cufftDoubleComplex *)&(*gpu_CC)[0],
                 (cufftDoubleComplex *)&(*gpu_y1)[0], 1);
    wb_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_y1);
    xb_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_y1, *gpu_x);
    cufftEnsureInitialization(512, CUFFT_Z2Z, 512, 512);
    cufftExecZ2Z(*cufftGlobalHandlePtr, (cufftDoubleComplex *)&(*gpu_x)[0],
                 (cufftDoubleComplex *)&(*gpu_CC)[0], 1);
    yb_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_CC);
    ac_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*c_gpu_idx);
    bc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*c_gpu_idx, *gpu_CC, *b_gpu_y);
    nx = 0;
    cudaMemcpy(&y[0], b_gpu_y, 8192ULL, cudaMemcpyDeviceToHost);
    rg00_im = y[0].re;
    ex_im = y[0].im;
    for (i = 0; i < 511; i++) {
      if (rtIsNaN(y[i + 1].re) || rtIsNaN(y[i + 1].im)) {
        SCALEA = false;
      } else if (rtIsNaN(rg00_im) || rtIsNaN(ex_im)) {
        SCALEA = true;
      } else {
        if ((fabs(rg00_im) > 8.9884656743115785E+307) || (fabs(ex_im) >
             8.9884656743115785E+307)) {
          SCALEA = true;
        } else {
          SCALEA = false;
        }

        if ((fabs(y[i + 1].re) > 8.9884656743115785E+307) || (fabs(y[i + 1].im) >
             8.9884656743115785E+307)) {
          SCALEB = true;
        } else {
          SCALEB = false;
        }

        if (SCALEA || SCALEB) {
          tmp_im = rt_hypotd_snf(rg00_im / 2.0, ex_im / 2.0);
          br = rt_hypotd_snf(y[i + 1].re / 2.0, y[i + 1].im / 2.0);
        } else {
          tmp_im = rt_hypotd_snf(rg00_im, ex_im);
          br = rt_hypotd_snf(y[i + 1].re, y[i + 1].im);
        }

        if (tmp_im == br) {
          absar = fabs(rg00_im);
          absar_dirtyOnCpu = true;
          tmp_re = fabs(ex_im);
          absbr = fabs(y[i + 1].re);
          tmp_im = fabs(y[i + 1].im);
          if (absar > tmp_re) {
            Ma = absar;
            absar = tmp_re;
          } else {
            Ma = tmp_re;
          }

          if (absbr > tmp_im) {
            tmp_re = absbr;
            tmp_re_dirtyOnCpu = true;
            absbr = tmp_im;
          } else {
            tmp_re = tmp_im;
            tmp_re_dirtyOnCpu = true;
          }

          if (Ma > tmp_re) {
            if (absar < absbr) {
              tmp_im = Ma - tmp_re;
              br = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + tmp_re / 2.0) *
                (absbr - absar);
            } else {
              tmp_im = Ma;
              br = tmp_re;
            }
          } else if (Ma < tmp_re) {
            if (absar > absbr) {
              br = tmp_re - Ma;
              tmp_im = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + tmp_re / 2.0) *
                (absar - absbr);
            } else {
              tmp_im = Ma;
              br = tmp_re;
            }
          } else {
            tmp_im = absar;
            br = absbr;
          }

          if (tmp_im == br) {
            tmp_im = rt_atan2d_snf(ex_im, rg00_im);
            br = rt_atan2d_snf(y[i + 1].im, y[i + 1].re);
            if (tmp_im == br) {
              br = y[i + 1].re;
              tmp_re = y[i + 1].im;
              if (tmp_im > 0.78539816339744828) {
                if (tmp_im > 2.3561944901923448) {
                  tmp_im = -ex_im;
                  br = -tmp_re;
                } else {
                  tmp_im = -rg00_im;
                  br = -br;
                }
              } else if (tmp_im > -0.78539816339744828) {
                tmp_im = ex_im;
                br = tmp_re;
              } else if (tmp_im > -2.3561944901923448) {
                tmp_im = rg00_im;
              } else {
                tmp_im = -ex_im;
                br = -tmp_re;
              }

              if (tmp_im == br) {
                tmp_im = 0.0;
                br = 0.0;
              }
            }
          }
        }

        SCALEA = (tmp_im < br);
      }

      if (SCALEA) {
        rg00_im = y[i + 1].re;
        ex_im = y[i + 1].im;
        nx = i + 1;
      }
    }

    cc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (buf2ft, buf1ft, *gpu_absb1, *gpu_absb2);
    if (absar_dirtyOnCpu) {
      cudaMemcpy(gpu_absar, &absar, 8ULL, cudaMemcpyHostToDevice);
    }

    dc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_absb1, gpu_absar);
    ec_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_absb1, gpu_absar);
    fc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_absb2);
    if (tmp_re_dirtyOnCpu) {
      cudaMemcpy(gpu_tmp_re, &tmp_re, 8ULL, cudaMemcpyHostToDevice);
      tmp_re_dirtyOnCpu = false;
    }

    gc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_absb2, gpu_tmp_re);
    hc_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_absb2, gpu_tmp_re);

    /*  slow code */
    /* rgzero = sum(abs(buf2ft(:)).^2)/(m*n);  */
    cudaMemcpy(&SD->f0.b_CC[0], gpu_CC, 4194304ULL, cudaMemcpyDeviceToHost);
    cudaMemcpy(&idx[0], c_gpu_idx, 1024ULL, cudaMemcpyDeviceToHost);
    absbr = SD->f0.b_CC[(idx[nx] + (nx << 9)) - 1].re;
    Ma = -SD->f0.b_CC[(idx[nx] + (nx << 9)) - 1].im;
    tmp_im = SD->f0.b_CC[(idx[nx] + (nx << 9)) - 1].re * absbr - SD->f0.b_CC
      [(idx[nx] + (nx << 9)) - 1].im * Ma;
    Ma = SD->f0.b_CC[(idx[nx] + (nx << 9)) - 1].re * Ma + SD->f0.b_CC[(idx[nx] +
      (nx << 9)) - 1].im * absbr;
    cudaMemcpy(&absar, gpu_absar, 8ULL, cudaMemcpyDeviceToHost);
    cudaMemcpy(&tmp_re, gpu_tmp_re, 8ULL, cudaMemcpyDeviceToHost);
    br = tmp_re / 262144.0 * (absar / 262144.0);
    if (Ma == 0.0) {
      absbr = tmp_im / br;
      Ma = 0.0;
    } else if (tmp_im == 0.0) {
      absbr = 0.0;
      Ma /= br;
    } else {
      absbr = tmp_im / br;
      Ma /= br;
    }

    tmp_im = rt_atan2d_snf(SD->f0.b_CC[(idx[nx] + (nx << 9)) - 1].im,
      SD->f0.b_CC[(idx[nx] + (nx << 9)) - 1].re);
    if (idx[nx] > 256) {
      row_shift = idx[nx] - 513;
    } else {
      row_shift = idx[nx] - 1;
    }

    if (nx + 1 > 256) {
      col_shift = nx - 512;
    } else {
      col_shift = nx;
    }

    cudaMemcpy(gpu_row_shift, &row_shift, 8ULL, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_tmp_im, &tmp_im, 8ULL, cudaMemcpyHostToDevice);
    tmp_im_dirtyOnCpu = false;
    ic_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (col_shift, gpu_row_shift, gpu_tmp_im, Ma, absbr, *gpu_dv1);
    nx = output->size[0] * output->size[1];
    output->size[0] = 1;
    output->size[1] = 4;
    emxEnsureCapacity_real_T(output, nx);
    gpuEmxMemcpyCpuToGpu_real_T(output, &inter_output, gpu_output);
    jc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_dv1, gpu_output);

    /*  Partial-pixel shift */
  } else {
    /*  First upsample by a factor of 2 to obtain initial estimate */
    /*  Embed Fourier data in a 2x larger array */
    c_dftregistration_coderGPU_kern<<<dim3(2048U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*b_gpu_CC);
    d_dftregistration_coderGPU_kern<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (buf1ft, *gpu_CC);
    CC_dirtyOnCpu = false;
    CC_dirtyOnGpu = true;
    for (nx = 0; nx < 2; nx++) {
      vstride = 1;
      for (k = 0; k < nx; k++) {
        vstride <<= 9;
      }

      npages = 1;
      for (k = 0; k <= -nx; k++) {
        npages <<= 9;
      }

      vspread = 511 * vstride;
      midoffset = (vstride << 8) - 1;
      i2 = 0;
      for (i = 0; i < npages; i++) {
        i1 = i2;
        i2 += vspread;
        for (j = 0; j < vstride; j++) {
          i1++;
          i2++;
          ia = i1 - 1;
          ib = i1 + midoffset;
          for (k = 0; k < 256; k++) {
            if (CC_dirtyOnGpu) {
              cudaMemcpy(&SD->f0.b_CC[0], gpu_CC, 4194304ULL,
                         cudaMemcpyDeviceToHost);
              CC_dirtyOnGpu = false;
            }

            tmp_re = SD->f0.b_CC[ia].re;
            tmp_im = SD->f0.b_CC[ia].im;
            SD->f0.b_CC[ia] = SD->f0.b_CC[ib];
            SD->f0.b_CC[ib].re = tmp_re;
            SD->f0.b_CC[ib].im = tmp_im;
            CC_dirtyOnCpu = true;
            ia += vstride;
            ib += vstride;
          }
        }
      }
    }

    e_dftregistration_coderGPU_kern<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (buf2ft, *gpu_x);
    x_dirtyOnCpu = false;
    x_dirtyOnGpu = true;
    for (nx = 0; nx < 2; nx++) {
      vstride = 1;
      for (k = 0; k < nx; k++) {
        vstride <<= 9;
      }

      npages = 1;
      for (k = 0; k <= -nx; k++) {
        npages <<= 9;
      }

      vspread = 511 * vstride;
      midoffset = (vstride << 8) - 1;
      i2 = 0;
      for (i = 0; i < npages; i++) {
        i1 = i2;
        i2 += vspread;
        for (j = 0; j < vstride; j++) {
          i1++;
          i2++;
          ia = i1 - 1;
          ib = i1 + midoffset;
          for (k = 0; k < 256; k++) {
            if (x_dirtyOnGpu) {
              cudaMemcpy(&SD->f0.x[0], gpu_x, 4194304ULL, cudaMemcpyDeviceToHost);
              x_dirtyOnGpu = false;
            }

            tmp_re = SD->f0.x[ia].re;
            tmp_im = SD->f0.x[ia].im;
            SD->f0.x[ia] = SD->f0.x[ib];
            SD->f0.x[ib].re = tmp_re;
            SD->f0.x[ib].im = tmp_im;
            x_dirtyOnCpu = true;
            ia += vstride;
            ib += vstride;
          }
        }
      }
    }

    if (CC_dirtyOnCpu) {
      cudaMemcpy(gpu_CC, &SD->f0.b_CC[0], 4194304ULL, cudaMemcpyHostToDevice);
    }

    if (x_dirtyOnCpu) {
      cudaMemcpy(gpu_x, &SD->f0.x[0], 4194304ULL, cudaMemcpyHostToDevice);
    }

    f_dftregistration_coderGPU_kern<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_CC, *b_gpu_CC, *gpu_x);
    b_CC_dirtyOnCpu = false;
    b_CC_dirtyOnGpu = true;

    /*  Compute crosscorrelation and locate the peak  */
    for (nx = 0; nx < 2; nx++) {
      vstride = 1;
      for (k = 0; k < nx; k++) {
        vstride <<= 10;
      }

      npages = 1;
      for (k = 0; k <= -nx; k++) {
        npages <<= 10;
      }

      vspread = 1023 * vstride;
      midoffset = (vstride << 9) - 1;
      i2 = 0;
      for (i = 0; i < npages; i++) {
        i1 = i2;
        i2 += vspread;
        for (j = 0; j < vstride; j++) {
          i1++;
          i2++;
          ia = i1 - 1;
          ib = i1 + midoffset;
          for (k = 0; k < 512; k++) {
            if (b_CC_dirtyOnGpu) {
              cudaMemcpy(&SD->f0.CC[0], b_gpu_CC, 16777216ULL,
                         cudaMemcpyDeviceToHost);
              b_CC_dirtyOnGpu = false;
            }

            tmp_re = SD->f0.CC[ia].re;
            tmp_im = SD->f0.CC[ia].im;
            SD->f0.CC[ia] = SD->f0.CC[ib];
            SD->f0.CC[ib].re = tmp_re;
            SD->f0.CC[ib].im = tmp_im;
            b_CC_dirtyOnCpu = true;
            ia += vstride;
            ib += vstride;
          }
        }
      }
    }

    if (b_CC_dirtyOnCpu) {
      cudaMemcpy(b_gpu_CC, &SD->f0.CC[0], 16777216ULL, cudaMemcpyHostToDevice);
    }

    g_dftregistration_coderGPU_kern<<<dim3(2048U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*b_gpu_CC, *c_gpu_y);
    cufftEnsureInitialization(1024, CUFFT_Z2Z, 1024, 1024);
    cufftExecZ2Z(*cufftGlobalHandlePtr, (cufftDoubleComplex *)&(*c_gpu_y)[0],
                 (cufftDoubleComplex *)&(*b_gpu_CC)[0], 1);
    h_dftregistration_coderGPU_kern<<<dim3(2048U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*b_gpu_CC);
    i_dftregistration_coderGPU_kern<<<dim3(2048U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*b_gpu_CC, *c_gpu_y);
    cufftEnsureInitialization(1024, CUFFT_Z2Z, 1024, 1024);
    cufftExecZ2Z(*cufftGlobalHandlePtr, (cufftDoubleComplex *)&(*c_gpu_y)[0],
                 (cufftDoubleComplex *)&(*b_gpu_CC)[0], 1);
    j_dftregistration_coderGPU_kern<<<dim3(2048U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*b_gpu_CC);

    /*  Calculate cross-correlation */
    k_dftregistration_coderGPU_kern<<<dim3(2U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*gpu_idx);
    l_dftregistration_coderGPU_kern<<<dim3(2U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*gpu_idx, *b_gpu_CC, *gpu_ex);
    nx = 0;
    cudaMemcpy(&ex[0], gpu_ex, 16384ULL, cudaMemcpyDeviceToHost);
    rg00_im = ex[0].re;
    ex_im = ex[0].im;
    for (i = 0; i < 1023; i++) {
      if (rtIsNaN(ex[i + 1].re) || rtIsNaN(ex[i + 1].im)) {
        SCALEA = false;
      } else if (rtIsNaN(rg00_im) || rtIsNaN(ex_im)) {
        SCALEA = true;
      } else {
        if ((fabs(rg00_im) > 8.9884656743115785E+307) || (fabs(ex_im) >
             8.9884656743115785E+307)) {
          SCALEA = true;
        } else {
          SCALEA = false;
        }

        if ((fabs(ex[i + 1].re) > 8.9884656743115785E+307) || (fabs(ex[i + 1].im)
             > 8.9884656743115785E+307)) {
          SCALEB = true;
        } else {
          SCALEB = false;
        }

        if (SCALEA || SCALEB) {
          tmp_im = rt_hypotd_snf(rg00_im / 2.0, ex_im / 2.0);
          br = rt_hypotd_snf(ex[i + 1].re / 2.0, ex[i + 1].im / 2.0);
        } else {
          tmp_im = rt_hypotd_snf(rg00_im, ex_im);
          br = rt_hypotd_snf(ex[i + 1].re, ex[i + 1].im);
        }

        if (tmp_im == br) {
          absar = fabs(rg00_im);
          tmp_re = fabs(ex_im);
          absbr = fabs(ex[i + 1].re);
          tmp_im = fabs(ex[i + 1].im);
          if (absar > tmp_re) {
            Ma = absar;
            absar = tmp_re;
          } else {
            Ma = tmp_re;
          }

          if (absbr > tmp_im) {
            tmp_re = absbr;
            absbr = tmp_im;
          } else {
            tmp_re = tmp_im;
          }

          if (Ma > tmp_re) {
            if (absar < absbr) {
              tmp_im = Ma - tmp_re;
              br = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + tmp_re / 2.0) *
                (absbr - absar);
            } else {
              tmp_im = Ma;
              br = tmp_re;
            }
          } else if (Ma < tmp_re) {
            if (absar > absbr) {
              br = tmp_re - Ma;
              tmp_im = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + tmp_re / 2.0) *
                (absar - absbr);
            } else {
              tmp_im = Ma;
              br = tmp_re;
            }
          } else {
            tmp_im = absar;
            br = absbr;
          }

          if (tmp_im == br) {
            tmp_im = rt_atan2d_snf(ex_im, rg00_im);
            br = rt_atan2d_snf(ex[i + 1].im, ex[i + 1].re);
            if (tmp_im == br) {
              br = ex[i + 1].re;
              tmp_re = ex[i + 1].im;
              if (tmp_im > 0.78539816339744828) {
                if (tmp_im > 2.3561944901923448) {
                  tmp_im = -ex_im;
                  br = -tmp_re;
                } else {
                  tmp_im = -rg00_im;
                  br = -br;
                }
              } else if (tmp_im > -0.78539816339744828) {
                tmp_im = ex_im;
                br = tmp_re;
              } else if (tmp_im > -2.3561944901923448) {
                tmp_im = rg00_im;
              } else {
                tmp_im = -ex_im;
                br = -tmp_re;
              }

              if (tmp_im == br) {
                tmp_im = 0.0;
                br = 0.0;
              }
            }
          }
        }

        SCALEA = (tmp_im < br);
      }

      if (SCALEA) {
        rg00_im = ex[i + 1].re;
        ex_im = ex[i + 1].im;
        nx = i + 1;
      }
    }

    /*  Obtain shift in original pixel grid from the position of the */
    /*  crosscorrelation peak  */
    cudaMemcpy(gpu_nx, &nx, 4ULL, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_row_shift, &row_shift, 8ULL, cudaMemcpyHostToDevice);
    m_dftregistration_coderGPU_kern<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_idx, gpu_nx, gpu_row_shift, *b_gpu_CC, gpu_CCmax);
    row_shift_dirtyOnCpu = false;
    row_shift_dirtyOnGpu = true;
    if (nx + 1 > 512) {
      b_nx = nx - 1024;
    } else {
      b_nx = nx;
    }

    col_shift = static_cast<real_T>(b_nx) / 2.0;

    /*  If upsampling > 2, then refine estimate with matrix multiply DFT */
    if (usfac > 2) {
      /* %% DFT computation %%% */
      /*  Initial shift estimate in upsampled grid */
      cudaMemcpy(&row_shift, gpu_row_shift, 8ULL, cudaMemcpyDeviceToHost);
      nx = static_cast<int32_T>(rt_roundd_snf(row_shift * static_cast<real_T>
        (usfac)));
      if (nx < 32768) {
        if (nx >= -32768) {
          yk = static_cast<int16_T>(nx);
        } else {
          yk = MIN_int16_T;
        }
      } else {
        yk = MAX_int16_T;
      }

      if (yk >= 0) {
        c_y = static_cast<uint16_T>(yk);
      } else if (yk == -32768) {
        c_y = 32768U;
      } else {
        c_y = static_cast<uint16_T>(-yk);
      }

      q = static_cast<uint16_T>(static_cast<uint32_T>(c_y) / usfac);
      c_y = static_cast<uint16_T>(static_cast<uint32_T>(c_y) -
        static_cast<uint16_T>(static_cast<uint32_T>(q) * usfac));
      if ((c_y > 0) && (c_y >= static_cast<int32_T>(static_cast<uint32_T>(usfac)
            >> 1) + (usfac & 1))) {
        q = static_cast<uint16_T>(q + 1);
      }

      z = static_cast<int16_T>(q);
      if (yk < 0) {
        z = static_cast<int16_T>(-q);
      }

      nx = static_cast<int32_T>(rt_roundd_snf(col_shift * static_cast<real_T>
        (usfac)));
      if (nx < 32768) {
        if (nx >= -32768) {
          yk = static_cast<int16_T>(nx);
        } else {
          yk = MIN_int16_T;
        }
      } else {
        yk = MAX_int16_T;
      }

      if (yk >= 0) {
        c_y = static_cast<uint16_T>(yk);
      } else if (yk == -32768) {
        c_y = 32768U;
      } else {
        c_y = static_cast<uint16_T>(-yk);
      }

      q = static_cast<uint16_T>(static_cast<uint32_T>(c_y) / usfac);
      c_y = static_cast<uint16_T>(static_cast<uint32_T>(c_y) -
        static_cast<uint16_T>(static_cast<uint32_T>(q) * usfac));
      if ((c_y > 0) && (c_y >= static_cast<int32_T>(static_cast<uint32_T>(usfac)
            >> 1) + (usfac & 1))) {
        q = static_cast<uint16_T>(q + 1);
      }

      b_z = static_cast<int16_T>(q);
      if (yk < 0) {
        b_z = static_cast<int16_T>(-q);
      }

      emxInit_creal_T(&out, 2, true);
      nx = static_cast<int32_T>(rt_roundd_snf(static_cast<real_T>(usfac) * 1.5));
      if (nx < 32768) {
        yk = static_cast<int16_T>(nx);
      } else {
        yk = MAX_int16_T;
      }

      c_z = static_cast<int16_T>(rt_roundd_snf(static_cast<real_T>(yk) / 2.0));

      /*  Center of output array at dftshift+1 */
      /*  Matrix multiply DFT around the current shift estimate */
      vspread = usfac * usfac;
      if (vspread > 32767) {
        vspread = 32767;
      }

      t_dftregistration_coderGPU_kern<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
        (buf1ft, buf2ft, *gpu_CC);
      emxInit_int16_T(&d_y, 2, true);
      nx = z * usfac;
      if (nx > 32767) {
        nx = 32767;
      } else {
        if (nx < -32768) {
          nx = -32768;
        }
      }

      nx = c_z - nx;
      if (nx > 32767) {
        nx = 32767;
      }

      cloc = static_cast<int16_T>(nx);
      nx = b_z * usfac;
      if (nx > 32767) {
        nx = 32767;
      } else {
        if (nx < -32768) {
          nx = -32768;
        }
      }

      nx = c_z - nx;
      if (nx > 32767) {
        nx = 32767;
      }

      coff = static_cast<int16_T>(nx);

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
      if (usfac > 63) {
        yk = MAX_int16_T;
      } else {
        yk = static_cast<int16_T>(usfac << 9);
      }

      u_dftregistration_coderGPU_kern<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (yk, d_gpu_y);
      nx = static_cast<int32_T>(rt_roundd_snf(static_cast<real_T>(usfac) * 1.5));
      if (nx < 32768) {
        yk = static_cast<int16_T>(nx);
      } else {
        yk = MAX_int16_T;
      }

      vstride = yk;
      nx = d_y->size[0] * d_y->size[1];
      d_y->size[0] = 1;
      d_y->size[1] = yk;
      emxEnsureCapacity_int16_T(d_y, nx);
      d_y->data[0] = 0;
      yk = 0;
      for (k = 0; k <= vstride - 2; k++) {
        yk = static_cast<int16_T>(yk + 1);
        d_y->data[k + 1] = yk;
      }

      emxInit_creal_T(&e_y, 2, true);
      c_y_dirtyOnGpu = false;
      nx = e_y->size[0] * e_y->size[1];
      e_y->size[0] = 1;
      e_y->size[1] = d_y->size[1];
      emxEnsureCapacity_creal_T(e_y, nx);
      c_y_dirtyOnCpu = true;
      validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((d_y->size[0]
        * d_y->size[1] - 1) + 1LL), &grid, &block, 1024U, 65535U);
      if (validLaunchParams) {
        gpuEmxMemcpyCpuToGpu_int16_T(d_y, &inter_y, e_gpu_y);
        gpuEmxMemcpyCpuToGpu_creal_T(e_y, &b_inter_y, f_gpu_y);
        v_dftregistration_coderGPU_kern<<<grid, block>>>(coff, e_gpu_y, f_gpu_y);
        c_y_dirtyOnCpu = false;
        c_y_dirtyOnGpu = true;
      }

      emxFree_int16_T(&d_y);
      gpuEmxFree_int16_T(&inter_y);
      emxInit_creal_T(&kernc, 2, true);
      kernc_dirtyOnGpu = false;
      nx = kernc->size[0] * kernc->size[1];
      kernc->size[0] = 512;
      if (c_y_dirtyOnGpu) {
        gpuEmxMemcpyGpuToCpu_creal_T(e_y, &b_inter_y);
      }

      kernc->size[1] = e_y->size[1];
      emxEnsureCapacity_creal_T(kernc, nx);
      kernc_dirtyOnCpu = true;
      nx = e_y->size[1] - 1;
      nx_dirtyOnCpu = true;
      b_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>(512LL *
        (nx + 1LL)), &b_grid, &b_block, 1024U, 65535U);
      if (b_validLaunchParams) {
        gpuEmxMemcpyCpuToGpu_creal_T(kernc, &inter_kernc, gpu_kernc);
        cudaMemcpy(gpu_nx, &nx, 4ULL, cudaMemcpyHostToDevice);
        nx_dirtyOnCpu = false;
        if (c_y_dirtyOnCpu) {
          gpuEmxMemcpyCpuToGpu_creal_T(e_y, &b_inter_y, f_gpu_y);
        }

        cudaMemcpy(gpu_b, (void *)&b[0], 1024ULL, cudaMemcpyHostToDevice);
        b_dirtyOnCpu = false;
        w_dftregistration_coderGPU_kern<<<b_grid, b_block>>>(f_gpu_y, d_gpu_y,
          *gpu_b, gpu_nx, gpu_kernc);
        kernc_dirtyOnCpu = false;
        kernc_dirtyOnGpu = true;
      }

      emxFree_creal_T(&e_y);
      gpuEmxFree_creal_T(&b_inter_y);
      if (kernc_dirtyOnCpu) {
        gpuEmxMemcpyCpuToGpu_creal_T(kernc, &inter_kernc, gpu_kernc);
      }

      if (nx_dirtyOnCpu) {
        cudaMemcpy(gpu_nx, &nx, 4ULL, cudaMemcpyHostToDevice);
      }

      x_dftregistration_coderGPU_kern<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (gpu_kernc, gpu_nx);
      cudaMemcpy(&nx, gpu_nx, 4ULL, cudaMemcpyDeviceToHost);
      c_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((nx - 1) +
        1LL), &c_grid, &c_block, 1024U, 65535U);
      if (c_validLaunchParams) {
        y_dftregistration_coderGPU_kern<<<c_grid, c_block>>>(gpu_nx, gpu_kernc);
        kernc_dirtyOnGpu = true;
      }

      emxInit_int16_T(&f_y, 2, true);
      if (usfac > 63) {
        yk = MAX_int16_T;
      } else {
        yk = static_cast<int16_T>(usfac << 9);
      }

      ab_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (yk, d_gpu_y);
      nx = static_cast<int32_T>(rt_roundd_snf(static_cast<real_T>(usfac) * 1.5));
      if (nx < 32768) {
        yk = static_cast<int16_T>(nx);
      } else {
        yk = MAX_int16_T;
      }

      vstride = yk;
      nx = f_y->size[0] * f_y->size[1];
      f_y->size[0] = 1;
      f_y->size[1] = yk;
      emxEnsureCapacity_int16_T(f_y, nx);
      f_y->data[0] = 0;
      yk = 0;
      for (k = 0; k <= vstride - 2; k++) {
        yk = static_cast<int16_T>(yk + 1);
        f_y->data[k + 1] = yk;
      }

      emxInit_creal_T(&d_z, 1, true);
      z_dirtyOnGpu = false;
      nx = d_z->size[0];
      d_z->size[0] = f_y->size[1];
      emxEnsureCapacity_creal_T(d_z, nx);
      z_dirtyOnCpu = true;
      d_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((f_y->
        size[1] - 1) + 1LL), &d_grid, &d_block, 1024U, 65535U);
      if (d_validLaunchParams) {
        gpuEmxMemcpyCpuToGpu_int16_T(f_y, &c_inter_y, g_gpu_y);
        gpuEmxMemcpyCpuToGpu_creal_T(d_z, &inter_z, gpu_z);
        bb_dftregistration_coderGPU_ker<<<d_grid, d_block>>>(d_gpu_y, cloc,
          g_gpu_y, gpu_z);
        z_dirtyOnCpu = false;
        z_dirtyOnGpu = true;
      }

      emxFree_int16_T(&f_y);
      gpuEmxFree_int16_T(&c_inter_y);
      cudaMemcpy(b_gpu_x, (void *)&x[0], 1024ULL, cudaMemcpyHostToDevice);
      cb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
        b_gpu_x, *b_gpu_y);
      emxInit_creal_T(&kernr, 2, true);
      nx = kernr->size[0] * kernr->size[1];
      if (z_dirtyOnGpu) {
        gpuEmxMemcpyGpuToCpu_creal_T(d_z, &inter_z);
      }

      kernr->size[0] = d_z->size[0];
      kernr->size[1] = 512;
      emxEnsureCapacity_creal_T(kernr, nx);
      gpuEmxMemcpyCpuToGpu_creal_T(kernr, &inter_kernr, gpu_kernr);
      if (z_dirtyOnCpu) {
        gpuEmxMemcpyCpuToGpu_creal_T(d_z, &inter_z, gpu_z);
      }

      db_dftregistration_coderGPU_ker<<<dim3(32767U, 1U, 1U), dim3(512U, 1U, 1U)>>>
        (gpu_z, gpu_kernr, *b_gpu_y);
      emxFree_creal_T(&d_z);
      gpuEmxFree_creal_T(&inter_z);
      cudaMemcpy(gpu_nx, &nx, 4ULL, cudaMemcpyHostToDevice);
      eb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (gpu_kernr, gpu_nx);
      cudaMemcpy(&nx, gpu_nx, 4ULL, cudaMemcpyDeviceToHost);
      e_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((nx - 1) +
        1LL), &e_grid, &e_block, 1024U, 65535U);
      if (e_validLaunchParams) {
        fb_dftregistration_coderGPU_ker<<<e_grid, e_block>>>(gpu_nx, gpu_kernr);
      }

      emxInit_creal_T(&g_y, 2, true);
      nx = g_y->size[0] * g_y->size[1];
      gpuEmxMemcpyGpuToCpu_creal_T(kernr, &inter_kernr);
      g_y->size[0] = kernr->size[0];
      g_y->size[1] = 512;
      emxEnsureCapacity_creal_T(g_y, nx);
      y_dirtyOnCpu = true;
      f_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>
        ((kernr->size[0] * 512 - 1) + 1LL), &f_grid, &f_block, 1024U, 65535U);
      if (f_validLaunchParams) {
        gpuEmxMemcpyCpuToGpu_creal_T(g_y, &d_inter_y, gpu_y);
        gb_dftregistration_coderGPU_ker<<<f_grid, f_block>>>(gpu_kernr, gpu_y);
        y_dirtyOnCpu = false;
      }

      alpha1.re = 1.0;
      alpha1.im = 0.0;
      beta1.re = 0.0;
      beta1.im = 0.0;
      cudaMemcpy(gpu_alpha1, &alpha1, 16ULL, cudaMemcpyHostToDevice);
      cudaMemcpy(gpu_beta1, &beta1, 16ULL, cudaMemcpyHostToDevice);
      if (y_dirtyOnCpu) {
        gpuEmxMemcpyCpuToGpu_creal_T(g_y, &d_inter_y, gpu_y);
      }

      cublasZgemm(cublasGlobalHandle, CUBLAS_OP_N, CUBLAS_OP_N, kernr->size[0],
                  512, 512, (cuDoubleComplex *)gpu_alpha1, (cuDoubleComplex *)
                  &inter_kernr.data[0], kernr->size[0], (cuDoubleComplex *)
                  &(*gpu_CC)[0], 512, (cuDoubleComplex *)gpu_beta1,
                  (cuDoubleComplex *)&d_inter_y.data[0], kernr->size[0]);
      nx = out->size[0] * out->size[1];
      gpuEmxMemcpyGpuToCpu_creal_T(g_y, &d_inter_y);
      out->size[0] = g_y->size[0];
      if (kernc_dirtyOnGpu) {
        gpuEmxMemcpyGpuToCpu_creal_T(kernc, &inter_kernc);
      }

      out->size[1] = kernc->size[1];
      emxEnsureCapacity_creal_T(out, nx);
      out_dirtyOnCpu = true;
      emxFree_creal_T(&kernr);
      gpuEmxFree_creal_T(&inter_kernr);
      g_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((g_y->
        size[0] * kernc->size[1] - 1) + 1LL), &g_grid, &g_block, 1024U, 65535U);
      if (g_validLaunchParams) {
        gpuEmxMemcpyCpuToGpu_creal_T(out, &inter_out, gpu_out);
        hb_dftregistration_coderGPU_ker<<<g_grid, g_block>>>(gpu_kernc, gpu_y,
          gpu_out);
        out_dirtyOnCpu = false;
      }

      emxInit_creal_T(&CC, 2, true);
      c_CC_dirtyOnGpu = false;
      alpha1.re = 1.0;
      alpha1.im = 0.0;
      beta1.re = 0.0;
      beta1.im = 0.0;
      cudaMemcpy(gpu_alpha1, &alpha1, 16ULL, cudaMemcpyHostToDevice);
      cudaMemcpy(gpu_beta1, &beta1, 16ULL, cudaMemcpyHostToDevice);
      if (out_dirtyOnCpu) {
        gpuEmxMemcpyCpuToGpu_creal_T(out, &inter_out, gpu_out);
      }

      cublasZgemm(cublasGlobalHandle, CUBLAS_OP_N, CUBLAS_OP_N, g_y->size[0],
                  kernc->size[1], 512, (cuDoubleComplex *)gpu_alpha1,
                  (cuDoubleComplex *)&d_inter_y.data[0], g_y->size[0],
                  (cuDoubleComplex *)&inter_kernc.data[0], 512, (cuDoubleComplex
        *)gpu_beta1, (cuDoubleComplex *)&inter_out.data[0], g_y->size[0]);
      absar = 262144.0 * static_cast<real_T>(vspread);
      nx = CC->size[0] * CC->size[1];
      gpuEmxMemcpyGpuToCpu_creal_T(out, &inter_out);
      CC->size[0] = out->size[0];
      CC->size[1] = out->size[1];
      emxEnsureCapacity_creal_T(CC, nx);
      c_CC_dirtyOnCpu = true;
      emxFree_creal_T(&g_y);
      gpuEmxFree_creal_T(&d_inter_y);
      emxFree_creal_T(&kernc);
      gpuEmxFree_creal_T(&inter_kernc);
      h_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((out->
        size[0] * out->size[1] - 1) + 1LL), &h_grid, &h_block, 1024U, 65535U);
      if (h_validLaunchParams) {
        cudaMemcpy(gpu_absar, &absar, 8ULL, cudaMemcpyHostToDevice);
        gpuEmxMemcpyCpuToGpu_creal_T(CC, &inter_CC, c_gpu_CC);
        ib_dftregistration_coderGPU_ker<<<h_grid, h_block>>>(gpu_absar, gpu_out,
          c_gpu_CC);
        c_CC_dirtyOnCpu = false;
        c_CC_dirtyOnGpu = true;
      }

      emxFree_creal_T(&out);
      gpuEmxFree_creal_T(&inter_out);
      emxInit_creal_T(&b_ex, 2, true);
      ex_dirtyOnGpu = false;

      /*  Locate maximum and map back to original pixel grid  */
      nx = b_ex->size[0] * b_ex->size[1];
      b_ex->size[0] = 1;
      if (c_CC_dirtyOnGpu) {
        gpuEmxMemcpyGpuToCpu_creal_T(CC, &inter_CC);
      }

      b_ex->size[1] = static_cast<uint16_T>(CC->size[1]);
      emxEnsureCapacity_creal_T(b_ex, nx);
      gpuEmxMemcpyCpuToGpu_creal_T(b_ex, &inter_ex, b_gpu_ex);
      jb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (b_gpu_ex, *gpu_uv);
      emxInit_uint16_T(&b_idx, 2, true);
      idx_dirtyOnGpu = false;
      nx = b_idx->size[0] * b_idx->size[1];
      b_idx->size[0] = 1;
      cudaMemcpy(&uv[0], gpu_uv, 4ULL, cudaMemcpyDeviceToHost);
      b_idx->size[1] = uv[1];
      emxEnsureCapacity_uint16_T(b_idx, nx);
      idx_dirtyOnCpu = true;
      i_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((uv[1] - 1)
        + 1LL), &i_grid, &i_block, 1024U, 65535U);
      if (i_validLaunchParams) {
        gpuEmxMemcpyCpuToGpu_uint16_T(b_idx, &inter_idx, b_gpu_idx);
        kb_dftregistration_coderGPU_ker<<<i_grid, i_block>>>(*gpu_uv, b_gpu_idx);
        idx_dirtyOnCpu = false;
        idx_dirtyOnGpu = true;
      }

      vstride = CC->size[0];
      npages = CC->size[1];
      j_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((npages -
        1) + 1LL), &j_grid, &j_block, 1024U, 65535U);
      if (j_validLaunchParams) {
        if (c_CC_dirtyOnCpu) {
          gpuEmxMemcpyCpuToGpu_creal_T(CC, &inter_CC, c_gpu_CC);
          c_CC_dirtyOnCpu = false;
        }

        if (idx_dirtyOnCpu) {
          gpuEmxMemcpyCpuToGpu_uint16_T(b_idx, &inter_idx, b_gpu_idx);
          idx_dirtyOnCpu = false;
        }

        lb_dftregistration_coderGPU_ker<<<j_grid, j_block>>>(c_gpu_CC, vstride,
          npages, b_gpu_idx, b_gpu_ex);
        idx_dirtyOnGpu = true;
        ex_dirtyOnGpu = true;
      }

      nx = 0;
      if (ex_dirtyOnGpu) {
        gpuEmxMemcpyGpuToCpu_creal_T(b_ex, &inter_ex);
      }

      vstride = b_ex->size[1];
      rg00_im = b_ex->data[0].re;
      ex_im = b_ex->data[0].im;
      for (i = 0; i <= vstride - 2; i++) {
        if (rtIsNaN(b_ex->data[i + 1].re) || rtIsNaN(b_ex->data[i + 1].im)) {
          SCALEA = false;
        } else if (rtIsNaN(rg00_im) || rtIsNaN(ex_im)) {
          SCALEA = true;
        } else {
          if ((fabs(rg00_im) > 8.9884656743115785E+307) || (fabs(ex_im) >
               8.9884656743115785E+307)) {
            SCALEA = true;
          } else {
            SCALEA = false;
          }

          if ((fabs(b_ex->data[i + 1].re) > 8.9884656743115785E+307) || (fabs
               (b_ex->data[i + 1].im) > 8.9884656743115785E+307)) {
            SCALEB = true;
          } else {
            SCALEB = false;
          }

          if (SCALEA || SCALEB) {
            tmp_im = rt_hypotd_snf(rg00_im / 2.0, ex_im / 2.0);
            br = rt_hypotd_snf(b_ex->data[i + 1].re / 2.0, b_ex->data[i + 1].im /
                               2.0);
          } else {
            tmp_im = rt_hypotd_snf(rg00_im, ex_im);
            br = rt_hypotd_snf(b_ex->data[i + 1].re, b_ex->data[i + 1].im);
          }

          if (tmp_im == br) {
            absar = fabs(rg00_im);
            tmp_re = fabs(ex_im);
            absbr = fabs(b_ex->data[i + 1].re);
            tmp_im = fabs(b_ex->data[i + 1].im);
            if (absar > tmp_re) {
              Ma = absar;
              absar = tmp_re;
            } else {
              Ma = tmp_re;
            }

            if (absbr > tmp_im) {
              tmp_re = absbr;
              absbr = tmp_im;
            } else {
              tmp_re = tmp_im;
            }

            if (Ma > tmp_re) {
              if (absar < absbr) {
                tmp_im = Ma - tmp_re;
                br = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + tmp_re / 2.0) *
                  (absbr - absar);
              } else {
                tmp_im = Ma;
                br = tmp_re;
              }
            } else if (Ma < tmp_re) {
              if (absar > absbr) {
                br = tmp_re - Ma;
                tmp_im = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + tmp_re / 2.0)
                  * (absar - absbr);
              } else {
                tmp_im = Ma;
                br = tmp_re;
              }
            } else {
              tmp_im = absar;
              br = absbr;
            }

            if (tmp_im == br) {
              tmp_im = rt_atan2d_snf(ex_im, rg00_im);
              br = rt_atan2d_snf(b_ex->data[i + 1].im, b_ex->data[i + 1].re);
              if (tmp_im == br) {
                br = b_ex->data[i + 1].re;
                tmp_re = b_ex->data[i + 1].im;
                if (tmp_im > 0.78539816339744828) {
                  if (tmp_im > 2.3561944901923448) {
                    tmp_im = -ex_im;
                    br = -tmp_re;
                  } else {
                    tmp_im = -rg00_im;
                    br = -br;
                  }
                } else if (tmp_im > -0.78539816339744828) {
                  tmp_im = ex_im;
                  br = tmp_re;
                } else if (tmp_im > -2.3561944901923448) {
                  tmp_im = rg00_im;
                } else {
                  tmp_im = -ex_im;
                  br = -tmp_re;
                }

                if (tmp_im == br) {
                  tmp_im = 0.0;
                  br = 0.0;
                }
              }
            }
          }

          SCALEA = (tmp_im < br);
        }

        if (SCALEA) {
          rg00_im = b_ex->data[i + 1].re;
          ex_im = b_ex->data[i + 1].im;
          nx = i + 1;
        }
      }

      emxFree_creal_T(&b_ex);
      gpuEmxFree_creal_T(&inter_ex);
      cudaMemcpy(gpu_nx, &nx, 4ULL, cudaMemcpyHostToDevice);
      if (c_CC_dirtyOnCpu) {
        gpuEmxMemcpyCpuToGpu_creal_T(CC, &inter_CC, c_gpu_CC);
      }

      if (idx_dirtyOnCpu) {
        gpuEmxMemcpyCpuToGpu_uint16_T(b_idx, &inter_idx, b_gpu_idx);
      }

      mb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (c_gpu_CC, gpu_nx, b_gpu_idx, gpu_CCmax);
      emxFree_creal_T(&CC);
      gpuEmxFree_creal_T(&inter_CC);
      nb_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
        (buf1ft, *gpu_CC);

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
      alpha1.re = 1.0;
      alpha1.im = 0.0;
      beta1.re = 0.0;
      beta1.im = 0.0;
      ob_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
        b_gpu_y, *b_gpu_kernr);
      cudaMemcpy(gpu_alpha1, &alpha1, 16ULL, cudaMemcpyHostToDevice);
      cudaMemcpy(gpu_beta1, &beta1, 16ULL, cudaMemcpyHostToDevice);
      cublasZgemm(cublasGlobalHandle, CUBLAS_OP_N, CUBLAS_OP_N, 1, 512, 512,
                  (cuDoubleComplex *)gpu_alpha1, (cuDoubleComplex *)
                  &(*b_gpu_kernr)[0], 1, (cuDoubleComplex *)&(*gpu_CC)[0], 512,
                  (cuDoubleComplex *)gpu_beta1, (cuDoubleComplex *)&(*b_gpu_y)[0],
                  1);
      y_dirtyOnGpu = true;
      pb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (d_gpu_y);
      b_y_dirtyOnGpu = true;
      for (iv0 = 0; iv0 < 512; iv0++) {
        if (y_dirtyOnGpu) {
          cudaMemcpy(&y[0], b_gpu_y, 8192ULL, cudaMemcpyDeviceToHost);
          y_dirtyOnGpu = false;
        }

        if (b_y_dirtyOnGpu) {
          cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
          b_y_dirtyOnGpu = false;
        }

        b_y.re += y[iv0].re - y[iv0].im * 0.0;
        b_y.im += y[iv0].re * 0.0 + y[iv0].im;
        b_y_dirtyOnCpu = true;
      }

      br = 262144.0 * static_cast<real_T>(vspread);
      if (b_y_dirtyOnGpu) {
        cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
      }

      if (b_y.im == 0.0) {
        Ma = b_y.re / br;
        rg00_im = 0.0;
      } else if (b_y.re == 0.0) {
        Ma = 0.0;
        rg00_im = b_y.im / br;
      } else {
        Ma = b_y.re / br;
        rg00_im = b_y.im / br;
      }

      qb_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
        (buf2ft, *gpu_CC);

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
      alpha1.re = 1.0;
      alpha1.im = 0.0;
      beta1.re = 0.0;
      beta1.im = 0.0;
      rb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
        b_gpu_y, *b_gpu_kernr);
      cudaMemcpy(gpu_alpha1, &alpha1, 16ULL, cudaMemcpyHostToDevice);
      cudaMemcpy(gpu_beta1, &beta1, 16ULL, cudaMemcpyHostToDevice);
      cublasZgemm(cublasGlobalHandle, CUBLAS_OP_N, CUBLAS_OP_N, 1, 512, 512,
                  (cuDoubleComplex *)gpu_alpha1, (cuDoubleComplex *)
                  &(*b_gpu_kernr)[0], 1, (cuDoubleComplex *)&(*gpu_CC)[0], 512,
                  (cuDoubleComplex *)gpu_beta1, (cuDoubleComplex *)&(*b_gpu_y)[0],
                  1);
      y_dirtyOnGpu = true;
      if (b_y_dirtyOnCpu) {
        cudaMemcpy(d_gpu_y, &b_y, 16ULL, cudaMemcpyHostToDevice);
        b_y_dirtyOnCpu = false;
      }

      sb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (d_gpu_y);
      b_y_dirtyOnGpu = true;
      for (iv0 = 0; iv0 < 512; iv0++) {
        if (y_dirtyOnGpu) {
          cudaMemcpy(&y[0], b_gpu_y, 8192ULL, cudaMemcpyDeviceToHost);
          y_dirtyOnGpu = false;
        }

        if (b_y_dirtyOnGpu) {
          cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
          b_y_dirtyOnGpu = false;
        }

        b_y.re += y[iv0].re - y[iv0].im * 0.0;
        b_y.im += y[iv0].re * 0.0 + y[iv0].im;
        b_y_dirtyOnCpu = true;
      }

      br = 262144.0 * static_cast<real_T>(vspread);
      if (b_y_dirtyOnGpu) {
        cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
      }

      if (b_y.im == 0.0) {
        absar = b_y.re / br;
        tmp_re = 0.0;
        tmp_re_dirtyOnCpu = true;
      } else if (b_y.re == 0.0) {
        absar = 0.0;
        tmp_re = b_y.im / br;
        tmp_re_dirtyOnCpu = true;
      } else {
        absar = b_y.re / br;
        tmp_re = b_y.im / br;
        tmp_re_dirtyOnCpu = true;
      }

      if (idx_dirtyOnGpu) {
        gpuEmxMemcpyGpuToCpu_uint16_T(b_idx, &inter_idx);
      }

      yk = static_cast<int16_T>((b_idx->data[nx] - c_z) - 1);
      cloc = static_cast<int16_T>(nx - c_z);
      emxFree_uint16_T(&b_idx);
      gpuEmxFree_uint16_T(&inter_idx);
      if (yk >= 0) {
        c_y = static_cast<uint16_T>(yk);
      } else {
        c_y = static_cast<uint16_T>(-yk);
      }

      q = static_cast<uint16_T>(static_cast<uint32_T>(c_y) / usfac);
      c_y = static_cast<uint16_T>(static_cast<uint32_T>(c_y) -
        static_cast<uint16_T>(static_cast<uint32_T>(q) * usfac));
      if ((c_y > 0) && (c_y >= static_cast<int32_T>(static_cast<uint32_T>(usfac)
            >> 1) + (usfac & 1))) {
        q = static_cast<uint16_T>(q + 1);
      }

      c_z = static_cast<int16_T>(q);
      if (yk < 0) {
        c_z = static_cast<int16_T>(-q);
      }

      row_shift = z + c_z;
      row_shift_dirtyOnGpu = false;
      row_shift_dirtyOnCpu = true;
      if (cloc >= 0) {
        c_y = static_cast<uint16_T>(cloc);
      } else {
        c_y = static_cast<uint16_T>(-cloc);
      }

      q = static_cast<uint16_T>(static_cast<uint32_T>(c_y) / usfac);
      c_y = static_cast<uint16_T>(static_cast<uint32_T>(c_y) -
        static_cast<uint16_T>(static_cast<uint32_T>(q) * usfac));
      if ((c_y > 0) && (c_y >= static_cast<int32_T>(static_cast<uint32_T>(usfac)
            >> 1) + (usfac & 1))) {
        q = static_cast<uint16_T>(q + 1);
      }

      z = static_cast<int16_T>(q);
      if (cloc < 0) {
        z = static_cast<int16_T>(-q);
      }

      col_shift = b_z + z;

      /*  If upsampling = 2, no additional pixel shift refinement */
    } else {
      n_dftregistration_coderGPU_kern<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
        (buf1ft, *gpu_CC);
      o_dftregistration_coderGPU_kern<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
        b_gpu_y, *gpu_CC);
      y_dirtyOnGpu = true;
      p_dftregistration_coderGPU_kern<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (d_gpu_y);
      b_y_dirtyOnGpu = true;
      for (iv0 = 0; iv0 < 512; iv0++) {
        if (y_dirtyOnGpu) {
          cudaMemcpy(&y[0], b_gpu_y, 8192ULL, cudaMemcpyDeviceToHost);
          y_dirtyOnGpu = false;
        }

        if (b_y_dirtyOnGpu) {
          cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
          b_y_dirtyOnGpu = false;
        }

        b_y.re += y[iv0].re;
        b_y.im += y[iv0].im;
        b_y_dirtyOnCpu = true;
      }

      if (b_y_dirtyOnGpu) {
        cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
      }

      if (b_y.im == 0.0) {
        tmp_re = b_y.re / 1024.0;
        tmp_im = 0.0;
      } else if (b_y.re == 0.0) {
        tmp_re = 0.0;
        tmp_im = b_y.im / 1024.0;
      } else {
        tmp_re = b_y.re / 1024.0;
        tmp_im = b_y.im / 1024.0;
      }

      if (tmp_im == 0.0) {
        Ma = tmp_re / 1024.0;
        rg00_im = 0.0;
      } else if (tmp_re == 0.0) {
        Ma = 0.0;
        rg00_im = tmp_im / 1024.0;
      } else {
        Ma = tmp_re / 1024.0;
        rg00_im = tmp_im / 1024.0;
      }

      q_dftregistration_coderGPU_kern<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
        (buf2ft, *gpu_CC);
      r_dftregistration_coderGPU_kern<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
        b_gpu_y, *gpu_CC);
      y_dirtyOnGpu = true;
      if (b_y_dirtyOnCpu) {
        cudaMemcpy(d_gpu_y, &b_y, 16ULL, cudaMemcpyHostToDevice);
        b_y_dirtyOnCpu = false;
      }

      s_dftregistration_coderGPU_kern<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (d_gpu_y);
      b_y_dirtyOnGpu = true;
      for (iv0 = 0; iv0 < 512; iv0++) {
        if (y_dirtyOnGpu) {
          cudaMemcpy(&y[0], b_gpu_y, 8192ULL, cudaMemcpyDeviceToHost);
          y_dirtyOnGpu = false;
        }

        if (b_y_dirtyOnGpu) {
          cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
          b_y_dirtyOnGpu = false;
        }

        b_y.re += y[iv0].re;
        b_y.im += y[iv0].im;
        b_y_dirtyOnCpu = true;
      }

      if (b_y_dirtyOnGpu) {
        cudaMemcpy(&b_y, d_gpu_y, 16ULL, cudaMemcpyDeviceToHost);
      }

      if (b_y.im == 0.0) {
        tmp_re = b_y.re / 1024.0;
        tmp_im = 0.0;
      } else if (b_y.re == 0.0) {
        tmp_re = 0.0;
        tmp_im = b_y.im / 1024.0;
      } else {
        tmp_re = b_y.re / 1024.0;
        tmp_im = b_y.im / 1024.0;
      }

      if (tmp_im == 0.0) {
        absar = tmp_re / 1024.0;
        tmp_re = 0.0;
        tmp_re_dirtyOnCpu = true;
      } else if (tmp_re == 0.0) {
        absar = 0.0;
        tmp_re = tmp_im / 1024.0;
        tmp_re_dirtyOnCpu = true;
      } else {
        absar = tmp_re / 1024.0;
        tmp_re = tmp_im / 1024.0;
        tmp_re_dirtyOnCpu = true;
      }
    }

    cudaMemcpy(&CCmax, gpu_CCmax, 16ULL, cudaMemcpyDeviceToHost);
    ex_im = CCmax.re * CCmax.re - CCmax.im * -CCmax.im;
    CCmax_im = CCmax.re * -CCmax.im + CCmax.im * CCmax.re;
    absbr = Ma * absar - rg00_im * tmp_re;
    rg00_im = Ma * tmp_re + rg00_im * absar;
    if (rg00_im == 0.0) {
      if (CCmax_im == 0.0) {
        Ma = ex_im / absbr;
        CCmax_im = 0.0;
      } else if (ex_im == 0.0) {
        Ma = 0.0;
        CCmax_im /= absbr;
      } else {
        Ma = ex_im / absbr;
        CCmax_im /= absbr;
      }
    } else if (absbr == 0.0) {
      if (ex_im == 0.0) {
        Ma = CCmax_im / rg00_im;
        CCmax_im = 0.0;
      } else if (CCmax_im == 0.0) {
        Ma = 0.0;
        CCmax_im = -(ex_im / rg00_im);
      } else {
        Ma = CCmax_im / rg00_im;
        CCmax_im = -(ex_im / rg00_im);
      }
    } else {
      absar = fabs(absbr);
      tmp_re = fabs(rg00_im);
      if (absar > tmp_re) {
        tmp_im = rg00_im / absbr;
        tmp_re = absbr + tmp_im * rg00_im;
        Ma = (ex_im + tmp_im * CCmax_im) / tmp_re;
        CCmax_im = (CCmax_im - tmp_im * ex_im) / tmp_re;
      } else if (tmp_re == absar) {
        if (absbr > 0.0) {
          tmp_im = 0.5;
        } else {
          tmp_im = -0.5;
        }

        if (rg00_im > 0.0) {
          tmp_re = 0.5;
        } else {
          tmp_re = -0.5;
        }

        Ma = (ex_im * tmp_im + CCmax_im * tmp_re) / absar;
        CCmax_im = (CCmax_im * tmp_im - ex_im * tmp_re) / absar;
      } else {
        tmp_im = absbr / rg00_im;
        tmp_re = rg00_im + tmp_im * absbr;
        Ma = (tmp_im * ex_im + CCmax_im) / tmp_re;
        CCmax_im = (tmp_im * CCmax_im - ex_im) / tmp_re;
      }
    }

    tmp_im = rt_atan2d_snf(CCmax.im, CCmax.re);

    /*  If its only one row or column the shift along that dimension has no */
    /*  effect. We set to zero. */
    if (row_shift_dirtyOnCpu) {
      cudaMemcpy(gpu_row_shift, &row_shift, 8ULL, cudaMemcpyHostToDevice);
    }

    cudaMemcpy(gpu_tmp_im, &tmp_im, 8ULL, cudaMemcpyHostToDevice);
    tmp_im_dirtyOnCpu = false;
    tb_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (col_shift, gpu_row_shift, gpu_tmp_im, CCmax_im, Ma, *gpu_dv1);
    nx = output->size[0] * output->size[1];
    output->size[0] = 1;
    output->size[1] = 4;
    emxEnsureCapacity_real_T(output, nx);
    gpuEmxMemcpyCpuToGpu_real_T(output, &inter_output, gpu_output);
    ub_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_dv1, gpu_output);
  }

  if (b_y_dirtyOnCpu) {
    cudaMemcpy(d_gpu_y, &b_y, 16ULL, cudaMemcpyHostToDevice);
  }

  if (tmp_im_dirtyOnCpu) {
    cudaMemcpy(gpu_tmp_im, &tmp_im, 8ULL, cudaMemcpyHostToDevice);
  }

  vc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
    (gpu_tmp_im, d_gpu_y);
  if (tmp_im == 0.0) {
    if (tmp_re_dirtyOnCpu) {
      cudaMemcpy(gpu_tmp_re, &tmp_re, 8ULL, cudaMemcpyHostToDevice);
    }

    yc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (d_gpu_y, gpu_tmp_re);
    ad_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (gpu_tmp_re, d_gpu_y);
  } else {
    if (tmp_re_dirtyOnCpu) {
      cudaMemcpy(gpu_tmp_re, &tmp_re, 8ULL, cudaMemcpyHostToDevice);
    }

    wc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (d_gpu_y, gpu_tmp_re);
    xc_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (gpu_tmp_im, gpu_tmp_re, d_gpu_y);
  }

  bd_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
    (d_gpu_y, buf2ft, Greg);

  /*  Compute registered version of buf2ft */
  if (usfac > 0) {
    if (b_dirtyOnCpu) {
      cudaMemcpy(gpu_b, (void *)&b[0], 1024ULL, cudaMemcpyHostToDevice);
    }

    cd_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_b, *gpu_Nr, *gpu_Nc);
    if (row_shift_dirtyOnGpu) {
      cudaMemcpy(&row_shift, gpu_row_shift, 8ULL, cudaMemcpyDeviceToHost);
    }

    row_shift = -row_shift;
    cudaMemcpy(gpu_row_shift, &row_shift, 8ULL, cudaMemcpyHostToDevice);
    dd_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*
      gpu_Nc, col_shift, *gpu_Nr, gpu_row_shift, Greg);
    ed_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (Greg);
    fd_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (gpu_tmp_im, d_gpu_y);
    if (tmp_im == 0.0) {
      id_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (d_gpu_y, gpu_tmp_re);
      jd_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (gpu_tmp_re, d_gpu_y);
    } else {
      gd_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (d_gpu_y, gpu_tmp_re);
      hd_dftregistration_coderGPU_ker<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (gpu_tmp_im, gpu_tmp_re, d_gpu_y);
    }

    kd_dftregistration_coderGPU_ker<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (d_gpu_y, buf2ft, Greg);
  }

  emlrtHeapReferenceStackLeaveFcnR2012b(emlrtRootTLSGlobal);
  cufftEnsureDestruction();
  gpuEmxMemcpyGpuToCpu_real_T(output, &inter_output);
  cudaFree(gpu_nx);
  cudaFree(*gpu_CC);
  cudaFree(gpu_tmp_re);
  cudaFree(gpu_tmp_im);
  cudaFree(*gpu_x);
  cudaFree(*b_gpu_CC);
  cudaFree(*c_gpu_y);
  cudaFree(*gpu_idx);
  cudaFree(*gpu_ex);
  cudaFree(gpu_absar);
  cudaFree(gpu_row_shift);
  cudaFree(*b_gpu_y);
  cudaFree(d_gpu_y);
  cudaFree(e_gpu_y);
  cudaFree(f_gpu_y);
  cudaFree(gpu_kernc);
  cudaFree(*gpu_b);
  cudaFree(g_gpu_y);
  cudaFree(gpu_z);
  cudaFree(*b_gpu_x);
  cudaFree(gpu_kernr);
  cudaFree(gpu_y);
  cudaFree(gpu_alpha1);
  cudaFree(gpu_beta1);
  cudaFree(gpu_out);
  cudaFree(c_gpu_CC);
  cudaFree(b_gpu_ex);
  cudaFree(b_gpu_idx);
  cudaFree(*gpu_uv);
  cudaFree(gpu_CCmax);
  cudaFree(*b_gpu_kernr);
  gpuEmxFree_real_T(&inter_output);
  cudaFree(gpu_output);
  cudaFree(*gpu_dv1);
  cudaFree(*gpu_y1);
  cudaFree(*c_gpu_idx);
  cudaFree(*gpu_absb1);
  cudaFree(*gpu_absb2);
  cudaFree(*gpu_dv);
  cudaFree(*gpu_Nr);
  cudaFree(*gpu_Nc);
}

/* End of code generation (dftregistration_coderGPU.cu) */
