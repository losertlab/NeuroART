function PsignalSetup
locset=0;
daqvendor='ni';
rig = '';
DevID = '';
disploc = '';
home = fileparts(which('Psignal'));
datapath = '';
fordeployment = '';
answer = {daqvendor,rig,DevID,disploc, home,fordeployment};
while ~locset
    prompt={'DAQ vendor',...
        'DAQ device ID:',...
        'Rig name:',...
        'GUI screen location:',...
        'Home directory',...
        'Data directory',...
        'For deployment (1=yes, 0=no)'};
    name='Psignal Setup';
    numlines=1;
    defaultanswer = {daqvendor,rig,DevID,disploc,home,datapath,fordeployment};
    answer=inputdlg(prompt,name,numlines,defaultanswer);
    if ~isempty(answer)
        if~isempty(answer{1}) && ~isempty(answer{2}) && ~isempty(answer{3}) && ...
                ~isempty(answer{4}) && ~isempty(answer{5}) && ~isempty(answer{6})
            daqvendor = answer{1};
            DevID = answer{2};
            rig  = answer{3};
            disploc = answer{4};
            home = answer{5};
            datapath = answer{6};
            fordeployment = answer{7};
            if ~sum(strcmpi(answer{4},{'northeast','southeast','northwest','southwest', ...
                    'center'}))
                warning(['GUI location must be either: northeast, northwest, southeast,',...
                    'southwest, or center'])
            else
                locset = 1;
            end
        else
            break
        end
    else
        break
    end
end
if ~isempty(answer)
    if~isempty(answer{1}) && ~isempty(answer{2}) && ~isempty(answer{3}) && ...
            ~isempty(answer{4}) && ~isempty(answer{5})
                bb=strsep(lower(home),'\');
        PathStart = find(~cellfun(@isempty,strfind(bb,'c:')));
        PathEnd= find(~cellfun(@isempty,strfind(bb,'psignal')));
        roothome = [];
        for i = PathStart:PathEnd
            roothome = [roothome bb{i} '\'];
        end
        save([home filesep 'PsignalConfig.mat'],'rig','DevID','disploc','daqvendor',...
            'datapath','roothome');
        if ~exist([roothome '\ExperimentGuiSettings.mat'])
            animals.names = [];
            animals.settings =[];
            save([roothome '\ExperimentGuiSettings.mat'], 'animals')
        end
        if str2num(fordeployment)
            psihome=fileparts(which('Psignal'));
            SoundObjList = cat(1,dir([psihome filesep 'SoundObjects/@*']),...
                dir([psihome filesep 'SoundObjects' filesep '@*']));
            TrialObjList = cat(1,dir([psihome filesep 'TrialObjects/@*']),...
                dir([psihome filesep 'Experiments' filesep 'TrialObjects' filesep '@*']));
            BehaveObjList = cat(1,dir([psihome filesep 'BehaviorObjects/@*']),...
                dir([psihome filesep 'Experiments' filesep 'BehaviorObjects' filesep '@*']));
            save([home filesep 'ObjectLists.mat'],'SoundObjList','TrialObjList',...
                'BehaveObjList');
            f=what(psihome);
            copyfile([psihome '\Psignal.png'] ,home,'f');
            copyfile([psihome '\LastValues.mat'] ,roothome,'f');
%             copyfile([psihome '\Waveforms'] ,[home '\Waveforms'],'f');
        end
    end
end
