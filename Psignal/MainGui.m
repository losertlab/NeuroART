function varargout = MainGui(varargin)
%This is the first GUI to popup in Psignal. It is where the global
%parameters are defined for the experiment.
% Nikolas A. Francis 2018

gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @MainGui_OpeningFcn, ...
    'gui_OutputFcn',  @MainGui_OutputFcn, ...
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

%Initialize settings
function MainGui_OpeningFcn(hObject, eventdata, handles, varargin)
handles.output = hObject;
guidata(hObject, handles);
global datapath MainGuiSettings globalparams home roothome
movegui(hObject,globalparams.disploc);
movegui(hObject,'onscreen');
set(hObject,'Name',globalparams.Rig);
PsignalImage=imread([home '\Psignal.png']);
figure(handles.figure1)
subplot(handles.PsignalPic),imagesc(PsignalImage);
axis image off;
set(handles.Continue,'Enable','on');
set(handles.User,'String','user');
set(handles.Animal,'String','animal');
set(handles.Device,'String',globalparams.Devices);
set(handles.Weight,'String','weight');
set(handles.HealthRating,'String','health rating');
set(handles.DataPath,'String','datapath');
set(handles.PhysHz,'String','4');
set(handles.Rig,'String',globalparams.Rig);
set(handles.Physiology,'String',{'~Phys','Phys'});
handles.Date = datetime;

%Read the settings from last time Psignal was used on this rig. All
%animal-specific settings are stored in each animal's folder.
Rig = lower(get(handles.Rig,'String'));
if exist([roothome '\RecentMainGuiSettings.mat'],'file')
    load([roothome '\RecentMainGuiSettings.mat'])
    if isfield(MainGuiSettings,Rig)
        set(handles.PhysHz,'String',MainGuiSettings.(Rig).PhysHz);
        set(handles.Physiology,'Value',MainGuiSettings.(Rig).Physiology);
        set(handles.Device,'Value',MainGuiSettings.(Rig).Device);
        handles.Date = MainGuiSettings.(Rig).Date;
    end
end
guidata(handles.figure1, handles);
Animal_Callback([], [], handles);
DataPath_Callback([], [], handles);
uiwait(handles.figure1);

%Output
function varargout = MainGui_OutputFcn(hObject, eventdata, handles)
if ~isempty(handles)
    varargout{1} = handles.quit_Psignal;
    varargout{2} = handles.globalparams;
    delete(handles.figure1);
else
    handles.quit_Psignal=1;
    varargout{2} = [];
    varargout{1}=handles.quit_Psignal;
end

