function [HW, globalparams] = ConfigAudio (globalparams)
%ConfigOS initializes the hardware to use the OS audio for sound output and "lick" input via microphone.
ShutdownHW([]);
HW=[];
HW.params.DAQ = 'audio';
daq.reset;
HW.params.fsAO=50000;
HW.params.PhysTrigDur=0.001;
soundin = 'Audio0'; %Check daq.getDevices to see which audio device to use
soundout = 'Audio2'; %Check daq.getDevices to see which audio device to use
%% Analog IO
HW.params.AInames = 'Lick';
HW.AI = daq.createSession('directsound');
HW.AO = daq.createSession('directsound');
addAudioOutputChannel(HW.AO, soundout, 1);
addAudioOutputChannel(HW.AO, soundout, 2);
addAudioInputChannel(HW.AI, soundin, 1);
HW.AI.Channels.Name = 'Lick';
HW.AI.Rate=max([1000 min(HW.AI.RateLimit)]);
HW.params.fsAI = HW.AI.Rate;
HW.AO.Channels(1).Name = 'SoundOut';
HW.AO.Channels(2).Name = 'PhysTrig';
HW.AO.Rate=HW.params.fsAO;
HW.AI.Rate=HW.params.fsAI;
HWAmpScale = get(HW.AO.Channels(1).Range);
HW.params.LineTrigger = [];
HW.params.HWAmpScale = HWAmpScale.Max;
HW.params.NumAOChan = 2;
%% Assign HW params to globalparams
globalparams.HWparams = HW.params;
HW.Calibration = [];
%% Pause switch
HW.params.pause = 0;
