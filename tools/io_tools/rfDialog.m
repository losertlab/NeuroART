function rfParams = rfDialog(inputParams)
    rfParams.thorSyncFile = '';
    rfParams.psignalFile = '';
    rfParams.JiSignalInfo = [];

    if inputParams.RF == 1 % Analyzing receptive fields
         promptMessage = sprintf('Do you have Ji Signal Files?');
         button = questdlg(promptMessage, 'Tuning Analysis', 'Yes', 'No', 'Yes');
         if strcmpi(button, 'Yes')
             [file,path] = uigetfile('*.mat','Please select the JiSignal file','stim_seq_210402_tones.mat');
             JiSignalFile = [path,file];
             rfParams.JiSignalInfo = load(JiSignalFile).preset_stim_seq;
         else
             [file,path] = uigetfile('*.h5','Please select the thorSync file','Episode001.h5');
             rfParams.thorSyncFile = [path,file];
             [file,path] = uigetfile('*.mat','Please select the pSignal file','RND_352r_2020_03_07_Phys_1.mat');
             rfParams.psignalFile = [path,file];
         end
    end
end