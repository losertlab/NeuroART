function best_score = vortexQuality(img)

% Normalize image
img = img - min(img(:));
img = img / max(img(:));

% Preprocessing: Gaussian smoothing to reduce noise
smoothed = imgaussfilt(img, 2);

% Adaptive thresholding to highlight bright structures
bw = imbinarize(smoothed, 'adaptive', 'Sensitivity', 0.5);

% Remove small specks/noise
bw_clean = bwareaopen(bw, 500);

% Fill holes inside ring
bw_filled = imfill(bw_clean, 'holes');

% Detect ring-shaped object: use regionprops
stats = regionprops(bw_filled, 'Area', 'Eccentricity', 'Solidity', 'BoundingBox', 'Centroid');

% Filter based on expected annular ring characteristics
ringCandidates = stats([stats.Area] > 900 & [stats.Solidity] < 0.99 & [stats.Eccentricity] < 0.82 & reshape(arrayfun(@(s) all(s.Centroid > 50), stats), 1, []));

if isempty(ringCandidates)
    %error('No valid vortex ring detected.');
    best_score = 0;
    return;
end

% Use the largest candidate (assuming the main vortex is largest)
[~, idx] = max([ringCandidates.Area]);
ring = ringCandidates(idx);

% Get centroid of the vortex object
cx = ring.Centroid(1);
cy = ring.Centroid(2);

% Create radial distance map
[H, W] = size(smoothed);
[XX, YY] = meshgrid(1:W, 1:H);
radii_map = sqrt((XX - cx).^2 + (YY - cy).^2);

% Search for brightest 6-pixel-thick annulus from center
r_min = 5;
r_max = floor(min(H, W)/2);
dr = 5;

best_score = -Inf;
best_mask = [];
best_mean = 0;
best_std = 0;
best_r = 0;

for r = r_min : (r_max - dr)
    annulus_mask = (radii_map >= r) & (radii_map < r + dr);
    ring_pixels = smoothed(annulus_mask);
    
    if numel(ring_pixels) < 100
        continue;
    end
    
    mean_val = mean(ring_pixels);
    std_val = std(ring_pixels);
    score = mean_val;  % objective
    
    if score > best_score
        best_score = score;
        best_mask = annulus_mask;
        best_mean = mean_val;
        best_std = std_val;
        best_r = r;
    end
end

best_score = best_mean / (best_std + 1e-5);