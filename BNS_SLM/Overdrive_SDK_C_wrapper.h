//
//:  Interface functions between LabVIEW/Matlab and C++ for Overdrive_SDK.
//
//   (c) Copyright Boulder Nonlinear Systems 2014-2016, All Rights Reserved.
//
// See the C++ header file Overdrive_SDK.h for detailed description of the
// function arguments.
// The functions Create_SDK and Delete_SDK are C functions that wrap the C++
// Overdrive_SDK class's constructor and destructor, respectively.
// Generally, bool values in the C++ functions are int values in the
// corresponding C functions:
//  * Most C++ functions return true to indicate success, or false to indicate
//    failure. In the C interface, the functions return 0 to indicate success
//    and 1 to indicate failure.
//  * Some C++ functions return true to indicate Yes, or false to indicate No.
//    In the C interface, the functions return 1 to indicate Yes, and 0 for No.

#ifndef OVERDRIVE_SDK_CWRAPPER_H_
#define OVERDRIVE_SDK_CWRAPPER_H_

#ifdef SDK_WRAPPER_EXPORTS
   #define OVERDRIVE_WRAPPER_API __declspec(dllexport)
#else
   #define OVERDRIVE_WRAPPER_API 
#endif

#ifdef __cplusplus
   extern "C" {     /* using a C++ compiler */
#endif


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   void Create_SDK();


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Configure_overdrive(
      int use_GPU_if_available,
      const char* static_regional_lut_file,
      unsigned int max_frames);


   /* ----------------- Delete_SDK ----------------------------------------- */
   OVERDRIVE_WRAPPER_API
   void Delete_SDK();


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   unsigned int Num_devices();


   // ----------------- Get_FPGA_ID -------------------------------------------
   OVERDRIVE_WRAPPER_API
   int Get_FPGA_ID(
      unsigned int board,
      unsigned int* fpga_id);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   unsigned int SLM_height(int board);



   /* ----------------- Use_external_pulse_on_flip ------------------------- */
   OVERDRIVE_WRAPPER_API
   void Use_external_pulse_on_flip(int generate_external_pulse);


   /* ----------------- Write_image ---------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Write_image(
      int board,
      const unsigned char* image,
      unsigned int image_size);


   /* ----------------- Write_overdrive_image ------------------------------ */
   OVERDRIVE_WRAPPER_API
   int Write_overdrive_image(
      int board,
      const unsigned char* target_phase);


   /* The next three functions are used together for interlaced calculations:
      ====================================================================== */

   /* ----------------- Calculate_overdrive_frames ------------------------- */
   OVERDRIVE_WRAPPER_API
   int Calculate_overdrive_frames(
      const unsigned char* target_phase);

   /* ----------------- Get_overdrive_frames ------------------------------- */
   OVERDRIVE_WRAPPER_API
   const unsigned char* Get_overdrive_frames(
      unsigned int* n_image_count,
      unsigned int* bytes_per_image);

   /* ----------------- Write_overdrive_frames ----------------------------- */
   OVERDRIVE_WRAPPER_API
   int Write_overdrive_frames(
      int board_number,
      unsigned int n_transient_frames,
      const unsigned char* images,
      unsigned int bytes_per_image);


   /* The next three functions are used together for precalculated sequences:
      ====================================================================== */

   /* ----------------- Precompute_frames ---------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Precompute_frames(
      const unsigned char* target_phase,
      unsigned int* byte_count);

   /* ----------------- Retrieve_precomputed_frames ------------------------ */
   OVERDRIVE_WRAPPER_API
   int Retrieve_precomputed_frames(
      unsigned char* frame_buffer);

   /* ----------------- Write_precomputed_frames --------------------------- */
   OVERDRIVE_WRAPPER_API
   int Write_precomputed_frames(
      int board,
      const unsigned char* frame_buffer,
      unsigned int max_display_frames);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Is_SIOD(unsigned int board);

   
   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Download_image_sequence(
      unsigned int board,
      unsigned int n_images,
      unsigned char* images,
      unsigned int   overdrive_frames_per_image);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Prepare_to_download_image_sequence(
      unsigned int board,
      unsigned int overdrive_frames_per_image);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Append_image_to_sequence(
      unsigned int         board,
      const unsigned char* image);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Completed_download_image_sequence(unsigned int board);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Restart_image_sequence(unsigned int board);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Stop_image_sequence(unsigned int board);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Select_sequence_subset(
      unsigned int board,
      unsigned int first_image_index,
      unsigned int n_subset_images);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Load_target_sequence_list(
      unsigned int        board,
      unsigned int        n_targets,
      const unsigned int* target_indices);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Stage_triggered_images(
      unsigned int board,
      unsigned int n_images,
      const unsigned char* target_images[],
      int use_overdrive);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Is_staged_write_complete();


   /* ----------------- Interrupts_enabled --------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Interrupts_enabled(int board_number);


   /* ----------------- Get_last_error_message ----------------------------- */
   OVERDRIVE_WRAPPER_API
   const char* Get_last_error_message();


   /*----------------- Load_overdrive_LUT_file ---------------------------- */
   OVERDRIVE_WRAPPER_API
   int Load_overdrive_LUT_file(const char* static_regional_lut_file);


   /* ----------------- Load_LUT_file -------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Load_LUT_file(
      int board,
      char* LUT_file);


   /* ----------------- Load_linear_LUT ------------------------------------ */
   OVERDRIVE_WRAPPER_API
   int Load_linear_LUT(int board);


   /* ----------------- Get_bits_per_pixel --------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Get_bits_per_pixel();


   /* ----------------- Get_version_info ----------------------------------- */
   OVERDRIVE_WRAPPER_API
   const char* Get_version_info();


   /* ----------------- SLM_power ------------------------------------------ */
   OVERDRIVE_WRAPPER_API
   int SLM_power(int power_state);


   /* ----------------- Set_power ------------------------------------------ */
   // Duplicates SLM_power
   OVERDRIVE_WRAPPER_API
   int Set_power(unsigned int board, int power_state);


   /* ----------------- Get_power ------------------------------------------ */
   OVERDRIVE_WRAPPER_API
   int Get_power(unsigned int board, int *power_state);


   /* ----------------- Set_true_frames ------------------------------------ */
   OVERDRIVE_WRAPPER_API
   int Set_true_frames(int true_frames);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Enable_triggering(float max_wait_s);


   /* ---------------------------------------------------------------------- */
   OVERDRIVE_WRAPPER_API
   int Disable_triggering(int wait_for_queued_holograms);


#if 0
   /* ----------------- Use_trigger_for_new_hologram ----------------------- */
   OVERDRIVE_WRAPPER_API
   int Use_trigger_for_new_hologram(
      int enable,
      float max_wait_s);
#endif

   // ------------------------------------------------------------------------------------------------
   OVERDRIVE_WRAPPER_API
   int Write_calibration_image(
      unsigned int board,
      unsigned int region_number,
      unsigned int n_regions,
      unsigned char stripe_depth);


   // ------------------------------------------------------------------------------------------------
   OVERDRIVE_WRAPPER_API
   int Read_LFSD_flex_temps(
      unsigned int  board,
      float* temps_deg_c);


   // ---------------------------------------------------------------------------------------------
   OVERDRIVE_WRAPPER_API
   int Read_LFSD_opamp_temps(
      unsigned int  board,
      float* temps_deg_c);


#ifdef BNS_INTERNAL_USE
   #include "Overdrive_SDK_internal_C_wrapper.h"
#endif

#ifdef __cplusplus
}
#endif

#endif // OVERDRIVE_SDK_CWRAPPER_H_
