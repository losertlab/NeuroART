function SaveBehaviorFigure(exptparams,outfile)
if isfield(exptparams,'ResultsFigure')
    try
        disp('Generating behavior png...');
        tpo=get(exptparams.ResultsFigure,'PaperOrientation');
        tpp=get(exptparams.ResultsFigure,'PaperPosition');
        set(exptparams.ResultsFigure,'PaperOrientation','portrait','PaperPosition',[0.5 0.5 10 7.5])
        drawnow;
        print('-dpng',['-f',num2str(exptparams.ResultsFigure.Number)],[outfile '.png'],'-painters');
        set(exptparams.ResultsFigure,'PaperOrientation',tpo,'PaperPosition',tpp)
    catch
        disp('***Behavior figure no longer exists...cannot save***')
    end
end