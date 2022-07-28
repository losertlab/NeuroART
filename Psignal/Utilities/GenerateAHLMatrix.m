%This script generates Psignal matrices, P, which are Time-index x Trial x M,
%where M is a task-related feature. P(i,t,m) is equal to eqither 1, 0, time value
%in seconds, or feature value, with 1 indicating  membership of a given feature
%or time value of event. Lick have a value of 1 whenever a lick occured.
%This script is specified for the Alternating High Low (AHL) trial object
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
    if ~isempty(strfind(PsignalFile,'AHL'))
        disp(['Processing ' PsignalFile])
        if strcmpi(input.PhysType,'2P')
            addpath(genpath('C:\Users\nfu\Dropbox\Code\2P'))
            %Extract timing parameters
            ThorFile = [NewPath input.expname{e} '\timing.txt'];
            TimingInfo = getTimingInfo(ThorFile, PsignalFile, fps);
            %Find Stim onset and offset frames, based on counting #frames that occur before/after Stim onset
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
        TotalTrials = exptevents(end).Trial;
        TrialLen = exptevents(end).StopTime;
        TrialObject = get(exptparams.TrialObject);
        OveralldB = TrialObject.OveralldB;
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
        PsignalMatrix.Tags = zeros(fps*TrialLen,TotalTrials,size(PsignalMatrix.TagNames(:,2),1));
        %Stimulus
        [t,trial,Note,toff,StimIndex] = evtimes(exptevents,'Stim*');
        for i = 1:TotalTrials
            if isempty(intersect(TimingInfo.rejectedtrials,i))
                bb = strsep(exptevents(StimIndex(i)).Note,',');
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
        save([SavePath 'PsignalMatrix.mat'],'PsignalMatrix')
    end
end