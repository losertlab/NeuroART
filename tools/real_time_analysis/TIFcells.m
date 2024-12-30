            
if count > 0

            app.selectedCellList = unique(app.selectedCellList); % remove duplicates
            hiddenCells = app.hidden;

            assignin("base","selectedCellList",app.selectedCellList);
            assignin("base","selectedCellsAll",app.selectedCellsAll);
            assignin("base","selectedCellFrameID",app.selectedCellFrameID);
            assignin("base","DFFsmooth",DFFsmooth);
            assignin("base","cellRanks",cellRanks);
            assignin("base","allRanks",allRanks);
            assignin("base","F",app.rawFluo);
            
            if colorbarCreated
                assignin("base","tuningInfo",tuningInfo);
            end
            
            promptMessage = sprintf('Save TIFs with labelled cells? (Optional)');
            button = questdlg(promptMessage, 'Real-time analysis completed', 'Yes', 'No', 'Yes');
            if strcmpi(button, 'Yes')
                lastFrame = saveLabelledNeurons(cellRanks(:,2:end),app.selectedCellList,hiddenCells,app.rawFluo,app.xcRaw,app.ycRaw,app.norm_meanIMG,app.numInitFrames,app.r,app.gap,app.exptId);
                promptMessage = sprintf('Images saved successfully!');
                msgbox(promptMessage,'Success','help');
            end

end