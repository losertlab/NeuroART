/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * regseqRT_coderGPU.cu
 *
 * Code generation for function 'regseqRT_coderGPU'
 *
 */

/* Include files */
#include "regseqRT_coderGPU.h"
#include "MWCudaDimUtility.h"
#include "MWLaunchParametersUtilities.h"
#include "math_constants.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Type Definitions */
#include "cufft.h"

/* Variable Definitions */
static cufftHandle cufftGlobalHandle;
static cufftHandle *cufftGlobalHandlePtr = NULL;
static int32_T cufftGlobalSize = 0U;
static int32_T cufftGlobalBatch = 0U;
static cufftType_t cufftGlobalType = CUFFT_R2C;
static int32_T cufftGlobalDist = 0U;

/* Function Declarations */
static __device__ real_T b_rt_atan2d_snf(real_T u0, real_T u1);
static __device__ real_T b_rt_hypotd_snf(real_T u0, real_T u1);
static void cufftEnsureDestruction();
static void cufftEnsureInitialization(int32_T nelem, cufftType_t type, int32_T
  batch, int32_T idist);
static __global__ void regseqRT_coderGPU_kernel1(const real_T currentFrame
  [262144], real_T dstYWorld[262144]);
static __global__ void regseqRT_coderGPU_kernel10(int16_T idx[512], creal_T X
  [262144], creal_T ex[512]);
static __global__ void regseqRT_coderGPU_kernel11(const int16_T idx[512],
  int32_T *absx11);
static __global__ void regseqRT_coderGPU_kernel12(const int32_T *absx11, const
  int32_T itmp, int16_T NonFillOutputLocY[2]);
static __global__ void regseqRT_coderGPU_kernel13(const int16_T
  NonFillOutputLocY[2], real_T z1[2]);
static __global__ void regseqRT_coderGPU_kernel14(const int8_T iv[3], const
  int8_T iv1[3], int16_T A[9]);
static __global__ void regseqRT_coderGPU_kernel15(const int16_T
  NonFillOutputLocY[2], int16_T A[9]);
static __global__ void regseqRT_coderGPU_kernel16(int16_T A[9]);
static __global__ void regseqRT_coderGPU_kernel17(real_T dstYWorld[262144],
  real_T dstXWorld[262144]);
static __global__ void regseqRT_coderGPU_kernel18(const int16_T A[9], real_T x[9]);
static __global__ void regseqRT_coderGPU_kernel19(const int16_T A[9], real_T x[9]);
static __global__ void regseqRT_coderGPU_kernel2(creal_T b_y1[262144]);
static __global__ void regseqRT_coderGPU_kernel20(const int16_T A[9], real_T x[9]);
static __global__ void regseqRT_coderGPU_kernel21(const real_T absbr, const
  real_T absar, const real_T x, const real_T br, const real_T absbi, const
  real_T absai, real_T dstYWorld[262144], real_T dstXWorld[262144], real_T
  srcXIntrinsic[262144]);
static __global__ void regseqRT_coderGPU_kernel22(real_T inputImagePadded[268324]);
static __global__ void regseqRT_coderGPU_kernel23(real_T inputImagePadded[268324]);
static __global__ void regseqRT_coderGPU_kernel24(const real_T currentFrame
  [262144], real_T inputImagePadded[268324]);
static __global__ void regseqRT_coderGPU_kernel25(const real_T inputImagePadded
  [268324], const real_T dstXWorld[262144], const real_T srcXIntrinsic[262144],
  real_T regFrame[262144]);
static __global__ void regseqRT_coderGPU_kernel26(real_T regFrame[262144]);
static __global__ void regseqRT_coderGPU_kernel27(const int16_T
  NonFillOutputLocY[2], int16_T *b_NonFillOutputLocY, int16_T
  *c_NonFillOutputLocY);
static __global__ void regseqRT_coderGPU_kernel28(const int16_T
  *NonFillOutputLocY, const int16_T *b_NonFillOutputLocY, int16_T VWorld[2],
  int16_T UWorld[2]);
static __global__ void regseqRT_coderGPU_kernel29(const int16_T
  NonFillOutputLocY[2], int16_T *b_NonFillOutputLocY, int16_T
  *c_NonFillOutputLocY);
static __global__ void regseqRT_coderGPU_kernel3(creal_T b_y1[262144], creal_T
  X[262144]);
static __global__ void regseqRT_coderGPU_kernel30(const int16_T
  *NonFillOutputLocY, const int16_T UWorld[2], const int16_T
  *b_NonFillOutputLocY, const int16_T UWorldClippedToBounds[2], real_T outputC[2],
  real_T z1[2], boolean_T containedSubscripts[2], int16_T c_NonFillOutputLocY[2],
  int16_T VWorld[2]);
static __global__ void regseqRT_coderGPU_kernel31(const int16_T VWorld[2], const
  int8_T tmp_data[2], const int32_T tmp_size[2], real_T rPix_data[2]);
static __global__ void regseqRT_coderGPU_kernel32(const int16_T
  NonFillOutputLocY[2], const int8_T tmp_data[2], const int32_T tmp_size[2],
  real_T rInit_data[2]);
static __global__ void regseqRT_coderGPU_kernel33(const int32_T *absx11, real_T
  rPix_data[2]);
static __global__ void regseqRT_coderGPU_kernel34(const real_T rPix_data[2],
  const int32_T *absx11, real_T cPix_data[2]);
static __global__ void regseqRT_coderGPU_kernel35(const int32_T *absx11, real_T
  rInit_data[2]);
static __global__ void regseqRT_coderGPU_kernel36(const real_T rInit_data[2],
  const int32_T *absx11, real_T rPix_data[2]);
static __global__ void regseqRT_coderGPU_kernel37(const real_T currentFrame
  [262144], const int32_T p2, const int32_T *absx11, const int32_T itmp, const
  int32_T absx31, const int32_T p1, int32_T p3, real_T regFrame[262144]);
static __global__ void regseqRT_coderGPU_kernel4(const creal_T b_template[262144],
  creal_T f[262144]);
static __global__ void regseqRT_coderGPU_kernel5(creal_T X[262144]);
static __global__ void regseqRT_coderGPU_kernel6(creal_T X[262144], creal_T f
  [262144]);
static __global__ void regseqRT_coderGPU_kernel7(creal_T b_y1[262144]);
static __global__ void regseqRT_coderGPU_kernel8(creal_T b_y1[262144], creal_T
  X[262144]);
static __global__ void regseqRT_coderGPU_kernel9(int16_T idx[512]);
static real_T rt_atan2d_snf(real_T u0, real_T u1);
static real_T rt_hypotd_snf(real_T u0, real_T u1);
static __device__ real_T rt_roundd_snf(real_T u);

