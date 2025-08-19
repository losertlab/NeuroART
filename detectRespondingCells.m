function out = detectRespondingCells(dff, varargin)

% Identify neurons with strong periodic modulation at a target frequency,
% after robust baseline correction of N x T dF/F traces (neurons x time).
%
% Name-Value options (key ones):
%   'Fs'              : sampling rate Hz (default 14.3)
%   'TargetFreq'      : Hz (default 0.5)
%   'FreqTol'         : Hz half-band around target (default 0.05)
%   'MinSNRdB'        : PSD SNR threshold (default 6)
%   'MinAC'           : autocorr threshold at target lag (default 0.2)
%   'DoPlots'         : show SNR & autocorr summary (default false)
%
% Baseline correction options:
%   'BaselineMethod'  : 'movmedian' (default) or 'asls'
%   'BaselineWinSec'  : window (sec) for movmedian (default 20)
%   'AslsLambda'      : smoothness for ASLS (default 1e5)
%   'AslsP'           : asymmetry 0<p<1 for ASLS (default 0.001)
%   'SubtractOnly'    : true (default) -> x := x - baseline
%                       false -> x := (x - baseline)./max(baseline,eps)
%
% Example:
% out = detect_freq_responders_NxT(dff,'Fs',14.3,'TargetFreq',0.5,...
%       'BaselineMethod','movmedian','BaselineWinSec',20,'DoPlots',true);

p = inputParser;
p.addParameter('Fs',14.3,@(x)isscalar(x)&&x>0);
p.addParameter('TargetFreq',0.5,@(x)isscalar(x)&&x>0);
p.addParameter('FreqTol',0.05,@(x)isscalar(x)&&x>0);
p.addParameter('MinSNRdB',6,@(x)isscalar(x));
p.addParameter('MinAC',0.2,@(x)isscalar(x));
p.addParameter('DoPlots',false,@(x)islogical(x)||ismember(x,[0 1]));

% Baseline opts
p.addParameter('BaselineMethod','movmedian',@(s) any(strcmpi(s,{'movmedian','asls'})));
p.addParameter('BaselineWinSec',20,@(x)isscalar(x)&&x>0);
p.addParameter('AslsLambda',1e5,@(x)isscalar(x)&&x>0);
p.addParameter('AslsP',0.001,@(x)isscalar(x)&&x>0 && x<1);
p.addParameter('SubtractOnly',true,@(x)islogical(x)||ismember(x,[0 1]));

p.parse(varargin{:});
S = p.Results;

[N,T] = size(dff);
fs = S.Fs;
f0 = S.TargetFreq;
targetPeriod = 1/f0;

% ---- PSD params ----
win = max(round(10*fs), 64);          % ~10 s window
nfft = 2^nextpow2(win);
overlap = round(0.5*win);

fLo = max(0.05, f0 - S.FreqTol);
fHi = f0 + S.FreqTol;
baseBand = @(f) (f>=0.05 & f<=2 & ~(f>=fLo & f<=fHi));

% ---- Autocorr params ----
L  = round(targetPeriod*fs);           % target lag in frames
Lw = 1;                                % +/- 1 frame tolerance
maxLag = min(round(4*targetPeriod*fs), T-1);

snrDB = nan(N,1);
acPeak = nan(N,1);
pass   = false(N,1);

% ---- Main loop ----
for i = 1:N
    x = dff(i,:);
    if any(isnan(x)), x = fillmissing(x,'linear'); end

    % ---------- Baseline correction ----------
    switch lower(S.BaselineMethod)
        case 'movmedian'
            w = max(3, round(S.BaselineWinSec*fs));   % frames
            % Robust drift estimate
            b = movmedian(x, w, 'omitnan', 'Endpoints','shrink');
        case 'asls'
            b = asls_baseline(x, S.AslsLambda, S.AslsP, 10);
    end

    if S.SubtractOnly
        x = x - b;
    else
        baseSafe = max(b, eps);
        x = (x - b) ./ baseSafe;
    end

    % Z-score & detrend after baseline removal
    x = detrend(zscore(x));

    % ---------- PSD ----------
    [pxx,f] = pwelch(x, hamming(win), overlap, nfft, fs, 'onesided');
    tgtMask  = (f>=fLo & f<=fHi);
    baseMask = baseBand(f);
    Ptarget = mean(pxx(tgtMask));
    Pbase   = median(pxx(baseMask));
    snrDB(i) = 10*log10(Ptarget / Pbase);

    % ---------- Autocorr ----------
    [ac,lags] = xcorr(x, maxLag, 'coeff');
    pos = lags>=0; ac = ac(pos); lags = lags(pos);
    k0 = find(lags==L,1,'first');
    if ~isempty(k0)
        kIdx = k0+(-Lw:Lw);
        kIdx = kIdx(kIdx>=1 & kIdx<=numel(ac));
        acPeak(i) = max(ac(kIdx));
    end

    pass(i) = (snrDB(i) >= S.MinSNRdB) && (~isnan(acPeak(i)) && acPeak(i) >= S.MinAC);
end

% ---- Output ----
out = struct();
out.isResponder = pass;
out.snrDB = snrDB;
out.acPeak = acPeak;
out.params = S;

if S.DoPlots
    figure('Color','w','Name','Frequency Responders (baseline-corrected)');
    subplot(2,1,1); stem(snrDB,'filled'); yline(S.MinSNRdB,'r--');
    xlabel('Neuron'); ylabel('SNR (dB)');
    title(sprintf('PSD SNR near %.3f Hz',f0));
    subplot(2,1,2); stem(acPeak,'filled'); yline(S.MinAC,'r--');
    xlabel('Neuron'); ylabel(sprintf('Autocorr @ ~%.2f s',targetPeriod));
    title('Autocorr criterion');
end
end

% ====== Helpers ======

function b = asls_baseline(y, lambda, p, nIter)
% Asymmetric Least Squares baseline (Eilers & Boelens, 2005)
% y: row vector. lambda: smoothness, p: asymmetry (0<p<1), nIter: iterations
y = y(:);
L = numel(y);
D = diff(speye(L),2);            % 2nd order difference
W = speye(L);
for k = 1:nIter
    w = p*(y > diag(W)*y) + (1-p)*(y <= diag(W)*y); % update via residual sign
    W = spdiags(w,0,L,L);
    C = W + lambda*(D'*D);
    b = C \ (W*y);
end
b = b(:).';

end