plot(app.UIAxes_3,indices_norm(2:51)*params.micronsPerPixel,normalizedCorr_norm(1:50,:),'.-');
            xlabel(app.UIAxes_3,'r (um)');
            ylabel(app.UIAxes_3,'C(r)');
            xticks(app.UIAxes_3,'auto');
            yticks(app.UIAxes_3,'auto');
            
            legends = cell(1,length(winSizes)-1);
            for i = 1:length(winSizes)-1
                legends{i} = ['W = ', num2str(winSizes(i+1))];
            end
            legend(app.UIAxes_3,legends)