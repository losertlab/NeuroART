//
//:  C++ interface to Overdrive_SDK DLL.
//
//   (c) Copyright Boulder Nonlinear Systems 2014-2017, All Rights Reserved.

#ifndef OVERDRIVE_SDK_H_
#define OVERDRIVE_SDK_H_

#ifdef OVERDRIVE_SDK_EXPORTS
   #define OVERDRIVE_SDK_API __declspec(dllexport)
#else
   #define OVERDRIVE_SDK_API
#endif

/// @file
///
/// Interface to the Overdrive SDK.
///
/// @section usage Using the Overdrive SDK
///
/// @subsection overview General Overview
/// Almost all SDK functions return a @c bool value to indicate success or
/// failure.
/// When a function returns @c false, call Get_last_error_message() to get a
/// text string with information about the failure.
///
/// There are three principal modes of operation using this SDK with Overdrive.
/// The sample code in Overdrive_SDK_example.cpp illustrates these modes.
/// @subsection sub1 Calculate and send frames to SLM
///  Simply call Write_overdrive_image() with the required target image. The
///  SDK will calculate the Overdrive frame sequence for the target, and send
///  the frames to the SLM.
/// @subsection sub2 Interlace calculations and sending frames to SLM
///  This mode removes the calculation delay overhead that is incurred between
///  successive calls to Write_overdrive_image() in the previous mode.
/// @subsection sub3 Pre-compute frames and store in memory before sending to SLM
   // <<>>
   // @subsection sub3 Load/save pre-calculated frames to files
   // <<>>

class Overdrive_SDK_impl;


class OVERDRIVE_SDK_API Overdrive_SDK
{
public:

   // ---------------------------------------------------------------------------------------------
   /// @brief Default constructor for the Overdrive SDK. Detects all SLMs on the system and
   ///        initializes for non-Overdrive use.
   ///
   /// @note To use Overdrive functionality, immediately call Configure_overdrive() with the
   ///       Overdrive-specific parameters.
   ///
   /// @sa Configure_overdrive, Get_last_error_message, Is_overdrive_available.
   // ---------------------------------------------------------------------------------------------
   Overdrive_SDK();


   // ---------------------------------------------------------------------------------------------
   /// @brief Encapsulate construction of Overdrive elements independently from the SLM board 
   ///        construction.
   ///
   /// @param use_GPU_if_available
   /// @param static_regional_lut_file
   /// @param max_frames
   /// @param cl_filename
   /// @return @c true if Overdrive functionality is available, otherwise @c false.
   /// When @c false is returned, the error message is also set, and can be retrieved with
   /// Get_last_error_message().
   // ---------------------------------------------------------------------------------------------
   bool Configure_overdrive(
      bool use_GPU_if_available,
      const char* static_regional_lut_file,
      unsigned int max_frames = 20U,
      const char* cl_filename = 0);


   // ---------------------------------------------------------------------------------------------
   /// @brief Destructor for the Overdrive SDK.
   // ---------------------------------------------------------------------------------------------
   ~Overdrive_SDK();


   // ---------------------------------------------------------------------------------------------
   /// @brief Returns the number of SLMs (controller boards) attached to the system. Use this 
   ///        function to determine if the SDK has any boards on which to operate.
   // ---------------------------------------------------------------------------------------------
   unsigned int Num_devices() const;


   // ---------------------------------------------------------------------------------------------
   /// @brief Reads the FPGA ID from the SLM controller.
   // ---------------------------------------------------------------------------------------------
   bool Get_FPGA_ID(
      unsigned int  board,
      unsigned int* fpga_id);


   // ---------------------------------------------------------------------------------------------
   /// @brief  Returns the number of pixel rows in an SLM, or 0 for an invalid index.
   ///
   /// @param  which_SLM   1-based slm index
   // ---------------------------------------------------------------------------------------------
   unsigned int SLM_height(unsigned int which_SLM);


   // ---------------------------------------------------------------------------------------------
   /// @brief Controls whether a hardware pulse is generated when the image is flipped.
   ///
   /// At startup, the SDK is configured to generate the hardware pulse on each image flip.
   ///
   /// @param generate_external_pulse @c true to enable the hardware pulse;
   ///                                @c false to disable it.
   /// Can be called at any time, and affects all subsequent writes to the SLM.
   /// @note This function does nothing for a LFSD SLM.
   // ---------------------------------------------------------------------------------------------
   void Use_external_pulse_on_flip(bool generate_external_pulse);


