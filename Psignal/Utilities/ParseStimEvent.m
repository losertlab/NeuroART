function varargout = ParseStimEvent (StimEvent,RemoveSpace)
% This function parse the StimEvent.Note into stimuli name and TarNontar field. It assumes the two are seperate by: ' , '
if nargin<2
    RemoveSpace = 1;
end
comma = findstr(StimEvent.Note, ',');
comma = [1 comma length(StimEvent.Note)];
for cnt1 = 1:length(comma)-1
    varargout{cnt1} = StimEvent.Note(comma(cnt1):comma(cnt1+1));
    varargout{cnt1} = strrep(varargout{cnt1}, ',','');
    if RemoveSpace
        varargout{cnt1} = strrep(varargout{cnt1}, ' ','');
    end
end
if length(varargout)<nargout
    varargout{end+1:nargout}=[];
end