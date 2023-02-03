function [] = SyncFigureUpdate(n, nR, Jstat, Wi, Wa, Ws, WiIDX, tspacing)
%% Define constants
T = WiIDX*Wi*Wa;
T0 = (WiIDX-Ws)*Wi*Wa;

if nargin<8 || isempty(tspacing)
    tspacing = Wi;
end

%% Update figure panels
%%% Spiking events
subplot(6,1,1:2)
hspacing = 5; %>=1, integer
vspacing = 1.5; %>=1
for ii=1:size(n,2)
    for jj=1:size(n,1)
        if n(jj,ii)
            set(line, 'XData', [hspacing,hspacing]*(ii+T0), 'YData', jj*vspacing+[-0.5, 0.5]+1, 'Color', 'k');
        end
    end
end
ylim([vspacing/2 vspacing*(size(n,1))+2+vspacing/2]); yticks( 1+vspacing*[1:size(n,1)] ); yticklabels( num2str( [1:size(n,1)]' ) );
ylabel('neuron index');
xlim([T0, T]*hspacing); xticks([T0:tspacing*Wa:T]*hspacing); xticklabels( num2str([T0:tspacing*Wa:T]') );

%%% r-th Order events
subplot(6,1,3:4)
hspacing = 5; %>=1, integer
vspacing = 1.5; %>=1
for ii=1:size(nR,2)
    for jj=2:size(nR,1)
        if nR(jj,ii)
            set(line, 'XData', [hspacing,hspacing]*(ii+T0), 'YData', (size(nR,1) - jj)*vspacing+[-0.5, 0.5]+1, 'Color', 'k');
        end
    end
end
ylim([vspacing/2 vspacing*(size(nR,1))-vspacing/2]); yticks( vspacing*[1:size(nR,1)-1] ); yticklabels( num2str( flipud([2:size(nR,1)]') ) );
ylabel({'r^{th}-Order','Events'});
xlim([T0, T]*hspacing); xticks([T0:tspacing*Wa:(T)]*hspacing); xticklabels( num2str([T0:tspacing*Wa:T]') );

%%% J-statistics
subplot(6,1,5:6)
imagesc(kron(Jstat(2:end,:), ones(1,1)), [-1 1]); colormap redblue;
yticks( 1:size(nR,1)-1 ); yticklabels( num2str( [2:size(nR,1)]' ) );
ylabel({'r^{th}-Order','Synchrony'});
xlim([0, T-T0]/Wa); xticks([0:tspacing*Wa:(T-T0)]/Wa); xticklabels( num2str([T0:tspacing*Wa:T]') );
xlabel('Time Bin');

end