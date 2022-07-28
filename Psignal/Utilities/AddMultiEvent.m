function events=AddMultiEvent(events,newEvents,trial)
% created BE 2010-7

for i=1:length(newEvents)
  if ~isempty(newEvents{i})
    events = AddEvent(events,newEvents{i},trial);
  end
end
