app.UIAxes_2.Visible = 0;
            app.UIAxes_3.Visible = 0;
            d = app.UITable_3.Data;
            [ind,~] = find(d(:,2)); % Only the non-zero entries in the selection column will be considered
            params = struct;
            
            params.start = app.startFrameEditField.Value;
            params.stop = app.stopFrameEditField.Value;

            F_win = app.rawFluo(:,params.start:min(app.currentFr, params.stop)); % fluorescence traces of all neurons within the considered window
            nCells = size(F_win,1);
            s = zeros(size(F_win,2),nCells);  
            for cellInd = 1:nCells
                [~, s(:,cellInd), ~] = deconvolveCa(F_win(cellInd,:)');
            end
                                        
            Spks = double(s > 0.005); % convert to binary, 1: spike 0: no spike

            settingParams();
            
            [indices_norm,normalizedCorr_norm]  = getAvgcorrelations(params);
            maxInd_norm = zeros(length(ind)+1,1); % add (0,0) point
            for i = 1:length(ind)
                [~,maxInd_norm(i+1)] = max(cumsum(normalizedCorr_norm(:,i)));
            end
            maxInd_norm = maxInd_norm + 1;
            winSizes = [0; params.windowSizes]; % to include (0,0) point
            
            % Plot Correlation length vs Windoe Size for original data
            plot(app.UIAxes_2,winSizes*params.micronsPerPixel,indices_norm(maxInd_norm)*params.micronsPerPixel,'o-');
            xlabel(app.UIAxes_2,'Window size (um)');
            ylabel(app.UIAxes_2,'Correlation Length (um)');
            xticks(app.UIAxes_2,'auto');
            yticks(app.UIAxes_2,'auto');