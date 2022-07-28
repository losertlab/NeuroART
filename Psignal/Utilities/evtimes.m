function [t,trial,Note,toff,EventIndex] = evtimes(events,note,trial)
note = upper (note);
if exist('trial','var'),
    tt=find(ismember(cat(1,events.Trial),trial));
else
    tt=1:length(events);
end
for ii=tt(:)',
    if isempty(events(ii).StopTime),
        events(ii).StopTime=events(ii).StartTime;
    end
end
notes=cell(length(tt),1);
[notes{:}]=deal(events(tt).Note);
notes = upper(notes);
if note(1)=='*' & note(end)=='*',
    ttfind=strfind(notes,note(2:end-1));
    tt2=[];
    for ii=1:length(ttfind),
        if ~isempty(ttfind{ii}),
            tt2=[tt2 ii];
        end
    end
elseif note(1)=='*',
    ttfind=strfind(notes,note(2:end));
    tt2=[];
    for ii=1:length(ttfind),
        if ~isempty(ttfind{ii}),
            % match has to be at end of note string
            if ttfind{ii}(end)==(length(notes{ii})-length(note)+2),
                tt2=[tt2 ii];
            end
        end
    end
elseif note(end)=='*',
    tt2=strmatch(note(1:end-1),notes);
else
    tt2=find(strcmp(notes,note));
end
tt2=tt(tt2);
t=cat(1,events(tt2).StartTime);
toff=cat(1,events(tt2).StopTime);
trial=cat(1,events(tt2).Trial);
Note=cell(length(tt2),1);
[Note{:}]=deal(events(tt2).Note);
EventIndex = tt2;
