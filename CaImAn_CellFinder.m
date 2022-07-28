function [extracted_centroids,ind_corr,ind_cnn,ind_exc] = CaImAn_CellFinder(Y)
% Input: Y = 2D motion corrected movie 
% dimensions: dimX*dimY*numberofFrames 

% Extracted from the CaImAn-MATLAB package
% Pnevmatikakis, E.A., Soudry, D., Gao, Y., Machado, T., Merel, J., ... & Paninski, L. (2016). Simultaneous denoising, deconvolution,
% and demixing of calcium imaging data. ? Neuron?89(2):285­299, ? http://dx.doi.org/10.1016/j.neuron.2015.11.037

%% load file
% gcp;                            % start cluster

tst = tic;

addpath(genpath('utilities'));
addpath(genpath('deconvolution'));
 
if ~isa(Y,'single');    Y = single(Y);  end         % convert to single

[d1,d2,T] = size(Y);                                % dimensions of dataset
d = d1*d2;                                          % total number of pixels

%% Set parameters

K = 250;                                          % number of components to be found
tau = 5; %default = 5                             % std of gaussian kernel (half size of neuron) 
p = 2;

options = CNMFSetParms(...   
    'd1',d1,'d2',d2,...                         % dimensionality of the FOV
    'p',p,...                                   % order of AR dynamics    
    'gSig',tau,...                              % half size of neuron
    'merge_thr',0.80,...                        % merging threshold  
    'nb',2,...                                  % number of background components    
    'min_SNR',3,...                             % minimum SNR threshold
    'space_thresh',0.5,...                      % space correlation threshold (dataset3 --> 0.3)
    'cnn_thr',0.2,...                            % threshold for CNN classifier 
    'ssub',4,...                            % spatial downsampling factor 
    'tsub',2, ...                            % temporal downsampling factor
    'spatial_parallel',0, ...                % don't use parallel processing
    'temporal_parallel',0 ...                % don't use parallel processing
    );
%% Data pre-processing

[P,Y] = preprocess_data(Y,p);

toc(tst)
%% fast initialization of spatial components using greedyROI and HALS

[Ain,Cin,bin,fin,center] = initialize_components(Y,K,tau,options,P);  % initialize

% display centers of found components
% Cn =  correlation_image(Y); 
% figure;imagesc(Cn);
% axis equal; axis tight; hold all;
% scatter(center(:,2),center(:,1),'mo');
% title('Center of ROIs found from initialization algorithm');
% drawnow;

toc(tst)
%% manually refine components (optional)
refine_components = false;  % flag for manual refinement
if refine_components
%     [Ain,Cin,center] = manually_refine_components(Y,Ain,Cin,center,Cn,tau,options);
end
    
%% update spatial components
Yr = reshape(Y,d,T);
[A,b,Cin] = update_spatial_components(Yr,Cin,fin,[Ain,bin],P,options);
toc(tst)
%% update temporal components
P.p = 0;    % set AR temporarily to zero for speed
[C,f,P,S,YrA] = update_temporal_components(Yr,A,b,Cin,fin,P,options);
toc(tst)
%% classify components

rval_space = classify_comp_corr(Y,A,C,b,f,options);
ind_corr = rval_space > options.space_thresh;           % components that pass the correlation test
                                        % this test will keep processes
toc(tst)                                        
%% further classification with cnn_classifier
try  % matlab 2017b or later is needed
    [ind_cnn,value] = cnn_classifier(A,[d1,d2],'cnn_model.h5',options.cnn_thr);
catch
    ind_cnn = true(size(A,2),1);                        % components that pass the CNN classifier
end     
toc(tst)                            
%% event exceptionality

fitness = compute_event_exceptionality(C+YrA,options.N_samples_exc,options.robust_std);
ind_exc = (fitness < options.min_fitness);
toc(tst)
%% select components

% keep = (ind_corr | ind_cnn) & ind_exc; 
keep = (ind_corr & ind_cnn); 

%% display kept and discarded components
A_keep = A(:,keep);
C_keep = C(keep,:);
% figure;
%     subplot(121); montage(extract_patch(A(:,keep),[d1,d2],[30,30]),'DisplayRange',[0,0.15]);
%         title('Kept Components');
%     subplot(122); montage(extract_patch(A(:,~keep),[d1,d2],[30,30]),'DisplayRange',[0,0.15])
%         title('Discarded Components');
%% merge found components
[Am,Cm,K_m,merged_ROIs,Pm,Sm] = merge_components(Yr,A_keep,b,C_keep,f,P,S,options);
toc(tst)
%%
display_merging = 0; % flag for displaying merging example
if and(display_merging, ~isempty(merged_ROIs))
    i = 1; %randi(length(merged_ROIs));
    ln = length(merged_ROIs{i});
    figure;
        set(gcf,'Position',[300,300,(ln+2)*300,300]);
        for j = 1:ln
            subplot(1,ln+2,j); imagesc(reshape(A_keep(:,merged_ROIs{i}(j)),d1,d2)); 
                title(sprintf('Component %i',j),'fontsize',16,'fontweight','bold'); axis equal; axis tight;
        end
        subplot(1,ln+2,ln+1); imagesc(reshape(Am(:,K_m-length(merged_ROIs)+i),d1,d2));
                title('Merged Component','fontsize',16,'fontweight','bold');axis equal; axis tight; 
        subplot(1,ln+2,ln+2);
            plot(1:T,(diag(max(C_keep(merged_ROIs{i},:),[],2))\C_keep(merged_ROIs{i},:))'); 
            hold all; plot(1:T,Cm(K_m-length(merged_ROIs)+i,:)/max(Cm(K_m-length(merged_ROIs)+i,:)),'--k')
            title('Temporal Components','fontsize',16,'fontweight','bold')
        drawnow;
end

%% refine estimates excluding rejected components

Pm.p = p;    % restore AR value
[A2,b2,C2] = update_spatial_components(Yr,Cm,f,[Am,b],Pm,options);

% [C2,f2,P2,S2,YrA2] =
% update_temporal_components(Yr,A2,b2,C2,f,Pm,options); % commented by Dulara 10/12/2021

toc(tst)
%% do some plotting

% [A_or,C_or,S_or,P_or] = order_ROIs(A2,C2,S2,P2); % order components % commented by Dulara 10/12/2021
% K_m = size(C_or,1);
%[C_df,~] = extract_DF_F(Yr,A_or,C_or,P_or,options); % extract DF/F values (optional)

% extracted_centroids = com(A_or(:,1:end),d1,d2); % commented by Dulara 10/12/2021
extracted_centroids = com(A2(:,1:end),d1,d2);
toc(tst)

% figure;
% [Coor,json_file] = plot_contours(A_or,Cn,options,1); % contour plot of spatial footprints
%savejson('jmesh',json_file,'filename');        % optional save json file with component coordinates (requires matlab json library)

% extracted_centroids = vertcat(json_file(:).centroid); %centroids of the kept ROIs

%% display components

% plot_components_GUI(Yr,A_or,C_or,b2,f2,Cn,options);

