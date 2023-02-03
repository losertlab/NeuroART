% Rough first pass function for determing the most population-correlated
% cells using noise correlations and minimum spanning tree sorting method
% Zac Bowen 2019
function cellRanking  = selectCorrNeuron(dff,mst)

R = triu(corrcoef(dff'),1); % upper traingular correlation coefficient matrix

%% Minimum spanning tree method

if (mst)
    nROIs = size(dff,1);
    r = tril(ones(nROIs,nROIs),-1); % lower traingle set to 1
    TMPedgeWeightsNoise = r + abs(R);
    edgeWeightsNoise = 1 - TMPedgeWeightsNoise; % Diagonal and lower triangle are zero
    G = getMST(edgeWeightsNoise');
    cellDegList = degree(G);   
else
    % Correlation based method (search for pairs of cells where the magnitude of correlation > 0.1)
    r = tril(corrcoef(dff'),-1); % lower traingular correlation coefficient matrix
    corr = abs(R+r) > 0.1;
    cellDegList = sum(corr,2);
end

%% Rank the cells based on the degree of connectivity

[~,cellRanking] = sort(cellDegList,'descend');

end
