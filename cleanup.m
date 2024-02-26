dll_name = 'Overdrive_SDK_C_wrapper';
dll_header = 'Overdrive_SDK_C_wrapper.h';
SLM_power_off_error = calllib(dll_name, 'SLM_power', 0);
calllib(dll_name, 'Delete_SDK'); % Always call Delete_SDK before exiting
disp(strcat(dll_name, ' exited.'));
unloadlibrary(dll_name);
fclose(fh);