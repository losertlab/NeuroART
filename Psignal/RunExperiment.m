function varargout = RunExperiment
%This is where the experiment's sounds and behavior control occur
global home globalparams exptparams HW StopExperiment datapath
StopExperiment = 0;
BehaveObject = exptparams.BehaveObject;
exptevents = [];
ContinueExp = 1;
exptparams.TotalRepetitions = 0;
TrialIndex = 0;
exptparams.StartTime = clock;
exptparams.comment = [...
    'Experiment: ',class(exptparams.BehaveObject),' ',...
    'TrialObject: ',class(exptparams.TrialObject),' ',...
    'Primary Class: ',get(exptparams.TrialObject, 'PrimaryClass'),' ',...
    'Probe Class: ',get(exptparams.TrialObject, 'ProbeClass')];
%% Main loop
TrialObject = get(exptparams.TrialObject);
if isfield(TrialObject,'Timer')
    TrialObjTimer = get(exptparams.TrialObject,'Timer'); %Experiment delay timer. TrialObjTimer units = min
    if sum(TrialObjTimer)~=0
        t1 = datetime('now');
        t2 = datestr(t1 + minutes(TrialObjTimer(1)));
        disp(['Pausing ' num2str(TrialObjTimer(1)) ' minutes before continuing at ' t2])
        pause(TrialObjTimer(1)*60)
        disp(['Running Experiment for ' num2str(TrialObjTimer(2)) ' minutes'])
        TimerTic = tic;
    end
