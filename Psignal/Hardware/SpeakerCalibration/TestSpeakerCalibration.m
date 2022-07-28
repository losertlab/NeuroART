function TestSpeakerCalibration(varargin)
%% This program tests calibrates
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @TestSpeakerCalibration_OpeningFcn, ...
    'gui_OutputFcn',  @TestSpeakerCalibration_OutputFcn, ...
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
function TestSpeakerCalibration_OpeningFcn(hObject, eventdata, handles, varargin)
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
caller=dbstack;
global globalparams
globalparams.dBSPLRef=get(handles.dBSPLRef,'string');
globalparams.Fband=get(handles.FBand,'string');
handles.output = hObject;
guidata(hObject, handles);
function varargout = TestSpeakerCalibration_OutputFcn(hObject, eventdata, handles)
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
function start_Callback(hObject, eventdata, handles)
global globalparams HW home roothome
if isdeployed
    load([home '\PsignalConfig.mat'])
elseif ~isdeployed
    [FileName FilePath] = uigetfile([home '\*.mat'],'Load PsignalConfig File');
    load([FilePath '\' FileName])
end
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
RecDevNum = get(handles.RecDevice,'Value');
globalparams.RecDevID = get(handles.RecDevice,'String');
globalparams.RecDevID =globalparams.RecDevID{RecDevNum};
%Initializing hardware
disp('Initializing hardware...');
globalparams.WaterTrigger=0;
[HW globalparams] = ConfigNI(globalparams);
globalparams.Backtract = HW.Calibration.Backtract;
globalparams.VRef=globalparams.HWparams.HWAmpScale;
globalparams.highcut = 100; %high-pass corner frequency for recordings
globalparams.speaker=HW.Calibration.Speaker;
globalparams.microphone=HW.Calibration.Microphone;
globalparams.dBSPLRef=str2num(get(handles.dBSPLRef,'string'));
globalparams.Fband=str2num(get(handles.FBand,'string'));
removeChannel(HW.AI,1) %First channel is always for Lick--don't need here.
removeChannel(HW.AO,2) %Only need one analog output.
HW.AI.Rate = HW.AO.Rate;
HW.AI.NotifyWhenDataAvailableExceeds = 20000; 
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
globalparams.R = HW.Calibration;
fprintf(['\n ====== Testing Calibrated Speaker [ ',globalparams.speaker,' ] on DAQ Device ',globalparams.Device,' ===== \n']);
%% If background subtraction is selected, record silence and plot spectrum
if globalparams.Backtract
    globalparams.NSteps = round(globalparams.LStim*globalparams.SR);
    Signal = zeros(globalparams.NSteps,1);
    Signal = Signal(:);
    Signal = [zeros(globalparams.PreSteps,1); Signal; zeros(globalparams.PostSteps,1)];
    fprintf(['\n ====== Recording Background Noise ====== \n'])
    HW=IOSetAnalogInDuration(HW,length(Signal)/globalparams.SR);
    IOStopAcquisition(HW);
    IOLoadSound(HW, Signal);
    IOStartAcquisition(HW);
    wait(HW.AI,length(Signal)/globalparams.SR+1)
    IOStopAcquisition(HW);
    %Collect recorded calibration noise
    [AllData AINames] = IOReadAIData(HW);
    BackgroundNoise = AllData;
    Range = [globalparams.PreSteps+1:length(BackgroundNoise)-globalparams.PostSteps];
    ramp = hanning(round(.2 * HW.params.fsAO*2));
    BackgroundNoise(1:length(ramp)) = BackgroundNoise(1:length(ramp)) .* ramp;
    BackgroundNoise(end-length(ramp)+1:end) = BackgroundNoise(end-length(ramp)+1:end) .* flipud(ramp);
    globalparams.R.Background = BackgroundNoise;
    globalparams.Backtract = 1;
    globalparams.AxisOpt = {'FontSize',7,'FontName','Helvetica Neue','XGrid','on','YGrid','on','Box','on'};
    globalparams.AxisLabelOpt = {'FontSize',8,'FontName','Helvetica Neue'};
    globalparams.XTick = [100,1000,10000]; globalparams.XTickLabel = {'100','1000','10000'};
    figure
    movegui(gcf,'center')
    title('Background Noise',globalparams.AxisLabelOpt{:});
    hold on
    [b a]=butter(3,globalparams.highcut/(HW.params.fsAO/2),'high');
    RespSpec = abs(fft(filtfilt(b,a,BackgroundNoise)));
    f=linspace(0,HW.params.fsAO,length(RespSpec));
    RespSpec = 10*log10(RespSpec./max(RespSpec));
    plot(f,RespSpec)
    set(gca,'XTick',globalparams.XTick,'XTickLabel',globalparams.XTickLabel ,'XScale','log');
    ylabel('dB (re. max)',globalparams.AxisLabelOpt{:});
    xlim([globalparams.highcut HW.params.fsAO/2])
    aa=axis;
    ylim([-60 10])
    grid on
    xlabel('Frequency (Hz)');
    globalparams.R.Backtract=1;
end
%Test Calibration
TestCalibration
%% Plot results
figure(globalparams.FIG+1);
movegui(gcf,'center')
clf;
globalparams.AxisOpt = {'FontSize',7,'FontName','Helvetica Neue','XGrid','on','YGrid','on','Box','on'};
globalparams.AxisLabelOpt = {'FontSize',8,'FontName','Helvetica Neue'};
globalparams.XTick = [100,1000,10000]; globalparams.XTickLabel = {'100','1000','10000'};
title('Tonal Amplitude Response',globalparams.AxisLabelOpt{:});
hold on
plot(globalparams.R.FVolFM,globalparams.R.VolFM);
set(gca,'XTick',globalparams.XTick,'XTickLabel',globalparams.XTickLabel ,'XScale','log');
ylabel('dB SPL',globalparams.AxisLabelOpt{:});
xlim([globalparams.R.FFM(1),globalparams.R.FFM(end)]);
aa=axis;
ylim([aa(3)-6 aa(4)+6]);
grid on
xlabel('Frequency (Hz)');
delete(handles.figure1);
fprintf(['\n ====== Done! ======\n']);

