function PsignalMatrix = GeneratePsignalMatrix(input)
%This script generates Psignal matrices, P, which are Time-index x Trial x M,
%where M is a task-related feature. P(i,t,m) is equal to eqither 1, 0, time value
%in seconds, or feature value, with 1 indicating  membership of a given feature
%or time value of event. Lick have a value of 1 whenever a lick occured.
fps = input.expectedFPS;
for e = 1:length(input.expname)
    PsignalFile = input.psignalfiles{e};
    if ~isempty(input.path)
        bb = strsep(input.path,'\');
        SavePath = input.savepath;
        SavePath = [SavePath input.animal '\' bb{5} '\' bb{6} '\' input.expname{e} '\'];
        PsignalFile=[input.path 'Psignal\' PsignalFile];
    end
    PsignalData = LoadPsignalParams(PsignalFile);
    globalparams = PsignalData.globalparams;
    disp(['Processing ' PsignalFile])
    TotalTrials = PsignalData.exptevents(end).Trial;
    TrialLen = PsignalData.exptevents(end).StopTime;
    OveralldB = PsignalData.TrialObject.OveralldB;
    StimOnset = ceil(PsignalData.Primary.PreStimSilence*fps)+1;
    StimOffset = ceil(PsignalData.Primary.PreStimSilence+PsignalData.Primary.Duration)*fps;
    %Trialobject dependent features
    if ~isempty(strfind(PsignalFile,'SHP'))
        BehaviorObject = PsignalData.Behavior;
        %Features
        PsignalMatrix=[];
        for i = 1:7
            PsignalMatrix.TagNames{i,1} = i;
        end
        PsignalMatrix.TagNames{1,2} = 'Hit';
        PsignalMatrix.TagNames{2,2} = 'Miss';
        PsignalMatrix.TagNames{3,2} = 'FirstResponse';
        PsignalMatrix.TagNames{4,2} = 'TargetLick';
        PsignalMatrix.TagNames{5,2} = 'StimOnset';
        PsignalMatrix.TagNames{6,2} = 'StimOffset';
        PsignalMatrix.TagNames{7,2} = 'StimLevel';
        %Preallocation of P
        PsignalMatrix.Tags = zeros(floor(fps*TrialLen),TotalTrials,size(PsignalMatrix.TagNames(:,2),1));
        %Performance
        if isfield(PsignalData.exptparams,'Performance')
            for i = 1:PsignalData.exptevents(end).Trial
                switch PsignalData.exptparams.Performance(i).ThisTrial
                    case 'Hit'
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Hit'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                    case 'Miss'
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Miss'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                end
            end
            %First response
            for i = 1:PsignalData.exptevents(end).Trial
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'FirstResponse'));
                if ~isnan(PsignalData.exptparams.FirstResponse(i,1))
                    PsignalMatrix.Tags(max([1 ceil(PsignalData.exptparams.FirstResponse(i,1).*fps)]),i,idx) = 1;
                end
            end
            %Licking
            [t,trial,Note,toff,StimIndex] = evtimes(PsignalData.exptevents,'Stim*');
            Ti=0;
            Pi=0;
            NTi=0;
            for i = 1:PsignalData.exptevents(end).Trial
                bb = strsep(PsignalData.exptevents(StimIndex(i)).Note,',');
                StimType = bb{3}(1:end);
                switch StimType
                    case 'Target'
                        Ti = Ti+1;
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Target'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                        ResponseData = PsignalData.exptparams.AverageResponse.tar(Ti,:);
                        Lfs = globalparams.HWparams.fsAI;
                        t=0:1/Lfs:(length(ResponseData)/Lfs)-(1/Lfs);
                        ResponseFrames=max(1,round(t(find(ResponseData)).*fps));
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'TargetLick'));
                        PsignalMatrix.Tags(ResponseFrames,i,idx) = 1;
                end
            end
        end
        %Stimulus
        [t,trial,Note,toff,StimIndex] = evtimes(PsignalData.exptevents,'Stim*');
        for i = 1:PsignalData.exptevents(end).Trial
            bb = strsep(PsignalData.exptevents(StimIndex(i)).Note,',');
            if isempty(strfind(Note{i},'Silence'))
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOnset'));
                PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOffset'));
                PsignalMatrix.Tags(StimOffset,i,idx) = 1;
            end
        end
    elseif ~isempty(strfind(PsignalFile,'ART'))
        BehaviorObject = PsignalData.Behavior;
        EarlyWindowOnset = StimOnset+floor(BehaviorObject.EarlyWindow(1)*fps);
        EarlyWindowOffset = StimOnset+floor(BehaviorObject.EarlyWindow(2)*fps)-1;
        ResponseWindowOnset = EarlyWindowOffset+1;
        ResponseWindowOffset =  ResponseWindowOnset+diff(BehaviorObject.ResponseWindow)*fps-1;
        %Features
        PsignalMatrix=[];
        for i = 1:21
            PsignalMatrix.TagNames{i,1} = i;
        end
        PsignalMatrix.TagNames{1,2} = 'Early';
        PsignalMatrix.TagNames{2,2} = 'Hit';
        PsignalMatrix.TagNames{3,2} = 'Miss';
        PsignalMatrix.TagNames{4,2} = 'FalseAlarm';
        PsignalMatrix.TagNames{5,2} = 'CorrectReject';
        PsignalMatrix.TagNames{6,2} = 'EarlyFalseAlarm';
        PsignalMatrix.TagNames{7,2} = 'EarlyHit';
        PsignalMatrix.TagNames{8,2} = 'FirstResponse';
        PsignalMatrix.TagNames{9,2} = 'TargetLick';
        PsignalMatrix.TagNames{10,2} = 'NonTargetLick';
        PsignalMatrix.TagNames{11,2} = 'ProbeLick';
        PsignalMatrix.TagNames{12,2} = 'StimOnset';
        PsignalMatrix.TagNames{13,2} = 'StimOffset';
        PsignalMatrix.TagNames{14,2} = 'StimFrequency';
        PsignalMatrix.TagNames{15,2} = 'StimLevel';
        PsignalMatrix.TagNames{16,2} = 'Target';
        PsignalMatrix.TagNames{17,2} = 'NonTarget';
        PsignalMatrix.TagNames{18,2} = 'Probe';
        PsignalMatrix.TagNames{19,2} = 'EarlyWindowOnset';
        PsignalMatrix.TagNames{20,2} = 'EarlyWindowOffset';
        PsignalMatrix.TagNames{21,2} = 'ResponseWindowOnset';
        PsignalMatrix.TagNames{22,2} = 'ResponseWindowOffset';
        %Preallocation of P
        PsignalMatrix.Tags = zeros(floor(fps*TrialLen),TotalTrials,size(PsignalMatrix.TagNames(:,2),1));
        %Performance
        if isfield(PsignalData.exptparams,'Performance')
            %Licking
            [t,trial,Note,toff,StimIndex] = evtimes(PsignalData.exptevents,'Stim*');
            Ti=0;
            Pi=0;
            NTi=0;
            for i = 1:PsignalData.exptevents(end).Trial
                bb = strsep(PsignalData.exptevents(StimIndex(i)).Note,',');
                StimType = bb{3}(1:end-1);
                switch StimType
                    case 'Target'
                        Ti = Ti+1;
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Target'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                        ResponseData = PsignalData.exptparams.AverageResponse.tar(Ti,:);
                        Lfs = globalparams.HWparams.fsAI;
                        t=0:1/Lfs:(length(ResponseData)/Lfs)-(1/Lfs);
                        ResponseFrames=max(1,round(t(find(ResponseData)).*fps));
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'TargetLick'));
                        PsignalMatrix.Tags(ResponseFrames,i,idx) = 1;
                    case 'NonTarget'
                        NTi = NTi+1;
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'NonTarget'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                        ResponseData = PsignalData.exptparams.AverageResponse.nontar(NTi,:);
                        Lfs = globalparams.HWparams.fsAI;
                        t=0:1/Lfs:(length(ResponseData)/Lfs)-(1/Lfs);
                        ResponseFrames=max(1,round(t(find(ResponseData)).*fps));
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'NonTargetLick'));
                        PsignalMatrix.Tags(ResponseFrames,i,idx) = 1;
                    case 'Probe'
                        Pi = Pi+1;
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Probe'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                        ResponseData = PsignalData.exptparams.AverageResponse.probe(Pi,:);
                        Lfs = globalparams.HWparams.fsAI;
                        t=0:1/Lfs:(length(ResponseData)/Lfs)-(1/Lfs);
                        ResponseFrames=max(1,round(t(find(ResponseData)).*fps));
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'ProbeLick'));
                        PsignalMatrix.Tags(ResponseFrames,i,idx) = 1;
                end
            end
            %First response
            for i = 1:PsignalData.exptevents(end).Trial
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'FirstResponse'));
                if ~isnan(PsignalData.exptparams.FirstResponse(i,1))
                    PsignalMatrix.Tags(max([1 ceil(PsignalData.exptparams.FirstResponse(i,1).*fps)]),i,idx) = 1;
                end
            end
            %Signal detection categories
            idx=find(strcmpi(PsignalMatrix.TagNames(:,2),'FirstResponse'));
            FirstResponse = PsignalMatrix.Tags(:,:,idx);
            for i = 1:PsignalData.exptevents(end).Trial
                switch PsignalData.exptparams.Performance(i).ThisTrial
                    case 'Hit'
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Hit'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                    case 'Miss'
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Miss'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                    case 'FalseAlarm'
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'FalseAlarm'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                    case 'CorrectReject'
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'CorrectReject'));
                        PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                    case 'Early'
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'NonTarget'));
                        NT=PsignalMatrix.Tags(StimOnset,i,idx);
                        idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Target'));
                        T=PsignalMatrix.Tags(StimOnset,i,idx);
                        firstlickframe = find(FirstResponse(:,i));
                        if firstlickframe > StimOnset && NT
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'EarlyFalseAlarm'));
                            PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                        elseif firstlickframe > StimOnset && T
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'EarlyHit'));
                            PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                        elseif firstlickframe < StimOnset
                            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'Early'));
                            PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                        end
                end
            end
            %Behavior Windows
            for i = 1:PsignalData.exptevents(end).Trial
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'EarlyWindowOnset'));
                PsignalMatrix.Tags(EarlyWindowOnset,i,idx) = 1;
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'EarlyWindowOffset'));
                PsignalMatrix.Tags(EarlyWindowOffset,i,idx) = 1;
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'ResponseWindowOnset'));
                PsignalMatrix.Tags(ResponseWindowOnset,i,idx) = 1;
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'ResponseWindowOffset'));
                PsignalMatrix.Tags(ResponseWindowOffset,i,idx) = 1;
            end
        end
        %Stimulus
        [t,trial,Note,toff,StimIndex] = evtimes(PsignalData.exptevents,'Stim*');
        for i = 1:PsignalData.exptevents(end).Trial
            bb = strsep(PsignalData.exptevents(StimIndex(i)).Note,',');
            if isempty(strfind(Note{i},'Silence'))
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOnset'));
                PsignalMatrix.Tags(StimOnset,i,idx) = 1;
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOffset'));
                PsignalMatrix.Tags(StimOffset,i,idx) = 1;
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimFrequency'));
                PsignalMatrix.Tags(StimOnset,i,idx) = bb{2};
                idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimLevel'));
                PsignalMatrix.Tags(StimOnset,i,idx) = OveralldB;
            end
        end
    elseif  ~isempty(strfind(PsignalFile,'AHL')) || ~isempty(strfind(PsignalFile,'RND'))
        %Features
        PsignalMatrix=[];
        for i = 1:4
            PsignalMatrix.TagNames{i,1} = i;
        end
        PsignalMatrix.TagNames{1,2} = 'StimOnset';
        PsignalMatrix.TagNames{2,2} = 'StimOffset';
        PsignalMatrix.TagNames{3,2} = 'StimFrequency';
        PsignalMatrix.TagNames{4,2} = 'StimLevel';
        %Preallocation of P
        PsignalMatrix.Tags = zeros(floor(fps*TrialLen),TotalTrials,size(PsignalMatrix.TagNames(:,2),1));
        %Stimulus
        [t,trial,Note,toff,StimIndex] = evtimes(PsignalData.exptevents,'Stim*');
        for i = 1:TotalTrials
            bb = strsep(PsignalData.exptevents(StimIndex(i)).Note,',');
            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOnset'));
            PsignalMatrix.Tags(StimOnset,i,idx) = 1;
            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimOffset'));
            PsignalMatrix.Tags(StimOffset,i,idx) = 1;
            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimFrequency'));
            PsignalMatrix.Tags(StimOnset,i,idx) = bb{2};
            idx = find(strcmpi(PsignalMatrix.TagNames(:,2),'StimLevel'));
            PsignalMatrix.Tags(StimOnset,i,idx) = OveralldB-bb{end};
        end
    end
    if ~isempty(input.path)
        save([SavePath 'PsignalMatrix.mat'],'PsignalMatrix')
    end
end