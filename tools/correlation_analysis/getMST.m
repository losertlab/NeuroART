%% Compute MSTs of pairwise relationships for response patterns
% Example responseMatrix dimensions are (cell,trial), with 1s and 0s to
% denote whether a cell is active or not on each trial (aka each column)
% Zac Bowen 2019

function G = getMST(pairRelationships)
%% Preallocations
% numPatterns = size(responseMatrix,2);
% [MST_length,rand_MST_length,RATIO,RATIO_err] = deal(NaN(numPatterns,1));
% [outputTree,G,indices,randG] = deal(cell(numPatterns,1));
% outputTree = deal(cell(numPatterns,1));

%% Loop through response patterns and get MST for each
% for s = 1:numPatterns
    
%     if sum(responseMatrix(:,s)) == 0; continue; end
    
    % Pull relationships of responsive cells
    %resp_ind = find(responseMatrix(:,s)); %commented by dulara 06/05/2020
    %respPairRelations = pairRelationships(resp_ind,resp_ind); %commented by dulara 06/05/2020

    % Turn response matrix into a sparse matrix
%     weights = pairRelationships(~isnan(pairRelationships)); 
%     [I,J] = find(~isnan(pairRelationships));
    S = sparse(pairRelationships); % 2018: had to flip I and J due to changes I made to pairwiseDist()
    
    % Calculate MST (Prim's algorithm -- best when #edges >> #nodes)
    [G,~] = minspantree(graph(S,'lower'),'Method','dense');  % DZ 030424 --> for older versions use [tree, ~] = graphminspantree(S,'Method','Prim')
    
    
    % Extract graph structure and tree for output
    % G = graph(tree,'lower'); % commented by dulara 03/04/2024, only for MATLAB versions older than 2022


%     outputTree{s} = tree;
    
    % Get length of tree (could maybe omit and pull from G later) % commented by dulara 05/06/2020
%     treeEdges = full(tree);
%     treeEdges = treeEdges(treeEdges~=0);
%     MST_length(s) = sum(treeEdges);
    
    % Writes indices for each cell pair that was included in the MST % commented by dulara 05/06/2020 
%     tmp = full(tree);
%     [tmp_c1,tmp_c2] = find(tmp);
%     resp_c1 = resp_ind(tmp_c1); % resp_ind were the original cell indices
%     resp_c2 = resp_ind(tmp_c2);
%     indices{s} = unique([resp_c1,resp_c2]);
         
    %% Draw MST with random cells from possible cells (same N)
    % Used to iterate this 100 times per pattern but it blew up
    % the output structure -- now will either omit this or use 
    % random patterns as input to this function.
    
%     rand_ind = datasample(1:size(responseMatrix,1),length(resp_ind),'Replace',false); %,'Weights',rand_prob_TC);
%     rand_ind = sort(rand_ind,'ascend');
%     
%     % Use random index to pull from actual pairwise relationships
%     rand_weightsTMP = pairRelationships(rand_ind,rand_ind);
%     rand_weights = rand_weightsTMP(~isnan(rand_weightsTMP));
%     [rand_I,rand_J] = find(~isnan(rand_weightsTMP));
% 
%     % Do same calculations as above but for random pattern
%     rand_S = sparse(rand_J,rand_I,rand_weights);
%     [rand_tree, ~] = graphminspantree(rand_S,'Method','Prim');
%     randG{s} = graph(rand_tree,'lower');
%     rand_tree_vals = full(rand_tree);
%     rand_tree_vals = rand_tree_vals(rand_tree_vals~=0);
%     rand_MST_length(s) = sum(rand_tree_vals);
% 
%     % Ratio of random MST length to true MST length to compare spread
%     % (this is far less meaningful without the 100 iterations here)
%     RATIO(s) = rand_MST_length(s) / MST_length(s);

% end

% Organize output structure
% MST_results.tree = outputTree;
% MST_results.G = G;
% MST_results.MST_length = MST_length;
% MST_results.indices = indices;
% MST_results.randMST_length = rand_MST_length;
% MST_results.randG = randG;
% MST_results.RATIO = RATIO;
% MST_results.RATIO_err = RATIO_err;

end

