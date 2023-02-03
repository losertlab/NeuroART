function data = extractJSON(filepath)
% Read data from json in file.
fid = fopen(filepath, "r");
raw = fread(fid,inf);
str = char(raw');
fclose(fid);
data = jsondecode(str);
end