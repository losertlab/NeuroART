Wi = winSizes(2:end); % window sizes
            rWi = indices_norm(maxInd_norm); % coorelation length for each window size
            rWi = rWi(2:end); % remove the value corresponding to Wi = 0
            
            ai = zeros(length(Wi),1);
            bi = zeros(length(Wi),1);
            
            for idx = 1:length(Wi)
                ai(idx) = rWi(idx)/Wi(idx);
            end
            
            for idx = 2:length(Wi)
                bi(idx) = (rWi(idx) - rWi(1))/log(Wi(idx)/Wi(1));
            end
            
            bi = bi(2:end); % avoid the first element, otherwise there will be a division by zero for the first bi value
            
            CVs = std(bi)/mean(bi);
            CVc = std(ai)/mean(ai);
            
            Kc = (CVs^2)/(CVc^2 + CVs^2);
            
            if ~isnan(Kc)
                app.KcEditField.Value = Kc;
            else
                app.KcEditField.Value = -1;
            end