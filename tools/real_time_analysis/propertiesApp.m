 xlim(app.UIAxes_best,[1 app.displayWin]); ylim(app.UIAxes_best,[lowerLim,lowerLim + (nRanks+1)*offSet]);
            h2 = plot(app.UIAxes_selected,x_val,NaN(app.displayWin,nROIs),'k');
            xlim(app.UIAxes_selected,[1 app.displayWin]); ylim(app.UIAxes_selected,[lowerLim,lowerLim + 8*offSet]); % DZ 030323, expected number of selected cells = 6, let the user specify this number in future. 
            
            app.UIAxes_selected.Position = [app.UIAxes_selected.Position(1:2) [app.Panel_3.Position(3:4) - 2*app.UIAxes_selected.Position(1:2)]];
            app.UIAxes_best.Position = [app.UIAxes_best.Position(1:2) [app.Panel_4.Position(3:4) - 2*app.UIAxes_best.Position(1:2)]];

            nr = floor((app.batch_size - app.last_init_frame)/app.gap);
            cellRanks = zeros(min(nROIs,nRanks),nr); % cellRanks = zeros(nROIs,1);
            allRanks = zeros(nROIs,nr);
            
            ntheta = 20;
            theta = linspace(0, 2*pi, ntheta)'; % Generate circular patches around the centroids of identified neurons
            app.xx = bsxfun(@plus, app.r .* cos(theta), app.xcRaw');
            app.yy = bsxfun(@plus, app.r .* sin(theta), app.ycRaw');
            ptc = patch(app.UIAxes, app.xx, app.yy, zeros(1,nROIs,3),'ButtonDownFcn',@(h,e) ImageClickCallback(app));
            
            cc = []; % color code
            
            neuron(1) = plot(app.UIAxes,NaN,NaN,'Color','r','LineWidth',2,'ButtonDownFcn',@(h,e) ImageClickCallback(app));
            neuron(2) = plot(app.UIAxes,NaN,NaN,'Color','g','LineWidth',2,'ButtonDownFcn',@(h,e) ImageClickCallback(app));
            neuron(3) = plot(app.UIAxes,NaN,NaN,'Color','b','LineWidth',2,'ButtonDownFcn',@(h,e) ImageClickCallback(app));
            
            if nRanks > 3
                for neu = 4:nRanks
                    neuron(neu) = plot(app.UIAxes,NaN,NaN,'Color','m','LineWidth',2,'ButtonDownFcn',@(h,e) ImageClickCallback(app));
                end
%                 neuron(5) = plot(app.UIAxes,NaN,NaN,'Color','m','LineWidth',2,'ButtonDownFcn',@(h,e) ImageClickCallback(app));
            end
            
            selected = plot(app.UIAxes,1:ntheta,NaN(ntheta,nROIs),'Color','y','LineWidth',2,'ButtonDownFcn',@(h,e) ImageClickCallback(app));
            xlim(app.UIAxes,[1 dimX]); ylim(app.UIAxes,[1 dimY]);

            i = 1;
            
            cb = colorbar(app.UIAxes7);
            cb.Visible = 'off';
            colorbarCreated = 0;

            stimStartFrame = 1; % initialization for Priarielink photostimulation testing
            
            
            frameSize = dimX*dimY;
            itr = app.numChannels*app.last_init_frame;
            offset = 2*frameSize*app.numChannels*app.last_init_frame;
            fh = app.rawFileHandle;
            timing = zeros(10,1);
            
            count = 1;
            timing(1) = toc(totStart);
            
            
            delete(findall(app.UIAxes,'Type','hggroup')); % remove tooltip text
                
            
            analysis()

            if i == 1
                f = app.NeuroARTUIFigure;
                mess = "No frame was generated in allowed time";
                title = "Missing real time input";    
                uialert(f, mess, title,'CloseFcn',@(h,e) close(f));
                return;
            end
            
            tstop = toc(totStart)