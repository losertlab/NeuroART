/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder.c
 *
 * Code generation for function 'computeDFF_new_coder'
 *
 */

/* Include files */
#include "computeDFF_new_coder.h"
#include "combineVectorElements.h"
#include "computeDFF_new_coder_data.h"
#include "computeDFF_new_coder_emxutil.h"
#include "computeDFF_new_coder_initialize.h"
#include "diff.h"
#include "find.h"
#include "imcrop.h"
#include "imgpolarcoord_coder.h"
#include "mean.h"
#include "nanmean.h"
#include "poly2mask.h"
#include "rt_nonfinite.h"
#include "slideWinSub.h"
#include "smooth.h"
#include "smooth2.h"
#include "sparse.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void computeDFF_new_coder(const emxArray_uint16_T *RegImg, double frameRate,
  const double pos_lst_data[], const int pos_lst_size[2], double r_pixels,
  double winsize, double percentBaselineSub, double neuropilSubPercent,
  emxArray_real_T *norm_meanIMG, coder_internal_sparse roiBW2_sp_data[], int
  roiBW2_sp_size[1], coder_internal_sparse_1 npBWout_sp_data[], int
  npBWout_sp_size[1], struct0_T *DFF, emxArray_real_T *normF, emxArray_real_T
  *npSubFluoSmooth, double xcRaw_data[], int xcRaw_size[1], double ycRaw_data[],
  int ycRaw_size[1], double *minNpSubFluo, double *maxAdjF)
{
  emxArray_real_T *meanIMG;
  double expectedNeuropilRadiusPix;
  double a;
  int n;
  int idx;
  double ex;
  int k;
  boolean_T exitg1;
  int i;
  double b_ex;
  double T;
  int loop_ub;
  emxArray_boolean_T *roiBWout;
  emxArray_int8_T *roiBW2;
  emxArray_boolean_T *npBWout;
  emxArray_real_T *rawFluo;
  emxArray_real_T *npFluo;
  emxArray_real_T *imgCrop;
  emxArray_real_T *tmpNeuron;
  emxArray_boolean_T *roiBWin;
  emxArray_int32_T *ii;
  emxArray_real_T *varargin_1;
  emxArray_real_T *b_i;
  emxArray_int32_T *r;
  emxArray_real_T *b_imgCrop;
  emxArray_int32_T *b_roiBW2;
  emxArray_boolean_T *c_roiBW2;
  emxArray_real_T *y11;
  emxArray_boolean_T *x2;
  int pp;
  int pos_lst_tmp;
  double pos_lst[4];
  int i1;
  emxArray_int32_T *oLapRoiY;
  emxArray_int32_T *jj;
  emxArray_int32_T *oLapRoiX;
  int b_loop_ub;
  int cc;
  double ROIIn[720];
  double RingPks[360];
  double ROIOut[720];
  double b_y1[360];
  int tmpNeuron_size[1];
  emxArray_real_T tmpNeuron_data;
  double b_tmpNeuron_data[256];
  double NeuropilIn[720];
  double smRoiBoundaries[1080];
  double NeuropilOut[720];
  double smNpBoundaries[1080];
  emxArray_boolean_T *b_roiBWout;
  double d;
  double d1;
  int i2;
  emxArray_real_T *tmp;
  double x11_data[361];
  double y11_data[361];
  double b_x2[360];
  double y2[360];
  emxArray_real_T c_tmpNeuron_data;
  double x21_data[361];
  double y21_data[361];
  int i_size[1];
  emxArray_boolean_T i_data;
  boolean_T b_i_data[255];
  static double tmp_data[7500];
  int tmp_size[2];
  signed char i3;
  emxArray_real_T *b_normF;
  int normF_size[2];
  static double normF_data[7511];
  emxArray_real_T *b_npSubFluoSmooth;
  if (isInitialized_computeDFF_new_coder == false) {
    computeDFF_new_coder_initialize();
  }

  emxInit_real_T(&meanIMG, 2);

  /*  remove calls to exist() to enable mex compilation */
  /*  narginchk(8,10) */
  /*  if ~exist('winsize','var'); winsize = 300; end % default to 300 frame window */
  /*  if ~exist('percentBaselineSub','var'); percentBaselineSub = 50; end % default to 50% baseline subtraction */
  /*  if ~exist('neuropilSubPercent','var'); neuropilSubPercent = 70; end % default to 70% neuropil subtraction */
  /*  total_start = tic; */
  /*  Dan experimental parameters */
  /*  cd(expDir) % expDir is the current working directory */
  /*  01/14/22 removed this section for speed up, also removed */
  /*  expDir,exptName,and imTemplate from the list of input arguments */
  /*  movID = ['DFF_' exptName]; */
  /*  datafile = [fullfile(expDir,movID) '.mat']; */
  /*  if ~exist(datafile,'file') */
  /*      disp(['Creating data file', datafile]); */
  /*      save(datafile,'movID','-v7.3');cl */
  /*  end */
  /*   */
  /*  warning('off','stats:pvaluedw:ExactUnavailable') */
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /* DW05062015 - added to make the ROI ring more flexible and dependent on mag */
  /* size rather than fixed */
  expectedNeuropilRadiusPix = 2.0 * r_pixels;

  /* need to remove outer boundary of neurons */
  a = neuropilSubPercent / 100.0;

  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  mean(RegImg, meanIMG);

  /*  Mean image for cell center clicking */
  n = meanIMG->size[0] * meanIMG->size[1];
  if (meanIMG->size[0] * meanIMG->size[1] <= 2) {
    if (meanIMG->size[0] * meanIMG->size[1] == 1) {
      ex = meanIMG->data[0];
    } else if ((meanIMG->data[0] > meanIMG->data[1]) || (rtIsNaN(meanIMG->data[0])
                && (!rtIsNaN(meanIMG->data[1])))) {
      ex = meanIMG->data[1];
    } else {
      ex = meanIMG->data[0];
    }
  } else {
    if (!rtIsNaN(meanIMG->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= meanIMG->size[0] * meanIMG->size[1])) {
        if (!rtIsNaN(meanIMG->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      ex = meanIMG->data[0];
    } else {
      ex = meanIMG->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        if (ex > meanIMG->data[k - 1]) {
          ex = meanIMG->data[k - 1];
        }
      }
    }
  }

  n = meanIMG->size[0] * meanIMG->size[1];
  if (meanIMG->size[0] * meanIMG->size[1] <= 2) {
    if (meanIMG->size[0] * meanIMG->size[1] == 1) {
      b_ex = meanIMG->data[0];
    } else if ((meanIMG->data[0] < meanIMG->data[1]) || (rtIsNaN(meanIMG->data[0])
                && (!rtIsNaN(meanIMG->data[1])))) {
      b_ex = meanIMG->data[1];
    } else {
      b_ex = meanIMG->data[0];
    }
  } else {
    if (!rtIsNaN(meanIMG->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= meanIMG->size[0] * meanIMG->size[1])) {
        if (!rtIsNaN(meanIMG->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      b_ex = meanIMG->data[0];
    } else {
      b_ex = meanIMG->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        if (b_ex < meanIMG->data[k - 1]) {
          b_ex = meanIMG->data[k - 1];
        }
      }
    }
  }

  n = meanIMG->size[0] * meanIMG->size[1];
  if (meanIMG->size[0] * meanIMG->size[1] <= 2) {
    if (meanIMG->size[0] * meanIMG->size[1] == 1) {
      T = meanIMG->data[0];
    } else if ((meanIMG->data[0] > meanIMG->data[1]) || (rtIsNaN(meanIMG->data[0])
                && (!rtIsNaN(meanIMG->data[1])))) {
      T = meanIMG->data[1];
    } else {
      T = meanIMG->data[0];
    }
  } else {
    if (!rtIsNaN(meanIMG->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= meanIMG->size[0] * meanIMG->size[1])) {
        if (!rtIsNaN(meanIMG->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      T = meanIMG->data[0];
    } else {
      T = meanIMG->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        if (T > meanIMG->data[k - 1]) {
          T = meanIMG->data[k - 1];
        }
      }
    }
  }

  i = norm_meanIMG->size[0] * norm_meanIMG->size[1];
  norm_meanIMG->size[0] = meanIMG->size[0];
  norm_meanIMG->size[1] = meanIMG->size[1];
  emxEnsureCapacity_real_T(norm_meanIMG, i);
  T = b_ex - T;
  loop_ub = meanIMG->size[0] * meanIMG->size[1];
  for (i = 0; i < loop_ub; i++) {
    norm_meanIMG->data[i] = (meanIMG->data[i] - ex) / T;
  }

  /*  save(datafile,'meanIMG','norm_meanIMG','imTemplate','-append'); % remove for speed */
  /*  Select neuron centers - FRAUNHOFER -- May have to make xc/yc an input to computeDFF given the Platform layout */
  loop_ub = pos_lst_size[0];
  xcRaw_size[0] = pos_lst_size[0];
  for (i = 0; i < loop_ub; i++) {
    xcRaw_data[i] = pos_lst_data[i + pos_lst_size[0]];
  }

  loop_ub = pos_lst_size[0];
  ycRaw_size[0] = pos_lst_size[0];
  if (0 <= loop_ub - 1) {
    memcpy(&ycRaw_data[0], &pos_lst_data[0], loop_ub * sizeof(double));
  }

  emxInit_boolean_T(&roiBWout, 3);

  /*  PREALLOCATE */
  /*  traceExtract_start = tic; */
  /*  number of cells */
  /*  pcimg1 = cell (nc , 16 , 360 ); */
  /*  pcimg = coder.nullcopy(pcimg1); */
  /*  imgCrop = cell ( nc , 32, 32 ); */
  /*  imgCrop = coder.nullcopy(imgCrop1); */
  /*  imgCropNorm1 = cell (nc, 32 , 32 ); */
  /*  imgCropNorm = coder.nullcopy(imgCropNorm1); */
  /*  roiBoundaries = cell ( nc , 360 , 3 ); */
  /*  smRoiBoundaries = cell ( nc , 360 , 3 ); */
  /*  ROIxvOut = cell ( nc , 360 , 1 ); */
  /*  ROIyvOut = cell ( nc , 360 , 1 ); */
  /*  ROIxvIn = cell ( nc , 360 , 1 ); */
  /*  ROIyvIn = cell ( nc , 360 , 1 ); */
  i = roiBWout->size[0] * roiBWout->size[1] * roiBWout->size[2];
  roiBWout->size[0] = meanIMG->size[0];
  roiBWout->size[1] = meanIMG->size[1];
  roiBWout->size[2] = pos_lst_size[0];
  emxEnsureCapacity_boolean_T(roiBWout, i);
  loop_ub = meanIMG->size[0] * meanIMG->size[1] * pos_lst_size[0];
  for (i = 0; i < loop_ub; i++) {
    roiBWout->data[i] = false;
  }

  emxInit_int8_T(&roiBW2, 3);

  /*  cell ( nc , size(norm_meanIMG,1) , size(norm_meanIMG,2)); */
  i = roiBW2->size[0] * roiBW2->size[1] * roiBW2->size[2];
  roiBW2->size[0] = meanIMG->size[0];
  roiBW2->size[1] = meanIMG->size[1];
  roiBW2->size[2] = pos_lst_size[0];
  emxEnsureCapacity_int8_T(roiBW2, i);
  loop_ub = meanIMG->size[0] * meanIMG->size[1] * pos_lst_size[0];
  for (i = 0; i < loop_ub; i++) {
    roiBW2->data[i] = 0;
  }

  emxInit_boolean_T(&npBWout, 3);

  /* neuropil */
  /*  npBoundaries = cell ( nc , 360 , 3 ); */
  /*  smNpBoundaries = cell ( nc , 360 , 3 ); */
  /*  NPxvOut = cell ( nc , 360 , 1 ); */
  /*  NPyvOut = cell ( nc , 360 , 1 ); */
  /*  NPxvIn = cell ( nc , 360 , 1 ); */
  /*  NPyvIn = cell ( nc , 360 , 1 ); */
  i = npBWout->size[0] * npBWout->size[1] * npBWout->size[2];
  npBWout->size[0] = meanIMG->size[0];
  npBWout->size[1] = meanIMG->size[1];
  npBWout->size[2] = pos_lst_size[0];
  emxEnsureCapacity_boolean_T(npBWout, i);
  loop_ub = meanIMG->size[0] * meanIMG->size[1] * pos_lst_size[0];
  for (i = 0; i < loop_ub; i++) {
    npBWout->data[i] = false;
  }

  emxInit_real_T(&rawFluo, 2);

  /*  PREALLOCATE FLUO AND NPFLUO MATRICES */
  i = rawFluo->size[0] * rawFluo->size[1];
  rawFluo->size[0] = RegImg->size[2];
  rawFluo->size[1] = pos_lst_size[0];
  emxEnsureCapacity_real_T(rawFluo, i);
  loop_ub = RegImg->size[2] * pos_lst_size[0];
  for (i = 0; i < loop_ub; i++) {
    rawFluo->data[i] = 0.0;
  }

  emxInit_real_T(&npFluo, 2);
  i = npFluo->size[0] * npFluo->size[1];
  npFluo->size[0] = RegImg->size[2];
  npFluo->size[1] = pos_lst_size[0];
  emxEnsureCapacity_real_T(npFluo, i);
  loop_ub = RegImg->size[2] * pos_lst_size[0];
  for (i = 0; i < loop_ub; i++) {
    npFluo->data[i] = 0.0;
  }

  i = npSubFluoSmooth->size[0] * npSubFluoSmooth->size[1];
  npSubFluoSmooth->size[0] = RegImg->size[2];
  npSubFluoSmooth->size[1] = pos_lst_size[0];
  emxEnsureCapacity_real_T(npSubFluoSmooth, i);
  loop_ub = RegImg->size[2] * pos_lst_size[0];
  for (i = 0; i < loop_ub; i++) {
    npSubFluoSmooth->data[i] = 0.0;
  }

  /*  FIND THE BOUNDARIES OF CLICKED NEURONS */
  i = pos_lst_size[0] - 1;
  emxInit_real_T(&imgCrop, 2);
  emxInit_real_T(&tmpNeuron, 2);
  emxInit_boolean_T(&roiBWin, 2);
  emxInit_int32_T(&ii, 1);
  emxInit_real_T(&varargin_1, 1);
  emxInit_real_T(&b_i, 1);
  emxInit_int32_T(&r, 1);
  emxInit_real_T(&b_imgCrop, 2);
  emxInit_int32_T(&b_roiBW2, 2);
  emxInit_boolean_T(&c_roiBW2, 1);
  emxInit_real_T(&y11, 1);
  emxInit_boolean_T(&x2, 2);
  for (pp = 0; pp <= i; pp++) {
    pos_lst_tmp = pp + pos_lst_size[0];
    pos_lst[0] = pos_lst_data[pos_lst_tmp] - 15.0;
    pos_lst[1] = pos_lst_data[pp] - 15.0;
    pos_lst[2] = 31.0;
    pos_lst[3] = 31.0;
    imcrop(meanIMG, pos_lst, imgCrop);
    n = imgCrop->size[0] * imgCrop->size[1];
    if (imgCrop->size[0] * imgCrop->size[1] <= 2) {
      if (imgCrop->size[0] * imgCrop->size[1] == 1) {
        ex = imgCrop->data[0];
      } else if ((imgCrop->data[0] > imgCrop->data[1]) || (rtIsNaN(imgCrop->
                   data[0]) && (!rtIsNaN(imgCrop->data[1])))) {
        ex = imgCrop->data[1];
      } else {
        ex = imgCrop->data[0];
      }
    } else {
      if (!rtIsNaN(imgCrop->data[0])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= imgCrop->size[0] * imgCrop->size[1])) {
          if (!rtIsNaN(imgCrop->data[k - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        ex = imgCrop->data[0];
      } else {
        ex = imgCrop->data[idx - 1];
        i1 = idx + 1;
        for (k = i1; k <= n; k++) {
          if (ex > imgCrop->data[k - 1]) {
            ex = imgCrop->data[k - 1];
          }
        }
      }
    }

    n = imgCrop->size[0] * imgCrop->size[1];
    if (imgCrop->size[0] * imgCrop->size[1] <= 2) {
      if (imgCrop->size[0] * imgCrop->size[1] == 1) {
        b_ex = imgCrop->data[0];
      } else if ((imgCrop->data[0] < imgCrop->data[1]) || (rtIsNaN(imgCrop->
                   data[0]) && (!rtIsNaN(imgCrop->data[1])))) {
        b_ex = imgCrop->data[1];
      } else {
        b_ex = imgCrop->data[0];
      }
    } else {
      if (!rtIsNaN(imgCrop->data[0])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= imgCrop->size[0] * imgCrop->size[1])) {
          if (!rtIsNaN(imgCrop->data[k - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        b_ex = imgCrop->data[0];
      } else {
        b_ex = imgCrop->data[idx - 1];
        i1 = idx + 1;
        for (k = i1; k <= n; k++) {
          if (b_ex < imgCrop->data[k - 1]) {
            b_ex = imgCrop->data[k - 1];
          }
        }
      }
    }

    n = imgCrop->size[0] * imgCrop->size[1];
    if (imgCrop->size[0] * imgCrop->size[1] <= 2) {
      if (imgCrop->size[0] * imgCrop->size[1] == 1) {
        T = imgCrop->data[0];
      } else if ((imgCrop->data[0] > imgCrop->data[1]) || (rtIsNaN(imgCrop->
                   data[0]) && (!rtIsNaN(imgCrop->data[1])))) {
        T = imgCrop->data[1];
      } else {
        T = imgCrop->data[0];
      }
    } else {
      if (!rtIsNaN(imgCrop->data[0])) {
        idx = 1;
      } else {
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= imgCrop->size[0] * imgCrop->size[1])) {
          if (!rtIsNaN(imgCrop->data[k - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        T = imgCrop->data[0];
      } else {
        T = imgCrop->data[idx - 1];
        i1 = idx + 1;
        for (k = i1; k <= n; k++) {
          if (T > imgCrop->data[k - 1]) {
            T = imgCrop->data[k - 1];
          }
        }
      }
    }

    T = b_ex - T;
    i1 = b_imgCrop->size[0] * b_imgCrop->size[1];
    b_imgCrop->size[0] = imgCrop->size[0];
    b_imgCrop->size[1] = imgCrop->size[1];
    emxEnsureCapacity_real_T(b_imgCrop, i1);
    loop_ub = imgCrop->size[0] * imgCrop->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_imgCrop->data[i1] = (imgCrop->data[i1] - ex) / T;
    }

    imgpolarcoord_coder(b_imgCrop, tmpNeuron);

    /*  this comes from Matlab Central Download */
    /*      imgCropNorm{pp} = (imgCrop{pp} - min(imgCrop{pp}(:)))  ./ (max(imgCrop{pp}(:)) - min(imgCrop{pp}(:))); */
    /*      pcimg{pp} = imgpolarcoord_coder(imgCropNorm{pp} );  % this comes from Matlab Central Download */
    /* reset vals to zero */
    /*      tmpNeuron = pcimg{pp}; %iterate this for each selected neuron */
    /*      roiBoundaries{pp} = [ ROIIn(:,1) ROIIn(:,2)  ROIOut(:,2) ]; % [PolarCoords (0-2Pi)    InnerRing     OuterRing] */
    loop_ub = tmpNeuron->size[0];
    b_loop_ub = tmpNeuron->size[0];
    for (cc = 0; cc < 360; cc++) {
      RingPks[cc] = 0.0;

      /*  for every direction - find the inner part of the ring */
      tmpNeuron_size[0] = loop_ub;
      for (i1 = 0; i1 < loop_ub; i1++) {
        b_tmpNeuron_data[i1] = tmpNeuron->data[i1 + tmpNeuron->size[0] * cc];
      }

      tmpNeuron_data.data = &b_tmpNeuron_data[0];
      tmpNeuron_data.size = &tmpNeuron_size[0];
      tmpNeuron_data.allocatedSize = 256;
      tmpNeuron_data.numDimensions = 1;
      tmpNeuron_data.canFreeData = false;
      diff(&tmpNeuron_data, b_i);
      i1 = varargin_1->size[0];
      varargin_1->size[0] = b_i->size[0];
      emxEnsureCapacity_real_T(varargin_1, i1);
      idx = b_i->size[0];
      for (i1 = 0; i1 < idx; i1++) {
        varargin_1->data[i1] = b_i->data[i1];
      }

      n = varargin_1->size[0];
      if (varargin_1->size[0] <= 2) {
        if (varargin_1->size[0] == 1) {
          ex = varargin_1->data[0];
        } else if ((varargin_1->data[0] > varargin_1->data[1]) || (rtIsNaN
                    (varargin_1->data[0]) && (!rtIsNaN(varargin_1->data[1])))) {
          ex = varargin_1->data[1];
        } else {
          ex = varargin_1->data[0];
        }
      } else {
        if (!rtIsNaN(varargin_1->data[0])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= varargin_1->size[0])) {
            if (!rtIsNaN(varargin_1->data[k - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (idx == 0) {
          ex = varargin_1->data[0];
        } else {
          ex = varargin_1->data[idx - 1];
          i1 = idx + 1;
          for (k = i1; k <= n; k++) {
            d = varargin_1->data[k - 1];
            if (ex > d) {
              ex = d;
            }
          }
        }
      }

      tmpNeuron_size[0] = b_loop_ub;
      for (i1 = 0; i1 < b_loop_ub; i1++) {
        b_tmpNeuron_data[i1] = tmpNeuron->data[i1 + tmpNeuron->size[0] * cc];
      }

      c_tmpNeuron_data.data = &b_tmpNeuron_data[0];
      c_tmpNeuron_data.size = &tmpNeuron_size[0];
      c_tmpNeuron_data.allocatedSize = 256;
      c_tmpNeuron_data.numDimensions = 1;
      c_tmpNeuron_data.canFreeData = false;
      diff(&c_tmpNeuron_data, b_i);
      i_size[0] = b_i->size[0];
      idx = b_i->size[0];
      for (i1 = 0; i1 < idx; i1++) {
        b_i_data[i1] = (b_i->data[i1] == ex);
      }

      i_data.data = &b_i_data[0];
      i_data.size = &i_size[0];
      i_data.allocatedSize = 255;
      i_data.numDimensions = 1;
      i_data.canFreeData = false;
      eml_find(&i_data, r);
      i1 = b_i->size[0];
      b_i->size[0] = r->size[0];
      emxEnsureCapacity_real_T(b_i, i1);
      idx = r->size[0];
      for (i1 = 0; i1 < idx; i1++) {
        b_i->data[i1] = r->data[i1];
      }

      /*  DW07122015_changed to make this more robust - seems to be working right now - continue testing */
      if (b_i->size[0] != 0) {
        if (b_i->size[0] > 1) {
          /* more than one pixel identified - grab the first one */
          i1 = (int)b_i->data[0];
          if ((i1 < r_pixels) && (i1 > 2)) {
            RingPks[cc] = i1;
          } else {
            RingPks[cc] = r_pixels;
          }
        } else {
          i1 = (int)b_i->data[0];
          if ((i1 < r_pixels) && (i1 > 2)) {
            RingPks[cc] = i1;
          } else {
            RingPks[cc] = r_pixels;
          }
        }
      } else if (cc + 1 == 1) {
        /*  if its'the first direction and no peaks are found */
        RingPks[0] = r_pixels;

        /* made this dependent on mag factor DW_02022015 */
      } else {
        RingPks[cc] = RingPks[cc - 1];
      }

      T = 0.017453292519943295 * ((double)cc + 1.0);
      ROIOut[cc] = T;
      ROIOut[cc + 360] = RingPks[cc];
      ROIIn[cc] = T;
      ROIIn[cc + 360] = RingPks[cc] - 2.0;

      /* %%%%%NEED TO INCLUDE NEUROPIL SIGNAL (SVOBODA LAB USES ~20 UM */
      /* %%%%%FROM CELL CTR EXCLUDING ROIs ALL OTHER NEURONS - NATURE */
      /* %%%%%2015) */
      NeuropilIn[cc] = T;
      NeuropilIn[cc + 360] = RingPks[cc] + 1.0;
      NeuropilOut[cc] = T;
      NeuropilOut[cc + 360] = expectedNeuropilRadiusPix;

      /* %%%%%NEED TO INCLUDE NEUROPIL SIGNAL (SVOBODA LAB USES ~20 UM */
      /* %%%%%FROM CELL CTR EXCLUDING ROIs ALL OTHER NEURONS - NATURE */
      /* %%%%%2015) */
      smRoiBoundaries[cc] = T;
    }

    smooth(*(double (*)[360])&ROIIn[360], RingPks);
    smooth(*(double (*)[360])&ROIOut[360], b_y1);

    /*  [PolarCoords (0-2pi)    InnerRing     OuterRing] */
    /* DW 11232015 - included neuropil variables */
    /*      npBoundaries{pp} = [ NeuropilIn(:,1) NeuropilIn(:,2) NeuropilOut(:,2) ]; */
    for (i1 = 0; i1 < 360; i1++) {
      smRoiBoundaries[i1 + 360] = RingPks[i1];
      smRoiBoundaries[i1 + 720] = b_y1[i1];
      smNpBoundaries[i1] = NeuropilIn[i1];
    }

    smooth(*(double (*)[360])&NeuropilIn[360], RingPks);
    smooth(*(double (*)[360])&NeuropilOut[360], b_y1);

    /*  CREATE MASKS FOR ALL CLICKED ROIS, THEN SHOW THEM -- DW 11232015 */
    /*  renamed variable for consistency */
    b_ex = pos_lst_data[pos_lst_tmp];
    ex = pos_lst_data[pp];
    for (k = 0; k < 360; k++) {
      smNpBoundaries[k + 360] = RingPks[k];
      smNpBoundaries[k + 720] = b_y1[k];
      d = cos(smRoiBoundaries[k]);
      T = smRoiBoundaries[k + 720];
      RingPks[k] = b_ex + T * d;
      d1 = sin(smRoiBoundaries[k]);
      b_y1[k] = ex + T * d1;
      T = smRoiBoundaries[k + 360];
      d = b_ex + T * d;
      b_x2[k] = d;
      d1 = ex + T * d1;
      y2[k] = d1;
    }

    /*      x1 = ROIxvOut{pp}; % added this section to enable mex compilation */
    /*      y1 = ROIyvOut{pp}; */
    /*      x2 = ROIxvIn{pp}; */
    /*      y2 = ROIyvIn{pp}; */
    if ((RingPks[359] != RingPks[0]) || (b_y1[359] != b_y1[0])) {
      /*  moved from poly2mask */
      idx = 361;
      x11_data[360] = RingPks[0];
      k = 361;
      memcpy(&x11_data[0], &RingPks[0], 360U * sizeof(double));
      memcpy(&y11_data[0], &b_y1[0], 360U * sizeof(double));
      y11_data[360] = b_y1[0];
    } else {
      idx = 360;
      k = 360;
      memcpy(&x11_data[0], &RingPks[0], 360U * sizeof(double));
      memcpy(&y11_data[0], &b_y1[0], 360U * sizeof(double));
    }

    if ((b_x2[359] != b_x2[0]) || (y2[359] != y2[0])) {
      /*  moved from poly2mask */
      n = 361;
      x21_data[360] = b_x2[0];
      cc = 361;
      memcpy(&x21_data[0], &b_x2[0], 360U * sizeof(double));
      memcpy(&y21_data[0], &y2[0], 360U * sizeof(double));
      y21_data[360] = y2[0];
    } else {
      n = 360;
      cc = 360;
      memcpy(&x21_data[0], &b_x2[0], 360U * sizeof(double));
      memcpy(&y21_data[0], &y2[0], 360U * sizeof(double));
    }

    i1 = b_i->size[0];
    b_i->size[0] = idx;
    emxEnsureCapacity_real_T(b_i, i1);
    loop_ub = idx - 1;
    for (i1 = 0; i1 <= loop_ub; i1++) {
      b_i->data[i1] = x11_data[i1];
    }

    i1 = y11->size[0];
    y11->size[0] = k;
    emxEnsureCapacity_real_T(y11, i1);
    loop_ub = k - 1;
    for (i1 = 0; i1 <= loop_ub; i1++) {
      y11->data[i1] = y11_data[i1];
    }

    poly2mask(b_i, y11, meanIMG->size[0], meanIMG->size[1], roiBWin);
    loop_ub = roiBWin->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_loop_ub = roiBWin->size[0];
      for (i2 = 0; i2 < b_loop_ub; i2++) {
        roiBWout->data[(i2 + roiBWout->size[0] * i1) + roiBWout->size[0] *
          roiBWout->size[1] * pp] = roiBWin->data[i2 + roiBWin->size[0] * i1];
      }
    }

    i1 = b_i->size[0];
    b_i->size[0] = n;
    emxEnsureCapacity_real_T(b_i, i1);
    loop_ub = n - 1;
    for (i1 = 0; i1 <= loop_ub; i1++) {
      b_i->data[i1] = x21_data[i1];
    }

    i1 = y11->size[0];
    y11->size[0] = cc;
    emxEnsureCapacity_real_T(y11, i1);
    loop_ub = cc - 1;
    for (i1 = 0; i1 <= loop_ub; i1++) {
      y11->data[i1] = y21_data[i1];
    }

    poly2mask(b_i, y11, meanIMG->size[0], meanIMG->size[1], roiBWin);
    loop_ub = roiBWout->size[0];
    b_loop_ub = roiBWout->size[1];
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        roiBW2->data[(i2 + roiBW2->size[0] * i1) + roiBW2->size[0] *
          roiBW2->size[1] * pp] = (signed char)(roiBWout->data[(i2 +
          roiBWout->size[0] * i1) + roiBWout->size[0] * roiBWout->size[1] * pp]
          - roiBWin->data[i2 + roiBWin->size[0] * i1]);
      }
    }

    loop_ub = roiBW2->size[0];
    b_loop_ub = roiBW2->size[1];
    i1 = imgCrop->size[0] * imgCrop->size[1];
    imgCrop->size[0] = roiBW2->size[0];
    imgCrop->size[1] = roiBW2->size[1];
    emxEnsureCapacity_real_T(imgCrop, i1);
    i1 = b_roiBW2->size[0] * b_roiBW2->size[1];
    b_roiBW2->size[0] = roiBW2->size[0];
    b_roiBW2->size[1] = roiBW2->size[1];
    emxEnsureCapacity_int32_T(b_roiBW2, i1);
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        i3 = roiBW2->data[(i2 + roiBW2->size[0] * i1) + roiBW2->size[0] *
          roiBW2->size[1] * pp];
        imgCrop->data[i2 + imgCrop->size[0] * i1] = i3;
        b_roiBW2->data[i2 + b_roiBW2->size[0] * i1] = i3;
      }
    }

    idx = roiBW2->size[0] * roiBW2->size[1];
    i1 = c_roiBW2->size[0];
    c_roiBW2->size[0] = idx;
    emxEnsureCapacity_boolean_T(c_roiBW2, i1);
    for (i1 = 0; i1 < idx; i1++) {
      c_roiBW2->data[i1] = (b_roiBW2->data[i1] < 0);
    }

    if (combineVectorElements(c_roiBW2) > 0) {
      /* accounts for inner diameter extending beyond outer diameter */
      n = roiBW2->size[0] * roiBW2->size[1] - 1;
      idx = 0;
      for (k = 0; k <= n; k++) {
        if ((signed char)imgCrop->data[k] < 0) {
          idx++;
        }
      }

      i1 = ii->size[0];
      ii->size[0] = idx;
      emxEnsureCapacity_int32_T(ii, i1);
      idx = 0;
      for (k = 0; k <= n; k++) {
        if ((signed char)imgCrop->data[k] < 0) {
          ii->data[idx] = k + 1;
          idx++;
        }
      }

      loop_ub = ii->size[0];
      for (i1 = 0; i1 < loop_ub; i1++) {
        imgCrop->data[ii->data[i1] - 1] = 0.0;
      }

      loop_ub = imgCrop->size[1];
      for (i1 = 0; i1 < loop_ub; i1++) {
        b_loop_ub = imgCrop->size[0];
        for (i2 = 0; i2 < b_loop_ub; i2++) {
          roiBW2->data[(i2 + roiBW2->size[0] * i1) + roiBW2->size[0] *
            roiBW2->size[1] * pp] = (signed char)imgCrop->data[i2 +
            imgCrop->size[0] * i1];
        }
      }
    }

    /* DW 11232015 - included for neuropil correction */
    /*  CREATE MASKS FOR NEUROPIL CLICKED ROIS, THEN SHOW THEM */
    for (k = 0; k < 360; k++) {
      d = cos(smNpBoundaries[k]);
      RingPks[k] = d;
      d1 = sin(smNpBoundaries[k]);
      b_y1[k] = d1;
      T = smNpBoundaries[k + 720];
      b_x2[k] = pos_lst_data[pos_lst_tmp] + T * d;
      y2[k] = pos_lst_data[pp] + T * d1;
    }

    b_poly2mask(b_x2, y2, meanIMG->size[0], meanIMG->size[1], roiBWin);
    loop_ub = roiBWin->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_loop_ub = roiBWin->size[0];
      for (i2 = 0; i2 < b_loop_ub; i2++) {
        npBWout->data[(i2 + npBWout->size[0] * i1) + npBWout->size[0] *
          npBWout->size[1] * pp] = roiBWin->data[i2 + roiBWin->size[0] * i1];
      }
    }

    for (i1 = 0; i1 < 360; i1++) {
      T = smNpBoundaries[i1 + 360];
      b_x2[i1] = pos_lst_data[pos_lst_tmp] + T * RingPks[i1];
      b_y1[i1] = pos_lst_data[pp] + T * b_y1[i1];
    }

    b_poly2mask(b_x2, b_y1, meanIMG->size[0], meanIMG->size[1], x2);
  }

  emxFree_boolean_T(&x2);
  emxFree_real_T(&y11);
  emxFree_boolean_T(&c_roiBW2);
  emxFree_real_T(&b_imgCrop);
  emxFree_int32_T(&r);
  emxFree_real_T(&b_i);
  emxFree_real_T(&varargin_1);
  emxFree_real_T(&tmpNeuron);
  emxFree_real_T(&imgCrop);

  /* DW 11232015 - adjusted for inclusion of neuropil correction */
  /*  correct for overlapping ROIs (exclude from both) */
  /*  disp('Adjusting ROI masks for overlap....'); */
  /*  tStartROICorr = tic; */
  /*  maskStack = false(size(meanIMG,1) , size(meanIMG,2) , nc); % 01/20/22 added for mex compilation */
  /*  for itr = 1:nc */
  /*      maskStack(itr) = roiBWout(:,:itr}; */
  /*  end */
  /*  first term of cat (i.e., '3') points to element-wise alignement/stacking of arrays */
  b_combineVectorElements(roiBWout, b_roiBW2);
  i = roiBWin->size[0] * roiBWin->size[1];
  roiBWin->size[0] = b_roiBW2->size[0];
  roiBWin->size[1] = b_roiBW2->size[1];
  emxEnsureCapacity_boolean_T(roiBWin, i);
  loop_ub = b_roiBW2->size[0] * b_roiBW2->size[1];
  for (i = 0; i < loop_ub; i++) {
    roiBWin->data[i] = (b_roiBW2->data[i] > 1);
  }

  emxFree_int32_T(&b_roiBW2);
  emxInit_int32_T(&oLapRoiY, 1);
  emxInit_int32_T(&jj, 1);
  c_eml_find(roiBWin, ii, jj);
  i = oLapRoiY->size[0];
  oLapRoiY->size[0] = ii->size[0];
  emxEnsureCapacity_int32_T(oLapRoiY, i);
  loop_ub = ii->size[0];
  for (i = 0; i < loop_ub; i++) {
    oLapRoiY->data[i] = ii->data[i];
  }

  emxInit_int32_T(&oLapRoiX, 1);
  i = oLapRoiX->size[0];
  oLapRoiX->size[0] = jj->size[0];
  emxEnsureCapacity_int32_T(oLapRoiX, i);
  loop_ub = jj->size[0];
  for (i = 0; i < loop_ub; i++) {
    oLapRoiX->data[i] = jj->data[i];
  }

  i = pos_lst_size[0] - 1;
  for (idx = 0; idx <= i; idx++) {
    i1 = oLapRoiX->size[0];
    for (k = 0; k < i1; k++) {
      roiBWout->data[((oLapRoiY->data[k] + roiBWout->size[0] * (oLapRoiX->data[k]
        - 1)) + roiBWout->size[0] * roiBWout->size[1] * idx) - 1] = false;
      roiBW2->data[((oLapRoiY->data[k] + roiBW2->size[0] * (oLapRoiX->data[k] -
        1)) + roiBW2->size[0] * roiBW2->size[1] * idx) - 1] = 0;
    }
  }

  /*  ROImap =  sum ( cat ( 3 , roiBW2{:} ) , 3 ); % first term of cat (i.e., '3') points to element-wise alignement/stacking of arrays */
  /*  tElapsedROICorr = toc(tStartROICorr); */
  /*  disp(['    Time elapsed for ROI mask Correction was ', num2str(tElapsedROICorr/60),' minutes']); */
  /*  correct for neuropil overlap with ROIs and */
  /*  disp('Adjusting neuropil masks for overlap....'); */
  /*  tStartNPilCorr = tic; */
  i = pos_lst_size[0] - 1;
  emxInit_boolean_T(&b_roiBWout, 2);
  for (cc = 0; cc <= i; cc++) {
    loop_ub = npBWout->size[0];
    b_loop_ub = npBWout->size[1];
    i1 = roiBWin->size[0] * roiBWin->size[1];
    roiBWin->size[0] = npBWout->size[0];
    roiBWin->size[1] = npBWout->size[1];
    emxEnsureCapacity_boolean_T(roiBWin, i1);
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        roiBWin->data[i2 + roiBWin->size[0] * i1] = npBWout->data[(i2 +
          npBWout->size[0] * i1) + npBWout->size[0] * npBWout->size[1] * cc];
      }
    }

    i1 = pos_lst_size[0] - 1;
    for (n = 0; n <= i1; n++) {
      loop_ub = roiBWout->size[0];
      b_loop_ub = roiBWout->size[1];
      i2 = b_roiBWout->size[0] * b_roiBWout->size[1];
      b_roiBWout->size[0] = roiBWout->size[0];
      b_roiBWout->size[1] = roiBWout->size[1];
      emxEnsureCapacity_boolean_T(b_roiBWout, i2);
      for (i2 = 0; i2 < b_loop_ub; i2++) {
        for (idx = 0; idx < loop_ub; idx++) {
          b_roiBWout->data[idx + b_roiBWout->size[0] * i2] = (roiBWout->data
            [(idx + roiBWout->size[0] * i2) + roiBWout->size[0] * roiBWout->
            size[1] * n] + roiBWin->data[idx + roiBWin->size[0] * i2] > 1);
        }
      }

      c_eml_find(b_roiBWout, ii, jj);
      i2 = jj->size[0];
      for (k = 0; k < i2; k++) {
        npBWout->data[((ii->data[k] + npBWout->size[0] * (jj->data[k] - 1)) +
                       npBWout->size[0] * npBWout->size[1] * cc) - 1] = false;
      }

      i2 = oLapRoiX->size[0];
      for (k = 0; k < i2; k++) {
        npBWout->data[((oLapRoiY->data[k] + npBWout->size[0] * (oLapRoiX->data[k]
          - 1)) + npBWout->size[0] * npBWout->size[1] * cc) - 1] = false;
      }
    }
  }

  emxFree_boolean_T(&b_roiBWout);
  emxFree_boolean_T(&roiBWout);

  /*  tElapsedNPilCorr = toc(tStartNPilCorr); */
  /*  disp(['    Time elapsed for Neuropil Mask Correction was ', num2str(tElapsedNPilCorr/60),' minutes']); */
  /*  traceExtract_finish = toc(traceExtract_start); */
  /*  fprintf('Trace extraction took %.1f minutes\n',traceExtract_finish/60) */
  /*  CALCULATE FLUORESCENCE TRACES for ALL STIMS */
  /*  calcF_start = tic; */
  i = pos_lst_size[0] - 1;
  emxInit_real_T(&tmp, 2);
  for (cc = 0; cc <= i; cc++) {
    loop_ub = roiBW2->size[0];
    b_loop_ub = roiBW2->size[1];
    i1 = roiBWin->size[0] * roiBWin->size[1];
    roiBWin->size[0] = roiBW2->size[0];
    roiBWin->size[1] = roiBW2->size[1];
    emxEnsureCapacity_boolean_T(roiBWin, i1);
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        roiBWin->data[i2 + roiBWin->size[0] * i1] = (roiBW2->data[(i2 +
          roiBW2->size[0] * i1) + roiBW2->size[0] * roiBW2->size[1] * cc] != 0);
      }
    }

    c_eml_find(roiBWin, ii, jj);
    loop_ub = ii->size[0];
    i1 = oLapRoiY->size[0];
    oLapRoiY->size[0] = ii->size[0];
    emxEnsureCapacity_int32_T(oLapRoiY, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      oLapRoiY->data[i1] = ii->data[i1];
    }

    loop_ub = jj->size[0];
    i1 = oLapRoiX->size[0];
    oLapRoiX->size[0] = jj->size[0];
    emxEnsureCapacity_int32_T(oLapRoiX, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      oLapRoiX->data[i1] = jj->data[i1];
    }

    i1 = tmp->size[0] * tmp->size[1];
    tmp->size[0] = oLapRoiY->size[0];
    tmp->size[1] = RegImg->size[2];
    emxEnsureCapacity_real_T(tmp, i1);
    loop_ub = oLapRoiY->size[0] * RegImg->size[2];
    for (i1 = 0; i1 < loop_ub; i1++) {
      tmp->data[i1] = rtNaN;
    }

    i1 = oLapRoiY->size[0];
    for (k = 0; k < i1; k++) {
      loop_ub = tmp->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        tmp->data[k + tmp->size[0] * i2] = RegImg->data[((oLapRoiY->data[k] +
          RegImg->size[0] * (oLapRoiX->data[k] - 1)) + RegImg->size[0] *
          RegImg->size[1] * i2) - 1];
      }
    }

    nanmean(tmp, tmp_data, tmp_size);
    loop_ub = rawFluo->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      rawFluo->data[i1 + rawFluo->size[0] * cc] = tmp_data[i1];
    }
  }

  /*  calcF_finish = toc(calcF_start); */
  /*  fprintf('Calculating fluorescence traces took %.1f minutes\n',calcF_finish/60) */
  /* DW 11232015 - included Neuropil calculation */
  /*  disp('Calculating Fluo traces for neuropil.....'); */
  /*  tStartfcalcNP = tic; */
  /* DW11132015 - apply to neuropil calculation..... */
  /*  if isempty(gcp('nocreate')) */
  /*      parpool(2) */
  /*  end */
  /*  for nn = 1:length(xc) */
  /*      [rNp,cNp] = find(npBWout{nn} ~= 0 ); */
  /*      npFluo(:,nn) = mean(mean(IMG(rNp,cNp,:),1),2); */
  /*  end */
  i = pos_lst_size[0] - 1;
  for (cc = 0; cc <= i; cc++) {
    loop_ub = npBWout->size[0];
    b_loop_ub = npBWout->size[1];
    i1 = roiBWin->size[0] * roiBWin->size[1];
    roiBWin->size[0] = npBWout->size[0];
    roiBWin->size[1] = npBWout->size[1];
    emxEnsureCapacity_boolean_T(roiBWin, i1);
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        roiBWin->data[i2 + roiBWin->size[0] * i1] = npBWout->data[(i2 +
          npBWout->size[0] * i1) + npBWout->size[0] * npBWout->size[1] * cc];
      }
    }

    c_eml_find(roiBWin, ii, jj);
    loop_ub = ii->size[0];
    i1 = oLapRoiY->size[0];
    oLapRoiY->size[0] = ii->size[0];
    emxEnsureCapacity_int32_T(oLapRoiY, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      oLapRoiY->data[i1] = ii->data[i1];
    }

    loop_ub = jj->size[0];
    i1 = oLapRoiX->size[0];
    oLapRoiX->size[0] = jj->size[0];
    emxEnsureCapacity_int32_T(oLapRoiX, i1);
    for (i1 = 0; i1 < loop_ub; i1++) {
      oLapRoiX->data[i1] = jj->data[i1];
    }

    i1 = tmp->size[0] * tmp->size[1];
    tmp->size[0] = oLapRoiY->size[0];
    tmp->size[1] = RegImg->size[2];
    emxEnsureCapacity_real_T(tmp, i1);
    loop_ub = oLapRoiY->size[0] * RegImg->size[2];
    for (i1 = 0; i1 < loop_ub; i1++) {
      tmp->data[i1] = rtNaN;
    }

    i1 = oLapRoiY->size[0];
    for (k = 0; k < i1; k++) {
      loop_ub = tmp->size[1];
      for (i2 = 0; i2 < loop_ub; i2++) {
        tmp->data[k + tmp->size[0] * i2] = RegImg->data[((oLapRoiY->data[k] +
          RegImg->size[0] * (oLapRoiX->data[k] - 1)) + RegImg->size[0] *
          RegImg->size[1] * i2) - 1];
      }
    }

    nanmean(tmp, tmp_data, tmp_size);
    loop_ub = npFluo->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      npFluo->data[i1 + npFluo->size[0] * cc] = tmp_data[i1];
    }
  }

  emxFree_int32_T(&jj);
  emxFree_int32_T(&ii);
  emxFree_int32_T(&oLapRoiX);
  emxFree_int32_T(&oLapRoiY);
  emxFree_real_T(&tmp);

  /*  tElapsedFcalcNP = toc(tStartfcalcNP); */
  /*  disp(['    Time Elapsed for calculating fluo for neuropil was ',num2str(tElapsedFcalcNP/60),' minutes']); */
  /*   */
  /*  clear IMG */
  loop_ub = rawFluo->size[0] * rawFluo->size[1];
  for (i = 0; i < loop_ub; i++) {
    rawFluo->data[i] -= a * npFluo->data[i];
  }

  /*  fluoAllCorr = fluoAllRaw - (percNP * NPfluoAll); */
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /*  PUT THE DE-NOISING, PARSING AND DFF CALCULATIONS HERE - %% DW05192016 added for trialBased analyses */
  /*  apply assymetrical (exponential) filter to traces - smoothing - MODIFIED FROM PLENZ LAB AVALANCHE CODE */
  /*  tStartDN = tic; */
  /*  disp('Denoising Ca2+ traces with Plenz Lab approach.....'); */
  T = 1.0 / frameRate;

  /*  Normalizing raw F to get rid of negative vals before dF/F calc % 7/2020 */
  n = rawFluo->size[0] * rawFluo->size[1];
  if (rawFluo->size[0] * rawFluo->size[1] <= 2) {
    if (rawFluo->size[0] * rawFluo->size[1] == 1) {
      ex = rawFluo->data[0];
    } else if ((rawFluo->data[0] > rawFluo->data[1]) || (rtIsNaN(rawFluo->data[0])
                && (!rtIsNaN(rawFluo->data[1])))) {
      ex = rawFluo->data[1];
    } else {
      ex = rawFluo->data[0];
    }
  } else {
    if (!rtIsNaN(rawFluo->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= rawFluo->size[0] * rawFluo->size[1])) {
        if (!rtIsNaN(rawFluo->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      ex = rawFluo->data[0];
    } else {
      ex = rawFluo->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        if (ex > rawFluo->data[k - 1]) {
          ex = rawFluo->data[k - 1];
        }
      }
    }
  }

  i = npFluo->size[0] * npFluo->size[1];
  npFluo->size[0] = rawFluo->size[0];
  npFluo->size[1] = rawFluo->size[1];
  emxEnsureCapacity_real_T(npFluo, i);
  loop_ub = rawFluo->size[0] * rawFluo->size[1];
  for (i = 0; i < loop_ub; i++) {
    npFluo->data[i] = rawFluo->data[i] - ex;
  }

  n = npFluo->size[0] * npFluo->size[1];
  if (npFluo->size[0] * npFluo->size[1] <= 2) {
    if (npFluo->size[0] * npFluo->size[1] == 1) {
      b_ex = npFluo->data[0];
    } else if ((npFluo->data[0] < npFluo->data[1]) || (rtIsNaN(npFluo->data[0]) &&
                (!rtIsNaN(npFluo->data[1])))) {
      b_ex = npFluo->data[1];
    } else {
      b_ex = npFluo->data[0];
    }
  } else {
    if (!rtIsNaN(npFluo->data[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= npFluo->size[0] * npFluo->size[1])) {
        if (!rtIsNaN(npFluo->data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      b_ex = npFluo->data[0];
    } else {
      b_ex = npFluo->data[idx - 1];
      i = idx + 1;
      for (k = i; k <= n; k++) {
        if (b_ex < npFluo->data[k - 1]) {
          b_ex = npFluo->data[k - 1];
        }
      }
    }
  }

  i = normF->size[0] * normF->size[1];
  normF->size[0] = npFluo->size[0];
  normF->size[1] = npFluo->size[1];
  emxEnsureCapacity_real_T(normF, i);
  loop_ub = npFluo->size[0] * npFluo->size[1];
  for (i = 0; i < loop_ub; i++) {
    normF->data[i] = npFluo->data[i] / b_ex;
  }

  emxFree_real_T(&npFluo);

  /* smoothed raw dR/R or dF/F - commented out 9/16, uncommented 6/2020 for slower volume data */
  if (T >= 0.2) {
    idx = 3;
  } else if (T >= 0.13) {
    idx = 5;

    /* 7 */
  } else {
    idx = 9;

    /* 13 */
  }

  /*  1 ->symmetric, 2->asym, T = frame time, Tau -> 1.5 */
  i = rawFluo->size[1];
  emxFree_real_T(&rawFluo);
  for (k = 0; k < i; k++) {
    /*  NEED FOR IMPROVEMENT - ELIMINATE FOR LOOP? */
    /* %%%% SYMMETRIC (new as of Sept2016) */
    loop_ub = normF->size[0];
    n = normF->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      tmp_data[i1] = normF->data[i1 + normF->size[0] * k];
    }

    normF_size[0] = 1;
    normF_size[1] = n;
    loop_ub = n - 1;
    if (0 <= loop_ub) {
      memcpy(&normF_data[0], &tmp_data[0], (loop_ub + 1) * sizeof(double));
    }

    smooth2(normF_data, normF_size, idx, T, tmp_data, tmp_size);
    loop_ub = npSubFluoSmooth->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      npSubFluoSmooth->data[i1 + npSubFluoSmooth->size[0] * k] = tmp_data[i1];
    }

    /* Maybe smooth after DFF calc? */
  }

  emxInit_real_T(&b_normF, 2);
  i = normF->size[1];
  i1 = normF->size[0];
  i2 = b_normF->size[0] * b_normF->size[1];
  b_normF->size[0] = normF->size[1];
  b_normF->size[1] = normF->size[0];
  emxEnsureCapacity_real_T(b_normF, i2);
  loop_ub = normF->size[0];
  for (i2 = 0; i2 < loop_ub; i2++) {
    b_loop_ub = normF->size[1];
    for (idx = 0; idx < b_loop_ub; idx++) {
      b_normF->data[idx + b_normF->size[0] * i2] = normF->data[i2 + normF->size
        [0] * idx];
    }
  }

  i2 = normF->size[0] * normF->size[1];
  normF->size[0] = normF->size[1];
  normF->size[1] = i1;
  emxEnsureCapacity_real_T(normF, i2);
  idx = i * i1;
  for (i = 0; i < idx; i++) {
    normF->data[i] = b_normF->data[i];
  }

  emxFree_real_T(&b_normF);
  emxInit_real_T(&b_npSubFluoSmooth, 2);

  /*  added by dulara on 05/24/2020 */
  /*  tElapsedDN = toc(tStartDN); */
  /*  disp(['     Time elapsed for denoising was ', num2str(tElapsedDN/60) , ' minutes']); */
  /*  % fluoAllSmooth dimensions are (frames,roi) */
  /*  F0 = median(fluoAllSmooth,1); % Median fluo for each roi */
  /*  tmpDF = bsxfun(@minus,fluoAllSmooth,F0); */
  /*  tmpDFF = bsxfun(@rdivide,tmpDF,F0); */
  /*  DFF.smooth = 100*tmpDFF'; */
  /*  performing moving average baseline subtraction for DFF */
  /*  winsize = round(exptVars.frameRate * winSizeSeconds); */
  i = b_npSubFluoSmooth->size[0] * b_npSubFluoSmooth->size[1];
  b_npSubFluoSmooth->size[0] = npSubFluoSmooth->size[1];
  b_npSubFluoSmooth->size[1] = npSubFluoSmooth->size[0];
  emxEnsureCapacity_real_T(b_npSubFluoSmooth, i);
  loop_ub = npSubFluoSmooth->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_loop_ub = npSubFluoSmooth->size[1];
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      b_npSubFluoSmooth->data[i1 + b_npSubFluoSmooth->size[0] * i] =
        npSubFluoSmooth->data[i + npSubFluoSmooth->size[0] * i1];
    }
  }

  slideWinSub(b_npSubFluoSmooth, winsize, percentBaselineSub, DFF->smooth);
  slideWinSub(normF, winsize, percentBaselineSub, DFF->raw);

  /*  convert to sparse matrices for efficiency during real time analysis */
  /*  added to allow mex compilation */
  i = pos_lst_size[0] - 1;
  i1 = roiBW2_sp_size[0];
  roiBW2_sp_size[0] = (short)pos_lst_size[0];
  c_emxEnsureCapacity_coder_inter(roiBW2_sp_data, roiBW2_sp_size, i1);
  i1 = npBWout_sp_size[0];
  npBWout_sp_size[0] = (short)pos_lst_size[0];
  d_emxEnsureCapacity_coder_inter(npBWout_sp_data, npBWout_sp_size, i1);
  emxFree_real_T(&b_npSubFluoSmooth);
  for (cc = 0; cc <= i; cc++) {
    loop_ub = roiBW2->size[0];
    b_loop_ub = roiBW2->size[1];
    i1 = meanIMG->size[0] * meanIMG->size[1];
    meanIMG->size[0] = roiBW2->size[0];
    meanIMG->size[1] = roiBW2->size[1];
    emxEnsureCapacity_real_T(meanIMG, i1);
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        meanIMG->data[i2 + meanIMG->size[0] * i1] = roiBW2->data[(i2 +
          roiBW2->size[0] * i1) + roiBW2->size[0] * roiBW2->size[1] * cc];
      }
    }

    b_sparse(meanIMG, roiBW2_sp_data[cc].d, roiBW2_sp_data[cc].colidx,
             roiBW2_sp_data[cc].rowidx, &roiBW2_sp_data[cc].m,
             &roiBW2_sp_data[cc].n, &roiBW2_sp_data[cc].maxnz);
    loop_ub = npBWout->size[0];
    b_loop_ub = npBWout->size[1];
    i1 = roiBWin->size[0] * roiBWin->size[1];
    roiBWin->size[0] = npBWout->size[0];
    roiBWin->size[1] = npBWout->size[1];
    emxEnsureCapacity_boolean_T(roiBWin, i1);
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        roiBWin->data[i2 + roiBWin->size[0] * i1] = npBWout->data[(i2 +
          npBWout->size[0] * i1) + npBWout->size[0] * npBWout->size[1] * cc];
      }
    }

    c_sparse(roiBWin, npBWout_sp_data[cc].d, npBWout_sp_data[cc].colidx,
             npBWout_sp_data[cc].rowidx, &npBWout_sp_data[cc].m,
             &npBWout_sp_data[cc].n, &npBWout_sp_data[cc].maxnz);
  }

  emxFree_boolean_T(&roiBWin);
  emxFree_boolean_T(&npBWout);
  emxFree_int8_T(&roiBW2);
  emxFree_real_T(&meanIMG);

  /*  clean up raw (remove bad ROIs) --> romove for speed up */
  /*  clean_start = tic; */
  /*  Streamlined the "cleaning" stage 6/2020 */
  /*  7/2020 update: might not be a necessary step anymore with normalization changes made above, saving regardless */
  /*  [DFF.smoothClean,keptRois.smoothClean] = cleanTrace(DFF.smooth); */
  /*  [DFF.rawClean,keptRois.rawClean] = cleanTrace(DFF.raw); */
  /*  DFF.smoothClean = DFF.smooth((min(DFF.smooth')> 2*median(min(DFF.smooth))),:); % DW09232016 changed this to be data-based rather than hardcoded / ZB09302016 changed mean to median */
  /*  % normFClean = normF((min(DFF.smooth')> 2*median(min(DFF.smooth))),:); % added on 05/24/2020 */
  /*   */
  /*  usedInds = find((min(DFF.smooth')> 2*median(min(DFF.smooth)))); */
  /*  xc = xc(usedInds); */
  /*  yc = yc(usedInds); */
  /*  dffParams.winsize = winsize; */
  /*  dffParams.percentBaselineSub = percentBaselineSub; */
  /*  dffParams.neuropilSubPercent = neuropilSubPercent; */
  /*  save(datafile, 'ROImap','npBWout','roiBW2','xc','yc','minNpSubFluo','maxAdjF','-append'); */
  /*  save(datafile, 'npSubFluo', 'npSubFluoSmooth','DFF','dffParams','xcRaw','ycRaw', '-append'); */
  /*   */
  /*  clean_finish = toc(clean_start); */
  /*  fprintf('Removing bad ROIs took %.1f minutes\n',clean_finish/60) */
  /*   */
  /*  total_finish = toc(total_start); */
  /*  fprintf('Script completed successfully in %.1f minutes\n',total_finish/60); */
  *minNpSubFluo = ex;
  *maxAdjF = b_ex;
}

/* End of code generation (computeDFF_new_coder.c) */
