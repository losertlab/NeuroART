dimX = app.dimXEditField.Value;
            dimY = app.dimYEditField.Value;
            minDim = min(dimX,dimY);
            if app.NoofwindowsizesDropDown.Value == '5'
                d = ones(5,2);
                d(:,1) = round([minDim/4; minDim/2; minDim*2/3; minDim*5/6; minDim]);
            elseif app.NoofwindowsizesDropDown.Value == '8'
                d = ones(8,2);   
                d(:,1) = round(minDim/8 *(1:8)');
            elseif app.NoofwindowsizesDropDown.Value == "10"
                d = ones(10,2);   
                d(:,1) = round(minDim/10 *(1:10)');
            else
                d = ones(25,2);   
                d(:,1) = round(minDim/25 *(1:25)');
            end
            
            app.UITable_3.Data = d;