/* Function Definitions */
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

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel1(const
  real_T currentFrame[262144], real_T dstYWorld[262144])
{
  uint32_T threadId;
  int32_T iy;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iy = static_cast<int32_T>(threadId % 512U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iy)) / 512U);
  if (i < 512) {
    dstYWorld[iy + (i << 9)] = currentFrame[i + (iy << 9)];
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel10
  (int16_T idx[512], creal_T X[262144], creal_T ex[512])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T iy;
  int32_T i;
  int32_T ix;
  boolean_T p;
  boolean_T SCALEA;
  real_T qx2;
  real_T qx1;
  real_T absar;
  real_T ry;
  real_T absbr;
  real_T Ma;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iy = static_cast<int32_T>(threadId);
  if (iy < 512) {
    iv0 = (iy * 511 + iy) + 1;
    ex[iy] = X[iv0 - 1];
    for (i = 0; i < 511; i++) {
      ix = (iv0 + i) + 1;
      if ((static_cast<int32_T>(isnan(X[ix - 1].re))) || (static_cast<int32_T>
           (isnan(X[ix - 1].im)))) {
        p = false;
      } else if ((static_cast<int32_T>(isnan(ex[iy].re))) ||
                 (static_cast<int32_T>(isnan(ex[iy].im)))) {
        p = true;
      } else {
        if ((static_cast<int32_T>(fabs(ex[iy].re) > 8.9884656743115785E+307)) ||
            (static_cast<int32_T>(fabs(ex[iy].im) > 8.9884656743115785E+307))) {
          SCALEA = true;
        } else {
          SCALEA = false;
        }

        if ((static_cast<int32_T>(fabs(X[ix - 1].re) > 8.9884656743115785E+307))
            || (static_cast<int32_T>(fabs(X[ix - 1].im) >
              8.9884656743115785E+307))) {
          p = true;
        } else {
          p = false;
        }

        if ((static_cast<int32_T>(SCALEA)) || (static_cast<int32_T>(p))) {
          qx2 = b_rt_hypotd_snf(ex[iy].re / 2.0, ex[iy].im / 2.0);
          qx1 = b_rt_hypotd_snf(X[ix - 1].re / 2.0, X[ix - 1].im / 2.0);
        } else {
          qx2 = b_rt_hypotd_snf(ex[iy].re, ex[iy].im);
          qx1 = b_rt_hypotd_snf(X[ix - 1].re, X[ix - 1].im);
        }

        if (qx2 == qx1) {
          absar = fabs(ex[iy].re);
          ry = fabs(ex[iy].im);
          absbr = fabs(X[ix - 1].re);
          qx2 = fabs(X[ix - 1].im);
          if (absar > ry) {
            Ma = absar;
            absar = ry;
          } else {
            Ma = ry;
          }

          if (absbr > qx2) {
            ry = absbr;
            absbr = qx2;
          } else {
            ry = qx2;
          }

          if (Ma > ry) {
            if (absar < absbr) {
              qx2 = Ma - ry;
              qx1 = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + ry / 2.0) * (absbr
                - absar);
            } else {
              qx2 = Ma;
              qx1 = ry;
            }
          } else if (Ma < ry) {
            if (absar > absbr) {
              qx1 = ry - Ma;
              qx2 = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + ry / 2.0) * (absar
                - absbr);
            } else {
              qx2 = Ma;
              qx1 = ry;
            }
          } else {
            qx2 = absar;
            qx1 = absbr;
          }

          if (qx2 == qx1) {
            qx2 = b_rt_atan2d_snf(ex[iy].im, ex[iy].re);
            qx1 = b_rt_atan2d_snf(X[ix - 1].im, X[ix - 1].re);
            if (qx2 == qx1) {
              qx1 = X[ix - 1].re;
              ry = X[ix - 1].im;
              if (qx2 > 0.78539816339744828) {
                if (qx2 > 2.3561944901923448) {
                  qx2 = -ex[iy].im;
                  qx1 = -ry;
                } else {
                  qx2 = -ex[iy].re;
                  qx1 = -qx1;
                }
              } else if (qx2 > -0.78539816339744828) {
                qx2 = ex[iy].im;
                qx1 = ry;
              } else if (qx2 > -2.3561944901923448) {
                qx2 = ex[iy].re;
              } else {
                qx2 = -ex[iy].im;
                qx1 = -ry;
              }

              if (qx2 == qx1) {
                qx2 = 0.0;
                qx1 = 0.0;
              }
            }
          }
        }

        p = (qx2 < qx1);
      }

      if (p) {
        ex[iy] = X[ix - 1];
        idx[iy] = static_cast<int16_T>(i + 2);
      }
    }
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel11(const
  int16_T idx[512], int32_T *absx11)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    if (static_cast<int32_T>(idx[*absx11]) > 256) {
      *absx11 = static_cast<int32_T>(idx[*absx11]) - 513;
    } else {
      *absx11 = static_cast<int32_T>(idx[*absx11]) - 1;
    }
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel12(const
  int32_T *absx11, const int32_T itmp, int16_T NonFillOutputLocY[2])
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    NonFillOutputLocY[0] = static_cast<int16_T>(itmp);
    NonFillOutputLocY[1] = static_cast<int16_T>(*absx11);
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel13(const
  int16_T NonFillOutputLocY[2], real_T z1[2])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T iy;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId);
  if (iv0 < 2) {
    if (static_cast<int32_T>(NonFillOutputLocY[iv0]) == 0) {
      iy = 0;
    } else {
      iy = static_cast<int32_T>(fmod(static_cast<real_T>(NonFillOutputLocY[iv0]),
        1.0));
      if ((static_cast<int32_T>(iy != 0)) && (static_cast<int32_T>
           (static_cast<int32_T>(NonFillOutputLocY[iv0]) < 0))) {
        iy++;
      }
    }

    z1[iv0] = static_cast<real_T>(iy);
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel14(const
  int8_T iv[3], const int8_T iv1[3], int16_T A[9])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 3) {
    A[3 * i] = static_cast<int16_T>(iv1[i]);
    A[3 * i + 1] = static_cast<int16_T>(iv[i]);
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel15(const
  int16_T NonFillOutputLocY[2], int16_T A[9])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 2) {
    A[3 * i + 2] = NonFillOutputLocY[i];
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel16
  (int16_T A[9])
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    A[8] = static_cast<int16_T>(1);
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel17
  (real_T dstYWorld[262144], real_T dstXWorld[262144])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T iy;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId % 512U);
  iy = static_cast<int32_T>((threadId - static_cast<uint32_T>(iv0)) / 512U);
  if (iy < 512) {
    dstXWorld[iv0 + (iy << 9)] = ((static_cast<real_T>(iy) + 1.0) - 0.5) + 0.5;
    dstYWorld[iv0 + (iy << 9)] = ((static_cast<real_T>(iv0) + 1.0) - 0.5) + 0.5;
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel18(const
  int16_T A[9], real_T x[9])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 9) {
    x[i] = static_cast<real_T>(A[i]);
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel19(const
  int16_T A[9], real_T x[9])
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    x[0] = static_cast<real_T>(A[2]);
    x[2] = static_cast<real_T>(A[0]);
    x[3] = static_cast<real_T>(A[5]);
    x[5] = static_cast<real_T>(A[3]);
    x[6] = 1.0;
    x[8] = static_cast<real_T>(A[6]);
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel2
  (creal_T b_y1[262144])
{
  uint32_T threadId;
  int32_T iv0;
  int32_T i;
  int32_T iy;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iy = static_cast<int32_T>(threadId % 255U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iy)) / 255U);
  if (i < 512) {
    iv0 = i << 9;
    b_y1[(iv0 + iy) + 257].re = b_y1[(iv0 - iy) + 255].re;
    b_y1[(iv0 + iy) + 257].im = -b_y1[(iv0 - iy) + 255].im;
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel20(const
  int16_T A[9], real_T x[9])
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    x[0] = 1.0;
    x[1] = static_cast<real_T>(A[0]);
    x[3] = static_cast<real_T>(A[4]);
    x[4] = static_cast<real_T>(A[3]);
    x[6] = static_cast<real_T>(A[7]);
    x[7] = static_cast<real_T>(A[6]);
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel21(
  const real_T absbr, const real_T absar, const real_T x, const real_T br, const
  real_T absbi, const real_T absai, real_T dstYWorld[262144], real_T dstXWorld
  [262144], real_T srcXIntrinsic[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    srcXIntrinsic[i] = ((((absai * dstXWorld[i] + absbi * dstYWorld[i]) + br) -
                         0.5) + 0.5) + 3.0;
    dstXWorld[i] = ((((x * dstXWorld[i] + absar * dstYWorld[i]) + absbr) - 0.5)
                    + 0.5) + 3.0;
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel22
  (real_T inputImagePadded[268324])
{
  uint32_T threadId;
  int32_T i;
  int32_T iy;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId % 518U);
  iy = static_cast<int32_T>((threadId - static_cast<uint32_T>(i)) / 518U);
  if (iy < 3) {
    inputImagePadded[i + 518 * iy] = 0.0;
    inputImagePadded[i + 518 * (iy + 515)] = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel23
  (real_T inputImagePadded[268324])
{
  uint32_T threadId;
  int32_T i;
  int32_T iy;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId % 3U);
  iy = static_cast<int32_T>((threadId - static_cast<uint32_T>(i)) / 3U);
  if (iy < 512) {
    inputImagePadded[i + 518 * (iy + 3)] = 0.0;
    inputImagePadded[(i + 518 * (iy + 3)) + 515] = 0.0;
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel24(
  const real_T currentFrame[262144], real_T inputImagePadded[268324])
{
  uint32_T threadId;
  int32_T i;
  int32_T iy;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId % 512U);
  iy = static_cast<int32_T>((threadId - static_cast<uint32_T>(i)) / 512U);
  if (iy < 512) {
    inputImagePadded[(i + 518 * (iy + 3)) + 3] = currentFrame[i + (iy << 9)];
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel25(
  const real_T inputImagePadded[268324], const real_T dstXWorld[262144], const
  real_T srcXIntrinsic[262144], real_T regFrame[262144])
{
  uint32_T threadId;
  int32_T iv0;
  real_T qx2;
  int32_T ix;
  int32_T iy;
  real_T qx1;
  real_T ry;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iv0 = static_cast<int32_T>(threadId);
  if (iv0 < 262144) {
    if ((static_cast<int32_T>((static_cast<int32_T>((static_cast<int32_T>
             (srcXIntrinsic[iv0] >= 1.0)) && (static_cast<int32_T>
             (srcXIntrinsic[iv0] <= 518.0)))) && (static_cast<int32_T>
           (dstXWorld[iv0] >= 1.0)))) && (static_cast<int32_T>(dstXWorld[iv0] <=
          518.0))) {
      if (srcXIntrinsic[iv0] <= 1.0) {
        ix = 1;
      } else if (srcXIntrinsic[iv0] <= 517.0) {
        ix = static_cast<int32_T>(floor(srcXIntrinsic[iv0]));
      } else {
        ix = 517;
      }

      if (dstXWorld[iv0] <= 1.0) {
        iy = 1;
      } else if (dstXWorld[iv0] <= 517.0) {
        iy = static_cast<int32_T>(floor(dstXWorld[iv0]));
      } else {
        iy = 517;
      }

      if (srcXIntrinsic[iv0] == static_cast<real_T>(ix)) {
        qx1 = inputImagePadded[(iy + 518 * (ix - 1)) - 1];
        qx2 = inputImagePadded[iy + 518 * (ix - 1)];
      } else if (srcXIntrinsic[iv0] == static_cast<real_T>(ix) + 1.0) {
        qx1 = inputImagePadded[(iy + 518 * ix) - 1];
        qx2 = inputImagePadded[iy + 518 * ix];
      } else {
        qx2 = (srcXIntrinsic[iv0] - static_cast<real_T>(ix)) /
          ((static_cast<real_T>(ix) + 1.0) - static_cast<real_T>(ix));
        if (inputImagePadded[(iy + 518 * (ix - 1)) - 1] == inputImagePadded[(iy
             + 518 * ix) - 1]) {
          qx1 = inputImagePadded[(iy + 518 * (ix - 1)) - 1];
        } else {
          qx1 = (1.0 - qx2) * inputImagePadded[(iy + 518 * (ix - 1)) - 1] + qx2 *
            inputImagePadded[(iy + 518 * ix) - 1];
        }

        if (inputImagePadded[iy + 518 * (ix - 1)] == inputImagePadded[iy + 518 *
            ix]) {
          qx2 = inputImagePadded[iy + 518 * (ix - 1)];
        } else {
          qx2 = (1.0 - qx2) * inputImagePadded[iy + 518 * (ix - 1)] + qx2 *
            inputImagePadded[iy + 518 * ix];
        }
      }

      if ((static_cast<int32_T>(dstXWorld[iv0] == static_cast<real_T>(iy))) || (
           static_cast<int32_T>(qx1 == qx2))) {
        qx2 = qx1;
      } else {
        if (!static_cast<int32_T>(dstXWorld[iv0] == static_cast<real_T>(iy) +
             1.0)) {
          ry = (dstXWorld[iv0] - static_cast<real_T>(iy)) / ((static_cast<real_T>
            (iy) + 1.0) - static_cast<real_T>(iy));
          qx2 = (1.0 - ry) * qx1 + ry * qx2;
        }
      }
    } else {
      qx2 = 0.0;
    }

    regFrame[iv0] = qx2;
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel26
  (real_T regFrame[262144])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    regFrame[i] = 0.0;
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel27(const
  int16_T NonFillOutputLocY[2], int16_T *b_NonFillOutputLocY, int16_T
  *c_NonFillOutputLocY)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *c_NonFillOutputLocY = NonFillOutputLocY[0];
    *b_NonFillOutputLocY = NonFillOutputLocY[1];
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel28(const
  int16_T *NonFillOutputLocY, const int16_T *b_NonFillOutputLocY, int16_T
  VWorld[2], int16_T UWorld[2])
{
  uint32_T threadId;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 2) {
    UWorld[i] = static_cast<int16_T>((511 * i - static_cast<int32_T>
      (*b_NonFillOutputLocY)) + 1);
    VWorld[i] = static_cast<int16_T>((511 * i - static_cast<int32_T>
      (*NonFillOutputLocY)) + 1);
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel29(const
  int16_T NonFillOutputLocY[2], int16_T *b_NonFillOutputLocY, int16_T
  *c_NonFillOutputLocY)
{
  uint32_T threadId;
  int32_T tmpIdx;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  tmpIdx = static_cast<int32_T>(threadId);
  if (tmpIdx < 1) {
    *c_NonFillOutputLocY = NonFillOutputLocY[0];
    *b_NonFillOutputLocY = NonFillOutputLocY[1];
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel3
  (creal_T b_y1[262144], creal_T X[262144])
{
  uint32_T threadId;
  int32_T iy;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iy = static_cast<int32_T>(threadId % 512U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iy)) / 512U);
  if (i < 512) {
    X[iy + (i << 9)] = b_y1[i + (iy << 9)];
  }
}

static __global__ __launch_bounds__(32, 1) void regseqRT_coderGPU_kernel30(const
  int16_T *NonFillOutputLocY, const int16_T UWorld[2], const int16_T
  *b_NonFillOutputLocY, const int16_T UWorldClippedToBounds[2], real_T outputC[2],
  real_T z1[2], boolean_T containedSubscripts[2], int16_T c_NonFillOutputLocY[2],
  int16_T VWorld[2])
{
  uint32_T threadId;
  int16_T i;
  int32_T b_i;
  int16_T i1;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  b_i = static_cast<int32_T>(threadId);
  if (b_i < 2) {
    i = static_cast<int16_T>(static_cast<int32_T>(UWorldClippedToBounds[b_i]) +
      static_cast<int32_T>(*b_NonFillOutputLocY));
    VWorld[b_i] = i;
    i1 = static_cast<int16_T>(static_cast<int32_T>(UWorld[b_i]) +
      static_cast<int32_T>(*NonFillOutputLocY));
    c_NonFillOutputLocY[b_i] = i1;
    containedSubscripts[b_i] = ((static_cast<int32_T>((static_cast<int32_T>((
      static_cast<int32_T>(static_cast<real_T>(static_cast<int32_T>(i)) >= 0.5))
      && (static_cast<int32_T>(static_cast<real_T>(static_cast<int32_T>(i)) <=
      512.5)))) && (static_cast<int32_T>(static_cast<real_T>(static_cast<int32_T>
      (i1)) >= 0.5)))) && (static_cast<int32_T>(static_cast<real_T>
      (static_cast<int32_T>(i1)) <= 512.5)));
    z1[b_i] = CUDART_NAN;
    outputC[b_i] = CUDART_NAN;
  }
}

static __global__ __launch_bounds__(1024, 1) void regseqRT_coderGPU_kernel31(
  const int16_T VWorld[2], const int8_T tmp_data[2], const int32_T tmp_size[2],
  real_T rPix_data[2])
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(tmp_size[1] - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    i = static_cast<int32_T>(idx);
    rPix_data[i] = (static_cast<real_T>(VWorld[static_cast<int32_T>(tmp_data[i])
      - 1]) - 0.5) + 0.5;
  }
}

static __global__ __launch_bounds__(1024, 1) void regseqRT_coderGPU_kernel32(
  const int16_T NonFillOutputLocY[2], const int8_T tmp_data[2], const int32_T
  tmp_size[2], real_T rInit_data[2])
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(tmp_size[1] - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    i = static_cast<int32_T>(idx);
    rInit_data[i] = (static_cast<real_T>(NonFillOutputLocY[static_cast<int32_T>
      (tmp_data[i]) - 1]) - 0.5) + 0.5;
  }
}

static __global__ __launch_bounds__(1024, 1) void regseqRT_coderGPU_kernel33(
  const int32_T *absx11, real_T rPix_data[2])
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(*absx11 - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iv0 = static_cast<int32_T>(idx);
    rPix_data[iv0] = rt_roundd_snf(rPix_data[iv0]);
  }
}

static __global__ __launch_bounds__(1024, 1) void regseqRT_coderGPU_kernel34(
  const real_T rPix_data[2], const int32_T *absx11, real_T cPix_data[2])
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  real_T ry;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(*absx11 - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iv0 = static_cast<int32_T>(idx);
    if (rPix_data[iv0] < 512.0) {
      ry = rPix_data[iv0];
    } else {
      ry = 512.0;
    }

    cPix_data[iv0] = ry;
  }
}

static __global__ __launch_bounds__(1024, 1) void regseqRT_coderGPU_kernel35(
  const int32_T *absx11, real_T rInit_data[2])
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(*absx11 - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iv0 = static_cast<int32_T>(idx);
    rInit_data[iv0] = rt_roundd_snf(rInit_data[iv0]);
  }
}

static __global__ __launch_bounds__(1024, 1) void regseqRT_coderGPU_kernel36(
  const real_T rInit_data[2], const int32_T *absx11, real_T rPix_data[2])
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iv0;
  real_T ry;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = static_cast<int64_T>(*absx11 - 1);
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iv0 = static_cast<int32_T>(idx);
    if (rInit_data[iv0] < 512.0) {
      ry = rInit_data[iv0];
    } else {
      ry = 512.0;
    }

    rPix_data[iv0] = ry;
  }
}

static __global__ __launch_bounds__(1024, 1) void regseqRT_coderGPU_kernel37(
  const real_T currentFrame[262144], const int32_T p2, const int32_T *absx11,
  const int32_T itmp, const int32_T absx31, const int32_T p1, int32_T p3, real_T
  regFrame[262144])
{
  uint32_T threadId;
  uint32_T threadStride;
  uint32_T idx;
  int32_T iy;
  int32_T i;
  int64_T loopEnd;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  threadStride = static_cast<uint32_T>(mwGetTotalThreadsLaunched());
  loopEnd = (static_cast<int64_T>(p1 - absx31) + 1LL) * (static_cast<int64_T>(p3
    - p2) + 1LL) - 1LL;
  for (idx = threadId; idx <= static_cast<uint32_T>(loopEnd); idx +=
       threadStride) {
    iy = static_cast<int32_T>(idx % (static_cast<uint32_T>(p1 - absx31) + 1U));
    i = static_cast<int32_T>((idx - static_cast<uint32_T>(iy)) /
      (static_cast<uint32_T>(p1 - absx31) + 1U));
    regFrame[(itmp + iy) + ((*absx11 + i) << 9)] = currentFrame[(absx31 + iy) +
      ((p2 + i) << 9)];
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel4(const
  creal_T b_template[262144], creal_T f[262144])
{
  uint32_T threadId;
  real_T ry;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    ry = f[i].re;
    f[i].re = b_template[i].re * f[i].re - b_template[i].im * -f[i].im;
    f[i].im = b_template[i].re * -f[i].im + b_template[i].im * ry;
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel5
  (creal_T X[262144])
{
  uint32_T threadId;
  int32_T i;
  real_T ry;
  real_T qx2;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    if (X[i].im == 0.0) {
      ry = X[i].re / 512.0;
      qx2 = 0.0;
    } else if (X[i].re == 0.0) {
      ry = 0.0;
      qx2 = X[i].im / 512.0;
    } else {
      ry = X[i].re / 512.0;
      qx2 = X[i].im / 512.0;
    }

    X[i].re = ry;
    X[i].im = qx2;
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel6
  (creal_T X[262144], creal_T f[262144])
{
  uint32_T threadId;
  int32_T iy;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iy = static_cast<int32_T>(threadId % 512U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iy)) / 512U);
  if (i < 512) {
    f[iy + (i << 9)] = X[i + (iy << 9)];
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel7
  (creal_T b_y1[262144])
{
  uint32_T threadId;
  int32_T i;
  real_T ry;
  real_T qx2;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  i = static_cast<int32_T>(threadId);
  if (i < 262144) {
    if (b_y1[i].im == 0.0) {
      ry = b_y1[i].re / 512.0;
      qx2 = 0.0;
    } else if (b_y1[i].re == 0.0) {
      ry = 0.0;
      qx2 = b_y1[i].im / 512.0;
    } else {
      ry = b_y1[i].re / 512.0;
      qx2 = b_y1[i].im / 512.0;
    }

    b_y1[i].re = ry;
    b_y1[i].im = qx2;
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel8
  (creal_T b_y1[262144], creal_T X[262144])
{
  uint32_T threadId;
  int32_T iy;
  int32_T i;
  threadId = static_cast<uint32_T>(mwGetGlobalThreadIndex());
  iy = static_cast<int32_T>(threadId % 512U);
  i = static_cast<int32_T>((threadId - static_cast<uint32_T>(iy)) / 512U);
  if (i < 512) {
    X[iy + (i << 9)] = b_y1[i + (iy << 9)];
  }
}

static __global__ __launch_bounds__(512, 1) void regseqRT_coderGPU_kernel9
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

static __device__ real_T rt_roundd_snf(real_T u)
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

void regseqRT_coderGPU(const creal_T b_template[262144], const real_T
  currentFrame[262144], real_T regFrame[262144], real_T *tx, real_T *ty)
{
  creal_T ex[512];
  int32_T absx11;
  real_T ex_re;
  real_T ex_im;
  int32_T absx31;
  int32_T itmp;
  boolean_T c_locationsInSourceMapToDestina;
  real_T z1[2];
  boolean_T SCALEB;
  real_T x;
  real_T br;
  real_T absar;
  int16_T A[9];
  real_T absai;
  real_T absbr;
  real_T absbi;
  static const int8_T iv[3] = { 0, 1, 0 };

  int16_T UWorld[2];
  real_T Ma;
  static const int8_T iv1[3] = { 1, 0, 0 };

  int16_T VWorld[2];
  real_T b_x[9];
  int16_T c_intrinsicSourceBoundingRectan[4];
  int32_T p1;
  int32_T p2;
  int32_T p3;
  int16_T d_intrinsicSourceBoundingRectan[4];
  int16_T UWorldClippedToBounds[2];
  real_T tinv[9];
  boolean_T containedSubscripts[2];
  real_T outputC[2];
  int32_T tmp_size[2];
  real_T rPix_data[2];
  int8_T tmp_data[2];
  int32_T cInit_size[2];
  int32_T rInit_size[2];
  real_T cPix_data[2];
  int8_T cPix_size[2];
  int8_T rPix_size[2];
  real_T (*gpu_dstYWorld)[262144];
  creal_T (*gpu_y1)[262144];
  creal_T (*gpu_f)[262144];
  creal_T (*gpu_X)[262144];
  int16_T (*gpu_idx)[512];
  creal_T (*gpu_ex)[512];
  int32_T *gpu_absx11;
  int16_T (*gpu_NonFillOutputLocY)[2];
  real_T (*gpu_z1)[2];
  int8_T (*gpu_iv)[3];
  int8_T (*gpu_iv1)[3];
  int16_T (*gpu_A)[9];
  real_T (*gpu_dstXWorld)[262144];
  real_T (*gpu_x)[9];
  real_T (*gpu_srcXIntrinsic)[262144];
  real_T (*gpu_inputImagePadded)[268324];
  int16_T *b_gpu_NonFillOutputLocY;
  int16_T *c_gpu_NonFillOutputLocY;
  int16_T (*gpu_VWorld)[2];
  int16_T (*gpu_UWorld)[2];
  int16_T (*gpu_UWorldClippedToBounds)[2];
  real_T (*gpu_outputC)[2];
  boolean_T (*gpu_containedSubscripts)[2];
  int8_T (*gpu_tmp_data)[2];
  int32_T (*gpu_tmp_size)[2];
  dim3 grid;
  dim3 block;
  boolean_T validLaunchParams;
  real_T (*gpu_rPix_data)[2];
  dim3 b_grid;
  dim3 b_block;
  boolean_T b_validLaunchParams;
  real_T (*gpu_rInit_data)[2];
  dim3 c_grid;
  dim3 c_block;
  boolean_T c_validLaunchParams;
  dim3 d_grid;
  dim3 d_block;
  boolean_T d_validLaunchParams;
  real_T (*gpu_cPix_data)[2];
  dim3 e_grid;
  dim3 e_block;
  boolean_T e_validLaunchParams;
  dim3 f_grid;
  dim3 f_block;
  boolean_T f_validLaunchParams;
  dim3 g_grid;
  dim3 g_block;
  boolean_T g_validLaunchParams;
  boolean_T z1_dirtyOnGpu;
  boolean_T outputC_dirtyOnGpu;
  boolean_T containedSubscripts_dirtyOnGpu;
  boolean_T rPix_data_dirtyOnGpu;
  boolean_T cPix_data_dirtyOnGpu;
  boolean_T tmp_data_dirtyOnCpu;
  boolean_T tmp_size_dirtyOnCpu;
  boolean_T exitg1;
  cudaMalloc(&gpu_cPix_data, 2U * sizeof(real_T));
  cudaMalloc(&gpu_outputC, 16ULL);
  cudaMalloc(&gpu_rInit_data, 2U * sizeof(real_T));
  cudaMalloc(&gpu_rPix_data, 2U * sizeof(real_T));
  cudaMalloc(&gpu_tmp_data, 2U * sizeof(int8_T));
  cudaMalloc(&gpu_tmp_size, 8ULL);
  cudaMalloc(&gpu_containedSubscripts, 2ULL);
  cudaMalloc(&gpu_UWorldClippedToBounds, 4ULL);
  cudaMalloc(&gpu_VWorld, 4ULL);
  cudaMalloc(&gpu_UWorld, 4ULL);
  cudaMalloc(&c_gpu_NonFillOutputLocY, 2ULL);
  cudaMalloc(&b_gpu_NonFillOutputLocY, 2ULL);
  cudaMalloc(&gpu_srcXIntrinsic, 2097152ULL);
  cudaMalloc(&gpu_inputImagePadded, 2146592ULL);
  cudaMalloc(&gpu_dstXWorld, 2097152ULL);
  cudaMalloc(&gpu_x, 72ULL);
  cudaMalloc(&gpu_A, 18ULL);
  cudaMalloc(&gpu_iv1, 3ULL);
  cudaMalloc(&gpu_iv, 3ULL);
  cudaMalloc(&gpu_z1, 16ULL);
  cudaMalloc(&gpu_NonFillOutputLocY, 4ULL);
  cudaMalloc(&gpu_absx11, 4ULL);
  cudaMalloc(&gpu_ex, 8192ULL);
  cudaMalloc(&gpu_idx, 1024ULL);
  cudaMalloc(&gpu_f, 4194304ULL);
  cudaMalloc(&gpu_X, 4194304ULL);
  cudaMalloc(&gpu_y1, 4194304ULL);
  cudaMalloc(&gpu_dstYWorld, 2097152ULL);
  tmp_data_dirtyOnCpu = false;
  cPix_data_dirtyOnGpu = false;
  rPix_data_dirtyOnGpu = false;

  /*  Add kernelfun pragma to trigger GPU kernel creation */
  regseqRT_coderGPU_kernel1<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
    (currentFrame, *gpu_dstYWorld);
  cufftEnsureInitialization(512, CUFFT_D2Z, 512, 512);
  cufftExecD2Z(*cufftGlobalHandlePtr, (cufftDoubleReal *)&(*gpu_dstYWorld)[0],
               (cufftDoubleComplex *)&(*gpu_y1)[0]);
  regseqRT_coderGPU_kernel2<<<dim3(255U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_y1);
  regseqRT_coderGPU_kernel3<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_y1,
    *gpu_X);
  cufftEnsureInitialization(512, CUFFT_Z2Z, 512, 512);
  cufftExecZ2Z(*cufftGlobalHandlePtr, (cufftDoubleComplex *)&(*gpu_X)[0],
               (cufftDoubleComplex *)&(*gpu_f)[0], CUFFT_FORWARD);
  regseqRT_coderGPU_kernel4<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
    (b_template, *gpu_f);
  cufftEnsureInitialization(512, CUFFT_Z2Z, 512, 512);
  cufftExecZ2Z(*cufftGlobalHandlePtr, (cufftDoubleComplex *)&(*gpu_f)[0],
               (cufftDoubleComplex *)&(*gpu_X)[0], 1);
  regseqRT_coderGPU_kernel5<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_X);
  regseqRT_coderGPU_kernel6<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_X, *
    gpu_f);
  cufftEnsureInitialization(512, CUFFT_Z2Z, 512, 512);
  cufftExecZ2Z(*cufftGlobalHandlePtr, (cufftDoubleComplex *)&(*gpu_f)[0],
               (cufftDoubleComplex *)&(*gpu_y1)[0], 1);
  regseqRT_coderGPU_kernel7<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_y1);
  regseqRT_coderGPU_kernel8<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_y1,
    *gpu_X);
  regseqRT_coderGPU_kernel9<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_idx);
  regseqRT_coderGPU_kernel10<<<dim3(1U, 1U, 1U), dim3(512U, 1U, 1U)>>>(*gpu_idx,
    *gpu_X, *gpu_ex);

  /*  find column maximums */
  absx11 = 0;
  cudaMemcpy(&ex[0], gpu_ex, 8192ULL, cudaMemcpyDeviceToHost);
  ex_re = ex[0].re;
  ex_im = ex[0].im;
  for (absx31 = 0; absx31 < 511; absx31++) {
    if (rtIsNaN(ex[absx31 + 1].re) || rtIsNaN(ex[absx31 + 1].im)) {
      c_locationsInSourceMapToDestina = false;
    } else if (rtIsNaN(ex_re) || rtIsNaN(ex_im)) {
      c_locationsInSourceMapToDestina = true;
    } else {
      if ((fabs(ex_re) > 8.9884656743115785E+307) || (fabs(ex_im) >
           8.9884656743115785E+307)) {
        c_locationsInSourceMapToDestina = true;
      } else {
        c_locationsInSourceMapToDestina = false;
      }

      if ((fabs(ex[absx31 + 1].re) > 8.9884656743115785E+307) || (fabs(ex[absx31
            + 1].im) > 8.9884656743115785E+307)) {
        SCALEB = true;
      } else {
        SCALEB = false;
      }

      if (c_locationsInSourceMapToDestina || SCALEB) {
        x = rt_hypotd_snf(ex_re / 2.0, ex_im / 2.0);
        br = rt_hypotd_snf(ex[absx31 + 1].re / 2.0, ex[absx31 + 1].im / 2.0);
      } else {
        x = rt_hypotd_snf(ex_re, ex_im);
        br = rt_hypotd_snf(ex[absx31 + 1].re, ex[absx31 + 1].im);
      }

      if (x == br) {
        absar = fabs(ex_re);
        absai = fabs(ex_im);
        absbr = fabs(ex[absx31 + 1].re);
        absbi = fabs(ex[absx31 + 1].im);
        if (absar > absai) {
          Ma = absar;
          absar = absai;
        } else {
          Ma = absai;
        }

        if (absbr > absbi) {
          absai = absbr;
          absbr = absbi;
        } else {
          absai = absbi;
        }

        if (Ma > absai) {
          if (absar < absbr) {
            x = Ma - absai;
            br = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + absai / 2.0) * (absbr
              - absar);
          } else {
            x = Ma;
            br = absai;
          }
        } else if (Ma < absai) {
          if (absar > absbr) {
            br = absai - Ma;
            x = (absar / 2.0 + absbr / 2.0) / (Ma / 2.0 + absai / 2.0) * (absar
              - absbr);
          } else {
            x = Ma;
            br = absai;
          }
        } else {
          x = absar;
          br = absbr;
        }

        if (x == br) {
          x = rt_atan2d_snf(ex_im, ex_re);
          br = rt_atan2d_snf(ex[absx31 + 1].im, ex[absx31 + 1].re);
          if (x == br) {
            br = ex[absx31 + 1].re;
            absbi = ex[absx31 + 1].im;
            if (x > 0.78539816339744828) {
              if (x > 2.3561944901923448) {
                x = -ex_im;
                br = -absbi;
              } else {
                x = -ex_re;
                br = -br;
              }
            } else if (x > -0.78539816339744828) {
              x = ex_im;
              br = absbi;
            } else if (x > -2.3561944901923448) {
              x = ex_re;
            } else {
              x = -ex_im;
              br = -absbi;
            }

            if (x == br) {
              x = 0.0;
              br = 0.0;
            }
          }
        }
      }

      c_locationsInSourceMapToDestina = (x < br);
    }

    if (c_locationsInSourceMapToDestina) {
      ex_re = ex[absx31 + 1].re;
      ex_im = ex[absx31 + 1].im;
      absx11 = absx31 + 1;
    }
  }

  if (absx11 + 1 > 256) {
    itmp = absx11 - 512;
  } else {
    itmp = absx11;
  }

  cudaMemcpy(gpu_absx11, &absx11, 4ULL, cudaMemcpyHostToDevice);
  regseqRT_coderGPU_kernel11<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>(*gpu_idx,
    gpu_absx11);
  *tx = itmp;
  cudaMemcpy(&absx11, gpu_absx11, 4ULL, cudaMemcpyDeviceToHost);
  *ty = absx11;
  regseqRT_coderGPU_kernel12<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>(gpu_absx11,
    itmp, *gpu_NonFillOutputLocY);
  regseqRT_coderGPU_kernel13<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
    (*gpu_NonFillOutputLocY, *gpu_z1);
  z1_dirtyOnGpu = true;
  c_locationsInSourceMapToDestina = true;
  absx11 = 0;
  exitg1 = false;
  while ((!exitg1) && (absx11 < 2)) {
    if (z1_dirtyOnGpu) {
      cudaMemcpy(&z1[0], gpu_z1, 16ULL, cudaMemcpyDeviceToHost);
      z1_dirtyOnGpu = false;
    }

    if (!(z1[absx11] == 0.0)) {
      c_locationsInSourceMapToDestina = false;
      exitg1 = true;
    } else {
      absx11++;
    }
  }

  if (c_locationsInSourceMapToDestina) {
    regseqRT_coderGPU_kernel26<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (regFrame);
    regseqRT_coderGPU_kernel27<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_NonFillOutputLocY, b_gpu_NonFillOutputLocY, c_gpu_NonFillOutputLocY);
    regseqRT_coderGPU_kernel28<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (b_gpu_NonFillOutputLocY, c_gpu_NonFillOutputLocY, *gpu_VWorld,
       *gpu_UWorld);
    cudaMemcpy(&UWorld[0], gpu_UWorld, 4ULL, cudaMemcpyDeviceToHost);
    c_intrinsicSourceBoundingRectan[0] = UWorld[0];
    c_intrinsicSourceBoundingRectan[1] = UWorld[1];
    c_intrinsicSourceBoundingRectan[2] = UWorld[1];
    c_intrinsicSourceBoundingRectan[3] = UWorld[0];
    cudaMemcpy(&VWorld[0], gpu_VWorld, 4ULL, cudaMemcpyDeviceToHost);
    d_intrinsicSourceBoundingRectan[0] = VWorld[0];
    d_intrinsicSourceBoundingRectan[1] = VWorld[0];
    d_intrinsicSourceBoundingRectan[2] = VWorld[1];
    d_intrinsicSourceBoundingRectan[3] = VWorld[1];
    c_locationsInSourceMapToDestina = false;
    absx11 = 0;
    exitg1 = false;
    while ((!exitg1) && (absx11 < 4)) {
      if ((c_intrinsicSourceBoundingRectan[absx11] >= 0.5) &&
          (c_intrinsicSourceBoundingRectan[absx11] <= 512.5) &&
          (d_intrinsicSourceBoundingRectan[absx11] >= 0.5) &&
          (d_intrinsicSourceBoundingRectan[absx11] <= 512.5)) {
        c_locationsInSourceMapToDestina = true;
        exitg1 = true;
      } else {
        absx11++;
      }
    }

    if (c_locationsInSourceMapToDestina) {
      absx11 = UWorld[0];
      if (1 > UWorld[0]) {
        absx11 = 1;
      }

      UWorldClippedToBounds[0] = static_cast<int16_T>(absx11);
      absx11 = UWorld[1];
      if (512 < UWorld[1]) {
        absx11 = 512;
      }

      UWorldClippedToBounds[1] = static_cast<int16_T>(absx11);
      absx11 = VWorld[0];
      if (1 > VWorld[0]) {
        absx11 = 1;
      }

      UWorld[0] = static_cast<int16_T>(absx11);
      absx11 = VWorld[1];
      if (512 < VWorld[1]) {
        absx11 = 512;
      }

      UWorld[1] = static_cast<int16_T>(absx11);
      regseqRT_coderGPU_kernel29<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (*gpu_NonFillOutputLocY, b_gpu_NonFillOutputLocY,
         c_gpu_NonFillOutputLocY);
      cudaMemcpy(gpu_UWorld, &UWorld[0], 4ULL, cudaMemcpyHostToDevice);
      cudaMemcpy(gpu_UWorldClippedToBounds, &UWorldClippedToBounds[0], 4ULL,
                 cudaMemcpyHostToDevice);
      regseqRT_coderGPU_kernel30<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
        (b_gpu_NonFillOutputLocY, *gpu_UWorld, c_gpu_NonFillOutputLocY,
         *gpu_UWorldClippedToBounds, *gpu_outputC, *gpu_z1,
         *gpu_containedSubscripts, *gpu_NonFillOutputLocY, *gpu_VWorld);
      containedSubscripts_dirtyOnGpu = true;
      outputC_dirtyOnGpu = true;
      z1_dirtyOnGpu = true;
      absx11 = 0;
      for (absx31 = 0; absx31 < 2; absx31++) {
        if (containedSubscripts_dirtyOnGpu) {
          cudaMemcpy(&containedSubscripts[0], gpu_containedSubscripts, 2ULL,
                     cudaMemcpyDeviceToHost);
          containedSubscripts_dirtyOnGpu = false;
        }

        if (containedSubscripts[absx31]) {
          absx11++;
        }
      }

      tmp_size[0] = 1;
      tmp_size[1] = absx11;
      tmp_size_dirtyOnCpu = true;
      itmp = 0;
      for (absx31 = 0; absx31 < 2; absx31++) {
        if (containedSubscripts_dirtyOnGpu) {
          cudaMemcpy(&containedSubscripts[0], gpu_containedSubscripts, 2ULL,
                     cudaMemcpyDeviceToHost);
          containedSubscripts_dirtyOnGpu = false;
        }

        if (containedSubscripts[absx31]) {
          tmp_data[itmp] = static_cast<int8_T>(absx31 + 1);
          tmp_data_dirtyOnCpu = true;
          itmp++;
        }
      }

      cInit_size[1] = absx11;
      validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((tmp_size[1]
        - 1) + 1LL), &grid, &block, 1024U, 65535U);
      if (validLaunchParams) {
        if (tmp_data_dirtyOnCpu) {
          cudaMemcpy(gpu_tmp_data, &tmp_data[0], tmp_size[0] * tmp_size[1] *
                     sizeof(int8_T), cudaMemcpyHostToDevice);
          tmp_data_dirtyOnCpu = false;
        }

        cudaMemcpy(gpu_tmp_size, &tmp_size[0], 8ULL, cudaMemcpyHostToDevice);
        tmp_size_dirtyOnCpu = false;
        regseqRT_coderGPU_kernel31<<<grid, block>>>(*gpu_VWorld, *gpu_tmp_data, *
          gpu_tmp_size, *gpu_rPix_data);
        rPix_data_dirtyOnGpu = true;
      }

      rInit_size[1] = tmp_size[1];
      b_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((tmp_size
        [1] - 1) + 1LL), &b_grid, &b_block, 1024U, 65535U);
      if (b_validLaunchParams) {
        if (tmp_data_dirtyOnCpu) {
          cudaMemcpy(gpu_tmp_data, &tmp_data[0], tmp_size[0] * tmp_size[1] *
                     sizeof(int8_T), cudaMemcpyHostToDevice);
        }

        if (tmp_size_dirtyOnCpu) {
          cudaMemcpy(gpu_tmp_size, &tmp_size[0], 8ULL, cudaMemcpyHostToDevice);
        }

        regseqRT_coderGPU_kernel32<<<b_grid, b_block>>>(*gpu_NonFillOutputLocY, *
          gpu_tmp_data, *gpu_tmp_size, *gpu_rInit_data);
      }

      absx11 = cInit_size[1];
      c_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((absx11 -
        1) + 1LL), &c_grid, &c_block, 1024U, 65535U);
      if (c_validLaunchParams) {
        cudaMemcpy(gpu_absx11, &absx11, 4ULL, cudaMemcpyHostToDevice);
        regseqRT_coderGPU_kernel33<<<c_grid, c_block>>>(gpu_absx11,
          *gpu_rPix_data);
        rPix_data_dirtyOnGpu = true;
      }

      cPix_size[1] = static_cast<int8_T>(cInit_size[1]);
      absx11 = static_cast<int8_T>(cInit_size[1]);
      d_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((absx11 -
        1) + 1LL), &d_grid, &d_block, 1024U, 65535U);
      if (d_validLaunchParams) {
        cudaMemcpy(gpu_absx11, &absx11, 4ULL, cudaMemcpyHostToDevice);
        regseqRT_coderGPU_kernel34<<<d_grid, d_block>>>(*gpu_rPix_data,
          gpu_absx11, *gpu_cPix_data);
        cPix_data_dirtyOnGpu = true;
      }

      absx11 = rInit_size[1];
      e_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((absx11 -
        1) + 1LL), &e_grid, &e_block, 1024U, 65535U);
      if (e_validLaunchParams) {
        cudaMemcpy(gpu_absx11, &absx11, 4ULL, cudaMemcpyHostToDevice);
        regseqRT_coderGPU_kernel35<<<e_grid, e_block>>>(gpu_absx11,
          *gpu_rInit_data);
      }

      rPix_size[1] = static_cast<int8_T>(rInit_size[1]);
      absx11 = static_cast<int8_T>(rInit_size[1]);
      f_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>((absx11 -
        1) + 1LL), &f_grid, &f_block, 1024U, 65535U);
      if (f_validLaunchParams) {
        cudaMemcpy(gpu_absx11, &absx11, 4ULL, cudaMemcpyHostToDevice);
        regseqRT_coderGPU_kernel36<<<f_grid, f_block>>>(*gpu_rInit_data,
          gpu_absx11, *gpu_rPix_data);
        rPix_data_dirtyOnGpu = true;
      }

      itmp = 0;
      for (absx31 = 0; absx31 < 2; absx31++) {
        if (containedSubscripts_dirtyOnGpu) {
          cudaMemcpy(&containedSubscripts[0], gpu_containedSubscripts, 2ULL,
                     cudaMemcpyDeviceToHost);
          containedSubscripts_dirtyOnGpu = false;
        }

        if (containedSubscripts[absx31]) {
          if (outputC_dirtyOnGpu) {
            cudaMemcpy(&outputC[0], gpu_outputC, 16ULL, cudaMemcpyDeviceToHost);
            outputC_dirtyOnGpu = false;
          }

          if (cPix_data_dirtyOnGpu) {
            cudaMemcpy(&cPix_data[0], gpu_cPix_data, cPix_size[1] * sizeof
                       (real_T), cudaMemcpyDeviceToHost);
            cPix_data_dirtyOnGpu = false;
          }

          outputC[absx31] = cPix_data[itmp];
          itmp++;
        }
      }

      itmp = 0;
      for (absx31 = 0; absx31 < 2; absx31++) {
        if (containedSubscripts_dirtyOnGpu) {
          cudaMemcpy(&containedSubscripts[0], gpu_containedSubscripts, 2ULL,
                     cudaMemcpyDeviceToHost);
          containedSubscripts_dirtyOnGpu = false;
        }

        if (containedSubscripts[absx31]) {
          if (z1_dirtyOnGpu) {
            cudaMemcpy(&z1[0], gpu_z1, 16ULL, cudaMemcpyDeviceToHost);
            z1_dirtyOnGpu = false;
          }

          if (rPix_data_dirtyOnGpu) {
            cudaMemcpy(&rPix_data[0], gpu_rPix_data, rPix_size[1] * sizeof
                       (real_T), cudaMemcpyDeviceToHost);
            rPix_data_dirtyOnGpu = false;
          }

          z1[absx31] = rPix_data[itmp];
          itmp++;
        }
      }

      if (z1_dirtyOnGpu) {
        cudaMemcpy(&z1[0], gpu_z1, 16ULL, cudaMemcpyDeviceToHost);
      }

      if (z1[0] > z1[1]) {
        itmp = 0;
      } else {
        itmp = static_cast<int32_T>(z1[0]) - 1;
      }

      if (outputC_dirtyOnGpu) {
        cudaMemcpy(&outputC[0], gpu_outputC, 16ULL, cudaMemcpyDeviceToHost);
      }

      if (outputC[0] > outputC[1]) {
        absx11 = 0;
      } else {
        absx11 = static_cast<int32_T>(outputC[0]) - 1;
      }

      absx31 = UWorld[0] - 1;
      p1 = UWorld[1] - 1;
      p2 = UWorldClippedToBounds[0] - 1;
      p3 = UWorldClippedToBounds[1] - 1;
      g_validLaunchParams = mwGetLaunchParameters(static_cast<real_T>(((p1 -
        absx31) + 1LL) * ((p3 - p2) + 1LL)), &g_grid, &g_block, 1024U, 65535U);
      if (g_validLaunchParams) {
        cudaMemcpy(gpu_absx11, &absx11, 4ULL, cudaMemcpyHostToDevice);
        regseqRT_coderGPU_kernel37<<<g_grid, g_block>>>(currentFrame, p2,
          gpu_absx11, itmp, absx31, p1, p3, regFrame);
      }
    }
  } else {
    cudaMemcpy(gpu_iv, (void *)&iv[0], 3ULL, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_iv1, (void *)&iv1[0], 3ULL, cudaMemcpyHostToDevice);
    regseqRT_coderGPU_kernel14<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>(*gpu_iv,
      *gpu_iv1, *gpu_A);
    regseqRT_coderGPU_kernel15<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
      (*gpu_NonFillOutputLocY, *gpu_A);
    regseqRT_coderGPU_kernel16<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>(*gpu_A);
    regseqRT_coderGPU_kernel17<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*gpu_dstYWorld, *gpu_dstXWorld);
    regseqRT_coderGPU_kernel18<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>(*gpu_A, *
      gpu_x);
    p1 = 0;
    p2 = 3;
    p3 = 6;
    cudaMemcpy(&A[0], gpu_A, 18ULL, cudaMemcpyDeviceToHost);
    absx11 = A[0];
    absx31 = static_cast<int32_T>(fabs(static_cast<real_T>(A[2])));
    if ((A[1] > absx11) && (1 > absx31)) {
      p1 = 3;
      p2 = 0;
      regseqRT_coderGPU_kernel20<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>(*gpu_A,
        *gpu_x);
    } else {
      if (absx31 > absx11) {
        p1 = 6;
        p3 = 0;
        regseqRT_coderGPU_kernel19<<<dim3(1U, 1U, 1U), dim3(32U, 1U, 1U)>>>
          (*gpu_A, *gpu_x);
      }
    }

    cudaMemcpy(&b_x[0], gpu_x, 72ULL, cudaMemcpyDeviceToHost);
    absbi = b_x[1] / b_x[0];
    b_x[1] /= b_x[0];
    absai = b_x[2] / b_x[0];
    b_x[2] /= b_x[0];
    b_x[4] -= absbi * b_x[3];
    b_x[5] -= absai * b_x[3];
    b_x[7] -= absbi * b_x[6];
    b_x[8] -= absai * b_x[6];
    if (fabs(b_x[5]) > fabs(b_x[4])) {
      itmp = p2;
      p2 = p3;
      p3 = itmp;
      b_x[1] = absai;
      b_x[2] = absbi;
      absbi = b_x[4];
      b_x[4] = b_x[5];
      b_x[5] = absbi;
      absbi = b_x[7];
      b_x[7] = b_x[8];
      b_x[8] = absbi;
    }

    absbi = b_x[5] / b_x[4];
    b_x[5] /= b_x[4];
    b_x[8] -= absbi * b_x[7];
    absbi = (b_x[5] * b_x[1] - b_x[2]) / b_x[8];
    absai = -(b_x[1] + b_x[7] * absbi) / b_x[4];
    tinv[p1] = ((1.0 - b_x[3] * absai) - b_x[6] * absbi) / b_x[0];
    tinv[p1 + 1] = absai;
    tinv[p1 + 2] = absbi;
    absbi = -b_x[5] / b_x[8];
    absai = (1.0 - b_x[7] * absbi) / b_x[4];
    tinv[p2] = -(b_x[3] * absai + b_x[6] * absbi) / b_x[0];
    tinv[p2 + 1] = absai;
    tinv[p2 + 2] = absbi;
    absbi = 1.0 / b_x[8];
    absai = -b_x[7] * absbi / b_x[4];
    tinv[p3] = -(b_x[3] * absai + b_x[6] * absbi) / b_x[0];
    tinv[p3 + 1] = absai;
    tinv[p3 + 2] = absbi;
    regseqRT_coderGPU_kernel21<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>(tinv
      [5], tinv[4], tinv[3], tinv[2], tinv[1], tinv[0], *gpu_dstYWorld,
      *gpu_dstXWorld, *gpu_srcXIntrinsic);
    regseqRT_coderGPU_kernel22<<<dim3(4U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*gpu_inputImagePadded);
    regseqRT_coderGPU_kernel23<<<dim3(3U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*gpu_inputImagePadded);
    regseqRT_coderGPU_kernel24<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (currentFrame, *gpu_inputImagePadded);
    regseqRT_coderGPU_kernel25<<<dim3(512U, 1U, 1U), dim3(512U, 1U, 1U)>>>
      (*gpu_inputImagePadded, *gpu_dstXWorld, *gpu_srcXIntrinsic, regFrame);
  }

  cufftEnsureDestruction();
  cudaFree(*gpu_dstYWorld);
  cudaFree(*gpu_y1);
  cudaFree(*gpu_X);
  cudaFree(*gpu_f);
  cudaFree(*gpu_idx);
  cudaFree(*gpu_ex);
  cudaFree(gpu_absx11);
  cudaFree(*gpu_NonFillOutputLocY);
  cudaFree(*gpu_z1);
  cudaFree(*gpu_iv);
  cudaFree(*gpu_iv1);
  cudaFree(*gpu_A);
  cudaFree(*gpu_x);
  cudaFree(*gpu_dstXWorld);
  cudaFree(*gpu_inputImagePadded);
  cudaFree(*gpu_srcXIntrinsic);
  cudaFree(b_gpu_NonFillOutputLocY);
  cudaFree(c_gpu_NonFillOutputLocY);
  cudaFree(*gpu_UWorld);
  cudaFree(*gpu_VWorld);
  cudaFree(*gpu_UWorldClippedToBounds);
  cudaFree(*gpu_containedSubscripts);
  cudaFree(*gpu_tmp_size);
  cudaFree(*gpu_tmp_data);
  cudaFree(*gpu_rPix_data);
  cudaFree(*gpu_rInit_data);
  cudaFree(*gpu_outputC);
  cudaFree(*gpu_cPix_data);
}

/* End of code generation (regseqRT_coderGPU.cu) */
