function stimWhite = IOCalibrationFilter(stim, WhiteningSpec, mic)
global HW globalparams
if size(stim,1)<size(stim,2)
    stim=stim';
end
stimSpec = fft(stim,length(WhiteningSpec));
stimSpecPhase = angle(stimSpec);
stimSpecdB=VolumeConversion(abs(stimSpec),'V2dB',mic);
WhiteningSpecdB=VolumeConversion(WhiteningSpec,'V2dB',mic);
stimSpecdBWhite = stimSpecdB + WhiteningSpecdB;
stimSpecWhite = VolumeConversion(stimSpecdBWhite,'dB2V',mic);
stimSpecWhite = stimSpecWhite.*exp(j.*stimSpecPhase);
stimWhite  = real(ifft(stimSpecWhite));
stimWhite = stimWhite(1:length(stim));

   