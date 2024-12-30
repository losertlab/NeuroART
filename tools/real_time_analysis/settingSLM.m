            if app.SelectforstimulationCheckBox.Value && app.SelectforstimulationCheckBox_2.Value
                cellIDs_selected = find(app.groupId == 1); % hard coded for now. change this to pick the selected groupID for stimulation
                cellIDs_correlated = app.topRankedCellIDs;
                cellIDs = union(cellIDs_correlated,cellIDs_selected); % to avoid duplicates
            elseif app.SelectforstimulationCheckBox_2.Value
                cellIDs = find(app.groupId == 1); % hard coded for now. change this to pick the selected groupID for stimulation
            elseif app.SelectforstimulationCheckBox.Value
                cellIDs = app.topRankedCellIDs;
            else
                return;
            end

            cellIDs_group2 = find(app.groupId == 2); % cell IDs of group 2 if available

            target_cells = horzcat(app.xcRaw(cellIDs),app.ycRaw(cellIDs));
            target_cells_group2 = horzcat(app.xcRaw(cellIDs_group2),app.ycRaw(cellIDs_group2));
            
            nTargets = size(target_cells,1);
            
            stimParams.VMode = app.VortexModeEditField.Value;
            stimParams.VLens = app.VirtualLensEditField.Value;
            stimParams.GS = app.GSIterationsEditField.Value;
            stimParams.yShift = app.XShiftEditField.Value;
            stimParams.xShift = app.YShiftEditField.Value;
            stimParams.delay = app.IntervalmsEditField.Value;
            stimParams.dll_name = app.dll_name;
            stimParams.board_number = app.board_number;
            stimParams.seqMode = app.EnableSequenceModeCheckBox.Value;