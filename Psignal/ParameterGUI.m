function varargout = ParameterGUI(varargin)
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @ParameterGUI_OpeningFcn, ...
    'gui_OutputFcn',  @ParameterGUI_OutputFcn, ...
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
function ParameterGUI_OpeningFcn(hObject, eventdata, handles, varargin)
pset=varargin{1};
if length(varargin)>3
    Alignment = varargin{4};
else
    Alignment = 'left';
end
% User can optionally pass the font weight in varargin{3)
if length(varargin) > 2
    FontWeight = varargin{3};
else
    FontWeight = 'normal';
end
if length(varargin)>1,
    title=varargin{2};
else
    title='Parameters';
end
% Choose default command line output for ParameterGUI
handles.output = hObject;
set(handles.figure1,'Units','pixels','Name',title);
aa=get(handles.figure1,'Position');
set(handles.figure1,'Position',[aa(1:2) 430 23*length(pset)+80]);
moveguiPlusRemote(hObject,'center');
set(handles.pushbutton1,'Units','pixels','FontWeight','bold');
aa=get(handles.pushbutton1,'Position');
set(handles.pushbutton1,'Position',[aa(1) 20 aa(3:4)]);
set(handles.pushbutton2,'Units','pixels','FontWeight','bold');
aa=get(handles.pushbutton2,'Position');
set(handles.pushbutton2,'Position',[aa(1) 20 aa(3:4)]);
figure(handles.figure1);
pos = [0 23*length(pset)+30];
for cnt1 = 1:length(pset)
    % had the field name, its style and its default value.
    uicontrol('Style','text','String',pset(cnt1).text,'FontWeight',FontWeight,'HorizontalAlignment','right','position',[pos-[0 23*(cnt1-1)+4] 180 20]);
    value=pset(cnt1).default;
    % some controls (like check box) dont have strings, but have Value property:
    switch pset(cnt1).style
        case {'edit','text'},
            DefValue = 0;
        case 'popupmenu',
            if iscell(value)
                value = value{1};
                DefValue = pset(cnt1).default{2};
            else
                DefValue = 1;
            end
        case 'checkbox',
            DefValue = ifstr2num(value);
            value='';
            if isempty(DefValue)
                DefValue =0;
            end
        case 'sound',
            DefValue = value;
            value='Edit ...';
            if isempty(DefValue)
                DefValue=0;
            end
        case 'rawid',
            DefValue = value;
            if DefValue==-1,
                value='New';
            end
            if isempty(DefValue)
                DefValue=0;
            end
    end
    % Default is not necessary, also for checkbox its empty
    if (isempty(value))
        value = '';
    end
    if isnumeric(value),
        value=mat2str(value);
    end
    if strcmpi(pset(cnt1).style,'sound'),
        paramHandle(cnt1) = uicontrol('Style','pushbutton','String',value,...
            'UserData',DefValue,'HorizontalAlignment','center',...
            'BackgroundColor',[1 1 1],'position',[pos-[-185 23*(cnt1-1)] 220 20]);
        set(paramHandle(cnt1),'Callback',...
            'ParameterGUI(''buttSound_Callback'',gcbo,[],guidata(gcbo))');
    elseif strcmpi(pset(cnt1).style,'rawid'),
        paramHandle(cnt1) = uicontrol('Style','pushbutton','String',value,...
            'UserData',DefValue,'HorizontalAlignment','center',...
            'BackgroundColor',[1 1 1],'position',[pos-[-185 23*(cnt1-1)] 220 20]);
        set(paramHandle(cnt1),'Callback',...
            'ParameterGUI(''buttRawID_Callback'',gcbo,[],guidata(gcbo))');
    elseif strcmpi(pset(cnt1).style,'checkbox'),
        paramHandle(cnt1) = uicontrol('Style',pset(cnt1).style,'String',value,...
            'Value',DefValue,'HorizontalAlignment',Alignment,...
            'position',[pos-[-185 23*(cnt1-1)] 20 20]);
    else
        paramHandle(cnt1) = uicontrol('Style',pset(cnt1).style,'String',value,...
            'Value',DefValue,'HorizontalAlignment',Alignment,...
            'BackgroundColor',[1 1 1],'position',[pos-[-185 23*(cnt1-1)] 220 20]);
    end
    if isfield(pset,'tooltip') && ~isempty(pset(cnt1).tooltip),
        set(paramHandle(cnt1),'ToolTipString',pset(cnt1).tooltip);
    end
end
handles.paramHandle = paramHandle;
handles.pset = pset;
% Update handles structure
guidata(hObject, handles);
uiwait(handles.figure1);
% --- Outputs from this function are returned to the command line.
function varargout = ParameterGUI_OutputFcn(hObject, eventdata, handles)
if ~isempty(handles)
    varargout{1} = handles.output;
    close(handles.figure1);
end
% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% OK button
global Output;
paramHandle = handles.paramHandle;
if ~isempty(paramHandle)
    Output = cell(0);
    for cnt1 = 1:length(paramHandle)
        % now get the value:
        tempSt = get(handles.paramHandle(cnt1),'String');
        tempIn = get(handles.paramHandle(cnt1),'Value');
        % check to see if its an edit box:
        switch handles.pset(cnt1).style,
            case 'edit'
                value = (tempSt);
                if isnumeric(handles.pset(cnt1).default),
                    value=str2num(value);
                end
            case 'popupmenu'
                value=tempSt(tempIn,:);
            case 'checkbox'
                value = tempIn;
            case {'sound','rawid'},
                % get properties of sound object that have been saved to a
                % structure in UserData
                value=get(handles.paramHandle(cnt1),'UserData');
        end
        % update the field
        Output{cnt1} = value;
    end
end
handles.output = Output;
guidata(gcbo, handles);
uiresume;
% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% delete taken care of elsewhere?
handles.output = [];
guidata(gcbo, handles);
uiresume;
function buttSound_Callback(hObject, eventdata, handles)
to=SoundGUI(get(hObject,'UserData'),'Sound');
if ~isempty(to),
    set(hObject,'UserData',get(to));
    guidata(gcbo, handles);
end
function buttRawID_Callback(hObject, eventdata, handles)
rawdata=dbchooserawfile(1,'Choose existing raw file or cancel for new');
if ~isempty(rawdata),
    set(hObject,'UserData',rawdata.rawid);
    set(hObject,'String',basename(rawdata.parmfile));
else
    set(hObject,'UserData',-1);
    set(hObject,'String','New');
end
guidata(gcbo, handles);


