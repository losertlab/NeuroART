function [s,profile] = RippleGenerator(rippleList, ripParams)
% TorcTarcGenerator generates multiple moving ripples via FFT
%	s = TorcTarcGenerator(rippleList, ripParams);
%   [s, profile] = TorcTarcGenerator(rippleList, ripParams);
%	rippleList = [Am1, w1, Om1, Ph1
%				 Am2, w2, Om2, Ph2
%				 ....
%      			 AmN, wN, OmN, PhN];
%		Am: modulation depth, 0 < Am < 1, DEFAULT = 1;
%		w: rate (Hz), integer preferred, typically, 1 .. 128, DEFAULT = 8;
%		Om: scale (cyc/oct), any real number, typically, .25 .. 4, DEFAULT
%		= 1;
%		Ph: (optional) sine-symmetry at f0 in radians, DEFAULT = 0.
%       N >= 1
%	ripParams = (optional) [T0, f0, BW, SF, CF, df, RO, AF, Mo];
%		T0: duartion (sec), DEFAULT = 1.
%		f0: lowest freq. (Hz), DEFAULT = 250.
%		BW: excitation band width (oct), DEFAULT = 5.
%		SF: sample freq. (Hz), should be power of 2, DEFAULT = 16384
%		CF: component-spacing flag, 1->log spacing, 0->harmonic, DEFAULT = 1
%		df: freq. spacing, in oct (CF=1) or in Hz (CF=0), DEFAULT = 1/16 or 1.
%		RO: roll-off (dB/oct), DEFAULT = 0 (CF=1) or 3 (CF=0)
%		AF: amplitude flag, 1->linear, 0->log (dB), DEFAULT = 1;
%		Mo: amp. total mod: 0<Mo<1 (AF=1); 0<Mo dB (AF=0); , DEF=0.9 or 10 dB
%		wM: Maximum temporal velocity to consider in Hz (DEFAULT = 120)
%       Ph: Component phases (0-Random,1-Save,2-load).
%	profile = spectro-temporal envelope *linear* profile;
%
% 11/12 Edited by Nikolas Francis (originally by Powen Ru, Jian Lin, Jonathan Simon and Tai-Shih
%Initialize Parameters
Am = rippleList(:,1);
w = rippleList(:,2);
Om = rippleList(:,3);
Ph = rippleList(:,4)-pi/2;
T0 = ripParams(1); 	% actual duration in seripParamss
f0 = ripParams(2);	% lowest freq
BW = ripParams(3);	% bandwidth, # of octaves
SF = ripParams(4);	% sample freq, 16384, must be an even number
CF = ripParams(5);	% component-spacing flag, 1->log spacing, 0->harmonic
df = ripParams(6);	% freq. spacing, in oct (CF=1) or in Hz (CF=0)
RO = ripParams(7);	% roll-off in dB/Oct
AF = ripParams(8);	% amplitude flag, 1->linear, 0->log (dB)
Mo = ripParams(9);	% amp. total mod: 0<Mo<1 (Af=1); 0<Mo dB (Af=0)
wM = ripParams(10);	% amp. total mod: 0<Mo<1 (Af=1); 0<Mo dB (Af=0)
PhFlag = ripParams(11);% Flag which determines how to set the compnent flags
if abs(round(abs(w)*sum(T0)-abs(w)*sum(T0))) > 1e-5
    error('Ripple Velocities not commensurate with Time')
end
max_rvel = max(max(abs(w)), 1/sum(T0));
max_rfrq = max(max(abs(Om)), 1/BW);
t_step = 1/(16*max_rvel);
f_step = 1/(16*max_rfrq);
t_env_size = round((sum(T0)/t_step)/2)*2;
f_env_size = round((BW/f_step)/2)*2;
t_env = [0:t_env_size-1]  *t_step;
f_env = [0:f_env_size-1].'*f_step;
% Compute the max and min of the modulation envelope
profile	= zeros(f_env_size,t_env_size);
for row = 1:size(rippleList,1)
    rip_amp 	= Am(row);
    rip_vel 	= w(row);
    rip_freq	= Om(row);
    rip_phase	= Ph(row)+pi/2;
    f_phase = 2*pi*rip_freq*f_env + rip_phase;
    t_phase = 2*pi*rip_vel*t_env;
    profile = profile + ...
        rip_amp*(sin(f_phase)*cos(t_phase)+cos(f_phase)*sin(t_phase));
end
min_pro = min(min(profile));
max_pro = max(max(profile));
max_pro = max(max_pro, -min_pro);
if abs(max_pro)<1e-7; max_pro = sum(Am);end % if all w = Om = Ph = 0
if AF==1
    t_step = 1/(4*max_rvel);% if linear, max_rvel is Nyquest freq for envelope, but 2 isn't enough.
else
    t_step = 1/(2*round(wM*sum(T0))/sum(T0));% otherwise, use wM as lowest freq for envelope, but 2
end
t_env_size = round((sum(T0)/t_step)/2)*2;
t_env = [0:t_env_size-1]*t_step;
% freq axis
if CF==0
    %compute harmonic tones freqs
    fr = df*(round(f0/df):round(2.^BW*f0/df)).';
else	%compute log-spaced tones freqs
    fr = f0*2.^((0:round(BW/df*2)/2-1)*df).';
end
f_env = log2(fr./f0);
f_env_size = length(fr);% # of component
%Compute the modulation envelope
profile	= zeros(f_env_size,t_env_size);
for row = 1:size(rippleList,1)
    rip_amp 	= Am(row);
    rip_vel		= w(row);
    rip_freq	= Om(row);
    rip_phase	= Ph(row)+pi/2;
    f_phase = 2*pi*rip_freq*f_env + rip_phase;
    t_phase = 2*pi*rip_vel*t_env;
    profile = profile + ...
        rip_amp*(sin(f_phase)*cos(t_phase)+cos(f_phase)*sin(t_phase));
end
profile = profile/max_pro;  % appropriately normalize
if AF==1
    profile = 1+profile*Mo; % shift so background = 1 & profile is envelope
else
    profile = 10.^(Mo/20*profile);
end
%%%%%%%%%%%%%%%%%%%%%%%% freq-domain AM %%%%%%%%%%%%%%%%%%%%%%%%%
L_sig = sum(T0)*SF;	% length of signal
% roll-off and phase relation
comp_phs_file = 'save_comp_phs';
switch PhFlag
    case 0
        th = 2*pi*rand(f_env_size,1);	% component phase, theta
    case 1
        th = 2*pi*rand(f_env_size,1);	% component phase, theta
        save(comp_phs_file,'th');%Save component phases
    case 2
        fp = load(comp_phs_file);
        th = fp.th;
    otherwise
        error('Invalid selection');
        return;
end;
r = 10.^(-log2(fr/f0)*RO/20);		% roll-off, RO = 20log10(r)
S = zeros(1, L_sig);	% memory allocation
t_env_size_2 = t_env_size/2;
for m = 1:f_env_size
    f_ind = round(fr(m)*T0);
    S_tmpA = fftshift(fft(profile(m,:)))*exp(j*th(m))*r(m)/t_env_size*L_sig/2;
    padzerosonleft = f_ind - t_env_size_2 - 1;
    padzerosonright = L_sig/2 - f_ind - t_env_size_2;
    if ((padzerosonleft > 0) & (padzerosonright > 0) )
        S_tmpB = [zeros(1,padzerosonleft),S_tmpA,zeros(1,padzerosonright)];
    elseif ((padzerosonleft <= 0) & (padzerosonright > 0) )
        S_tmpB = [S_tmpA(1 - padzerosonleft:end),zeros(1,padzerosonright)];
    elseif ((padzerosonleft > 0) & (padzerosonright <= 0) )
        S_tmpB = [zeros(1,padzerosonleft),S_tmpA(1:end+padzerosonright)];
    end
    S_tmpC = [0, S_tmpB, 0, fliplr(conj(S_tmpB))];
    S = S + S_tmpC; % don't really have to do it all--know from padzeros which ones to do...
end
s=real(ifft(S));
s = s'./max(abs(s));
