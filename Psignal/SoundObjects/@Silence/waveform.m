function [w, event,o]=waveform(o,void,IsRef);
global globalparams HW
fs=globalparams.HWparams.fsAO;
event = [];
% the parameters of tone object
Duration = (get(o,'Duration')); % duration is second
PreStimSilence = (get(o,'PreStimSilence'));
PostStimSilence = (get(o,'PostStimSilence'));
% If more than two values are specified, choose a random number between the two:
if length(PreStimSilence)>1
    PreStimSilence = PreStimSilence(1) + diff(PreStimSilence) * rand(1);
end
if length(PostStimSilence)>1
    PostStimSilence = PostStimSilence(1) + diff(PostStimSilence) * rand(1);
end
%
Names = get(o,'Names');
% generate the tone
timesamples = (1 : Duration*fs) / fs;
w=zeros(size(timesamples));
% Now, put it in the silence:
w = [zeros(round(PreStimSilence*fs),1) ; w(:) ;zeros(round(PostStimSilence*fs),1)];
% and generate the event structure:
event = struct('Note',['PreStimSilence , ' Names{:}],...
    'StartTime',0,'StopTime',PreStimSilence,'Trial',[]);
event(2) = struct('Note',['Stim , ' Names{:}],'StartTime'...
    ,PreStimSilence, 'StopTime', PreStimSilence+Duration,'Trial',[]);
event(3) = struct('Note',['PostStimSilence , ' Names{:}],...
    'StartTime',PreStimSilence+Duration, 'StopTime',PreStimSilence+Duration+PostStimSilence,'Trial',[]);