end
disp('Starting experiment...')
while ContinueExp == 1
    exptparams.TrialObject = ObjUpdate(exptparams.TrialObject);
    if ~ContinueExp,
        break;
    end
    %Set randomized parameters
    exptparams = RandomizeSequence(exptparams.TrialObject, exptparams, ...
        globalparams, 1, 1);
    %% Trial loop
    iTrial=0;
    while iTrial<get(exptparams.TrialObject,'NumberOfTrials')
        TrialIndex = TrialIndex+1; %Total trial counter
        disp(['Trial ' num2str(TrialIndex)])
        iTrial = iTrial+1;  %Within-rep trial counter
        exptparams.InRepTrials = iTrial;
        exptparams.TotalTrials = TrialIndex;
        %% PREPARE TRIAL
        [TrialSound, StimEvents, exptparams.TrialObject] = ...
            waveform(exptparams.TrialObject, iTrial);
        if ~isempty(HW.Calibration)
            HW.SoftwareAttendB = HW.Calibration.cdBSPL-min([HW.Calibration.cdBSPL get(exptparams.TrialObject, 'OveralldB')]);
        else
            HW.SoftwareAttendB = 0;
        end
        %Prepare additional triggers
        if strcmpi(globalparams.Physiology,'Phys')
            [StimEvents TrialSound] = TTLOutput(globalparams, exptparams, TrialSound, StimEvents);
        elseif HW.params.NumAOChan > 1
            for i = 2:HW.params.NumAOChan
                removeChannel(HW.AO,i) %Only need one analog output.
            end
            HW.params.NumAOChan = 1;
        end
        exptparams.LogDuration = LogDuration(BehaveObject, HW, StimEvents, ...
            globalparams, exptparams, TrialIndex);
        %% Match sound and DAQ durations
        IOSetAnalogInDuration(HW,exptparams.LogDuration);
        TrialSound(floor(exptparams.LogDuration.*HW.params.fsAO),end) = 0;
        drawnow;
        %% Behavioral-conditional experiment initiation
        exptparams = CanStart(BehaveObject, HW, StimEvents, globalparams, ...
            exptparams, TrialIndex);
        if StopExperiment
            ContinueExp = ContinueOrNot;
        end
        if ~ContinueExp
            TrialIndex = TrialIndex - 1;
            break
        end
        HW = IOLoadSound(HW, TrialSound);
        %% Begin experiment
        tic
        [StartEvent,HW] = IOStartAcquisition(HW);
        timerobj=[];
        [BehaviorEvents, exptparams, timerobj] = BehaviorControl(BehaveObject, ...
            HW, StimEvents, globalparams, exptparams, TrialIndex);
        [TrialStopEvent,HW] = IOStopAcquisition(HW);
        toc
        %% Collect trial data
        exptevents = AddMultiEvent(exptevents,{StartEvent,StimEvents,BehaviorEvents, ...
            TrialStopEvent},TrialIndex);
        ResponseData = [];
        if ~strcmpi(exptparams.BehaveObjectClass,'Passive')
            [ResponseData ResponseNames] = IOReadAIData(HW);
            Data=[];
            for i=1:length(ResponseNames)
                AIRange=get(HW.AI.Channels(i).Range);
                Data.(ResponseNames{i}) = round(ResponseData(:,i))>(AIRange.Max./sqrt(2));
            end
            exptparams.AnalogInputs = ResponseNames;
        else
            exptparams.AnalogInputs = 'None';
        end
        if ~strcmpi(exptparams.BehaveObjectClass,'Passive')
            %% Analyze performance and display
            exptparams = PerformanceAnalysis(BehaveObject, HW, StimEvents, ...
                globalparams, exptparams, TrialIndex, Data.Lick);
            exptparams = BehaviorDisplay(BehaveObject, HW, StimEvents, ...
                globalparams, exptparams, TrialIndex, Data.Lick,TrialSound);
        else
            exptparams = BehaviorDisplay(BehaveObject, HW, StimEvents, ...
                globalparams, exptparams, TrialIndex);
        end
        if ~isempty(timerobj) && strcmpi(timerobj.Running,'off')
            delete(timerobj)
        end
        if isfield(TrialObject,'Timer')
            TrialObjTimer = get(exptparams.TrialObject,'Timer'); %Experiment delay timer. TrialObjTimer units = min
            if sum(TrialObjTimer)~=0
                TimerToc = toc(TimerTic);
                if TimerToc >= TrialObjTimer(2)*60;
                    t1 = datetime('now');
                    t2 = datestr(t1 + minutes(TrialObjTimer(3)));
                    disp(['Pausing ' num2str(TrialObjTimer(3)) ' minutes before continuing at ' t2])
                    pause(TrialObjTimer(3)*60)
                    TimerTic = tic;
                end
            end
        end
        WriteMFile(globalparams,exptparams,exptevents)
    end
    exptparams.TotalRepetitions = exptparams.TotalRepetitions + 1;
    
    %% Pause switch
    pauseidx = 0;
    if HW.params.pause
        pauseidx = IOPauseRead(HW);
        if pauseidx == 1
            isrunning=1;
        else
            isrunning=0;
            disp('Paused....')
        end
        try
            save([home filesep 'PsignalRunning.mat'],'isrunning');
        end
        while pauseidx ==  0
            pauseidx = IOPauseRead(HW);
        end
    end
    %% Check if experiment should continue
    if ContinueExp && ~exptparams.ContinuousTraining
        ContinueExp = ContinueOrNot;
    end
end
exptparams.StopTime = clock;
if ~strcmpi(exptparams.BehaveObjectClass,'Passive')
    %% Update display
    exptparams = BehaviorDisplay(BehaveObject, HW, StimEvents, globalparams, ...
        exptparams, TrialIndex, [], []);
else
    delete(exptparams.Figure);
end
%% Shutdown pump
if ~strcmpi(exptparams.BehaveObjectClass,'Passive')
    try IOControlPump(HW,'stop')
        IOControlShock(HW,0,'stop');
    end
end
%% Remove pause files
if HW.params.pause
    if exist([home filesep 'PsignalRunning.mat'])
        delete([home filesep 'PsignalRunning.mat']);
    end
end
varargout{1} = exptevents;
varargout{2} = exptparams;
%% Local functions
function ContinueExp = ContinueOrNot
global StopExperiment globalparams
FP = get(0,'DefaultFigurePosition');
MP = get(0,'MonitorPosition');
SS = get(0,'ScreenSize');
set(0,'DefaultFigurePosition',[10,MP(4)/2-SS(2),FP(3:4)]);
UserInput = questdlg(['Continue ' globalparams.Rig ' experiment?']);
set(0,'DefaultFigurePosition',FP);
ContinueExp = strcmpi(UserInput, 'Yes') | strcmpi(UserInput,'Cancel');
if ContinueExp==1
    StopExperiment = 0;
end