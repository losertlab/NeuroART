function o = set (o,varargin)
% This is a generic set function for all userdefined classes. Copy this 
% function in your objects directory.

% Nima Mesgarani, October 2005, mnima@umd.edu

switch nargin
    case 1
    % if one argument, display all the fields
        get(o);
    % or two inputs, just return the field
    case 2 
        get(o,varargin{1});
    case 3
        % if single property, change its value
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
        % Only one property is allowed
    otherwise 
        error('%s \n%s','Error using ==> set','Too many input arguments.');
end
caller = dbstack;
if length(caller)>1
    if ~strcmpi(caller(2).name, 'ObjUpdate')
        % if its not called from ObjUpdate function, run it. 
        % without this check, it will become a close loop.
%         disp(sprintf('called by %s',caller(2).name));
        o = ObjUpdate (o);
    end
else
    o = ObjUpdate(o);
end