%Callback functions
function User_Callback(hObject, eventdata, handles)
global datapath home
load([home '\PsignalConfig.mat'],'datapath')
datapath = [datapath '\' handles.User.String];
set(handles.DataPath,'String',datapath)
Animal_Callback([], [], handles);
guidata(handles.figure1, handles);

function Animal_Callback(hObject, eventdata, handles)
Animal = lower(get(handles.Animal,'String'));
set(handles.Animal,'String',lower(Animal));
global MainGuiSettings datapath
if ~isempty(findstr(datapath,handles.User.String))
    if ~isempty(hObject)
        Animal=lower(get(hObject,'String'));
    else
        Animal = lower(get(handles.Animal,'String'));
    end
    if exist([datapath '\' Animal '\AnimalInfo.mat'],'file')==2
        load ([datapath '\' Animal '\AnimalInfo.mat'],'AnimalInfo')
        if isfield(AnimalInfo,'Weight')
            set(handles.Weight,'String',AnimalInfo.Weight);
        end
        if isfield(AnimalInfo,'HealthRating')
            set(handles.HealthRating,'String',AnimalInfo.HealthRating);
        end
    end
elseif ~strcmpi(Animal,'animal')
    warndlg('***Must enter user first***')
end
guidata(handles.figure1, handles);

function Rig_Callback(hObject, eventdata, handles)

function Weight_Callback(hObject, eventdata, handles)
guidata(handles.figure1, handles);

function Physiology_Callback(hObject, eventdata, handles)
guidata(handles.figure1, handles);

function Continue_Callback(hObject, eventdata, handles)
global datapath home
if strcmpi(get(handles.User,'String'),'User') || strcmpi(get(handles.User,...
        'String'),'Animal')|| strcmpi(get(handles.User,'String'),'Weight')
    msgbox('Experiment, Animal and Weight inputs required before continuing','Psignal','error');
    return
end
if isempty(datapath)
    msgbox('***Datapath not specified***','Psignal','error');
    return
end
Animal = lower(get(handles.Animal,'String'));
if strcmpi(Animal, 'animal')
    msgbox('Animal Name is Required','Psignal','error');
elseif ~exist([datapath '\' Animal '\AnimalInfo.mat'],'file')
    msgbox('New Animal Information Must Be Saved First','Psignal','error');
    return
end
handles.globalparams = Createglobalparams(handles);
store_settings(handles);
quit_Psignal=0;
handles.quit_Psignal = quit_Psignal;
guidata(handles.figure1, handles);
uiresume;

function Info_Callback(hObject, eventdata, handles)
Animal = lower(get(handles.Animal,'String'));
if strcmpi(Animal,'Animal')
    msgbox('******No animal was selected******','Psignal','error');
    return
end
AnimalInformation({Animal});

function DataPath_Callback(hObject, eventdata, handles)
global datapath
if ~isempty(get(handles.DataPath,'String')) && ~strcmpi(handles.DataPath.String,'datapath')
    datapath = get(handles.DataPath,'String');
end
guidata(handles.figure1, handles);

function PhysHz_Callback(hObject, eventdata, handles)
guidata(handles.figure1, handles);

function HealthRating_Callback(hObject, eventdata, handles)
guidata(handles.figure1, handles);

function Device_Callback(hObject, eventdata, handles)
guidata(handles.figure1, handles);

function Device_CreateFcn(hObject, eventdata, handles)

function CalbSpeaker_Callback(hObject, eventdata, handles)
handles.globalparams = Createglobalparams(handles);
SpeakerCalibration;

function Chart_Callback(hObject, eventdata, handles)
global datapath
Animal = lower(get(handles.Animal,'String'));
HealthChart=[];
if exist([datapath '\' Animal '\HealthChart.mat'],'file')
    load([datapath '\' Animal '\HealthChart.mat']);
    Fobj=figure('Position', [100, 100, 1049, 800]);
    movegui(Fobj,'center');
    movegui(Fobj,'onscreen');
    Dates = unique(HealthChart(:,3));
    HealthData = [];
    for i=1:length(Dates)
        idx=find(~cellfun(@isempty,strfind(HealthChart(:,3),Dates{i})),1,'last');
        if HealthChart{idx,1} ~= 0
            if ~isempty(HealthChart{idx,1}) && ~isempty(HealthChart{idx,2})
                HealthData = [HealthData; HealthChart(idx,:)];
            end
        end
    end
    [AX,H1,H2] = plotyy(1:size(HealthData,1),cell2mat(HealthData(:,1)), ...
        1:size(HealthData,1),cell2mat(HealthData(:,2)));
    H1.Marker='s';
    H2.Marker='s';
    H1.MarkerSize=10;
    H1.MarkerFaceColor='b';
    H2.MarkerFaceColor='k';
    H1.Color='b';
    H2.Color='k';
    AX(1).YColor='b';
    AX(2).YColor='k';
    ylabel(AX(1),'Weight (g)')
    ylabel(AX(2),'Health Rating (1-5)')
    if exist([datapath '\' Animal '\AnimalInfo.mat'],'file')==2
        load ([datapath '\' Animal '\AnimalInfo.mat'],'AnimalInfo')
        title(['Health Chart: ' Animal ' (Pull weight=' AnimalInfo.PullWeight 'g)'])
        hold on
        plot([1 size(HealthData,1)],[str2num(AnimalInfo.PullWeight) ...
            str2num(AnimalInfo.PullWeight)],'r--','LineWidth',2);
        set(AX(1),'ylim',[10 40])
        set(AX(1),'YTick',10:5:40)
        set(AX(2),'ylim',[0 5])
        set(AX(2),'YTick',0:5)
    else
        title(['Health Chart: ' Animal])
    end
    set(gca,'XTick',1:size(HealthData,1),'XTickLabel',HealthData(:,3),'XTickLabelRotation',45)
    xlabel('Experiment Date')
    grid on
else
    msgbox('No health chart for this animal','Psignal','error');
    return
end

function water_Callback(hObject, eventdata, handles)
t1=get(handles.Device,'Value');
t2=get(handles.Device,'String');
temp.Device = t2{t1};
temp.WaterTrigger=1;
if  ~isfield(handles,'WaterHW')
    disp('Initializing hardware...');
    handles.WaterHW = ConfigNI(temp);
    guidata(handles.figure1, handles);
end
if isfield(handles,'WaterHW') && get(handles.water,'Value')
    IOControlPump (handles.WaterHW,'start');
end
if isfield(handles,'WaterHW') && ~get(handles.water,'Value')
    IOControlPump (handles.WaterHW,'stop');
end

%Create functions
function User_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function Animal_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function Weight_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function Physiology_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function DataPath_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function Rig_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function PhysHz_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function HealthRating_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

%Local Functions
function store_settings (handles)
global MainGuiSettings home datapath roothome
Rig = lower(get(handles.Rig,'String'));
Animal = lower(get(handles.Animal,'String'));
if ~strcmpi(Animal,'animal')
    MainGuiSettings.(Rig).DataPath = get(handles.DataPath,'String');
    MainGuiSettings.(Rig).Rig = lower(get(handles.Rig,'String'));
    MainGuiSettings.(Rig).PhysHz = get(handles.PhysHz,'String');
    MainGuiSettings.(Rig).Physiology = get(handles.Physiology,'Value');
    MainGuiSettings.(Rig).Device = get(handles.Device,'Value');
    MainGuiSettings.(Rig).Date = datetime;
    save([roothome '\RecentMainGuiSettings.mat'],'MainGuiSettings')
    
    %Current weight and health rating
    if exist([datapath '\' Animal '\AnimalInfo.mat'],'file')==2
        load ([datapath '\' Animal '\AnimalInfo.mat'],'AnimalInfo')
        AnimalInfo.Weight = get(handles.Weight,'String');
        AnimalInfo.HealthRating = get(handles.HealthRating,'String');
        save ([datapath '\' Animal '\AnimalInfo.mat'],'AnimalInfo')
    end
    
    %Health chart
    HealthChart=[];
    if exist([datapath '\' Animal '\HealthChart.mat'],'file')
        load([datapath '\' Animal '\HealthChart.mat']);
    end
    HealthChart = [HealthChart; {str2num(handles.globalparams.Weight)} ...
        {str2num(handles.globalparams.HealthRating)} {handles.globalparams.Date}];
    save([datapath '\' Animal '\HealthChart.mat'],'HealthChart');
end

function globalparams = Createglobalparams(handles)
global globalparams
t2=get(handles.User,'String');
globalparams.User = t2;
t2=get(handles.Animal,'String');
globalparams.Animal = t2;
t2=get(handles.Weight,'String');
globalparams.Weight = t2;
t2=get(handles.HealthRating,'String');
globalparams.HealthRating = t2;
t2=get(handles.DataPath,'String');
globalparams.DataPath = t2;
t2=get(handles.Rig,'String');
globalparams.Rig = (t2);
t2=get(handles.PhysHz,'String');
globalparams.PhysHz = t2;
t1=get(handles.Physiology,'Value');
t2=get(handles.Physiology,'String');
globalparams.Physiology = t2{t1};
t1=get(handles.Device,'Value');
t2=get(handles.Device,'String');
globalparams.Device = t2{t1};
globalparams.Date = datestr(now,29);








