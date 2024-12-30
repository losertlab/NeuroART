        
function [x,y] = deletingROIs(app)
            roi = drawpoint(app.UIAxes,'Visible',"off",'LabelVisible','hover');
            x = roi.Position(1);
            y = roi.Position(2);
            delete(roi);
        end
        
        
        