   // ---------------------------------------------------------------------------------------------
   /// @brief Write a non-overdrive image to the SLM controlled by @c board.
   ///
   /// @param  board       Index of the board with the required SLM. The index is 1-based
   ///                     (not 0-based).
   /// @param  image       The image to write to the SLM.
   /// @param  image_size  SLM width or height (a square SLM is assumed).
   ///
   /// @return @c true if the image was written successfully, otherwise @c false.
   /// @sa Get_last_error_message
   // ---------------------------------------------------------------------------------------------
   bool Write_image(
      int board,
      const unsigned char* image,
      unsigned int image_size);


   // ---------------------------------------------------------------------------------------------
   /// @brief Writes an image to the SLM using an intermediate transient frame sequence calculated
   /// with Overdrive.
   ///
   /// @param board         Index of the board with the required SLM. The index is 1-based
   ///                      (not 0-based).
   /// @param target_phase  Image of the target phase for the SLM. Phase values from 0 to 1.0
   ///                      correspond to pixel values from 0 to 255.
   ///
   /// @note For a LFSD SLM, only the last frame of the sequence is written to the SLM, unless
   ///       firmware implements Overdrive directly.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Write_overdrive_image(
      int board,
      const unsigned char* target_phase);


   /// @defgroup Interlaced_calc Interlaced Overdrive Calculations
   /// @{
   //
   // The next three functions are used together for interlaced calculations:
   // ======================================================================

   // ---------------------------------------------------------------------------------------------
   /// @brief Calculates the series of frames to be sent to the SLM to transition to
   /// @c target_phase using Overdrive.
   ///
   /// This function returns immediately, without waiting for the calculations to complete.
   /// Call Get_overdrive_frames next to retrieve the Overdrive frame sequence.
   /// Any phase correction set by a call to Set_phase_correction_file() or Set_phase_correction()
   /// is applied in the calculation.
   /// @param target_phase  Image of the target phase for the SLM. Phase values
   ///                      from 0 to 1.0 correspond to pixel values 0 to 255.
   ///
   /// @note This function is not currently available for a LFSD SLM.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Set_phase_correction_file, Set_phase_correction, Get_overdrive_frames,
   ///     Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Calculate_overdrive_frames(const unsigned char* target_phase);


   // ---------------------------------------------------------------------------------------------
   /// @brief Returns the series of frames to be sent to the SLM, calculated by the previous call
   /// to Calculate_overdrive_frames.
   ///
   /// Values returned by this function are typically used in a subsequent call to
   /// Write_overdrive_frames.
   /// (This function does not return until the overdrive calculations are complete.)
   /// @param n_image_count   Set by this function to the number of frames in this Overdrive frame
   ///                        sequence.
   ///                        This parameter must not be NULL. Initial value is ignored.
   /// @param bytes_per_image Set by this function to the number of bytes to send to the SLM for
   ///                        each image.
   ///                        This parameter must not be NULL. Initial value is ignored.
   ///
   /// @note This function is not currently available for a LFSD SLM.
   /// @return Non-null pointer if there were no errors, otherwise 0.
   /// @sa Calculate_overdrive_frames, Write_overdrive_frames, Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   const unsigned char* Get_overdrive_frames(
      unsigned int* n_image_count,
      unsigned int* bytes_per_image);


   // ---------------------------------------------------------------------------------------------
   /// @brief Sends the series of frames retrieved by Get_overdrive_frames to the SLM.
   ///
   /// @param board_number        Index of the board with the required SLM. The index is
   ///                            1-based (not 0-based).
   /// @param n_transient_frames  Number of frames to send to the SLM.
   /// @param images              Pointer to the array of image pixel data.
   /// @param bytes_per_image     Number of bytes to send to the SLM for each image.
   ///
   /// @note This function is not currently available for a LFSD SLM.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Calculate_overdrive_frames, Get_overdrive_frames, Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Write_overdrive_frames(
      int board_number,
      unsigned int n_transient_frames,
      const unsigned char* images,
      unsigned int bytes_per_image);


   /// @} // close group Interlaced_calc


   /// @defgroup Precalculated_OD Precalculated Overdrive Sequencing
   /// @{
   //
   // The next three functions are used together for precalculated sequences:
   // ======================================================================

   // ---------------------------------------------------------------------------------------------
   /// @brief Precomputes the series of frames to be sent to the SLM to transition to
   /// @c target_phase using Overdrive.
   ///
   /// @param target_phase       Image of the target phase for the SLM. Phase values from
   ///                           0 to 1.0 correspond to pixel values 0 and 255 respectively.
   /// @param frame_buffer_size  Set by this function to the number of bytes required to store the
   ///                           sequence of frames.
   ///                           This parameter must not be NULL. Initial value is ignored.
   ///
   /// @note This function is not currently available for a LFSD SLM.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Precompute_frames(
      const unsigned char* target_phase,
      unsigned int* frame_buffer_size);


   // ---------------------------------------------------------------------------------------------
   /// @brief Retrieves the data for a previously-calculated series of frames.
   /// Typically a call to this function is preceded by a call to Calculate_transient_frames.
   ///
   /// @param frame_buffer  Pointer to a caller-provided memory area of sufficient size to store
   ///                      the frame data.
   ///
   /// @note This function is not currently available for a LFSD SLM.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Calculate_overdrive_frames, Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Retrieve_precomputed_frames(unsigned char* frame_buffer);


   // ---------------------------------------------------------------------------------------------
   /// @brief Writes the sequence of frames in @c frame_buffer to the SLM.
   ///
   /// @param board               Index of the board with the required SLM. The index is 1-based
   ///                            (not 0-based).
   /// @param frame_buffer        Contains the sequence of frames to be written to the SLM.
   /// @param max_display_frames  0 to display all frames in the sequence;
   ///                            non-zero to display no more than @c max_display_frames of the
   ///                            frames in @c frame_buffer.
   /// @note This function is not currently available for a LFSD SLM.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Write_precomputed_frames(
      int board,
      const unsigned char* frame_buffer,
      unsigned int max_display_frames = 0U);


   /// @} // close group Precalculated_OD


   // ---------------------------------------------------------------------------------------------
   /// @brief Return @c true if images must be downloaded to board.
   // ---------------------------------------------------------------------------------------------
   bool Is_SIOD(unsigned int board);


