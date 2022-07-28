function SpeakerCalibration(varargin)

%Initialize GUI
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
%Find DAQ devices
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
end
guidata(handles.figure1, handles);
movegui(hObject,'center');
movegui(hObject,'onscreen');
handles.output = hObject;
guidata(hObject, handles);

function varargout = SpeakerCalibration_OutputFcn(hObject, eventdata, handles)
function start_Callback(hObject, eventdata, handles)
%Initialize variables and hardware
global globalparams HW home roothome Noise Response
globalparams.speakercalibration = 1;
RecDevNum = get(handles.RecDevice,'Value');
globalparams.RecDevID = get(handles.RecDevice,'String');
globalparams.RecDevID =globalparams.RecDevID{RecDevNum};
disp('Initializing hardware...');
globalparams.WaterTrigger=0;
[HW globalparams] = ConfigNI(globalparams);
globalparams.speaker=HW.Calibration.Speaker;
globalparams.microphone=HW.Calibration.Microphone;
globalparams.VRef=str2num(get(handles.VRef,'string'));
globalparams.dBSPLRef=str2num(get(handles.dBSPLRef,'string'));
globalparams.Fband=str2num(get(handles.FBand,'string'));
%First channel is always for Lick--don't need here, and only need one analog output
removeChannel(HW.AI,1)
removeChannel(HW.AO,2)
HW.AI.Rate = HW.AO.Rate;
HW.params.fsAI = HW.params.fsAO;
HW.params.NumAOChan = 1;
HW.AI.NotifyWhenDataAvailableExceeds = 20000;
if ~isdeployed
    SpeakerPath = home;
else
    SpeakerPath = [roothome '\' globalparams.Rig];
end
%Duration of calibration sounds
globalparams.LStim=10;
globalparams.SR=HW.params.fsAO;
%high-pass corner frequency for recordings
globalparams.highcut = 100;
globalparams.SpeakerPath = SpeakerPath;

%Record a broad-noise that is set to +/- the maximum output voltage (VRef)
fprintf(['\n ====== Calibrating Speaker [ ',globalparams.speaker,' ] on DAQ Device ',globalparams.Device,' ===== \n']);
globalparams.R=[];
globalparams.NSteps = round(globalparams.LStim*globalparams.SR);
Noise = wgn(1,globalparams.NSteps,1);
Noise = Noise(:);
ramp = hanning(round(.2 * HW.params.fsAO*2));
ramp = ramp(1:floor(length(ramp)/2));
Noise(1:length(ramp)) = Noise(1:length(ramp)) .* ramp;
Noise(end-length(ramp)+1:end) = Noise(end-length(ramp)+1:end) .* flipud(ramp);
norm = globalparams.VRef/max(abs(Noise));
Noise = norm * Noise;
fprintf(['\n ====== Calibration Noise Playing (',num2str(globalparams.LStim),' s) ====== \n'])
HW=IOSetAnalogInDuration(HW,length(Noise)/globalparams.SR);
IOStopAcquisition(HW);
IOLoadSound(HW, Noise);
IOStartAcquisition(HW);
wait(HW.AI,length(Noise)/globalparams.SR+1)
IOStopAcquisition(HW);
[AllData AINames] = IOReadAIData(HW);
Response = AllData;
Response(1:length(ramp)) = Response(1:length(ramp)) .* ramp;
Response(end-length(ramp)+1:end) = Response(end-length(ramp)+1:end) .* flipud(ramp);
fprintf(['\n ====== Done Recording ======\n']);
globalparams.Fig=figure('position',[177   216   735   745]);
set(gcf,'Name',['Speaker: ',globalparams.speaker,' (SR=',num2str(globalparams.SR),'Hz)'],'MenuBar','none','Toolbar','figure');

%Estimate the whitening filter
EstimateTF
uicontrol('style','Pushbutton','String','Record Noise','Value',0,...
    'Units','normalized','Pos',[.7,.85,.2,.06],'Callback',@RecordNoise);
uicontrol('style','Togglebutton','String','Set Amplifier Gain','Value',0,...
    'Units','normalized','Pos',[.7,.78,.2,.06],'Callback',@Gain);
uicontrol('style','Pushbutton','String','Test Calibration','Value',0,...
    'Units','normalized','Pos',[.7,.71,.2,.06],'Callback',@TestCalib);
uicontrol('style','Pushbutton','String','Save','Value',0,...
    'Units','normalized','Pos',[.7,.64,.2,.06],'Callback',@savecalb);

%Record a range of tone frequencies set to +/- the maximum output voltage (VRef)
function TestCalib(source,callbackdata)
TestCalibration

%Estimate amplifier gain
function Gain(source,callbackdata)
EstimateGain

%Record a broad-noise that is set to +/- the maximum output voltage (VRef)
function RecordNoise(source,callbackdata)
global globalparams HW Response Noise
fprintf(['\n ====== Calibration Noise Playing (',num2str(globalparams.LStim),' s) ====== \n'])
HW=IOSetAnalogInDuration(HW,length(Noise)/globalparams.SR);
IOStopAcquisition(HW);
IOLoadSound(HW, Noise);
IOStartAcquisition(HW);
wait(HW.AI,length(Noise)/globalparams.SR+1)
IOStopAcquisition(HW);
[AllData AINames] =IOReadAIData(HW);
Response = AllData;
ramp = hanning(round(.2 * HW.params.fsAO*2));
ramp = ramp(1:floor(length(ramp)/2));
Response(1:length(ramp)) = Response(1:length(ramp)) .* ramp;
Response(end-length(ramp)+1:end) = Response(end-length(ramp)+1:end) .* flipud(ramp);
fprintf(['\n ====== Done Recording ======\n']);
EstimateTF

%Save calibration
function savecalb(source,callbackdata)
global globalparams
FileName = [globalparams.SpeakerPath,'\SpeakerCalibration_',globalparams.speaker,'_',globalparams.microphone,'_',globalparams.Rig,'.mat'];
globalparams.R.VRef = globalparams.VRef;
globalparams.R.dBSPLRef = globalparams.dBSPLRef;
R = globalparams.R;
fprintf(['\n ====== Saving Calibration ======\n']);
save(FileName,'R');
globalparams = rmfield(globalparams,'speakercalibration');
fprintf(['\n ====== Done! ======\n']);

%Callback functions
function RecDevice_Callback(hObject, eventdata, handles)
function RecDevice_CreateFcn(hObject, eventdata, handles)
function speaker_Callback(hObject, eventdata, handles)
function speaker_CreateFcn(hObject, eventdata, handles)
function microphone_Callback(hObject, eventdata, handles)
function microphone_CreateFcn(hObject, eventdata, handles)
function device_Callback(hObject, eventdata, handles)
function device_CreateFcn(hObject, eventdata, handles)
function dBSPLRef_Callback(hObject, eventdata, handles)
function dBSPLRef_CreateFcn(hObject, eventdata, handles)
function FBand_Callback(hObject, eventdata, handles)
function FBand_CreateFcn(hObject, eventdata, handles)
function VRef_Callback(hObject, eventdata, handles)
function VRef_CreateFcn(hObject, eventdata, handles)
function home_Callback(hObject, eventdata, handles)
function home_CreateFcn(hObject, eventdata, handles)



