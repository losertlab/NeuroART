function cellCentroids = detectCellCentroids(batchSettings, exptVars, inputParams, regImg)
    % Find cell centers.
    % promptMessage = sprintf('Select the method for cell finding');
    % button = questdlg(promptMessage, 'Image Registration Completed', 'Manual', 'CaImAn', 'Cite-on', 'CaImAn'); % can't have more than 3 options
    if inputParams.CFIND == 1 % Manual
    %     meanIMG = std(double(regImg),0,3); % Mean image for cell center clicking
        meanIMG = mean(double(regImg),3); % Mean image for cell center clicking
        norm_meanIMG = (meanIMG - repmat(min(meanIMG(:)),size(meanIMG))) ./ repmat(range(meanIMG(:)),size(meanIMG));
        figure; imagesc(norm_meanIMG); colormap('gray'); axis('square')
        SelectText = ['Click on Neuron Centers...' newline 'Press Enter after all the cells are selected' newline 'Press delete if you want to deselect the last selected cell'];
        disp ( SelectText );
        [xc, yc] = getpts; %  manually select centers of the neurons
        cellCentroids(:,1) = yc;
        cellCentroids(:,2) = xc;
        clear xc yc
    elseif inputParams.CFIND == 4 % From File
        [file,path] = uigetfile; % select the .mat file which contains the variable, "ptsIdx": column 2 --> x, column 3 --> y coordinates
        if file == 0
            fprintf('No file was selected ... \n');
            return;
        else
            load([path file],'ptsIdx');
            cellCentroids(:,1) = ptsIdx(:,2); %yc
            cellCentroids(:,2) = ptsIdx(:,3); %xc
        end
    elseif inputParams.CFIND == 3 % Cite-on
    %%     system('activate cite-on & python test.py')
        ups = 2.0;
        th = 0.5;
        citeonPrompt = ['activate citeon & python test.py' ' -x ' num2str(exptVars.dimX) ' -y ' num2str(exptVars.dimY) ' -n ' num2str(length(batchSettings.frameBlock)) ' -p "' inputParams.imageFile '" -u ' num2str(ups) ' -t ' num2str(th)];
        [~,b] = system(citeonPrompt);
        disp(b);
        T = readtable('cell_coordinates.csv');
        
        cellCentroids(:,1) = T.Var2; %yc
        cellCentroids(:,2) = T.Var1; %xc 
    else
        [cellCentroids,~,~,~] = CaImAn_CellFinder(regImg); % CaImAn cell finder
    end
end