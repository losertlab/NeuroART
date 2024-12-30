function [Devs, Ms, h] = MkPP_Updates(X, W, beta, Rmax, C, incl_idx, kappa, GD_Iter, k)
%%
% Updates to Hist-Indep MkPP Model with up to Rmax-order events and
% Computation of Deviance Differences
%
% Estimate the Full Model, each Reduced Model for r=2:Rmax, deviance 
% differences, and if potential synchrony is suppressive or facilitative
%
%% Full Model Update
M = size(incl_idx,1);

mu_k = zeros(M, 1);
for ii=1:GD_Iter
%     grad = W * ( X - (1/(1-beta)) * ( exp(mu_k) )/( 1+sum(exp(mu_k)) ) ); % Approximation for large k
    grad = W * ( X - ((1-beta^k)/(1-beta)) * ( exp(mu_k) )/( 1+sum(exp(mu_k)) ) );
    mu_k = mu_k + kappa*grad;
end

lambdastar = exp(mu_k) / ( 1 + sum( exp(mu_k) ) );

%% Compute Log-Odds
lambda = zeros(C,1);
bi_incl_idx = de2bi(incl_idx, C);
for cc=1:C
    tmp = find( bi_incl_idx(:,cc) );
    lambda(cc) = sum( lambdastar(tmp) );
end

bi_incl_idx = de2bi(incl_idx, C);
odds = zeros(M, 1);
for mm=1:M
    k_tmp = find( bi_incl_idx(mm,:) );
    odds(mm) = prod( lambda(k_tmp) ) / prod( 1 - lambda(k_tmp) );
end

%% Reduced Model Updates and Deviance Difference Computation
ord_idx = sum(bi_incl_idx,2);

Devs = zeros(Rmax,1);
Ms = zeros(Rmax,1); 
h = zeros(Rmax,1);
% parfor r=1:Rmax
for r=1:Rmax
    ordRidx = sort(find(ord_idx == r), 'ascend');
    if isempty(ordRidx) || r==1
        Ms(r) = M - length(ordRidx);
        Devs(r) = 0;
        h(r) = 0;
        continue;
    end
    
    Md = length( ordRidx ); Ms(r) = Md;
    Mr = M - Md;
    
    SrCompidx = 1:M; SrCompidx(ordRidx) = [];
    
    muR_k = zeros(Mr, 1);
    XR = X(SrCompidx);
    
    for ii=1:GD_Iter
%         grad = W * ( XR - (1/(1-beta)) * ( exp(muR_k) )/( 1+sum(exp(muR_k)) ) ); % Approximation for large k
        grad = W * ( XR - ((1-beta^k)/(1-beta)) * ( exp(muR_k) )/( 1+sum(exp(muR_k)) ) );
        muR_k = muR_k + kappa*grad;
    end
    
    muR_tmp = muR_k;
    muR_k = log(odds);
    muR_k(SrCompidx, :) = muR_tmp;

	ll_diff = W * (  (mu_k - muR_k)' * X - ...
                ( log( 1+sum(exp(mu_k)) ) - log( 1+sum(exp(muR_k)) ) )  );
    
	Devs(r) = 2*(1+beta)*ll_diff;
    h(r) = -sign( sum( muR_k(ordRidx)-mu_k(ordRidx) ) ); % facilitation or suppression
    
end

end