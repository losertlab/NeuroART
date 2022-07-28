function varargout = AnimalHistoryNotes(varargin)
%This is were the inforation on a given animal is accessed.
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @AnimalHistoryNotes_OpeningFcn, ...
    'gui_OutputFcn',  @AnimalHistoryNotes_OutputFcn, ...
    'gui_LayoutFcn',  [] , ...
    'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end
if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
function AnimalHistoryNotes_OpeningFcn(hObject, eventdata, handles, varargin)
handles.output = hObject;
Animal=varargin{1}{1};
handles.Animal = Animal;
guidata(hObject, handles);
function varargout = AnimalHistoryNotes_OutputFcn(hObject, eventdata, handles)
varargout{1} = handles.output;
%% Take notes in notepad
function notes_Callback(hObject, eventdata, handles)
global datapath
if ~exist([datapath '\' handles.Animal],'dir')
    mkdir([datapath '\' handles.Animal]);
end
eval(['!notepad ' [datapath '\' handles.Animal,'\AnimalNotes.txt']])
uiresume
%% Load figure
function load_Callback(hObject, eventdata, handles)
global datapath
[filename filepath] = uigetfile('*.png;*.jpg','Task Performance Figure', [datapath '\' handles.Animal]);
try
    performancefig=imread([filepath filename]);
    figure(handles.figure1)
    subplot(handles.TaskPerformance),imagesc(performancefig);
    axis image off
catch
end
