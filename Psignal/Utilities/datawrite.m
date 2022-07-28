function datawrite(filename,data,fs)
if isempty(filename),
    disp('not saving data');
    return
end
[sampcount,chancount]=size(data);
if ~exist(filename,'file'),
    drawnow;
    checkone=1;
else
    checkone=0;
end
if checkone && ~exist(filename,'file'),
    if exist('trialnumber','var') && trialnumber>1,
        error('first trial must be numbered 1');
    else
        trialnumber=1;
    end
    if ~exist('fs','var'),
        error('New evp: must specify fs and fsaux');
    end
    % new file: create main header
    [fid,sError]=fopen(filename,'w','l');
    disp(['Creating file ',filename,'... ',sError]);
    % save parms ... start with two 0's to identify it as small format
    header=[chancount fs 0 0 0];
    count=fwrite(fid,header(1:3),'uint32');
    count=fwrite(fid,header(4:5),'single');
else
    [fid,sError]=fopen(filename,'r+','l');
    header=fread(fid,5,'uint32');
    if header(1)~=chancount,
        error('channel count mismatch');
    end
    trialcount0=header(3);
    if ~exist('trialnumber','var'),
        trialnumber=trialcount0+1;
    elseif trialnumber~=trialcount0+1,
        error('trial number mismatch');
    end
    fseek(fid,0,1); %go to end of file
end
% write trial header
hcount=fwrite(fid,sampcount,'uint32');
count=0;
for ii=1:chancount,
    count=fwrite(fid,data(:,ii),'short');
end
fseek(fid,0,-1); %go to beginning of file
header(3)=trialnumber;
count=fwrite(fid,header(1:3),'uint32');
fclose(fid);

