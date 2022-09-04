function [thorSyncFile, pSignalFile, jiSignalInfo] = rfDialog(rf)
    thorSyncFile = '';
    pSignalFile = '';
    jiSignalInfo = [];
    if rf == 1 % Analyzing receptive fields
        promptMessage = sprintf('Do you have Ji Signal Files?');
        if strcmpi(questdlg(promptMessage, 'Tuning Analysis', 'Yes', 'No', 'Yes'), 'Yes')
            [file,path] = uigetfile('*.mat','Please select the JiSignal file','stim_seq_210402_tones.mat');
            JiSignalFile = [path,file];
            jiSignalInfo = load(JiSignalFile).preset_stim_seq;
        else
            [file,path] = uigetfile('*.h5','Please select the thorSync file','Episode001.h5');
            thorSyncFile = [path,file];
            [file,path] = uigetfile('*.mat','Please select the pSignal file','RND_352r_2020_03_07_Phys_1.mat');
            pSignalFile = [path,file];
        end
    end
end