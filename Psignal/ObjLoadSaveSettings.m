function varargout = ObjLoadSaveSettings (o, action, index, fname)
% ObjLoadSaveDefaults is a method of class SoundObject and can be used to store and load the properties of object o from a file. Multiple profiles
% can be saved and retrieved using index. The defaults are saved in the directory of the object o, under the name LastValues.mat
global globalparams exptparams home datapath roothome
animal = globalparams.Animal;
if nargin<3
    index = 1;
end
if nargin<2
    action = 'r';
end
if nargout>0
    varargout{1} = o;
end
object_spec = what(class(o));
values=[];
if exist(fname,'file')
    load (fname);
    descriptor = strsep(object_spec(1).path,'@');
    if exist('SavedValues')
        if isfield(SavedValues, descriptor{end})
            values = SavedValues.(descriptor{end});
        end
    end
else
    values=[];
end
fields = get(o,'UserDefinableFields');
if strcmp(action, 'w')
    % get the values first
    cnt2 = 1;
    for cnt1 = 1:length(fields)/3 % fields have name, type and default values.
        values{cnt1,index}= get(o, fields{cnt2});
        cnt2 = cnt2+3;
    end
    SavedValues.(descriptor{end}) = values;
    save (fname, 'SavedValues','-append');
else
    % load the values to the object. if the requested index does not exist, use the first index
    if size(values,2) < index
        index = 1;
    end
    cnt2 = 1;
    for cnt1 = 1:length(fields)/3
        if cnt1<=length(values) && ~isempty(values{cnt1,index})
            % delete the spaces at the end:
            if ischar(values{cnt1,index}) && strcmpi(values{cnt1,index},' ')
                values{cnt1,index} = strtok(values{cnt1,index});
            end
            o = set(o,fields{cnt2}, values{cnt1,index});
        else
            o = set(o,fields{cnt2}, get(o,fields{cnt2}));
        end
        cnt2 = cnt2 + 3;
    end
    varargout{1} = o;
end
