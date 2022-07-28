function varargout = AnimalInformation(varargin)
%This GUI displays the information abour a given animal, and allows storage
%of that information.
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @AnimalInformation_OpeningFcn, ...
    'gui_OutputFcn',  @AnimalInformation_OutputFcn, ...
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
function AnimalInformation_OpeningFcn(hObject, eventdata, handles, varargin)
%% Initialize info
handles.output = hObject;
Animal = varargin{1}{1};
set(handles.Animal,'String',Animal);
global datapath
if exist([datapath '\' Animal,'\AnimalInfo.mat'],'file')==2
    load ([datapath '\' Animal,'\AnimalInfo.mat'],'AnimalInfo')
    set(handles.DOB,'String',AnimalInfo.DOB);
    set(handles.sex,'String',AnimalInfo.sex);
    set(handles.strain,'String',AnimalInfo.strain);
    set(handles.CID,'String',AnimalInfo.CID);
    set(handles.owner,'String',AnimalInfo.owner);
    set(handles.protocol,'String',AnimalInfo.protocol);
    set(handles.InitialWeight,'String',AnimalInfo.InitialWeight);
    set(handles.pullweight,'String',AnimalInfo.PullWeight);
    AnimalInfo.PullWeight;
    set(handles.youth,'value',AnimalInfo.Youth);
end
guidata(hObject, handles);
%% Output
function varargout = AnimalInformation_OutputFcn(hObject, eventdata, handles)
varargout{1} = handles.output;
function save_Callback(hObject, eventdata, handles)
global datapath home
if strcmpi(datapath,home)
    msgbox('***Data Path not specified***','BaphyLite','error');
    return
end
%save information
Animal = get(handles.Animal,'String');
global datapath
AnimalInfo.Animal = get(handles.Animal,'String');
AnimalInfo.DOB = get(handles.DOB,'String');
AnimalInfo.sex = get(handles.sex,'String');
AnimalInfo.strain = get(handles.strain,'String');
AnimalInfo.CID = get(handles.CID,'String');
AnimalInfo.owner = get(handles.owner,'String');
AnimalInfo.protocol = get(handles.protocol,'String');
AnimalInfo.InitialWeight = get(handles.InitialWeight,'String');
AnimalInfo.PullWeight = num2str(str2num(AnimalInfo.InitialWeight)*0.8);
set(handles.pullweight,'String',AnimalInfo.PullWeight);
AnimalInfo.Youth = get(handles.youth,'value');
if ~exist([datapath '\' Animal],'dir')
    mkdir([datapath '\' Animal]);
end
save ([datapath '\' Animal,'\AnimalInfo.mat'],'AnimalInfo')
% Quit
guidata(handles.figure1, handles);
uiresume;
delete(handles.figure1);
function Animal_Callback(hObject, eventdata, handles)
function Animal_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function DOB_Callback(hObject, eventdata, handles)
function DOB_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function sex_Callback(hObject, eventdata, handles)
function sex_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function strain_Callback(hObject, eventdata, handles)
function strain_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function CID_Callback(hObject, eventdata, handles)
function CID_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function owner_Callback(hObject, eventdata, handles)
function owner_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function protocol_Callback(hObject, eventdata, handles)
function protocol_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function InitialWeight_Callback(hObject, eventdata, handles)
PullWeight = num2str(str2num(get(handles.InitialWeight,'String'))*0.8);
set(handles.pullweight,'String',PullWeight);
guidata(hObject,handles);
function InitialWeight_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pullweight_Callback(hObject, eventdata, handles)
function pullweight_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function youth_Callback(hObject, eventdata, handles)
function youth_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function SID_Callback(hObject, eventdata, handles)
function SID_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function notes_Callback(hObject, eventdata, handles)
Animal = get(handles.Animal,'String');
AnimalHistoryNotes({Animal});
function recordingsites_Callback(hObject, eventdata, handles)
global datapath RecordingSites Animal surface cm surfaceimg
Animal = get(handles.Animal,'String');
FileName=[];
if exist([datapath '\' Animal,'\RecordingSites.mat'],'file')==2
    load ([datapath '\' Animal,'\RecordingSites.mat'])
    surface = imread([datapath '\' Animal,'\surface.png']);
    surfaceimg = figure;
    imshow(surface,[])
else
    [FileName,PathName,FilterIndex] = uigetfile('*.seq','Load surface image');
    if FileName ~= 0
        [headerInfo, surfaces] = strmpix2matlab([PathName FileName],0);
        framepix = 20;
        shift=64;
        disp(' ')
        for i = 1:size(surfaces,3)
            surfaces(:,:,i) = circshift(squeeze(surfaces(:,:,i))',shift)';
        end
        surfaces = surfaces(framepix:end-framepix-1,framepix:end-framepix-1,:);
        surface = squeeze(mean(surfaces,3));
        surface=imsharpen(adapthisteq(surface./max(max(abs(surface)))));
        surface=flipud(surface)';
        imwrite(surface,[datapath '\' Animal,'\surface.png']);
        surfaceimg = figure;
        imshow(surface,[])
    else
        return
    end
end
hold on
title(['Surface: ' Animal])
T=text(size(surface,2)/2,20,'M','fontsize',20,'fontweight','bold','color','w');
T=text(20,size(surface,2)/2,'A','fontsize',20,'fontweight','bold','color','w');
set(T, 'rotation', 90)
cm=jet(size(RecordingSites,1)+1);
for i = 1:size(RecordingSites,1)
    plot(RecordingSites(i,1),RecordingSites(i,2),'o','markersize',10,'markerfacecolor',cm(i,:))
end
if ~isempty(RecordingSites)
    h=legend(num2str([1:size(RecordingSites,1)]'),'location','eastoutside');
end
uicontrol('Style', 'pushbutton', 'String', 'Save Sites',...
    'Position', [10 10 100 25],...
    'Callback', @savesites);
uicontrol('Style', 'pushbutton', 'String', 'New Site',...
    'Position', [550 10 100 25],...
    'Callback', @NewSite);
function savesites(source,callbackdata)
global RecordingSites datapath Animal
save ([datapath '\' Animal,'\RecordingSites.mat'],'RecordingSites')
function NewSite(source,callbackdata)
global RecordingSites cm surfaceimg
figure(surfaceimg)
hold on
cm=jet(size(RecordingSites,1)+1);
c=[];
r=[];
[c,r] = ginput;
if ~isempty(c)
    RecordingSites = [RecordingSites; round([c r])];
    plot(RecordingSites(end,1),RecordingSites(end,2),'o','markersize',10,'markerfacecolor',cm(end,:))
    h=legend(num2str([1:size(RecordingSites,1)]'),'location','eastoutside');
end




