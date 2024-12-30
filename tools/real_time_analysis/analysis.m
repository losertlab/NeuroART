            while i <= app.batch_size
                
                if i == 3000
                    timing(2) = toc(totStart);
                end
                
                app.currentFr = app.last_init_frame + i;
                
                if (app.scope == 1) || (app.scope == 2) || (app.scope == 3) || (app.scope == 4) || (app.scope == 6 && app.imgType == ".raw")
                
                    fseek(fh,0,'eof');
                    numFramesRecorded = floor(ftell(fh)/(frameSize*2));
        
                    if numFramesRecorded > itr % detection of a new frame, assuming the raw file isn't fully preallocated
                        if app.numChannels==1 || mod(itr+1,app.numChannels) == app.greenChannel || (mod(itr+1,app.numChannels) == 0 && app.greenChannel == app.numChannels) % Only read if it belongs to the Green Channel
                            fseek(fh,offset,'bof');
                            currentIMG = reshape(fread(fh, frameSize, 'uint16=>uint16'),[dimX dimY]); % Read the (itr+1)th frame
%                             currentIMG = currentIMG'; %DZ 081822, for MM data manually converted to raw files 
                            count = 1;
                            itr = itr + 1;
                            offset = 2*itr*frameSize; % uint16 --> 2 bytes
                        else
                            count = 1;
                            itr = itr + 1;
                            offset = 2*itr*frameSize; % uint16 --> 2 bytes
                            pause(0.002);
                            continue;
                        end
                    else
                        count = count + 1;
                        if count == 3000 % wait for 1 min
                            disp('No frame was generated in allowed time');
                            count = -1;
                        else
                            pause(0.002);
                            continue;
                        end
                    end

                elseif (app.scope == 2 && app.imgType == ".tif") % Only valid if Bruker spits out tif file sequences with proper indexing
                    [filepath,name,ext] = fileparts(app.ImageFile);
                    fName = fullfile(filepath, [name '_Ch' num2str(app.greenChannel) '_' sprintf('%05d',app.currentFr) '.ome' ext]); % Format of the Bruker tif filenames 
             
                    if isfile(fName) % detection of a new frame
                        currentIMG = imread(fName); % Read the new frame
                        count = 1;
                    else
                        count = count + 1;
                        if count == 3000 % wait for 1 min
                            disp('No frame was generated in allowed time');
                            count = -1;
                        else
                            pause(0.002);
                            continue;
                        end
                    end
                    
                elseif (app.scope == 5 && app.imgType == ".tif") || (app.scope == 6 && app.imgType == ".tif") % Only valid if the Nikon microscope saves images as a tif stack
                    
                    frameid = app.last_init_frame + i;

                    % Initialize variables
                    wait = 2; % timeout in seconds

                    try
                        % Attempt to read the current frame from the stack
                        currentIMG = imread(app.ImageFile, frameid);
                        count = 1;
                    catch    
                        % If the frame does not exist, wait for 1 second and check again           
                        pause(wait);
                                
                        % After timeout, try reading the frame again 
                        try              
                            currentIMG = imread(app.ImageFile, frameid); 
                            count = 1;
                        catch              
                            disp('No new frame detected within timeout period. Exiting loop.');  
                            tstop = toc(totStart)
                            count = -1; % indicate that a new frame was not captured
                        end
                    end
                    
                else
                    count = -1;
                end
                
                if count == -1 && i == 1
                    close(gcf);
                    break;
                elseif count == -1 && i > 1
                    
                    app.selectedCellList = unique(app.selectedCellList); % remove duplicates
                    hiddenCells = app.hidden;
                    assignin("base","selectedCellList",app.selectedCellList);
                    assignin("base","selectedCellsAll",app.selectedCellsAll);
                    assignin("base","selectedCellFrameID",app.selectedCellFrameID);
%                     assignin("base","hiddenCells",hiddenCells)
                    assignin("base","DFFsmooth",DFFsmooth);
                    assignin("base","cellRanks",cellRanks);
                    assignin("base","allRanks",allRanks);
                    assignin("base","F",app.rawFluo);
