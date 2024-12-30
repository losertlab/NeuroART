if inputParams.CFIND == 1 % Manual
    % stdImg = stdImg_window(regImg,20);
    % normStdIMG = (stdImg - repmat(min(stdImg(:)),size(stdImg))) ./ repmat(range(stdImg(:)),size(stdImg));
    figure; imagesc(normMeanIMG); colormap('gray'); axis('square'); clim([0 0.8]);
    selectText = ['Click on Neuron Centers...' newline 'Press Enter after all the cells are selected' newline 'Press delete if you want to deselect the last selected cell'];
    if verbose, disp (selectText),end
    [xc, yc] = getpts; %  manually select centers of the neurons
    cell_centroids(:,1) = yc;
    cell_centroids(:,2) = xc;
    clear xc yc
elseif inputParams.CFIND == 4 % From File
    [file,path] = uigetfile; % select the .mat file which contains the variable, "ptsIdx": column 2 --> x, column 3 --> y coordinates
    if file == 0
        fprintf('No file was selected ... \n');
        return;
    else
        load([path file],'cell_centroids');
    end
elseif inputParams.CFIND == 3 % Cite-on
    ups = 2.0; % upscaling factor
    th = 0.5;  % threshold
    [~,cmdOutput] = system(['activate cite-on & python citeon.py' ' -x ' num2str(exptVars.dimX) ' -y ' num2str(exptVars.dimY) ' -n ' num2str(length(batchSettings.frameBlock)) ' -p ' inputParams.imageFile ' -u ' num2str(ups) ' -t ' num2str(th)]);
    T = readtable('cell_coordinates.csv');
    clear cell_centroids; % clear previously loaded cell coordinates
    cell_centroids(:,1) = T.Var2; %yc
    cell_centroids(:,2) = T.Var1; %xc 
elseif inputParams.CFIND == 5 % cellPose
    [~,cmdOutput] = system(['activate cellpose & python cellPose.py' ' -n ' num2str(length(batchSettings.frameBlock)) ' -p ' inputParams.imageFile]);
    T = readtable('centroids.csv');
    clear cell_centroids; % clear previously loaded cell coordinates
    cell_centroids(:,1) = T.Y; %yc
    cell_centroids(:,2) = T.X; %xc 
% elseif inputConfig.python.use_python && inputParams.CFIND == 2
%     cell_centroids = locateCentroids(inputConfig.python.complex_dynamics_path, inputConfig.python.env);
else
    if ~verbose
        [caimanTextOutput,cell_centroids,~,~,~] = evalc('CaImAn_CellFinder(regImg,caimanParams)'); % suppressing printed statements
    else
        [cell_centroids,~,~,~] = CaImAn_CellFinder(regImg,caimanParams); % CaImAn cell finder
    end
end

