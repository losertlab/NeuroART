%INPUTSDLG DEMO (Enhanced input dialog box with multiple data types)
% Written by: Takeshi Ikuma
% Last Updated: May 5 2010
%
% Updated for additional functions F. Hatz 2013

Title = 'Please select the red channel folder ...';
%%%% SETTING DIALOG OPTIONS
% Options.WindowStyle = 'modal';
Options.Resize = 'on';
Options.FontSize = 9;
Options.Interpreter = 'tex';
Options.CancelButton = 'on';
Options.ApplyButton = 'on';
Options.AlignControls = 'on';
Options.ButtonNames = {'Continue','Cancel'}; 
Prompt = {};
Formats = {};
DefAns = struct([]);

Prompt(1,:) = {'Initial batch folder (red channel)','INTBATCH',[]};
Formats(1,1).type = 'edit';
Formats(1,1).format = 'dir';
Formats(1,1).size = 450; %[-1 0];
parts = strsplit(pwd, filesep);
parent_folder = strjoin(parts(1:end-1), filesep);
DefAns(1).INTBATCH = parent_folder;

Prompt(end+1,:) = {'Image identifier','IMGSEQ',[]};
Formats(4,1).type = 'edit';
Formats(4,1).format = 'text';
Formats(4,1).size = 200;
DefAns.IMGSEQ = 'Red_';

[Answer2,Cancelled2] = inputsdlg(Prompt,Title,Formats,DefAns,Options);

clear DefAns Formats Options Prompt Title