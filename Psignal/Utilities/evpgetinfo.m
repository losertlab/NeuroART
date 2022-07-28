% function [SpikechannelCount,AuxChannelCount,TrialCount,Spikefs,Auxfs,LFPChannelCount,LFPfs]=evpgetinfo(filename);
%
% displays information about evp file, returns number of spike- and
% auxillary channels and number of trials
%
% created SVD 2005-11-07
%
function [spikechancount,auxchancount,trialcount,spikefs,auxfs,lfpchancount,lfpfs]=evpgetinfo(filename);

if nargout==0,
    VERBOSE=1;
else
    VERBOSE=0;
end

[pp,bb,ee]=fileparts(filename);
bbpref=strsep(bb,'_');
bbpref=bbpref{1};
checkrawevp=[pp filesep 'raw' filesep bbpref filesep bb '.001.1' ee];
if exist(checkrawevp,'file'),
    filename=checkrawevp;
end

saveheader=0;
evpfilename=filename;

if ~exist(filename,'file'),
    [bb,pp]=basename(filename);
    hfilename=[pp 'tmp' filesep bb '.head'];
    if ~exist(hfilename,'file'),
        filename=evpmakelocal(filename);
        saveheader=1;
    else
        filename=hfilename;
    end
elseif strcmp(filename((end-3):end),'.tgz'),
    % compressed from MANTA system
    [bb,pp]=basename(filename);
    hfilename=[strrep(pp,'raw','tmp') strrep(bb,'.tgz','*.evp.head')];
    dd=dir(hfilename);
    if ~isempty(dd),
        hfilename=[strrep(pp,'raw','tmp') dd(1).name];
        filename=hfilename;
    else
        saveheader=1;
        hfilename=strrep(hfilename,'*','');
        filename=evpmakelocal(filename);
    end
end
[fid,sError]=fopen(filename,'r','l');
if fid==-1,
    error(['evp file ',filename,' not found']);
end

EVPVersion = double(fread(fid,1,'uint32'));
if EVPVersion<5 ||...
        EVPVersion==5 && ~isempty(findstr(filename,'.head')),
    header=[EVPVersion,double(fread(fid,6,'uint32'))', ...
        double(fread(fid,3,'single'))'];
    
    spikechancount=header(2);
    auxchancount=header(3);
    spikefs=header(4);
    auxfs=header(5);
    trialcount=header(6);
    lfpchancount=header(7);
    lfpfs=header(8);
    
    if header(2)==100 && header(3)>1000 && header(4)>1000 && EVPVersion==5,
        warning('incorrect .head file version, regenerating...');
        delete(hfilename);
        [spikechancount,auxchancount,trialcount,spikefs,auxfs,lfpchancount,lfpfs]=evpgetinfo(evpfilename)
    end
    
    
elseif EVPVersion==5,
    %EVP version 5
    headerLength = fread(fid,1,'uint32');
    header = [EVPVersion,headerLength,fread(fid,headerLength-2,'double')'];
    auxfs = 1000;
    basefilename = regexp(filename,'(?<basename>.*).001.1.evp','tokens','once');
    
    
    
    % GET NUMBER OF ELECTRODES
    i=0; while exist([basefilename{1},'.001.',sprintf('%d',i+1),'.evp'],'file'); i=i+1; end;
    spikechancount = i;
    % GET NUMBER OF TRIALS
    i=0; while exist([basefilename{1},'.',sprintf('%03d',i+1),'.1.evp'],'file'); i=i+1; end;
    trialcount = i;
    
    %files = dir([basefilename{1},'*.evp']);
    %filenames = {files.name};
    %data = regexp(filenames,'(?<basename>[a-zA-Z0-9_]*)\.(?<trials>[0-9]{3,5})\.(?<enums>[0-9]{1,3})\.evp','names');
    %data = [data{:}];
    %trialcount = length(unique({data.trials}));
    %spikechancount =length(unique({data.enums}));
    spikefs = header(7);
    lfpchancount = spikechancount;
    lfpfs = spikefs;
    auxchancount = 0;
    
    header=[EVPVersion spikechancount auxchancount spikefs auxfs trialcount lfpchancount lfpfs 0 0];
end

if VERBOSE,
    fprintf('file: %s\n',filename);
    fprintf(['evpversion: %d\nspikechancount: %d\nauxchancount: %d\n',...
        'fsspike: %d\nfsaux: %d\ntrials: %d\nlfpchancount: %d\nlfpfs: %.1f\n'],...
        EVPVersion,spikechancount,auxchancount,spikefs,auxfs,trialcount,lfpchancount,lfpfs);
end
fclose(fid);

if saveheader,
    fprintf('saving header for future speedup: %s\n',hfilename);
    pp=fileparts(hfilename);
    unix(['mkdir -p ',pp]);
    
    [fid,sError]=fopen(hfilename,'w','l');
    fwrite(fid,header(1:7),'uint32');
    fwrite(fid,header(8:10),'single');
    fclose(fid);
end

return

if header(1)==3,
    for tt=1:trialcount,
        trhead=fread(fid,2,'uint32');
        if VERBOSE,
            fprintf('trial %d: %d X %d spike samples; %d X %d aux samples\n',...
                tt,trhead(1),spikechancount,trhead(2),auxchancount);
        end
        
        % jump to start of next trial
        fseek(fid,(trhead(1)*spikechancount+trhead(2)*auxchancount)*2,0);
    end
else
    for tt=1:trialcount,
        trhead=fread(fid,3,'uint32');
        if VERBOSE,
            fprintf('trial %d: %d X %d spike samples; %d X %d aux samples; %d X %d lfp samples\n',...
                tt,trhead(1),spikechancount,trhead(2),auxchancount,trhead(3),lfpchancount);
        end
        
        % jump to start of next trial
        fseek(fid,(trhead(1)*spikechancount+trhead(2)*auxchancount+trhead(3)*lfpchancount)*2,0);
    end
end

fclose(fid);

