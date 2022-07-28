 function handles =  WF_getPsignalInfo(handles,SavePath)

% if a raw file path is given convert it to handles format
if ischar(handles) || isstring(handles)|| iscell(handles) && length(handles) == 1 

    if isdir(handles) 
      handles = getPsignalInfoFromDir(handles);
      return 
    end 
      
    load(char(handles)) 
    handles = struct('Psignalfile', handles)  ; 
       
    
    
end 



if isstruct(handles)
load(handles.Psignalfile);
end 

    
pfs=str2double(globalparams.PhysHz);
if pfs == 0 
    pfs = 30;
end 
handles.pfs= pfs;

try
    PrimaryHandle = get(exptparams.TrialObject,'PrimaryHandle');
    handles.PrimaryDuration = get(PrimaryHandle,'Duration');
    handles.PreStimSilence  = get(PrimaryHandle,'PreStimSilence');
    handles.PostStimSilence = get(PrimaryHandle,'PostStimSilence');
    handles.BackgroundNoise = get(PrimaryHandle,'BackgroundNoise');
    handles.OverallDB       = get(exptparams.TrialObject,'OveralldB');
    handles.Class            = get(PrimaryHandle,'type');
    handles.Trialindicies   = get(exptparams.TrialObject','TrialIndices');
catch
    
   
        PrimaryHandle = exptparams.TrialObject.PrimaryHandle;
        handles.PrimaryDuration = PrimaryHandle.Duration ;
        handles.PreStimSilence  = PrimaryHandle.SoundObject.PreStimSilence ;
        handles.PostStimSilence = PrimaryHandle.SoundObject.PostStimSilence ;
        handles.BackgroundNoise = PrimaryHandle.BackgroundNoise ;
        handles.OverallDB       = exptparams.TrialObject.OveralldB;
        handles.Class            = PrimaryHandle.Type;
        handles.Trialindicies   = exptparams.TrialObject.TrialIndices;
        
end
handles.framespertrial = handles.pfs*(handles.PreStimSilence +...
                 handles.PrimaryDuration + ...
                 handles.PostStimSilence);
           % tone params 
             try
                 if   contains(get(PrimaryHandle,'type'),'Tone')
                     events = struct2cell(exptevents);
                     events = squeeze(events(1,1,:));
                     s_idx = cellfun(@(x) startsWith(x,'Stim '),events);
                     events = events(s_idx);
                     events =  cellfun(@(y) strsplit(y,',')  ,events,'UniformOutput',false);
                     events = cellfun(@(y) y([2,4]) ,events,'UniformOutput',0);
                     events = cellfun(@(y) [y{1} , y{2}] ,events,'UniformOutput',0);
                     events= strtrim(events);
                     FreqLevel = cellfun(@(y) strsep(y,' '), events,...
                                'UniformOutput',0);
                            
                    handles.FreqLevelOrder = events;
                    handles.Freqs   = cellfun(@(y) y{1},FreqLevel);
                    handles.Levels  = cellfun(@(y) char(string(y{2})),...
                        FreqLevel,'UniformOutput',0);
                    handles.Levels = cellfun(@(x) str2num(x{1}),...
                    regexp(handles.Levels,'-?[0-9]\d*','match'),'UniformOutput',1);
                    handles.Levels =  handles.OverallDB + handles.Levels  ;
                    handles.uFreqs  = unique(cellfun(@(y) y{1},FreqLevel));
                    handles.uLevels = unique(handles.Levels);
                    
                 end
                 
             catch
                 warning(sprintf('Tone param extration failed on %s',...
                         handles.Psignalfile))
             end
           %behavior  
             try
                 if strmatch(exptparams.BehaveObjectClass,'GoNogo')
                    handles.ResponseWin    = exptparams.ResponseWin;
                    handles.FirstResponse  = exptparams.FirstResponse;
                    handles.AverageResponse= exptparams.AverageResponse;
                    handles.Performance    = exptparams.Performance; 
                    handles.Hits     = [exptparams.Performance.Hit];
                    handles.Early    = [exptparams.Performance.EarlyTrial]-2;
                    handles.Miss     = [exptparams.Performance.Miss];
                    % cutting out total from ID
        
                    handles.Hits = handles.Hits(1:end-1);
                    handles.Early = handles.Early(1:end-1);
                    handles.Miss = handles.Miss(1:end-1);
                    if length(handles.uLevels)>1 
                    for lvl = 1:length(handles.uLevels)
                        idx = handles.Levels == handles.uLevels(lvl);
                        handles.HitsLevels{lvl} = handles.Hits(idx);
                        handles.EarlyLevels{lvl} =  handles.Early(idx); 
                        handles.MissLevels{lvl} =  handles.Miss(idx);
                    end 
                    end 
                    
                 end 
                 
          
                 
             catch
                 warning(sprintf('Behavior param extraction failed on %s',...
                         handles.Psignalfile))
             end
           
          % timing params   
          if isfield(exptevents,'Timestamp')
              handles.sessionStartTime = exptevents(1).Timestamp;
              handles.soundTimes = [[exptevents.Timestamp] + seconds(handles.PreStimSilence);...
                  [exptevents.Timestamp] + seconds(handles.PreStimSilence)+...
                  seconds(handles.PrimaryDuration)];
              if (handles.BackgroundNoise(1) ~= -99) && (length(handles.BackgroundNoise)>1)
                  handles.NoiseTimes = [[exptevents.Timestamp] + seconds(handles.BackgroundNoise(2));...
                      [exptevents.Timestamp] + seconds(handles.BackgroundNoise(3))];
              end
          end

                 
             
             
             
             if nargin == 2
                  save([SavePath '\' 'PsignalInfo'], 'handles')
             end 
             
 end 
 

 
       
  