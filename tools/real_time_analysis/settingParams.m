params.DFF = Spks'; % NxT deconvolved spikes
            params.coordinates = app.coordinates; % [x,y] coordinate matrix

            params.avgStepSize = app.averagingFactorEditField.Value;
            params.dimX = app.dimXEditField.Value;
            params.dimY = app.dimYEditField.Value;
            params.micronsPerPixel = app.micronsPerPixelEditField.Value;
            params.distStepSize = app.DistancestepsizeEditField.Value;
            params.type = app.typeDropDown.Value;
            params.frameRate = app.FrameRateEditField.Value;
            params.minNeuronsPerWin = app.MinneuronsperwindowEditField.Value;
            params.windowSizes = d(ind,1);
            params.overlapFactor = app.OverlapfactorEditField.Value;
            params.correlations = app.CorrelationsDropDown.Value;
            params.centerWindow = app.CenterthewindowCheckBox.Value;