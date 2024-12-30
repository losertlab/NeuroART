            if app.UseacumulativewindowCheckBox.Value
                app.mstWin = 0; % The value that indicates cumulative window
            else
                wsize = floor(app.CorrelationAnalysisWindowsecondsSlider.Value *app.imagingFreq);
                if wsize < app.currentFr + 3 - app.smoothwin
                    app.mstWin = wsize;
                else
                    promptMessage = sprintf('Window size for correlation analysis exceeds the available number of frames');
                    msgbox(promptMessage,'Error','error');
                end
            end