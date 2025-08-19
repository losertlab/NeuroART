function ui_view_responders(meanImg, dff, cellCoords, out, Fs)
% UI to visualize responding cells and collect coordinates for export.
% meanImg   : [H x W] mean image of FOV
% dff       : [N x T] dF/F traces (neurons x time)
% cellCoords: [N x 2] [x,y] pixel coordinates per neuron
% out       : struct with field isResponder (logical N x 1)
% Fs        : sampling rate (Hz)

% ---- Basic checks ----

assert(size(cellCoords,1)==size(dff,1) && size(cellCoords,2)==2, ...
    'cellCoords must be N x 2 and match number of neurons');
assert(isfield(out,'isResponder'), 'out.isResponder not found');
isResp = logical(out.isResponder(:));
Fs = double(Fs);

% ---- Build UI ----
ui = uifigure('Name','Responding Cell Explorer','Color','w','Position',[100 100 1100 600]);

% Axes: image (left) and trace (right)
axImg   = uiaxes(ui,'Position',[25 80 520 500]); axis(axImg,'image'); axImg.Box='on';
axTrace = uiaxes(ui,'Position',[560 330 520 250]); axTrace.Box='on';
xlabel(axTrace,'Time (s)'); ylabel(axTrace,'dF/F'); title(axTrace,'Cell Trace');

% Table for selected cells
tbl = uitable(ui,'Position',[560 80 520 230], ...
    'ColumnName',{'Idx','X','Y'}, 'ColumnEditable',[false false false], ...
    'Data', zeros(0,3), 'Tag','selectionTable');

% Buttons
btnAdd    = uibutton(ui,'Text','Add to Table','Position',[25 25 120 30], 'Enable','off');
btnClear  = uibutton(ui,'Text','Clear Table','Position',[155 25 120 30], ...
    'ButtonPushedFcn',@(src,evt) set(tbl,'Data',zeros(0,3)));
btnExport = uibutton(ui,'Text','Export Coordinates','Position',[285 25 120 30], ...
    'ButtonPushedFcn',@(src,evt) exportTable(tbl));

% Info label
lbl = uilabel(ui,'Text','Click a highlighted cell to view its DF/F trace.','Position',[420 28 340 25]);

% ---- Show image ----
imagesc(axImg, meanImg); colormap(axImg,'gray'); axis(axImg,'image'); axis(axImg,'off');
title(axImg,'Responding Cells');

% ---- Overlay responders as small circles ----
respIdx = find(isResp);
xyResp  = cellCoords(respIdx, :);
hold(axImg,'on');
s = scatter(axImg, xyResp(:,1), xyResp(:,2), 36, 'c', 'o', ...
    'LineWidth',1.5,'MarkerFaceColor','none','HitTest','on','PickableParts','all');
hold(axImg,'off');

% Make markers pickable one by one by storing their indices
% We’ll use a trick: create an invisible scatter with per-point ButtonDownFcn via plot().
% Alternatively, use a common callback and map click to nearest point.

% Keep state
state.currentIdx = [];
state.highlight  = []; % handle to a red circle for the current
guidata(ui, state);

% Set a common click callback on the axes (we’ll find nearest responder)
axImg.ButtonDownFcn = @(src,evt) onClickImg(evt, axImg, xyResp, respIdx, dff, axTrace, Fs, ui, btnAdd);

% Also allow clicking directly on the scatter
s.ButtonDownFcn = @(src,evt) onClickImg(evt, axImg, xyResp, respIdx, dff, axTrace, Fs, ui, btnAdd);

% Add-to-table callback
btnAdd.ButtonPushedFcn = @(src,evt) addCurrentToTable(ui, tbl, cellCoords);

end

% ====== Callbacks ======

function onClickImg(evt, axImg, xyResp, respIdx, dff, axTrace, Fs, ui, btnAdd)
% Find nearest responding cell to click location and show its trace
cp = evt.IntersectionPoint;  % [x y z]
xy = cp(1:2);
% Nearest neighbor among responder coords
d2 = sum((xyResp - xy).^2, 2);
[~, k] = min(d2);
cellIdx = respIdx(k);

% Update trace
t = (0:size(dff,2)-1)/Fs;
plot(axTrace, t, dff(cellIdx,:),'k-','LineWidth',1.0);
title(axTrace, sprintf('Cell %d', cellIdx));
xlabel(axTrace,'Time (s)'); ylabel(axTrace,'dF/F');
grid(axTrace,'on');

% Highlight on image
state = guidata(ui);
if isgraphics(state.highlight)
    delete(state.highlight);
end
hold(axImg,'on');
state.highlight = plot(axImg, xyResp(k,1), xyResp(k,2), 'ro', 'MarkerSize',2, 'LineWidth',1.5);
hold(axImg,'off');
state.currentIdx = cellIdx;
guidata(ui, state);

% Enable Add-to-Table
btnAdd.Enable = 'on';
end

function addCurrentToTable(ui, tbl, cellCoords)
state = guidata(ui);
if isempty(state.currentIdx), return; end
idx = state.currentIdx;
xy  = cellCoords(idx,:);
data = tbl.Data;
% Avoid duplicates
if ~isempty(data) && any(data(:,1)==idx)
    return;
end
tbl.Data = [data; [idx, xy]];
end

function exportTable(tbl)

data = tbl.Data;
if isempty(data)
    uialert(ancestor(tbl,'figure'),'Table is empty.','Export');
    return;
end
[fn, fp] = uiputfile('cell_centroids.mat','Save selected cells as MAT');
if isequal(fn,0), return; end

% Swap X and Y columns before saving (Idx stays first)
data_swapped = [data(:,3), data(:,2)];

% Save to MAT file
cell_centroids = data_swapped;
save(fullfile(fp,fn), 'cell_centroids');

end
