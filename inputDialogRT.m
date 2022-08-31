%INPUTSDLG DEMO (Enhanced input dialog box with multiple data types)
% Written by: Takeshi Ikuma
% Last Updated: May 5 2010
%
% Updated for additional functions F. Hatz 2013
function [inputParams, canceled] = inputDialogRT(dataPath)
    Title = 'Input Parameters';
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
    
    % Prompt(1,:) = {['Please enter all the required input parameters'],[],[]};
    % Formats(1,1).type = 'text';
    % Formats(1,1).size = [-1 0];
    % Formats(1,1).span = [1 2]; % item is 1 field x 2 fields
    
    Prompt(1,:) = {'Average neuron radius', 'R','microns'};
    Formats(1,1).type = 'edit';
    Formats(1,1).format = 'integer';
    Formats(1,1).labelloc = 'leftmiddle';
    Formats(1,1).size = 50; % automatically assign the height
    DefAns(1).R = 5;
    
    Prompt(end+1,:) = {'DF/F display width','DISPLAY','frames'};
    Formats(2,1).type = 'edit';
    Formats(2,1).format = 'integer';
    Formats(2,1).limits = [50 5000]; 
    Formats(2,1).size = 50;
    Formats(2,1).labelloc = 'leftmiddle';
    DefAns.DISPLAY = 200;
    
    Prompt(end+1,:) = {'Experiment ID','EXID',[]};
    Formats(3,1).type = 'edit';
    Formats(3,1).format = 'text';
    Formats(3,1).size = 200;
    DefAns.EXID = 'RT_test1';
    
    Prompt(end+1,:) = {'Image folder','IMGFOLDER',[]};
    Formats(4,1).type = 'edit';
    Formats(4,1).format = 'dir';
    Formats(4,1).size = 250; %[-1 0];
    % parts = strsplit(pwd, filesep);
    parts = strsplit(dataPath, filesep);
    parent_folder = strjoin(parts(1:end-1), filesep);
    % DefAns.IMGFOLDER = 'C:\Users\Losert Lab\Documents\Data';
    
    Prompt(end+1,:) = {'Image file name','IMG',[]};
    Formats(5,1).type = 'edit';
    Formats(5,1).format = 'text';
    Formats(5,1).size = 200;
    DefAns.IMG = 'greenchannel_6000';
    
    Prompt(end+1,:) = {'Image format','FORMAT',[]};
    Formats(6,1).type = 'list';
    Formats(6,1).style = 'popupmenu';
    Formats(6,1).items = {'raw','TIF'};
    % DefAns.FORMAT = 'raw';
    
    Prompt(end+1,:) = {'Imaging system','SCOPE',[]};
    Formats(7,1).type = 'list';
    Formats(7,1).style = 'popupmenu';
    Formats(7,1).items = {'Thorlabs B-Scope','Bruker Ultima 2P+','Bruker Ultima 2P','BEAMM(ScanImage)','MOM-Thors','Offline Mode'};
    %DefAns.SCOPE = 'Thorlabs B-Scope';
    
    Prompt(end+1,:) = {'Number of channels','NUMCHAN',[]};
    Formats(8,1).type = 'list';
    Formats(8,1).style = 'popupmenu';
    Formats(8,1).items = {'1','2','3','4'};
    % DefAns.NUMCHAN = '1';
    
    Prompt(end+1,:) = {'Analyze Receptive Fields?','RF',[]};
    Formats(9,1).type = 'list';
    Formats(9,1).style = 'popupmenu';
    % Formats(9,1).format = 'text';
    Formats(9,1).items = {'Yes','No'};
    % Formats(9,1).span = [2 1];  % item is 3 fields x 1 field
    DefAns.RF = 2;  
    
    Prompt(end+1,:) = {'Red Channel Available?','RCHAN',[]};
    Formats(10,1).type = 'list';
    Formats(10,1).style = 'popupmenu';
    % Formats(10,1).format = 'text';
    Formats(10,1).items = {'No','Yes'};
    % Formats(10,1).span = [2 1];  % item is 3 fields x 1 field
    % DefAns.RCHAN = 2; 
    
    Prompt(end+1,:) = {'Cell Finding Method','CFIND',[]};
    Formats(11,1).type = 'list';
    Formats(11,1).style = 'popupmenu';
    % Formats(11,1).format = 'text';
    Formats(11,1).items = {'Manual','CaImAn','Cite-on','From File'};
    % Formats(11,1).span = [4 1];  % item is 3 fields x 1 field
    DefAns.CFIND = 3; 
    
    Prompt(end+1,:) = {'ROI Type','ROI',[]};
    Formats(12,1).type = 'list';
    Formats(12,1).style = 'popupmenu';
    % Formats(12,1).format = 'text';
    Formats(12,1).items = {'Not Filled','Filled'};
    % Formats(12,1).span = [2 1];  % item is 3 fields x 1 field
    % DefAns.ROI = 2; 
    
    Prompt(end+1,:) = {'Network Analysis Method','NET',[]};
    Formats(13,1).type = 'list';
    Formats(13,1).style = 'popupmenu';
    % Formats(13,1).format = 'text';
    Formats(13,1).items = {'MST','Correlations'};
    % Formats(13,1).span = [2 1];  % item is 3 fields x 1 field
    % DefAns.NET = 1; 
    
    Prompt(end+1,:) = {'Correlation Analysis Window','CWIN',[]};
    Formats(14,1).type = 'list';
    Formats(14,1).style = 'popupmenu';
    % Formats(14,1).format = 'text';
    Formats(14,1).items = {'Cumulative','Fixed'};
    % Formats(14,1).span = [2 1];  % item is 3 fields x 1 field
    % DefAns.CWIN = 1; 
    
    Prompt(end+1,:) = {'Correlation Window size (if fixed)', 'CWINF','seconds'};
    Formats(15,1).type = 'edit';
    Formats(15,1).format = 'integer';
    Formats(15,1).labelloc = 'leftmiddle';
    Formats(15,1).size = 50; 
    DefAns.CWINF = 5;
    
    % Prompt(end+1,:) = {'Imaging Frequency','FQ','Hz'};
    % Formats(7,1).type = 'edit';
    % Formats(7,1).format = 'integer';
    % Formats(7,1).limits = [1 100]; 
    % Formats(7,1).size = 100;
    % DefAns.FQ = 30;
    
    [inputParams,canceled] = inputsdlg(Prompt,Title,Formats,DefAns,Options);
    
    clear d DefAns files Formats Options Prompt Title
    
    % Prompt(end+1,:) = {'Action','Action',[]};
    % Formats(3,4).type = 'list';
    % Formats(3,4).style = 'togglebutton';
    % Formats(3,4).format = 'text';
    % Formats(3,4).items = {'Bid';'Decline';'Pass'};
    % Formats(3,4).span = [3 1];  % item is 3 fields x 1 field
    % DefAns.Action = 'Decline'; % = 'Decline'
    
    % Prompt(end+1,:) = {'Save Bidding History To','SaveFile',[]};
    % Formats(5,1).type = 'edit';
    % Formats(5,1).format = 'file';
    % Formats(5,1).limits = [1 0]; % use uiputfile
    % Formats(5,1).size = [-1 0];
    % Formats(5,1).span = [1 3];  % item is 1 field x 3 fields
    % DefAns.SaveFile = files{2};
    % Prompt(end+1,:) = {'Select Item Files','ItemFiles',[]};
    % Formats(6,1).type = 'edit';
    % Formats(6,1).format = 'file';
    % Formats(6,1).limits = [0 5]; % multi-select files
    % Formats(6,1).size = [-1 -1];
    % Formats(6,1).items = {'*.itm','Auction Item File';'*.*','All Files'};
    % Formats(6,1).span = [1 3];  % item is 1 field x 3 fields
    % DefAns.ItemFiles = files(3:end);
    % Prompt(end+1,:) = {'Choose Currency','MoneyUnit',[]};
    % Formats(7,1).type = 'list';
    % Formats(7,1).format = 'text';
    % Formats(7,1).style = 'radiobutton';
    % Formats(7,1).items = {'U.S. Dollar' 'Euro';'Japanese Yen' ''};
    % % Formats(7,1).span = [2 1];  % item is 2 field x 1 fields
    % DefAns.MoneyUnit = 'Japanese Yen';%3; % yen
    % Prompt(end+1,:) = {'Item Table','Table',[]};
    % Formats(7,2).type = 'table';
    % Formats(7,2).format = {'char', {'left','right'}, 'numeric' 'logical'}; % table (= table in main dialog) / window (= table in separate dialog)
    % Formats(7,2).items = {'Row' 'Left/Right' 'Value' 'Done'};
    % Formats(7,2).size = [373 73];
    % Formats(7,2).span = [1 3];  % item is 2 field x 1 fields
    % DefAns.Table = {'Row1' 'left'  10 false
    %                 'Row2' 'right' 12 true};
    % Prompt(end+1,:) = {'Bidding Rate','BidRate',[]};
    % Formats(8,1).type = 'range';
    % DefAns.BidRate = 0.75;
    % Prompt(end+1,:) = {'Press Me!','',''};
    % Formats(8,2).type = 'button';
    % Formats(8,2).callback = @(~,~,handles,k)msgbox(sprintf('You just pressed %s button',get(handles(k),'String')),'modal');
    % Prompt(end+1,:) = {'Item List','List',[]};
    % Formats(8,3).type = 'list';
    % Formats(8,3).style = 'popupmenu';
    % Formats(8,3).items = {'Black','White','Red','Blue','Green','Yellow','Orange'};
    % Prompt(end+1,:) = {'Item Color','Color',[]};
    % Formats(8,4).type = 'color';
    % DefAns.Color = [1 0 0];
    % Prompt(end+1,:) = {'Item Vector','Vector',[]};
    % Formats(9,1).type = 'edit';
    % Formats(9,1).format = 'vector';
    % Formats(9,1).limits = [0 5]; % 5 parameters max
    % Prompt(end+1,:) = {'Date (dd-mmm-yyyy)', 'Date',[]};
    % Formats(9,2).type = 'edit';
    % Formats(9,2).format = 'date';
    % Formats(9,2).limits = 'dd-mmm-yyyy'; % with time: 'dd-mmm-yyyy HH:MM:SS'
    % Formats(9,2).size = 100;
    % DefAns.Date = [2013 1 2 0 0 0];
    % Prompt(end+1,:) = {'Memo','Memo',[]};
    % Formats(10,1).type = 'edit';
    % Formats(10,1).format = 'text';
    % Formats(10,1).limits = [0 9]; % default: show 20 lines
    % Formats(10,1).size = [-1 0];
    % Formats(10,1).span = [1 4];  % item is 4 fields x 3 fields
    % Prompt(end+1,:) = {'Auction Sites:','Site',[]};
    % Formats(11,1).type = 'list';
    % Formats(11,1).style = 'listbox';
    % Formats(11,1).format = 'text'; % Answer will give value shown in items, disable to get integer
    % Formats(11,1).items = {'www.auction1.com';'www.auction2.com';'www.bidme.com';'www.bestvalu.com'};
    % Formats(11,1).limits = [0 4]; % multi-select
    % Formats(11,1).size = [140 80];
    % % Formats(11,1).span = [3 1];  % item is 2 fields x 1 fields
    % DefAns.Site = {'www.auction1.com','www.bidme.com'};
    % Prompt(end+1,:) = {'X', 'X','(-4, 12)'};
    % Formats(11,2).type = 'edit';
    % Formats(11,2).format = 'float';
    % Formats(11,2).limits = [-4 12];
    % Formats(11,2).size = 80;
    % Formats(11,2).callback = @(~,~,h,k)set(h(k+2),'String',num2str(str2double(get(h(k),'String'))*str2double(get(h(k+1),'String'))));
    % Prompt(end+1,:) = {'Y', 'Y','(non-negative)'};
    % Formats(11,3).type = 'edit';
    % Formats(11,3).format = 'integer';
    % Formats(11,3).limits = [0 100];
    % Formats(11,3).size = 80;
    % Formats(11,3).callback = @(~,~,h,k)set(h(k+1),'String',num2str(str2double(get(h(k-1),'String'))*str2double(get(h(k),'String'))));
    % Prompt(end+1,:) = {'X*Y','XY',[]};
    % Formats(11,4).type = 'edit';
    % Formats(11,4).format = 'float';
    % Formats(11,4).enable = 'inactive';
    % Formats(11,4).size = [100 25];
    % DefAns.X = pi;
    % DefAns.Y = 4;
    % DefAns.XY = DefAns.X*DefAns.Y;
end
