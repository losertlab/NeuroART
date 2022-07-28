function WriteMFile(globalparams,exptparams,exptevents)
if isfield(exptparams,'ResultsFigure')
    exptparams=rmfield(exptparams,'ResultsFigure');
end
if isfield(exptparams,'Figure')
    exptparams=rmfield(exptparams,'Figure');
end
if isfield(exptparams,'FigureHandle')
    exptparams=rmfield(exptparams,'FigureHandle');
end
save(globalparams.mfilename,'globalparams','exptparams','exptevents')
