function LoadMFile(mfile)
%Load data from psygnal experiment
fid = fopen(mfile,'r');
tline = fgetl(fid);
while ischar(tline)
    eval(tline);
        tline = fgetl(fid);
end
fclose(fid)