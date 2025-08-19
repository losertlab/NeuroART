addpath(genpath('tools'));
addpath(genpath('Psignal'));
addpath(genpath('BNS_SLM'));

%% Set image aquisition file path and the file format

% start acquisition in micromanager software before running the rest of the code

imgFolder = 'C:\Users\Sauron\Desktop\BK_Testing\';
fileFormat   = '*.tif';   % '.tif' for micromanager

captureDelay  = 0.20;                  % seconds to wait after SLM update (tune as needed)
lb            = [-100, -100];          % lower bounds for [xShift, yShift]
ub            = [ 100,  100];          % upper bounds for [xShift, yShift]
swarmSize     = 20;                    % number of particles
maxIters      = 1000;                    % PSO iterations

%% Set stimulation parameters

stimParams = struct();
stimParams.seqMode = 0; % no need to run in sequential mode for aberration corrrection
stimParams.niter = 1;

stimParams.VMode = 5; % vortex mode
stimParams.VLens = 600; % virtual lens
stimParams.GS = 3; % number of iterations in the Gerchberg-Saxton algorithm
stimParams.dll_name = "Overdrive_SDK_C_wrapper";
stimParams.board_number = 1;

calllib('PiUsb', 'piSetShutterState', 1, app.stimShutter); % open USB shutter

%% Optimize the phase mask center

target_cells = [0,0]; % optical vortex at the center
zernike = zeros(1,14); % zernike polynomial coefficients (initially set all to zero)

stimParams.xShift = 0; % x coordinate of the phase mask center; optimized by PSO
stimParams.yShift = 0; % y coordinate of the phase mask center; optimized by PSO

[~,msk,I] = stimulateBNS_SLM(target_cells, stimParams,zernike); % send commands to BNS SLM

%% Optimization using PSO (maximize score by minimizing -score) ---

objFun = @(xy) -scoreForShifts(xy, target_cells, stimParams, zernike, ...
                               imgFolder, fileFormat, captureDelay);

opts = optimoptions('particleswarm', ...
    'SwarmSize', swarmSize, ...
    'MaxIterations', maxIters, ...
    'Display', 'iter', ...
    'UseParallel', false, ...           % set true if you can update/read in parallel safely
    'FunctionTolerance', 1e-3);

[bestXY, bestNegScore, exitflag, output] = particleswarm(objFun, 2, lb, ub, opts);
bestScore = -bestNegScore;

fprintf('Best xShift=%.3f, yShift=%.3f | score=%.4f (exitflag=%d)\n', ...
        bestXY(1), bestXY(2), bestScore, exitflag);

% Apply best settings once more (optional)
stimParams.xShift = bestXY(1);
stimParams.yShift = bestXY(2);
[~, ~, ~] = stimulateBNS_SLM(target_cells, stimParams, zernike);

% Save coordinates of the phase mask center
writematrix(bestXY, 'phaseMaskCenter.csv');

calllib('PiUsb', 'piSetShutterState', 0, app.stimShutter); % close USB shutter

function negScore = scoreForShifts(xy, target_cells, stimParams, zernike, imgFolder, filePattern, captureDelay)
    % xy = [xShift, yShift]
    xs = xy(1);
    ys = xy(2);

    % Set shifts (note: xShift <- xs, yShift <- ys)
    stimParams.xShift = xs;
    stimParams.yShift = ys;

    % Send command to SLM
    try
        [~, ~, ~] = stimulateBNS_SLM(target_cells, stimParams, zernike);
    catch ME
        warning('stimulateBNS_SLM failed: %s', '%s',ME.message);
        negScore = 1e6;  % penalize failure
        return;
    end

    % Allow camera/acquisition to update the latest image
    pause(captureDelay);

    % Read the newest image (with a short wait loop)
    IMG = readNewestImage(imgFolder, filePattern, 1.0); % up to 1 s wait
    if isempty(IMG)
        warning('No image found in %s (pattern %s).', imgFolder, filePattern);
        negScore = 1e6;
        return;
    end

    % Compute score and return negative for maximization
    try
        s = vortexQuality(IMG);
        negScore = -s;
    catch ME
        warning('vortexQuality failed: %s', ME.message);
        negScore = 1e6;
    end
end

function IMG = readNewestImage(folder, pattern, maxWaitSec)
    % Returns the newest image in folder matching pattern.
    % Will retry for up to maxWaitSec to catch fresh frames.
    if nargin < 3, maxWaitSec = 0; end
    tStart = tic;
    IMG = [];
    lastName = '';

    while toc(tStart) <= maxWaitSec
        d = dir(fullfile(folder, pattern));
        if ~isempty(d)
            % pick newest by datenum
            [~, idx] = max([d.datenum]);
            newest = d(idx);
            if ~strcmp(newest.name, lastName)  % new file appeared
                try
                    IMG = imread(fullfile(newest.folder, newest.name));
                    return;
                catch
                    % sometimes file is mid-write; brief pause and retry
                    pause(0.05);
                end
            end
        end
        pause(0.05);
    end

    % One final attempt without the "new name" guard
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