%                     assignin("base","mstWin_new",app.mstWin);
%                     evalin('base', ['save(''', app.exptId ''')']);
                    
                    if colorbarCreated
                        assignin("base","tuningInfo",tuningInfo);
                    end
                    
                    promptMessage = sprintf('Save TIFs with labelled cells? (Optional)');
                    button = questdlg(promptMessage, 'Real-time analysis completed', 'Yes', 'No', 'Yes');
                    if strcmpi(button, 'No')
                        close(app.NeuroARTUIFigure); % DZ 030522
                        return; % Or break or continue
                    else
                        lastFrame = saveLabelledNeurons(cellRanks(:,2:end),app.selectedCellList,hiddenCells,app.rawFluo,app.xcRaw,app.ycRaw,app.norm_meanIMG,app.numInitFrames,app.r,app.gap,app.exptId);
                    end
                    
                    promptMessage = sprintf('Images saved successfully!');
                    msgbox(promptMessage,'Success','help');
                    
                    pause(3);
                    close(app.NeuroARTUIFigure); % DZ 030522
                
                    break
                end
    
    
                %% Register the current frame (Using Sam's motion correction code)
    
                [currentRegIMG, ~, ~] = regseqRT(app.imTemplate,currentIMG);

                if i == 3000
                    timing(3) = toc(totStart);
                end
    
                % Compute DF/F of the current frame 
                
                [newDFF, rawFluoNew,~,~,~,~] = RTcomputeDFF(app.roiBW2,app.npBWout,currentRegIMG,app.dffwindow,app.percent, app.rawFluo(:,app.currentFr-1-app.dffwindow:app.currentFr-1), T, app.symmFLAG, app.smoothwin,app.minNpSubFluo,app.maxAdjF,app.filled); %return updated DFF trace
                
                % assignin("base","newDFF",newDFF);
                

                if i == 3000
                    timing(4) = toc(totStart);
                end
                
                % append new DFF and raw fluorescence value
    
                DFFsmooth(:,app.currentFr-app.smoothwin+1:app.currentFr) = newDFF;
                app.rawFluo(:,app.currentFr-app.smoothwin+1:app.currentFr) = rawFluoNew;
    
                %% Minimum spanning tree / correlation based approach to find the most population-correlated cells
                %% update grayscale patches to indicate DF/F
    
                %% Indicate the most critical cells for excitation --> Top 3: green,magenta,blue 4,5: blue (update the figure every 30 frames)
    
                if app.currentFr > app.numAvailFrames && mod(i,app.gap)==0 && i>app.smoothwin % update every 15 frames (if app.gap = 15), avoid displaying frames that were acquired during cell finding

                    app.FrameNoEditField.Value = app.currentFr;

                    if app.debugMode % do not update cell rankings while identifying stimulatable neurons (debug mode)
                       
                        if app.debugIndex == length(app.target_cellIDs) %  finished testing all the considered neurons
                            % find which neurons responded (based on DF/F) to photostimulation
                            dff = DFFsmooth(:,1:app.currentFr); % DF/F traces upto current frame
                            stimDFF = DFFsmooth(:,stimStartFrame:app.currentFr); % DF/F traces corresponding to the duration of photostimulation
                            responses = findStimulatableNeurons(app.target_cellIDs,dff,stimDFF); % evaluate whether the neurons responded to photostimulation
                            
                            app.UITable2.Data = table(app.xcRaw(app.target_cellIDs),app.ycRaw(app.target_cellIDs),responses); % update table to indicate responded cells (response = 1)
                            app.debugMode = 0; % reset variables after finishing testing
                            app.debugIndex = 0; % reset variables after finishing testing
                            app.ClickStarttoidentifystimulatablecellsLabel.Text = "Testing completed!";

                        else
                            if app.PhotostimulationSwitch.Value == "Simultaneous"
                                stimStartFrame = app.currentFr;
                                app.debugIndex = length(app.target_cellIDs);
                                target_cells = horzcat(app.xcRaw(app.target_cellIDs),app.ycRaw(app.target_cellIDs)); % stimulate all neurons simultaneously
                            else % sequential stimulation mode
                                if (app.debugIndex == 0); stimStartFrame = app.currentFr; end % frame number where we start stimulation
                                app.debugIndex = app.debugIndex + 1;
                                target_cells = [app.xcRaw(app.target_cellIDs(app.debugIndex)),app.ycRaw(app.target_cellIDs(app.debugIndex))]; % one neuron at a time
                            end
                            stimulatePrairieLink(target_cells, app.stimulationParams, app.exptVars.dimX, app.exptVars.dimY); % send commands to PrairieLink
                        end
                    else
                        if (~app.mstWin)
                            cellRank = selectCorrNeuron(DFFsmooth(:,1:app.currentFr-app.smoothwin+2),app.mst,inactiveCells); %cumulative
                        else
                            cellRank = selectCorrNeuron(DFFsmooth(:,app.currentFr-app.smoothwin+3-app.mstWin:app.currentFr-app.smoothwin+2),app.mst,inactiveCells); % sliding window  
                        end
                        allRanks(:,i/app.gap) = cellRank;
                                       
                        numElements = min(nROIs,nRanks);
                        cellRanks(:,i/app.gap) = cellRank(1:numElements); % Each column contains ids of top nRanks (e.g. 5) ranked cells during each GUI update
    
                        cellRank = cellRank(1:numElements);
                        app.topRankedCellIDs = cellRank;

                        %% Tuning analysis
      
                        if app.TuningAnalysisCheckBox.Value % Run this if the tuning analysis checkbox is selected
                            
                            if mod(i,30*app.gap)==0
                                
                                if isempty(app.JiSignalInfo)
                                    tuningInfo = getTuningRealTimeFunction(DFFsmooth(:,1:app.currentFr-app.smoothwin+2),app.exptVars,app.psignalFile,app.thorSyncFile);
                                else
                                    tuningInfo = getTuningRealTimeFunction_Ji(DFFsmooth(:,1:app.currentFr-app.smoothwin+2),app.exptVars,app.JiSignalInfo);
                                end
                                if isempty(cc)
                                    cc(1,:,:) = jet(numel(tuningInfo.kHzVals));
                                end
                                
                                set(ptc,'CData',cc(:,tuningInfo.BFval,:));
                                
                                if ~colorbarCreated
                                    cc1 = jet(numel(tuningInfo.kHzVals));
                                    colormap(app.UIAxes7,cc1)
                                    stepsize = 1/numel(tuningInfo.kHzVals);
                                    cb.Ticks = stepsize/2:stepsize:1-stepsize/2;
                                    cb.TickLabels = tuningInfo.kHzVals;
                                    colorbarCreated = 1;
                                    
                                end
                                app.kHzLabel.Visible = 'on';
                                cb.Visible = 'on';
                                
                            end
                            
                        else
                            cb.Visible = 'off';
                            app.kHzLabel.Visible = 'off';
                            set(ptc,'CData',(newDFF(:,3)-lowerLim_patch)./((upperLim_patch-lowerLim_patch))); % update DF/F values in grayscale patches
                        end
    
                        %% Synchrony analysis (Babadi Lab)
    
                        if app.SynchronyAnalysisCheckBox.Value % Run this if the synchrony analysis checkbox is selected                        
                            
                            
                            if mod(i,30*app.gap)==0
                                
                                frameID = i + app.last_init_frame; % don't use app.currentFr that will increase during the analysis
                                % spike deconvolution of top 10 ranked neurons
                                syncAnalysisWinSize = 900; % number of frames considered at a time for spike deconvolution + synchrony analysis
                                cellIDtop10 = allRanks(1:9,i/app.gap);
    
                                F10 = app.rawFluo(cellIDtop10,frameID-syncAnalysisWinSize+1:frameID); % fluorescence traces of top10 neurons
                                
    
                                % synchrony analysis
                               
                                Rmax = 4;
                                Wi = 10;
                                Wa = 15;
                                Ws = syncAnalysisWinSize/(Wi*Wa);
                                [JstatIm,ii] = SyncAnalysisOnline(F10,Wi,Wa,Ws,Rmax);
    
                                % Plot the Jstats to indicate the order of synchrony
    
                                % 'redblue' colormap to use instead of colormap('redblue') function that generates a blank figure
    
                                cmap = redblue(256); 
                                
    %                             set(syncMap,'CData', kron(JstatIm(2:end,:), ones(1,1)));
                                app.UIAxes9.Visible = 'on';
                                set(app.UIAxes9,'Colormap', cmap);
                                imagesc(app.UIAxes9, kron(JstatIm(2:end,:), ones(1,1)), [-1 1]); %colormap(app.UIAxes9,'redblue');
                                yticks(app.UIAxes9, 1:Rmax-1 ); yticklabels(app.UIAxes9, num2str( [2:Rmax]' ) );
                                tspacing = Wi;
                                T0 = (ii-Ws)*Wi*Wa;
                                xlim(app.UIAxes9,[0, syncAnalysisWinSize-T0]/Wa); xticks(app.UIAxes9,[0:tspacing*Wa:(syncAnalysisWinSize-T0)]/Wa); xticklabels(app.UIAxes9,num2str([T0:tspacing*Wa:syncAnalysisWinSize]') );                           
                                
                            end
                            
                        else
                            app.UIAxes9.Visible = 'off';
                        end
                    
                        % DZ 030522
                        % cellRank --> column vector of top ranked cell ids. length could be < 5 (because of removing bad ROIs)
                        
                        for idx = 1:length(cellRank)
                            set(neuron(idx),'XData',app.xx(:,cellRank(idx)),'YData',app.yy(:,cellRank(idx)));
                        end
                        
                        if app.selectedID
                            if ~ismember(app.selectedID,app.selectedCellList)
                                app.groupId(app.selectedID) = str2double(app.GroupIDDropDown.Value);
                                app.selectedCellList = [app.selectedCellList app.selectedID];
                                app.selectedCellsAll = [app.selectedCellsAll app.selectedID];
                                app.selectedCellFrameID = [app.selectedCellFrameID app.currentFr];
                                set(selected(app.selectedID),'XData',app.xx(:,app.selectedID),'YData',app.yy(:,app.selectedID),'Color', colors(app.groupId(app.selectedID)));
                            else
                                app.selectedCellList = setdiff(app.selectedCellList,app.selectedID);
                                set(selected(app.selectedID),'XData',NaN,'YData',NaN);
                                app.hidden = app.selectedID;
                                app.groupId(app.selectedID) = 0;
                            end
                            app.selectedID = 0; 
                        end
                    
                        if app.loadCorrelated % load x, y coordinates of top ranked cells
                            xc = app.xcRaw(cellRank);
                            yc = app.ycRaw(cellRank);
                            group = ones(length(cellRank),1);
                            app.UITable.Data = table(xc,yc,group);
                            app.loadCorrelated = 0;
                        end
                        
                        if app.loadSelected % load x, y coordinates of the selected cells
                            xc = app.xcRaw(app.selectedCellList);
                            yc = app.ycRaw(app.selectedCellList);
                            group = app.groupId(app.selectedCellList);
                            app.UITable_2.Data = table(xc,yc,group);
                            app.loadSelected = 0;
                        end
                    
                    end
                                       
                    for idx = 1:length(cellRank)
                        set(h1(idx), {'YData'}, num2cell(offSet*(idx-1) + DFFsmooth(cellRank(idx),app.currentFr-app.smoothwin-app.displayWin+3:app.currentFr-app.smoothwin+2),2));
                    end                  
                    
                    if ~isempty(app.selectedCellList)
                        for idx = 1:length(app.selectedCellList)
                            set(h2(app.selectedCellList(idx)), {'YData'}, num2cell(1.5*offSet*(mod(idx-1,10)) + DFFsmooth(app.selectedCellList(idx),app.currentFr-app.smoothwin-app.displayWin+3:app.currentFr-app.smoothwin+2),2));
                        end  
                    end
                    
                    % Remove the graph of the recently deselected cell
                    
                    if ~isempty(app.hidden)
                        set(h2(app.hidden),'YData', NaN);
                        app.hidden = [];
                    end
                                        
                     
                    drawnow limitrate; % update GUI figures and plots
                    
                end            
    
    
                i = i + 1;
    
            end