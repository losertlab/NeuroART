/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * main.c
 *
 * Code generation for function 'main'
 *
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include files */
#include "main.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_emxAPI.h"
#include "computeDFF_new_coder_terminate.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Declarations */
static void argInit_d1000x2_real_T(double result_data[], int result_size[2]);
static double argInit_real_T(void);
static unsigned short argInit_uint16_T(void);
static emxArray_uint16_T *c_argInit_UnboundedxUnboundedxU(void);
static void main_computeDFF_new_coder(void);

/* Function Definitions */
static void argInit_d1000x2_real_T(double result_data[], int result_size[2])
{
  int idx0;
  double result_tmp;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result_size[0] = 2;
  result_size[1] = 2;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 2; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result_tmp = argInit_real_T();
    result_data[idx0] = result_tmp;

    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result_data[idx0 + 2] = result_tmp;
  }
}

static double argInit_real_T(void)
{
  return 0.0;
}

static unsigned short argInit_uint16_T(void)
{
  return 0U;
}

static emxArray_uint16_T *c_argInit_UnboundedxUnboundedxU(void)
{
  emxArray_uint16_T *result;
  static const int iv[3] = { 2, 2, 2 };

  int idx0;
  int idx1;
  int idx2;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result = emxCreateND_uint16_T(3, iv);

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
      for (idx2 = 0; idx2 < result->size[2U]; idx2++) {
        /* Set the value of the array element.
           Change this value to the value that the application requires. */
        result->data[(idx0 + result->size[0] * idx1) + result->size[0] *
          result->size[1] * idx2] = argInit_uint16_T();
      }
    }
  }

  return result;
}

static void main_computeDFF_new_coder(void)
{
  emxArray_real_T *norm_meanIMG;
  static c_emxArray_coder_internal_spars roiBW2_sp;
  static d_emxArray_coder_internal_spars npBWout_sp;
  struct0_T DFF;
  emxArray_real_T *normF;
  emxArray_real_T *npSubFluoSmooth;
  emxArray_uint16_T *RegImg;
  double frameRate;
  double pos_lst_data[2000];
  int pos_lst_size[2];
  double xcRaw_data[1000];
  int xcRaw_size[1];
  double ycRaw_data[1000];
  int ycRaw_size[1];
  double minNpSubFluo;
  double maxAdjF;
  emxInitArray_real_T(&norm_meanIMG, 2);
  emxInit_emxArray_coder_internal_sparse_(&roiBW2_sp);
  emxInit_emxArray_coder_internal_sparse_1(&npBWout_sp);
  emxInit_struct0_T(&DFF);
  emxInitArray_real_T(&normF, 2);
  emxInitArray_real_T(&npSubFluoSmooth, 2);

  /* Initialize function 'computeDFF_new_coder' input arguments. */
  /* Initialize function input argument 'RegImg'. */
  RegImg = c_argInit_UnboundedxUnboundedxU();
  frameRate = argInit_real_T();

  /* Initialize function input argument 'pos_lst'. */
  argInit_d1000x2_real_T(pos_lst_data, pos_lst_size);

  /* Call the entry-point 'computeDFF_new_coder'. */
  computeDFF_new_coder(RegImg, frameRate, pos_lst_data, pos_lst_size,
                       argInit_real_T(), argInit_real_T(), argInit_real_T(),
                       argInit_real_T(), norm_meanIMG, roiBW2_sp.data,
                       roiBW2_sp.size, npBWout_sp.data, npBWout_sp.size, &DFF,
                       normF, npSubFluoSmooth, xcRaw_data, xcRaw_size,
                       ycRaw_data, ycRaw_size, &minNpSubFluo, &maxAdjF);
  emxDestroyArray_real_T(npSubFluoSmooth);
  emxDestroyArray_real_T(normF);
  emxDestroy_struct0_T(DFF);
  emxDestroy_emxArray_coder_internal_sparse_1(npBWout_sp);
  emxDestroy_emxArray_coder_internal_sparse_(roiBW2_sp);
  emxDestroyArray_real_T(norm_meanIMG);
  emxDestroyArray_uint16_T(RegImg);
}

int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* The initialize function is being called automatically from your entry-point function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_computeDFF_new_coder();

  /* Terminate the application.
     You do not need to do this more than one time. */
  computeDFF_new_coder_terminate();
  return 0;
}

/* End of code generation (main.c) */
