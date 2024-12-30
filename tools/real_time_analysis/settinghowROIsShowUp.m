if nROIs < 250 && nROIs >= nRanks  % limit the number of allowed ROIs for speed
                h1 = plot(app.UIAxes_best,x_val,NaN(app.displayWin,nRanks));
                % display df/f of top three neurons in red, blue, green; others in magenta
                set(h1, {'color'}, cellstr(num2cell(['r'; 'g'; 'b'; repmat('m',nRanks-3,1)]))); 
                % set(h1, {'color'}, {'r'; 'g'; 'b'; 'm'; 'm'; 'm'; 'm'; 'm'; 'm'; 'm'});
else
                nRanks = 3;
                h1 = plot(app.UIAxes_best,x_val,NaN(app.displayWin,nRanks));
                set(h1, {'color'}, {'r'; 'g'; 'b'});
end