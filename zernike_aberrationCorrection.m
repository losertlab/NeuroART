addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

%% Set image aquisition file path and the file format

% start acquisition in micromanager software before running the rest of the code

imgFolder = 'C:\Users\Sauron\Desktop\BK_Testing\';
fileFormat   = '*.tif';   % '.tif' for micromanager

%% Set stimulation parameters

stimParams = struct();
stimParams.seqMode = 0; % no need to run in sequential mode for aberration corrrection
stimParams.niter = 1;

stimParams.VMode = 5; % vortex mode
stimParams.VLens = 600; % virtual lens
stimParams.GS = 3; % number of iterations in the Gerchberg-Saxton algorithm
stimParams.dll_name = "Overdrive_SDK_C_wrapper";
stimParams.board_number = 1;

xy = readmatrix('phaseMaskCenter.csv');  % reads the coordinates of the optimal phase mask center

stimParams.xShift = xy(1);
stimParams.yShift = xy(2);

target_cells = [0,0]; % optical vortex at the center

calllib('PiUsb', 'piSetShutterState', 1, app.stimShutter); % open USB shutter

%% Find the Zernike polynomial coefficients to compensate for other aberrations

% correction for tip/tilt, astigmatism, coma, trefoil, and spherical aberrations

% 14 coeffients with respective (n,m) values:
% (1,-1),(1,1),(2,-2),(2,0),(2,2),(3,-3),(3,-1),(3,1),(3,3),(4,-4),(4,-2),(4,0),(4,2),(4,4)

% optimize each coefficient within range, -0.2: 0.02 : 0.2

% overwrite zernike_coeffs.csv with the optimized coefficients found in
% this aberration correction session

% --- Settings for acquisition timing ---
captureDelay   = 0.20;        % seconds to wait after SLM update before reading image
maxWaitForImg  = 1.0;         % max seconds to wait for newest file to be readable

% --- Zernike configuration ---
numCoeffs = 14;
testVals  = -0.2:0.02:0.2;    % range per coefficient
bestCoeffs  = zeros(1, numCoeffs);
bestScores  = -inf(1, numCoeffs);

% (Optional) names for logging
zernike_nm = { '(1,-1)','(1,1)','(2,-2)','(2,0)','(2,2)', ...
               '(3,-3)','(3,-1)','(3,1)','(3,3)', ...
               '(4,-4)','(4,-2)','(4,0)','(4,2)','(4,4)'};

% Pre-allocate (for speed/logging)
zernikeTry = zeros(1, numCoeffs);

for k = 1:numCoeffs
    fprintf('\n--- Coefficient %d %s ---\n', k, zernike_nm{k});
    bestVal_k   = 0;
    bestScore_k = -inf;

    for v = testVals
        % Set only the k-th coefficient; keep others zero
        zernikeTry = zeros(1, numCoeffs);
        zernikeTry(k) = v;

        % Apply to SLM
        [~,msk,I] = stimulateBNS_SLM(target_cells, stimParams,zernikeTry);

        % Give acquisition time to save the newest frame
        pause(captureDelay);

        % Read newest image; retry briefly to avoid mid-write errors
        IMG = readNewestImage(imgFolder, fileFormat, maxWaitForImg);
        if isempty(IMG)
            warning('No readable image for k=%d, v=%.3f. Skipping...', k, v);
            continue;
        end

        % Evaluate the vortex quality
        try
            s = vortexQuality(IMG);
        catch ME
            warning('vortexQuality failed (k=%d, v=%.3f): %s', k, v, ME.message);
            continue;
        end

        % Keep the best value for this coefficient
        if s > bestScore_k
            bestScore_k = s;
            bestVal_k   = v;
        end

        fprintf('  k=%2d %-7s v=%+.3f  score=%.5f %s\n', ...
            k, zernike_nm{k}, v, s, ternary(s==bestScore_k,'<-- best',''));
    end

    % Store best per-coefficient (independent)
    bestCoeffs(k) = bestVal_k;
    bestScores(k) = bestScore_k;
    fprintf('>>> Best for coeff %2d %s: v=%+.3f | score=%.5f\n', ...
        k, zernike_nm{k}, bestVal_k, bestScore_k);
end

% save results
writematrix(bestCoeffs, 'zernike_coeffs.csv');
fprintf('\nSaved optimized zernike coefficients to: zernike_coeffs.csv\n');

calllib('PiUsb', 'piSetShutterState', 0, app.stimShutter); % close USB shutter

function IMG = readNewestImage(folder, pattern, maxWaitSec)
    if nargin < 3, maxWaitSec = 0; end
    tStart = tic;
    IMG = [];
    lastTry = '';

    while toc(tStart) <= maxWaitSec
        d = dir(fullfile(folder, pattern));
        if ~isempty(d)
            [~, idx] = max([d.datenum]); % newest by timestamp
            newest = d(idx);
            if ~strcmp(newest.name, lastTry)
                try
                    IMG = imread(fullfile(newest.folder, newest.name));
                    return;
                catch
                    pause(0.05); % file might be mid-write
                end
                lastTry = newest.name;
            end
        end
        pause(0.05);
    end

    % Final attempt without guarding name changes
    d = dir(fullfile(folder, pattern));
    if ~isempty(d)
        [~, idx] = max([d.datenum]);
        newest = d(idx);
        try
            IMG = imread(fullfile(newest.folder, newest.name));
        catch
            IMG = [];
        end
    end
end

function out = ternary(cond, a, b)
    if cond, out = a; else, out = b; end
end
