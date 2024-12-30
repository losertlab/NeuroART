function ImageClickCallback(app)
            c = get(app.UIAxes,'CurrentPoint');
            x = c(1,1);
            y = c(1,2);
            cdiff = app.coordinates - [x y];
            [~,ind] = min(sum(cdiff.^2,2));
            app.selectedID = ind;
            pause(0.5);
            % Remove the label of the selected point
            delete(findall(app.UIAxes,'Type','hggroup')); % remove tooltip text
            delete(findobj(app.UIAxes, 'Type', 'text'));
            % delete(findobj(app.UIAxes, 'Type', 'text', 'String', num2str(ind)));
        end