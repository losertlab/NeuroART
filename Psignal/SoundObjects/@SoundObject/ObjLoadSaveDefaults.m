function varargout = ObjLoadSaveDefaults (o, action, index)
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
fname = [roothome filesep 'LastValues.mat'];
values.(animal)=[];
if exist(fname,'file')
    load (fname);
    descriptor = strsep(object_spec(1).path,'@');
    if isfield(LastValues, descriptor{end})
        values = LastValues.(descriptor{end});
    end
else
    values.(animal)=[];
end
if ~isfield(values,animal)
    values.(animal)=[];
end
fields = get(o,'UserDefinableFields');
if strcmp(action, 'w')
    % get the values first
    cnt2 = 1;
    for cnt1 = 1:length(fields)/3 % fields have name, type and default values.
        values.(animal){cnt1,index}= get(o, fields{cnt2});
        cnt2 = cnt2+3;
    end
    LastValues.(descriptor{end}) = values;
    save (fname, 'LastValues');
elseif ~isempty(values.(animal))
    % load the values to the object. if the requested index does not exist, use the first index
    if size(values.(animal),2) < index
        index = 1;
    end
    cnt2 = 1;
    for cnt1 = 1:length(fields)/3
        if cnt1<=length(values.(animal)) && ~isempty(values.(animal){cnt1,index})
            % delete the spaces at the end:
            if ischar(values.(animal){cnt1,index}) && strcmpi(values.(animal){cnt1,index},' ')
                values.(animal){cnt1,index} = strtok(values.(animal){cnt1,index});
            end
            o = set(o,fields{cnt2}, values.(animal){cnt1,index});
        else
            o = set(o,fields{cnt2}, get(o,fields{cnt2}));
        end
        cnt2 = cnt2 + 3;
    end
    varargout{1} = o;
end
