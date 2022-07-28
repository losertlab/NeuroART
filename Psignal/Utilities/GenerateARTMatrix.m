%This script generates Psignal matrices, P, which are Time-index x Trial x M,
%where M is a task-related feature. P(i,t,m) is equal to eqither 1, 0, time value
%in seconds, or feature value, with 1 indicating  membership of a given feature
%or time value of event. Lick have a value of 1 whenever a lick occured.
%This script is specified for the Auditory Recognition Task (ART) object.
for e = 1:length(input.expname)
    %Effective frame rate for imaging.
    fps = input.expectedFPS;
    PsignalFile = input.psignalfiles{e};
    ExpName = input.expname{e};
    OriginalPath = [input.path ExpName '\'];
    bb = strsep(OriginalPath,'\');
    NewPath = [input.savepath input.animal '\' bb{5} '\' bb{6} '\'];
    SavePath = [NewPath ExpName '\'];
    PsignalFile=[NewPath 'Psignal\' input.psignalfiles{e}];
    load(PsignalFile);
    Primary = get(get(exptparams.TrialObject,'PrimaryHandle'));
        disp(['Processing ' PsignalFile])
        if strcmpi(input.PhysType,'2P')
            addpath(genpath('C:\Users\nfu\Dropbox\Code\2P'))
            %Extract timing parameters
            ThorFile = [NewPath input.expname{e} '\timing.txt'];
            TimingInfo = getTimingInfo(ThorFile, PsignalFile, fps);
            %Find tone onset and offset frames, based on counting #frames that occur before/after tone onset
            StimOnset = [];
            StimOffset=[];
            StimOnsetTime = Primary.PreStimSilence;
            StimOffsetTime = Primary.PreStimSilence+Primary.Duration;
            for mm = 1:size(TimingInfo.FrameTiming,2)
                tmp = TimingInfo.FrameTiming(:,mm)<StimOnsetTime;
                tmptmp = find(tmp>0);
                StimOnset = [StimOnset tmptmp(end)+1];
                tmp = TimingInfo.FrameTiming(:,mm)<=StimOffsetTime;
                tmptmp = find(tmp>0);
                StimOffset = [StimOffset tmptmp(end)];
                clear tmp tmptmp
            end
        end
        TotalTrials = exptevents(end).Trial-length(TimingInfo.rejectedtrials);
        TrialLen = exptevents(end).StopTime;
        TrialObject =get(exptparams.TrialObject);
                OveralldB = TrialObject.OveralldB;
                %%% Trialobject dependent features
               if ~isempty(strfind(PsignalFile,'ART'))     
        BehaviorObject = get(exptparams.BehaveObject);
        EarlyWindowOnset = ones(1,length(StimOnset));
        EarlyWindowOffset = StimOnset-1;
        ResponseWindowOnset = StimOnset+(BehaviorObject.EarlyWindow(2)*fps);
        ResponseWindowOffset =  ResponseWindowOnset+diff(BehaviorObject.ResponseWindow)*fps;
        %Task-related features
        PsignalMatrix=[];
        for i = 1:20
            PsignalMatrix.TagNames{i,1} = i;
        end
        PsignalMatrix.TagNames{1,2} = 'Hit';
        PsignalMatrix.TagNames{2,2} = 'Miss';
        PsignalMatrix.TagNames{3,2} = 'FalseAlarm';
        PsignalMatrix.TagNames{4,2} = 'CorrectRejection';
        PsignalMatrix.TagNames{5,2} = 'Early';
        PsignalMatrix.TagNames{6,2} = 'FirstResponse';
        PsignalMatrix.TagNames{7,2} = 'TargetLick';
        PsignalMatrix.TagNames{8,2} = 'NonTargetLick';
        PsignalMatrix.TagNames{9,2} = 'ProbeLick';
        PsignalMatrix.TagNames{10,2} = 'StimOnset';
        PsignalMatrix.TagNames{11,2} = 'StimOffset';
        PsignalMatrix.TagNames{12,2} = 'StimFrequency';
        PsignalMatrix.TagNames{13,2} = 'StimLevel';
        PsignalMatrix.TagNames{14,2} = 'Target';
        PsignalMatrix.TagNames{15,2} = 'NonTarget';
        PsignalMatrix.TagNames{16,2} = 'Probe';
        PsignalMatrix.TagNames{17,2} = 'EarlyWindowOnset';
        PsignalMatrix.TagNames{18,2} = 'EarlyWindowOffset';
        PsignalMatrix.TagNames{19,2} = 'ResponseWindowOnset';
        PsignalMatrix.TagNames{20,2} = 'ResponseWindowOffset';
        %Preallocation of P
        PsignalMatrix.Tags = zeros(fps*TrialLen,TotalTrials,size(PsignalMatrix.TagNames(:,2),1));
        %Performance
        if isfield(exptparams,'Performance')
            for i = 1:exptevents(end).Trial
                if isempty(intersect(TimingInfo.rejectedtrials,i))
                    switch exptparams.Performance(i).ThisTrial
                        case 'Hit'
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Hit'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                        case 'Miss'
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Miss'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                        case 'FalseAlarm'
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'FalseAlarm'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                        case 'CorrectReject'
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'CorrectReject'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                        case 'Early'
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Early'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                    end
                end
            end
            %First response
            for i = 1:exptevents(end).Trial
                if isempty(intersect(TimingInfo.rejectedtrials,i))
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'FirstResponse'));
                    if ~isnan(exptparams.FirstResponse(i,1))
                        PsignalMatrix.Tags(max([1 ceil(exptparams.FirstResponse(i,1).*fps)]),i,idx) = 1;
                    end
                end
            end
            %Licking
            [t,trial,Note,toff,StimIndex] = evtimes(exptevents,'Stim*');
            Ti=0;
            Pi=0;
            NTi=0;
            for i = 1:exptevents(end).Trial
                if isempty(intersect(TimingInfo.rejectedtrials,i))
                    bb = strsep(exptevents(StimIndex(i)).Note,',');
                    StimType = bb{3}(1:end-1);
                    switch StimType
                        case 'Target'
                            Ti = Ti+1;
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Target'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                            ResponseData = [0 max(0,diff(exptparams.AverageResponse.tar(Ti,:)))];
                            ResponseData =max(0, ResponseData./max(ResponseData));
                            Lfs = globalparams.HWparams.fsAI;
                            t=0:1/Lfs:(length(ResponseData)/Lfs)-(1/Lfs);
                            ResponseFrames=max(1,floor(t(find(ResponseData)).*fps));
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'TargetLick'));
                            PsignalMatrix.Tags(ResponseFrames,i,idx) = 1;
                        case 'NonTarget'
                            NTi = NTi+1;
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'NonTarget'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                            ResponseData = [0 max(0,diff(exptparams.AverageResponse.nontar(NTi,:)))];
                            ResponseData =max(0, ResponseData./max(ResponseData));
                            Lfs = globalparams.HWparams.fsAI;
                            t=0:1/Lfs:(length(ResponseData)/Lfs)-(1/Lfs);
                            ResponseFrames=max(1,floor(t(find(ResponseData)).*fps));
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'NonTargetLick'));
                            PsignalMatrix.Tags(ResponseFrames,i,idx) = 1;
                        case 'Probe'
                            Pi = Pi+1;
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Probe'));
                            PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                            ResponseData = [0 max(0,diff(exptparams.AverageResponse.probe(Pi,:)))];
                            ResponseData =max(0, ResponseData./max(ResponseData));
                            Lfs = globalparams.HWparams.fsAI;
                            t=0:1/Lfs:(length(ResponseData)/Lfs)-(1/Lfs);
                            ResponseFrames=max(1,floor(t(find(ResponseData)).*fps));
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'ProbeLick'));
                            PsignalMatrix.Tags(ResponseFrames,i,idx) = 1;
                    end
                end
            end
            %Behavior Windows
            for i = 1:exptevents(end).Trial
                if isempty(intersect(TimingInfo.rejectedtrials,i))
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'EarlyWindowOnset'));
                    PsignalMatrix.Tags(EarlyWindowOnset(i),i,idx) = 1;
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'EarlyWindowOffset'));
                    PsignalMatrix.Tags(EarlyWindowOffset(i),i,idx) = 1;
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'ResponseWindowOnset'));
                    PsignalMatrix.Tags(ResponseWindowOnset(i),i,idx) = 1;
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'ResponseWindowOffset'));
                    PsignalMatrix.Tags(ResponseWindowOffset(i),i,idx) = 1;
                end
            end
        end
        %Stimulus
        [t,trial,Note,toff,StimIndex] = evtimes(exptevents,'Stim*');
        for i = 1:exptevents(end).Trial
            if isempty(intersect(TimingInfo.rejectedtrials,i))
                bb = strsep(exptevents(StimIndex(i)).Note,',');
                if isempty(strfind(Note{i},'Silence'))
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOnset'));
                    PsignalMatrix.Tags(StimOnset(i),i,idx) = 1;
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOffset'));
                    PsignalMatrix.Tags(StimOffset(i),i,idx) = 1;
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimFrequency'));
                    PsignalMatrix.Tags(StimOnset(i),i,idx) = bb{2};
                    idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimLevel'));
                    PsignalMatrix.Tags(StimOnset(i),i,idx) = OveralldB-bb{end};
                end
            end
        end
        save([SavePath 'PsignalMatrix.mat'],'PsignalMatrix')
    end
end