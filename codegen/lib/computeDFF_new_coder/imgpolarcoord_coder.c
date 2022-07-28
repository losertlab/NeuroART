/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * imgpolarcoord_coder.c
 *
 * Code generation for function 'imgpolarcoord_coder'
 *
 */

/* Include files */
#include "imgpolarcoord_coder.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxutil.h"
#include "computeDFF_new_coder_rtwutil.h"
#include "poly2mask.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void imgpolarcoord_coder(const emxArray_real_T *img, emxArray_real_T *pcimg)
{
  int cy_tmp;
  int cx;
  int minval;
  int r;
  int j;
  double i;
  int a;
  double b_a;

  /*  IMGPOLARCOORD converts a given image from cartesian coordinates to polar */
  /*  coordinates. */
  /*  */
  /*  Input: */
  /*         img  : bidimensional image. */
  /*       radius : radius length (# of pixels to be considered). */
  /*       angle  : # of angles to be considered for decomposition. */
  /*  */
  /*  Output: */
  /*        pcimg : polar coordinate image. */
  /*  */
  /*  Usage Example: */
  /*   im=double(imread('cameraman.tif')); */
  /*   fim=fft2(im); */
  /*   pcimg=iapolarcoord(im); */
  /*   fpcimg=iapolarcoord(fim); */
  /*   figure; subplot(2,2,1); imagesc(im); colormap gray; axis image; */
  /*   title('Input image');  subplot(2,2,2); */
  /*   imagesc(log(abs(fftshift(fim)+1)));  colormap gray; axis image; */
  /*   title('FFT');subplot(2,2,3); imagesc(pcimg); colormap gray; axis image; */
  /*   title('Polar Input image');  subplot(2,2,4); */
  /*   imagesc(log(abs(fpcimg)+1));  colormap gray; axis image; */
  /*   title('Polar FFT'); */
  /*  */
  /*  Notes: */
  /*   The software is provided "as is", without warranty of any kind. */
  /*   Javier Montoya would like to thank prof. Juan Carlos Gutierrez for his */
  /*   support and suggestions, while studying polar-coordinates. */
  /*   Authors: Juan Carlos Gutierrez & Javier Montoya. */
  cy_tmp = (int)rt_roundd_snf((double)img->size[0] / 2.0);
  cx = (int)rt_roundd_snf((double)img->size[1] / 2.0);
  minval = (int)fmin(cy_tmp, rt_roundd_snf((double)img->size[1] / 2.0));
  r = pcimg->size[0] * pcimg->size[1];
  pcimg->size[0] = minval;
  pcimg->size[1] = 360;
  emxEnsureCapacity_real_T(pcimg, r);
  j = minval * 360;
  for (r = 0; r < j; r++) {
    pcimg->data[r] = 0.0;
  }

  i = 1.0;
  for (r = 0; r < minval; r++) {
    j = 0;
    for (a = 0; a < 360; a++) {
      b_a = (double)a * 0.017453292519943295;
      pcimg->data[((int)i + pcimg->size[0] * j) - 1] = img->data[((cy_tmp + (int)
        rt_roundd_snf((double)r * sin(b_a))) + img->size[0] * ((cx + (int)
        rt_roundd_snf((double)r * cos(b_a))) - 1)) - 1];
      j++;
    }

    i++;
  }
}

/* End of code generation (imgpolarcoord_coder.c) */
