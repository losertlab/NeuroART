function o = set (o,varargin)
switch nargin
    case 1
        get(o);
    case 2
        get(o,varargin{1});
    case 3
        fields = fieldnames(o);
        index = find(strcmpi (fields,varargin{1})==1);
        if isempty(index)
            object_field = o.(fields{end});
            if isobject(object_field)
                o.(fields{end}) = set(object_field,varargin{1},varargin{2});
            else
                error('%s \n%s', ['There is no ''' varargin{1} ''' property in specified class']);
            end
        else
            o.(fields{index})=varargin{2};
        end
    otherwise
        error('%s \n%s','Error using ==> set','Too many input arguments.');
end
caller = dbstack;
if length(caller)>1
    if ~strcmpi(caller(2).name, 'ObjUpdate')
        o = ObjUpdate (o);
    end
else
    o = ObjUpdate(o);
end