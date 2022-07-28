function events=AddEvent(events,note,trial,starttime,stoptime)
% append behavior or stimulus event onto list of event timestamps during a trial
ecount=length(events)+1;
if isstruct(events) && ~isempty(events)
    if ~isfield(events,'StopTime')
        events.StopTime=[];
    end
    if ~isfield(events,'Trial')
        events.Trial =0;
    end
end
if isstruct(note),
    for cnt1 = 1:length(note)
        if ~isfield(note(cnt1),'StopTime'),
            note(cnt1).StopTime=[];
        end
        if ~isfield(note(cnt1),'Trial'),
            note(cnt1).Trial=0;
        end
        if length(events)==0,
            events=note(cnt1);
        else
            FN = fieldnames(note);
            for i=1:length(FN)
                events(ecount).(FN{i}) = note(cnt1).(FN{i});
            end
        end
        events(ecount).Trial=trial;
        ecount = length(events) + 1;
    end
else
    ecount=length(events)+1;
    events(ecount).Note=note;
    events(ecount).StartTime=starttime;
    if exist('stoptime','var'),
        events(ecount).StopTime=stoptime;
    end
    events(ecount).Trial=trial;
end