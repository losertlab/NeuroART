function [HW, globalparams] = ConfigNI (globalparams)
%ConfigHW initializes the hardware based on which rig is used
% Nikolas A. Francis 2018

%Shutdown any running hardware
ShutdownHW([]);

%Default parameters
HW=[];
HW.params.DAQ = 'ni';
HW.params.fsAO=200000;
HW.params.fsAI=1000;
HW.params.PhysTrigDur=0.01;

%NI board ID
DAQID = globalparams.Device;
daq.reset;

%Analog input setup
HW.AI = daq.createSession('ni');
HW.AI.Rate=HW.params.fsAI;
HW.AI.TriggersPerRun = 1;
if isfield(globalparams,'RecDevID')
    HW.AIch(1)=addAnalogInputChannel(HW.AI, globalparams.RecDevID, 0, 'Voltage');
    HW.AIch(2)=addAnalogInputChannel(HW.AI, globalparams.RecDevID, 1, 'Voltage');
else
    HW.AIch(1)=addAnalogInputChannel(HW.AI, DAQID, 0, 'Voltage');
    HW.AIch(2)=addAnalogInputChannel(HW.AI, DAQID, 1, 'Voltage');
end
HW.AIch(1).Name = 'Lick';
HW.AIch(2).Name = 'Microphone';
HW.AIch(1).Range = [-5 5];
HW.AIch(2).Range = [-5 5];
HW.AIch(1).TerminalConfig = 'SingleEnded';
HW.AIch(2).TerminalConfig = 'SingleEnded';

%NotifyWhenDataAvailableExceeds parameter determines how fast the AI data
%index is updated. In other words, it determines the online AI sampling
%period = HW.AI.NotifyWhenDataAvailableExceeds/HW.params.fsAI. Here we use
%a 200 Hz online AI sampling rate vs. 1 khz aquisition rate
HW.AI.NotifyWhenDataAvailableExceeds = 5;
HW.params.fsAIonline=double(HW.params.fsAI/HW.AI.NotifyWhenDataAvailableExceeds);
warning('off', 'daq:Session:tooFrequent')

%Analog output setup
HW.AO = daq.createSession('ni');
HW.AO.Rate=HW.params.fsAO;
HW.AO.TriggersPerRun = 1;
HW.AOch(1)=addAnalogOutputChannel(HW.AO, DAQID, 0, 'Voltage');
HW.AOch(2)=addAnalogOutputChannel(HW.AO, DAQID, 1, 'Voltage');
HW.AOch(1).Name = 'SoundOut';
HW.AOch(2).Name = 'PhysTrig';
HW.AOch(1).Range = [-10 10];
HW.AOch(2).Range = [-10 10];
HWAmpScale = get(HW.AO.Channels(1).Range);
HW.params.HWAmpScale = HWAmpScale.Max;
HW.params.NumAOChan = 2;
HW.DIO = daq.createSession('ni');

%Digital IO setup
warning('off', 'daq:Session:onDemandOnlyChannelsAdded')
%Set DIO and trigger ports/lines by NI device
if ~isempty(strfind(DAQID, '6215')) && isempty(strfind(DAQID, 'PsiBox'))
    HW.DIOch(1:2)=addDigitalChannel(HW.DIO, DAQID, 'Port1/Line0:1', 'OutputOnly');
    HW.DIOch(3)=addDigitalChannel(HW.DIO, DAQID, 'Port1/Line2', 'OutputOnly');
    HW.DIOch(4)=addDigitalChannel(HW.DIO, DAQID, 'Port0/Line2', 'InputOnly');
    addTriggerConnection(HW.AI,'External',{[DAQID '/PFI0']},'StartTrigger');
    HW.AI.Connections.TriggerCondition = 'RisingEdge';
    addTriggerConnection(HW.AO,'External',{[DAQID '/PFI1'],},'StartTrigger');
    HW.AO.Connections.TriggerCondition = 'RisingEdge';
    HW.params.pause = 0;
    HW.DIOch(1).Name = 'TrigAI';
    HW.DIOch(2).Name = 'TrigAO';
    HW.DIOch(3).Name = 'Solenoid';
    HW.DIOch(4).Name = 'Lick';
    HW.params.LineReset = [0 0 0];
    HW.DIO.outputSingleScan([HW.params.LineReset]);
    HW.params.LineReset = [0 0 0];
    HW.params.LineTrigger = [1 1 0];
    HW.Calibration.Speaker = 'ES1';
    HW.Calibration.Microphone = 'ANL9401';
elseif ~isempty(strfind(DAQID, '6343'))
    %Add additional AO channel
    HW.AOch(3)=addAnalogOutputChannel(HW.AO, DAQID, 2, 'Voltage');
    HW.AOch(3).Name = 'OptTrig1';
    HW.AOch(3).Range = [-10 10];
    HW.params.NumAOChan = 3;
    %Ports 0 is InputOnly. Ports 1 and 2 are OutputOnly.
    HW.DIOch(1:2)=addDigitalChannel(HW.DIO, DAQID, 'Port2/Line1:2', 'OutputOnly');
    HW.DIOch(3)=addDigitalChannel(HW.DIO, DAQID, 'Port2/Line3', 'OutputOnly');
    HW.DIOch(4)=addDigitalChannel(HW.DIO, DAQID, 'Port0/Line24', 'InputOnly');
    addTriggerConnection(HW.AI,'External',{[DAQID '/PFI12']},'StartTrigger');
    HW.AI.Connections.TriggerCondition = 'RisingEdge';
    addTriggerConnection(HW.AO,'External',{[DAQID '/PFI13'],},'StartTrigger');
    HW.AO.Connections.TriggerCondition = 'RisingEdge';
    HW.AIch(1).TerminalConfig = 'Differential';
    HW.AIch(2).TerminalConfig = 'Differential';
    HW.params.pause = 0;
    HW.DIOch(1).Name = 'TrigAI';
    HW.DIOch(2).Name = 'TrigAO';
    HW.DIOch(3).Name = 'Solenoid';
    HW.DIOch(4).Name = 'Lick';
    HW.params.LineReset = [0 0 0];
    HW.DIO.outputSingleScan([HW.params.LineReset]);
    HW.params.LineReset = [0 0 0];
    HW.params.LineTrigger = [1 1 0];
    HW.Calibration.Speaker = 'ES1';
    HW.Calibration.Microphone = 'ANL9401';