/// @defgroup LFSD_sequencing LFSD Sequencing Functions (Multi-Sequence and SIOD)
   /// @{
   //
   // The next seven functions are used together for downloading image (hologram) sequences to the
   // LFSD controller:
   // =============================================================================================

   // ---------------------------------------------------------------------------------------------
   /// @brief Downloads a sequence of images to the SLM controller.
   ///
   /// Call this function if you have a contiguous array of the images for an image sequence.
   /// If you prefer to download each image in the sequence separately, then do not use this
   /// function, but instead use the three functions following this one:
   /// * Call Prepare_to_download_image_sequence() once.
   /// * Call Append_image_to_sequence() once per image in the sequence.
   /// * Call Completed_download_image_sequence() once to start sequencing.
   ///
   /// This function is only intended for use with the LFSD SLM.
   /// Once all the images have been downloaded, an image in the sequence is displayed each time a
   /// trigger is received. The images are displayed on the SLM in order, starting from the first
   /// image, and cycling back to the first image after the last image in the sequence.
   /// To cycle through a subset of the image sequence, call function Select_sequence_subset().
   /// Without a call to this function, the entire sequence will be used.
   /// @param board                       Board number.
   /// @param n_images                    Number of images in the sequence.
   /// @param images                      Pointer to the images.
   /// @param overdrive_frames_per_image   1 - no Overdrive is used, but the regional LUT is
   ///                                         applied.
   ///                                    >1 - fixed number of Overdrive frames is used for each
   ///                                         input image.
   /// @note The LFSD controller can store 4.5GiB of image data. Each frame in the sequence uses
   ///       (n * n) bytes, where n is the pixel width of the SLM. One frame is reserved for
   ///       internal use. The total number of frames in the sequence is
   ///       (n_images * overdrive_frames_per_image).
   ///       Hence, the maximum number of images in the sequence must satisfy:
   ///       n_images < (4.5 * 2^30) / (n * n * overdrive_frames_per_image).
   /// @return @c true if the images were downloaded successfully.
   /// @sa Restart_image_sequence, Stop_image_sequence, Select_sequence_subset.
   // ---------------------------------------------------------------------------------------------
   bool Download_image_sequence(
      unsigned int         board,
      unsigned int         n_images,
      const unsigned char* images,
      unsigned int         overdrive_frames_per_image = 1);


   // ---------------------------------------------------------------------------------------------
   /// @brief  Readies the firmware to receive an image sequence.
   ///
   /// This function is only intended for use with the LFSD SLM. Call this function once before one
   /// or more calls to Append_image_to_sequence().
   /// @param board                      Board number.
   /// @param overdrive_frames_per_image  1 - no Overdrive is used, but the regional LUT is
   ///                                        applied.
   ///                                   >1 - fixed number of Overdrive frames is used for each
   ///                                        input image.
   /// @return @c true if a valid board number was provided.
   /// @sa Append_image_to_sequence, Download_image_sequence, Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Prepare_to_download_image_sequence(
      unsigned int board,
      unsigned int overdrive_frames_per_image = 1);


   // ---------------------------------------------------------------------------------------------
   /// @brief  Appends an image to the image sequence.
   ///
   /// This function is only intended for use with the LFSD SLM.
   /// Internally, the SDK will send @c overdrive_frames_per_image to the SLM controller.
   ///
   /// @param board  Board number.
   /// @param image  Pointer to the image.
   /// @return @c false if the total frame count would have been exceeded by downloading
   ///         @c overdrive_frames_per_image new frames. (See Download_image_sequence() comments
   ///         for determining the maximum number of images and frames.)
   /// @sa Prepare_to_download_image_sequence, Download_image_sequence, Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Append_image_to_sequence(
      unsigned int         board,
      const unsigned char* image);


   // ---------------------------------------------------------------------------------------------
   /// @brief  Re-enables the external trigger to start sequencing.
   ///
   /// This function is only intended for use with the LFSD SLM.
   /// @param board   Board number.
   /// @return @c true if successful.
   /// @sa Prepare_to_download_image_sequence, Append_image_to_sequence, Download_image_sequence,
   ///     Get_last_error_message.
   // ---------------------------------------------------------------------------------------------
   bool Completed_download_image_sequence(unsigned int board);


   // ---------------------------------------------------------------------------------------------
   /// @brief Restarts the sequence of images from the first image in the image subset, or the
   ///        entire sequence if no subset has been defined.
   ///
   /// @param board  Board number.
   /// @return @c true if a valid board number is provided and the SLM is an LFSD. If an image
   ///         sequence is not currently being displayed, then this function does nothing.
   // ---------------------------------------------------------------------------------------------
   bool Restart_image_sequence(unsigned int board);


   // ---------------------------------------------------------------------------------------------
   /// @brief Stops the image sequence on the current image.
   ///
   /// @param board  Board number.
   /// @return @c true if a valid board number is provided and the SLM is an LFSD. If an image
   ///         sequence is not currently being displayed, then this function does nothing.
   // ---------------------------------------------------------------------------------------------
   bool Stop_image_sequence(unsigned int board);


   // ---------------------------------------------------------------------------------------------
   /// @brief Selects a subset of the image sequence for display.
   ///
   /// @param board              Board number.
   /// @param first_image_index  Index (0-based) of the first image in the
   ///                           subset to be displayed.
   /// @param n_subset_images    Number of images in the subset.
   /// @note The last two parameters are in units of images, even if there are multiple overdrive
   ///       frames per image.
   /// @note When this function is called, the image sequence is immediately stopped, equivalent to
   ///       calling Stop_image_sequence() first. After selecting the subset, call
   ///       Restart_image_sequence() to resume responding to triggers.
   /// @return @c true if a valid board number is provided, the SLM is a LFSD, and the first image
   ///         index and the number of images in the subset are valid for the number of images in
   ///         the previously-downloaded image sequence.
   // ---------------------------------------------------------------------------------------------
   bool Select_sequence_subset(
      unsigned int board,
      unsigned int first_image_index,
      unsigned int n_subset_images);


   // ---------------------------------------------------------------------------------------------
   /// @brief  Load a target sequence list to an LFSD that implements overdrive in firmware.
   ///
   /// @param board           Board number (1-based).
   /// @param n_targets       Number of target images in the sequence (should be <= number of
   ///                        images downloaded using Append_image_to_sequence or
   ///                        Download_image_sequence.
   /// @param target_indices  Array of target indices (the first target is at index 1).
   /// @note This function should be called after Complete_sequence_download(), if the targets are
   /// to be displayed an order different from the order in which they were appended to the
   /// image sequence.
   /// @note For an LFSD that implements overdrive in firmware, use this function instead of
   ///       Select_sequence_subset.
   /// @sa Append_image_to_sequence, Complete_sequence_download, Download_image_sequence.
   // ---------------------------------------------------------------------------------------------
   bool Load_target_sequence_list(
      unsigned int        board,
      unsigned int        n_targets,
      const unsigned int* target_indices);


   /// @} // end of group LFSD_sequencing

   // ---------------------------------------------------------------------------------------------
   /// @brief  Computes overdrive image sequences and stores them, ready for writing after a call
   ///         to Enable_triggering().
   ///
   //@@@ Need more information here.
   /// @note This function cannot be used with an LFSD SLM.
   // ---------------------------------------------------------------------------------------------
   bool Stage_triggered_images(
      unsigned int board,
      unsigned int n_images,
      const unsigned char* target_images[],
      bool use_overdrive);


   // ---------------------------------------------------------------------------------------------
   /// @brief  Check to see if staged images have all been written.
   // ---------------------------------------------------------------------------------------------
   bool Is_staged_write_complete() const;


   // ---------------------------------------------------------------------------------------------
   /// @brief Allows user to see if interrupts are enabled on the attached hardware.
   ///
   /// @param board_number        Index of the board with the required SLM. The index is 1-based
   ///                            (not 0-based).
   /// @return @c true if support for interrupts has been set in hardware, otherwise @c false
   // ---------------------------------------------------------------------------------------------
   bool Interrupts_enabled(int board_number);


   // ---------------------------------------------------------------------------------------------
   /// @todo  Retrieves a count of the number of triggers "processed". This number may be larger
   ///        than the number of images written at the time it is called, as interrupts can be
   ///        flushed if there are no queued frames to write.
   ///
   /// @param board
   /// @param counts  Pointer to at least four 32-bit values.
   // ---------------------------------------------------------------------------------------------
   bool Get_trigger_counts(
      unsigned int  board,
      unsigned int* counts);


   // ---------------------------------------------------------------------------------------------
   /// @brief Returns a pointer to the string corresponding to the last error condition detected.
   /// If no error has been detected, the string is empty.
   ///
   /// @return Null-terminated C string.
   // ---------------------------------------------------------------------------------------------
   const char* Get_last_error_message() const;


   // ---------------------------------------------------------------------------------------------
   /// @brief Loads a new set of LUT data for Overdrive calculations.
   ///
   /// @param static_regional_lut_file  File with regional LUT data. If NULL, overdrive is turned
   ///                                  off.
   ///
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message
   // ---------------------------------------------------------------------------------------------
   bool Load_overdrive_LUT_file(const char* static_regional_lut_file);


   // ---------------------------------------------------------------------------------------------
   /// @brief Loads the specified LUT file for non-Overdrive operation.
   ///
   /// @param  board    Index of the board with the required SLM. The index is 1-based
   ///                  (not 0-based).
   /// @param LUT_file  Fully-qualified path to LUT file.
   /// @note This function is not currently available for a LFSD SLM.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message
   // ---------------------------------------------------------------------------------------------
   bool Load_LUT_file(
      int board,
      const char* LUT_file);


   // ---------------------------------------------------------------------------------------------
   /// @brief Forces a linear LUT to be loaded to the SLM controller for non-overdrive operation.
   ///
   /// @param board  Index of the board with the required SLM. The index is 1-based (not 0-based).
   /// @note This function is not currently available for a LFSD SLM.
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message
   // ---------------------------------------------------------------------------------------------
   bool Load_linear_LUT(int board);


   // ---------------------------------------------------------------------------------------------
   /// @brief Returns the number of bits for each pixel on the SLM (typically 8 or 16).
   ///
   /// @return Number of bits per pixel.
   // ---------------------------------------------------------------------------------------------
   unsigned int Get_bits_per_pixel(int board = 1) const;


   // ---------------------------------------------------------------------------------------------
   /// @brief Returns a pointer to the string with SDK version information.
   ///
   /// @return Null-terminated C string.
   // ---------------------------------------------------------------------------------------------
   const char* Get_version_info() const;


   // ---------------------------------------------------------------------------------------------
   /// @brief Retrieves the power state for @c board.
   ///
   /// @param board        Index of the board with the required SLM. The index
   ///                     is 1-based (not 0-based).
   /// @param power_state  @c true for ON, @c false for OFF
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message
   // ---------------------------------------------------------------------------------------------
   bool Get_SLM_power_state(
      int board,
      bool* power_state);


   // ---------------------------------------------------------------------------------------------
   /// @brief Turns the SLM on or off for @c board.
   ///
   /// @param board        Index of the board with the required SLM. The index
   ///                     is 1-based (not 0-based).
   /// @param power_state  @c true for ON, @c false for OFF
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message
   // ---------------------------------------------------------------------------------------------
   bool SLM_power(
      int board,
      bool power_state);


   // ---------------------------------------------------------------------------------------------
   /// @brief Turns all SLMs on or off.
   ///
   /// @param power_state  @c true for ON, @c false for OFF
   /// @return @c true if there were no errors, otherwise @c false.
   /// @sa Get_last_error_message
   // ---------------------------------------------------------------------------------------------
   bool SLM_power(bool power_state);


   // ---------------------------------------------------------------------------------------------
   /// @brief Sets the number of true frames used by the SLM controller.
   ///
   /// @param  true_frames
   /// @return @c true if there were no errors, otherwise @c false.
   // ---------------------------------------------------------------------------------------------
   bool Set_true_frames(int true_frames);


   // ---------------------------------------------------------------------------------------------
   /// @brief Configures hardware to wait for triggers (on Input B) before writing holograms.
   ///
   /// @param  max_wait_s  Upper limit on time to wait for a trigger to arrive.
   /// @return @c true if there were no errors, otherwise @c false.
   // ---------------------------------------------------------------------------------------------
   bool Enable_triggering(float max_wait_s);


   // ---------------------------------------------------------------------------------------------
   /// @brief Configures hardware to stop looking for triggers (on Input B). Staged mode will be
   ///        disabled if it was enabled.
   ///
   /// @param  wait_for_queued_holograms  If @c true, hardware will not be disabled until after
   ///                                    currently queued holograms are written. Otherwise
   ///                                    remaining holograms are flushed.
   /// @return @c true if there were no errors, otherwise @c false.
   // ---------------------------------------------------------------------------------------------
   bool Disable_triggering(bool wait_for_queued_holograms);


   // ---------------------------------------------------------------------------------------------
   /// @brief Create a stripe pattern in-situ and send it to the SLM.
   // ---------------------------------------------------------------------------------------------
   bool Write_calibration_image(
      unsigned int board,
      unsigned int region_number,
      unsigned int n_regions,
      unsigned char stripe_depth);


   // ---------------------------------------------------------------------------------------------
   /// @brief Read an array of 4 floating point temperature values for the flex cable.
   // ---------------------------------------------------------------------------------------------
   bool Read_LFSD_flex_temps(
      unsigned int  board,
      float* temps_deg_c);


   // ---------------------------------------------------------------------------------------------
   /// @brief Read an array of 4 floating point temperature values for the op-amp board.
   // ---------------------------------------------------------------------------------------------
   bool Read_LFSD_opamp_temps(
      unsigned int  board,
      float* temps_deg_c);


#ifdef BNS_INTERNAL_USE      // Additional include for BNS diagnostics.
   #include "Overdrive_SDK_internal.h"
#endif


private:

   Overdrive_SDK_impl* m_sdk_pimpl;

   // Copy constructor and assignment operator are declared private so that
   // they cannot be used outside the class.
   Overdrive_SDK(const Overdrive_SDK& r);
   const Overdrive_SDK& operator=(const Overdrive_SDK& r);

}; //lint !e1712  No use for default constructor.

#endif   // #ifndef OVERDRIVE_SDK_H_
