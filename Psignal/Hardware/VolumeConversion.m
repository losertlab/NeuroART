function out = VolumeConversion(in,direction,Microphone)
% VolumeConversion converts between SPL [dB] and Voltage
% at the output of the Etymotic Research ER7C microphone amp.
%
%  The logarithmic relationship
%
%    SPL_1rel0 [dB] = 20 * log_10 (V1/V0)
%
%  between the relative  Sound Pressure Level (SPL_1rel0) in dB units and V1
%  with respect to the base voltage V0 is assumed. Absolute SPLs are attained
%  by adding SPL_1rel0 and SPL(V0), which was 94dB in this case.
%  The corresponding V0 = RMS of a Sine (@1000Hz,X dB),
%   where X depends on the calibration system.
%
% Measurement Setup for:
% - Etymotic ER-7C
%  The well defined probe tone (1000Hz, 94dB)is generated
%  in the little hole on the upper right hand.
%  The rubber recording tube has to be inserted into this tube for calibration.
%  The switch under "Output" produces two probe tones:
%   Setting:  '0'    :  corresponds to 94dB (Typical value for V0: 0.049)
%   Setting: '-20' :  corresponds to 74 dB (Typical value for V0: 0.005)
%
% - BK 4944 A
%  Amplification set to 10x
%
%  The measured voltage was sent to a NIDAQ-card,
%  where it was digitized and measured.
%
% see also: M_SpeakerCalibration
%
% This file is part of MANTA licensed under the GPL. See MANTA.m for details.

switch Microphone
    case 'BK4944A'
        dBSPL0 = 94;
        V0 = 0.0814; % Measurements in Volts based on 100x amplification: 94 dB SPL = 1 Pa. BK4944A measures at 0.000814 V/Pa.
    case 'ANL9401'
        dBSPL0 = 94;
        V0 = 1; % Measurements in Volts based on default amplification gain: 94 dB SPL = 1 Pa. ANL9401 measures at 1 V/Pa.
    otherwise error('Microphone not tested yet.');
end
if strcmp(direction,'dB2V')
    out = V0*10.^((in-dBSPL0)/20);
elseif strcmp(direction,'V2dB')
    out = dBSPL0 + 20*log10(in/V0);
else error('Not a valid conversion!');
end