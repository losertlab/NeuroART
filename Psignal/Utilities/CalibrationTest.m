function CalibrationTest
%% This program tests speaker calibration
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @SpeakerCalibration_OpeningFcn, ...
    'gui_OutputFcn',  @SpeakerCalibration_OutputFcn, ...
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
function SpeakerCalibration_OpeningFcn(hObject, eventdata, handles, varargin)
dev=daq.getDevices;
Devices=[];
for i = 1:length(dev)
    if strcmpi(dev(i).Vendor.ID,'ni')
        Devices = [Devices; {dev(i).ID}];
    end
end
if isempty(Devices)
    warning('DAQ device not found')
    return
else
    set(handles.RecDevice,'String',Devices);
    guidata(handles.figure1, handles);
end
movegui(hObject,'center');
movegui(hObject,'onscreen');
caller=dbstack;
global globalparams
set(hObject,'Name',globalparams.Rig);
globalparams.VRef=get(handles.VRef,'value');
globalparams.dBSPLRef=get(handles.dBSPLRef,'string');
globalparams.Fband=get(handles.FBand,'string');
handles.output = hObject;
guidata(hObject, handles);


global globalparams HW home
load([home '\PsignalConfig.mat'])
globalparams.Rig = rig;
globalparams.disploc = disploc;
dev=daq.getDevices;
Devices=[];
for i = 1:length(dev)
    if strfind(dev(i).ID,DevID);
        Devices = [Devices; dev(i).ID];
    end
end
if isempty(Devices)
    warning('DAQ device not found')
    return
end
globalparams.Device = Devices;
globalparams.speakercalibration = 1;
RecDevNum = get(handles.RecDevice,'Value');
globalparams.RecDevID = get(handles.RecDevice,'String');
globalparams.RecDevID =globalparams.RecDevID{RecDevNum};
%Initializing hardware
disp('Initializing hardware...');
[HW globalparams] = ConfigNI(globalparams);
globalparams.speaker=HW.Calibration.Speaker;
globalparams.microphone=HW.Calibration.Microphone;
globalparams.VRef=str2num(get(handles.VRef,'string'));
globalparams.dBSPLRef=str2num(get(handles.dBSPLRef,'string'));
globalparams.Fband=str2num(get(handles.FBand,'string'));
removeChannel(HW.AI,1) %First channel is always for Lick--don't need here.
removeChannel(HW.AO,2) %Only need one analog output.
HW.AI.Rate = HW.AO.Rate;
HW.AIch(2).Range = [-2 2];
HW.params.fsAI = HW.params.fsAO;
HW.params.NumAOChan = 1;
SpeakerPath = home;
globalparams.LStim=30; %Duration of initial noise
globalparams.SR=HW.params.fsAO;
globalparams.PreDur=0.05; %Duration of silence before noise
globalparams.PostDur=0.05; %Duration of silence after noise
globalparams.FIG=1;
globalparams.RampDur = 0.005; %Calibration sound ramps
globalparams.SpeakerPath = SpeakerPath;
globalparams.ImpRespDur = 0.1; %Max duration of impulse response to keep
globalparams.NFFT = round(globalparams.ImpRespDur*globalparams.SR);
globalparams.Colors = struct('Signal',[0,0,0],'Response',[1,0,0],'Filter',[0,0,1]);
globalparams.PreSteps = round(globalparams.PreDur*globalparams.SR);
globalparams.PostSteps = round(globalparams.PostDur*globalparams.SR);
fprintf(['\n ====== Calibrating Speaker [ ',globalparams.speaker,' ] on DAQ Device ',globalparams.Device,' ===== \n']);