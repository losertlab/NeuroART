function AddToMFile(fname, StructName, data, writeidx, rec_flag)
% This function write the structure 'data' to the file specified in fid.
% if no event, just return:
if isempty(data)
    return;
end
if nargin<5
    rec_flag=0;
end
% by default save all records
if nargin<4
    writeidx=1:length(data);
elseif isempty(writeidx)
    writeidx = 1:length(data);
end;
% mfile should have been created by baphy, so that if it exists, baphy asks the user for permission to overwrite, validation of name , database, etc.
% If it does not exist at this point, baphy had been unable to create it:
if ~exist(fname)
    error(sprintf('%s',['The file ' fname ' does not exist!']));
else
    fid = fopen(fname,'a');  % this might generate multiple handles but all point to the same file
end
if ~rec_flag % only first time
    fprintf(fid,'\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n');
    fprintf(fid,'%% ''%s'' is a %s: ',StructName, class(data));
    fprintf(fid,'\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n');
end
% if its an object, call recursively using get method:
if isobject(data)
    fclose (fid);
    AddToMfile (fname, [StructName], get(data),[],1);
    return;
end
fields = fieldnames(data);
% if data is an array,
for cnt1 = writeidx,
    % write all the fields
    for cnt2 = 1:length(fields)
        switch class(data(cnt1).(fields{cnt2}))
            case {'char', 'double'}
                % for numbers and strings
                try
                    fprintf(fid,'%s(%d).%s = ', StructName, cnt1,fields{cnt2});
                catch
                    % reopen if closed for some reason
                    fid = fopen(fname,'a');
                    fprintf(fid,'%s(%d).%s = ', StructName, cnt1,fields{cnt2});
                end
                WriteToFile(fid,data(cnt1).(fields{cnt2}));
                fprintf(fid,';\n');
            case 'cell' % for cells
                if ~isempty(data(cnt1).(fields{cnt2}))
                    if isnumeric(data(cnt1).(fields{cnt2}){1}) | ischar(data(cnt1).(fields{cnt2}){1})
                        fprintf(fid,'%s(%d).%s = {', StructName, cnt1, fields{cnt2});
                        for cnt3=1:length(data(cnt1).(fields{cnt2}));
                            WriteToFile(fid,data(cnt1).(fields{cnt2}){cnt3});
                            if cnt3 < length(data(cnt1).(fields{cnt2}))
                                fprintf(fid,', ');
                            end
                        end
                        fprintf(fid,'};\n');
                    else % this is a cell array of something else (structure, object, etc)
                        for cnt3=1:length(data(cnt1).(fields{cnt2}))
                            AddToMFile (fname, [StructName '(' num2str(cnt1) ').' fields{cnt2} ...
                                '{' num2str(cnt3) '}'], data(cnt1).(fields{cnt2}){cnt3},[],1);
                        end
                    end
                else
                    fprintf(fid,'%s(%d).%s = {};\n', StructName, cnt1, fields{cnt2});
                end
            case 'struct' % if its structure in structure, call recursively
                AddToMFile (fname, [StructName '(' num2str(cnt1) ').' fields{cnt2}], ...
                    data(cnt1).(fields{cnt2}),[],1);
            case 'matlab.ui.Figure'
                %Do nothing
            otherwise  % then it has to be a user defined object,, call recursively.
                fprintf(fid, '\n%% %s(%d).%s = %s\n', StructName, cnt1, fields{cnt2}, ['handle of a ' class(data(cnt1).(fields{cnt2})) ' object']);
                AddToMFile (fname, [StructName '(' num2str(cnt1) ').' fields{cnt2}], get(data(cnt1).(fields{cnt2})),[],1);
                fprintf(fid,'\n');
        end
    end
end
try
    fclose(fid);
catch
    
end

% This function writes a string or a numeric to the file
function WriteToFile (fid,a)
if isnumeric(a)
    fprintf(fid,'%s',mat2string(a));
else
    fprintf(fid,'''%s''',a);
end

