function exptparams = Display (o, HW, StimEvents, globalparams, exptparams, TrialIndex)
if ~isfield(exptparams, 'Figure')
    exptparams.Figure = figure('Position', [10, 200, 320, 120],'MenuBar','none');
    movegui(exptparams.Figure,globalparams.disploc);
    set(exptparams.Figure,'Name',globalparams.Device);
end
uicontrol('Style', 'pushbutton', 'String', 'Stop','fontsize',24,...
    'Position', [10 10 300 100],...
    'Callback', @Exit);
function Exit(source,callbackdata)
global StopExperiment;
StopExperiment = 1;

