function a = writeTorcInfo(fname,rippleList,ripParams)
% This function writes a .txt file for Daqsc for torc described by rippleList and ripParams
if nargin<1,
    error('please mention torc text filename');
end;
% default rippleList and ripParamsitions
rippleList0  = [1,  8, 1,  0];
ripParams0 = [1, 125, 5, 16000, 1, 1/20, 0, 1, 0.9, 120];
% arguments
if nargin < 2
    rippleList = rippleList0;
end
if nargin < 3
    ripParams = ripParams0;
end
if size(rippleList,2) < 4
    rippleList(:,4) = rippleList0(4);
end
for k = 2:10
    if length(ripParams) < k
        ripParams(k) = ripParams0(k);
    end
end
fid = fopen(fname,'w','b');
a = fprintf(fid,'%s%d%s\n','Sampling frequency              = ',ripParams(4),' Hz');
a = fprintf(fid,'%s%d%s\n','Ripple peak                     = ',ripParams(9)*100 ,' %');
a = fprintf(fid,'%s%d%s\n','Lower frequency component       = ',ripParams(2), ' Hz');
a = fprintf(fid,'%s%d%s\n','Upper frequency component       = ',ripParams(2)*2^ripParams(3), ' Hz');
if ripParams(5)
    a = fprintf(fid,'%s%d\n','Number of components            = ',ripParams(3)/ripParams(6));
else
    fr = ripParams(6)*(round(ripParams(2)/ripParams(6)):round(2.^ripParams(3)*ripParams(2)/ripParams(6))).';
    a = fprintf(fid,'%s%d\n','Number of components            = ',length(fr));
end;
a = fprintf(fid,'%s%d\n','Components harmonically spaced  = ',~ripParams(5));
if ripParams(5)
    a = fprintf(fid,'%s%d%s\n','Harmonic spacing                = ',0,' Hz');
else
    a = fprintf(fid,'%s%d%s\n','Harmonic spacing                = ',ripParams(6),' Hz');
end;
a = fprintf(fid,'%s%d%s\n','Spectral Power Decay            = ',ripParams(7),' dB/octave');
a = fprintf(fid,'%s%d\n','Components random phase         = ',1);
a = fprintf(fid,'%s%3.2f\n','Time duration                   = ',num2str(ripParams(1)));
a = fprintf(fid,'%s\n',['Ripple amplitudes               = (',...
    sprintf('%3.2f  ',rippleList(:,1)),')']);
a = fprintf(fid,'%s\n',['Ripple frequencies              = (',...
    sprintf('%3.2f  ',rippleList(:,3)),') cyc/oct']);
a = fprintf(fid,'%s\n',['Ripple phase shifts             = (',...
    sprintf('%3.2f  ',rippleList(:,4)), ') deg']);
a = fprintf(fid,'%s\n',['Angular frequencies             = (',...
    sprintf('%3.2f  ',rippleList(:,2)),') Hz']);
fclose(fid);
