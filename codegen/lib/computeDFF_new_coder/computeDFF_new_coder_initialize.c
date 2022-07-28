/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * computeDFF_new_coder_initialize.c
 *
 * Code generation for function 'computeDFF_new_coder_initialize'
 *
 */

/* Include files */
#include "computeDFF_new_coder_initialize.h"
#include "computeDFF_new_coder.h"
#include "computeDFF_new_coder_data.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void computeDFF_new_coder_initialize(void)
{
  rt_InitInfAndNaN();
  isInitialized_computeDFF_new_coder = true;
}

/* End of code generation (computeDFF_new_coder_initialize.c) */
