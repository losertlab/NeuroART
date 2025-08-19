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

            if size(target_cells_group2,1) > 0 % In the case of stimulation of two groups (one phase mask per group)
                nTargets = 2;
            else
                nTargets = size(target_cells,1);
            end

            stimParams = struct();
            stimParams.seqMode = app.EnableSequenceModeCheckBox.Value;
            stimParams.interval = app.IntervalmsEditField.Value;
            stimParams.niter = app.IterationsEditField.Value;

            if ~(stimParams.seqMode) && (app.SLM == "BNS") % In sequence mode these calculations are done once the user clicks the "download to SLM" button (BNS SLM only)          
                
                stimParams.VMode = app.VortexModeEditField.Value;
                stimParams.VLens = app.VirtualLensEditField.Value;
                stimParams.GS = app.GSIterationsEditField.Value;
                stimParams.yShift = app.XShiftEditField.Value;
                stimParams.xShift = app.YShiftEditField.Value;
                stimParams.dll_name = app.dll_name;
                stimParams.board_number = app.board_number;
                
                assignin("base","target_cells",target_cells);
                writematrix(target_cells,'stimulationTargets.csv');
            end
            if app.SLM_ON && app.SLM == "BNS" % Boulder Nonlinear Systems SLM
                if stimParams.seqMode
                    disp('seqMode');

                    timerObj = timer('Name','MyTimer',               ...
                       'Period',stimParams.interval/1000,            ... 
                       'StartDelay',0,                 ... 
                       'TasksToExecute',stimParams.niter,           ... 
                       'ExecutionMode','fixedSpacing', ...
                       'TimerFcn',@(~,~)stimulate_RT(nTargets,app.dll_name,app.board_number)); 

                    start(timerObj);

                else
                    zernike = readmatrix('zernike_coeffs.csv'); % read the zernike polynomial coefficients identified during aberration correction
                    xy = readmatrix('phaseMaskCenter.csv'); % coordinates of the center of the phase mask that provides the most uniform vortex intensity pattern (identified using "centerPhaseMask.m")

                    stimParams.xShift = xy(2);
                    stimParams.yShift = xy(1);
                    
                    [~,msk,I] = stimulateBNS_SLM(target_cells, stimParams,zernike); % send commands to BNS SLM
                    for iter = 1:stimParams.niter
                        calllib('PiUsb', 'piSetShutterState', 1, app.stimShutter); % open USB shutter
                        pause(0.03); % stim. duration = 300ms (should be a user specified parameter in future)
                        calllib('PiUsb', 'piSetShutterState', 0, app.stimShutter); % close USB shutter
                        pause(stimParams.interval/1000); % specified by the user in milliseconds (convert to seconds)
                    end

                    imshow(msk,'Parent', app.UIAxes8);
                    imshow(I,'Parent', app.UIAxes8_2);
                end

            elseif app.SLM == "DMD"
                
                stimParams.seqMode = app.EnableSequenceModeCheckBox_2.Value;
                stimParams.testMode = app.EnableTestModeCheckBox.Value;
                stimParams.radius = app.RadiuspixelsEditField.Value;
                stimParams.pulseDuration= app.PulseDurationmsEditField.Value;
                stimParams.interval = app.RepeatEverymsEditField.Value;
                stimParams.niter = app.IterationsEditField_2.Value;
                stimParams.background = app.BackgroundIntensityEditField.Value;
                stimParams.stimGroup = app.StimGroupEditField.Value; % id of the group that needs to be stimulated (test mode only)
                stimParams.G1Size = app.Group1SizeEditField.Value; % number of cells selected from G1 for stimulation

                stimulateNikonDMD(target_cells,target_cells_group2,stimParams); % calculate stimulation images and send stim. parameters to the DMD

            elseif app.SLM == "PrairieLink"

%                 app.StimulationBNSTab. % how to diable BNS tab in this mode?
     
                stimParams.DStim = app.DStimulationDropDown.Value;
                stimParams.zOffset = app.ZoffsetEditField.Value;
                stimParams.isSpiral = 'False';
                if app.PatternDropDown.Value == "Spiral"
                    stimParams.isSpiral = 'True';
                end
                paramsUsedInStim();
                stimulatePrairieLink(target_cells, stimParams, app.exptVars.dimX, app.exptVars.dimY); % send commands to PrairieLink
                
            else
                disp('Stimulation method is not available');
            end