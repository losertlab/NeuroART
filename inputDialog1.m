%INPUTSDLG DEMO (Enhanced input dialog box with multiple data types)
% Written by: Takeshi Ikuma
% Last Updated: May 5 2010
%
% Updated for additional functions F. Hatz 2013

def_initialBatchSize = 601; % default values for the input dialog
def_BatchSize = 1000;
Title = 'Initial Batch Settings';
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

Prompt(1,:) = {'Initial batch: start', 'START','(frame ID)'};
Formats(1,1).type = 'edit';
Formats(1,1).format = 'integer';
Formats(1,1).limits = [1 5000]; 
Formats(1,1).size = 50;
% Formats(2,2).unitsloc = 'bottomleft';
DefAns(1).START = 1;

Prompt(end+1,:) = {'Initial batch: end','END','(frame ID)'};
Formats(2,1).type = 'edit';
Formats(2,1).format = 'integer';
Formats(2,1).limits = [1 5000]; 
Formats(2,1).size = 100;
DefAns.END = def_initialBatchSize;

Prompt(end+1,:) = {'Max. number of frames','BSIZE','(after initial batch)'};
Formats(3,1).type = 'edit';
Formats(3,1).format = 'integer';
Formats(3,1).limits = [100 100000]; 
Formats(3,1).size = 100;
DefAns.BSIZE = max(100,def_BatchSize);
% 
Prompt(end+1,:) = {'Figure updating frequency','GAP','frames'};
Formats(4,1).type = 'edit';
Formats(4,1).format = 'integer';
Formats(4,1).limits = [1 100000]; % Figures should be updated at least twice
Formats(4,1).size = 100;
DefAns.GAP = 30;

Prompt(end+1,:) = {'Green channel ID','GREENCHAN',[]};
Formats(5,1).type = 'list';
Formats(5,1).style = 'popupmenu';
Formats(5,1).items = cellstr(string(1:numChannels));

[Answer1,Cancelled1] = inputsdlg(Prompt,Title,Formats,DefAns,Options);

clear d DefAns files Formats Options Prompt Title
