            app.StartRealTimeAnalysisButton.Enable = 0;
            app.meanIMGhandle = imshow(app.norm_meanIMG,'Parent', app.UIAxes, 'DisplayRange', [0, 0.8]);
            set(app.meanIMGhandle,'ButtonDownFcn',@(h,e) ImageClickCallback(app));
            set(app.meanIMGhandle, 'PickableParts', 'all');
            hold(app.UIAxes,'on');
            nROIs = length(app.xcRaw);
            DFFsmooth = app.DFF.raw; % 12/27/24 changed this to run without smoothing

            maxDFF = max(app.DFF.smooth,[],2);
            inactiveCells = maxDFF < 65; % DZ 031523 to remove inactive cells from the correlation analysis
            
            dimX = app.exptVars.dimX;
            dimY = app.exptVars.dimY;

            T = 1/app.exptVars.frameRate;
            app.groupId = zeros(nROIs,1);
            colors = ['y', 'c', 'k'];
            A = max(DFFsmooth,[],2);
            B = sort(A,'ascend');
            upperLim = B(ceil(nROIs*0.70)); % 0.9 problem with NaN values
            upperLim_patch = B(ceil(nROIs*0.90))*1.5; % this value is based on the initial batch, multiply by a factor of 1.5 to better indicate much higher DF/F values occured during RT analysis
            A = min(DFFsmooth,[],2);
            B = sort(A,'descend');
            lowerLim = B(ceil(nROIs*0.70));
            lowerLim_patch = B(ceil(nROIs*0.90));
            DFFsmooth = horzcat(DFFsmooth,zeros(nROIs,app.batch_size));
            app.rawFluo = horzcat(app.rawFluo,zeros(nROIs,app.batch_size));
            
            offSet = (upperLim-lowerLim)*2.5; % DZ 100422 multiply by 2.5 to reduce overlap

            x_val = 1:app.displayWin;
            
            nRanks = app.SelectTopSpinner.Value;