elseif ~isempty(strfind(DAQID, '6251'))
    %Ports 0 is InputOnly. Ports 1 and 2 are OutputOnly.
    HW.DIOch(1:2)=addDigitalChannel(HW.DIO, DAQID, 'Port2/Line2:3', 'OutputOnly');
    HW.DIOch(3)=addDigitalChannel(HW.DIO, DAQID, 'Port2/Line0', 'OutputOnly');
    HW.DIOch(4)=addDigitalChannel(HW.DIO, DAQID, 'Port0/Line0', 'InputOnly');
    addTriggerConnection(HW.AI,'External',{[DAQID '/PFI1']},'StartTrigger');
    HW.AI.Connections.TriggerCondition = 'RisingEdge';
    addTriggerConnection(HW.AO,'External',{[DAQID '/PFI2'],},'StartTrigger');
    HW.AO.Connections.TriggerCondition = 'RisingEdge';
    addCounterOutputChannel(HW.AO,DAQID, 'ctr1', 'PulseGeneration');
    HW.AIch(1).TerminalConfig = 'Differential';
    HW.AIch(2).TerminalConfig = 'Differential';
    HW.params.pause = 0;
    HW.DIOch(1).Name = 'TrigAI';
    HW.DIOch(2).Name = 'TrigAO';
    HW.DIOch(3).Name = 'Solenoid';
    HW.DIOch(4).Name = 'Lick';
    HW.params.LineReset = [0 0 0];
    HW.DIO.outputSingleScan([HW.params.LineReset]);
    HW.params.LineReset = [0 0 0];
    HW.params.LineTrigger = [1 1 0];
    HW.Calibration.Speaker = 'ES1';
    HW.Calibration.Microphone = 'ANL9401';
elseif ~isempty(strfind(DAQID, '6351'))
    %Ports 0 is InputOnly. Ports 1 and 2 are OutputOnly.
    HW.DIOch(1:2)=addDigitalChannel(HW.DIO, DAQID, 'Port2/Line1:2', 'OutputOnly');
    HW.DIOch(3)=addDigitalChannel(HW.DIO, DAQID, 'Port2/Line0', 'OutputOnly');
    HW.DIOch(4)=addDigitalChannel(HW.DIO, DAQID, 'Port0/Line0', 'InputOnly');
    addTriggerConnection(HW.AI,'External',{[DAQID '/PFI1']},'StartTrigger');
    HW.AI.Connections.TriggerCondition = 'RisingEdge';
    addTriggerConnection(HW.AO,'External',{[DAQID '/PFI2'],},'StartTrigger');
    HW.AO.Connections.TriggerCondition = 'RisingEdge';
    addCounterOutputChannel(HW.AO,DAQID, 'ctr1', 'PulseGeneration');
    HW.AIch(1).TerminalConfig = 'Differential';
    HW.AIch(2).TerminalConfig = 'SingleEnded';
    HW.params.pause = 0;
    HW.DIOch(1).Name = 'TrigAI';
    HW.DIOch(2).Name = 'TrigAO';
    HW.DIOch(3).Name = 'Solenoid';
    HW.DIOch(4).Name = 'Lick';
    HW.params.LineReset = [0 0 0];
    HW.DIO.outputSingleScan([HW.params.LineReset]);
    HW.params.LineReset = [0 0 0];
    HW.params.LineTrigger = [1 1 0];
    HW.Calibration.Speaker = 'ES1';
    HW.Calibration.Microphone = 'ANL9401';
elseif ~isempty(strfind(lower(DAQID), 'psibox'))
    HW.DIOch(1:3)=addDigitalChannel(HW.DIO, DAQID, 'Port1/Line0:2', 'OutputOnly');
    HW.DIOch(4:7)=addDigitalChannel(HW.DIO, DAQID, 'Port0/Line0:3', 'InputOnly');
    HW.DIOch(8)=addDigitalChannel(HW.DIO, DAQID, 'Port1/Line3', 'OutputOnly');
    HW.params.pause = 1;
    HW.DIOch(3).Name = 'Solenoid';
    HW.DIOch(4).Name = 'Lick1';
    HW.DIOch(5).Name = 'Lick2';
    if HW.params.pause
        HW.DIOch(6).Name = 'Running';
    end
    HW.DIOch(8).Name = 'LED';
    HW.params.LineReset = [0 0 0 0];
    HW.DIO.outputSingleScan([HW.params.LineReset]);
    HW.params.LineTrigger = [];
    HW.Calibration.Speaker = 'ES1';
    HW.Calibration.Microphone = 'ANL9401';
end

%Load Speaker Calibration
if ~isfield(globalparams,'speakercalibration') && ~isfield(HW.Calibration,'cdBSPL') && ~globalparams.WaterTrigger
    HW.Calibration = IOLoadSpeakerCalibration(HW.Calibration);
end

%Assign HW params to globalparams
globalparams.HWparams = HW.params;

