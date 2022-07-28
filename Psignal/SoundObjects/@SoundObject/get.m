function varargout = get (varargin)
% This is a generic get function for all child classes. It means, its a get
% function for a class that has a parent object, e.g. any user defined
% sound class, any trial class, etc.

% Nima Mesgarani, October 2005, mnima@umd.edu

% check the number of inputs
switch nargin
    case 1
        % if property is specified, display all the fields if no output
        % otherwise return them in varargout
        fields = fieldnames(varargin{1});
        if nargout==0 , disp(sprintf('\t'));end
        if isobject(varargin{1}.(fields{end})) % if there is an object field first return that.
            order = [length(fields) 1:length(fields)-1];
        else
            order = 1:length(fields);
        end
        for cnt1 = order
            % if no output required, just display the fields. otherwise
            % put them as fields of output variable
            if isobject(varargin{1}.(fields{cnt1})) % if its an object, call its method
                % if this is parent, get the properties
                if cnt1==max(order)
                    ObjProp = get(varargin{1}.(fields{cnt1}));
                    Objfields = fieldnames(ObjProp);
                    for cnt2 =1:length(Objfields)
                        varargout{1}.(Objfields{cnt2}) = ObjProp.(Objfields{cnt2});
                    end
                else
                    varargout{1}.(fields{cnt1}) = varargin{1}.(fields{cnt1});
                end
            else
                varargout{1}.(fields{cnt1}) = varargin{1}.(fields{cnt1});
            end
        end
    case 2
        % if single property is specified, display it or return its value
        % based on argout
        fields = fieldnames(varargin{1});
        index = find(strcmpi (fields,varargin{2})==1);
        if isempty(index)
            if isobject(varargin{1}.(fields{end}))
                % go to the parent
                object_field = varargin{1}.(fields{end});
                temp = get(object_field,varargin{2});
            else
                error('%s \n%s', ['There is no ''' varargin{2} ''' property in specified class']);
            end
        else
            temp = varargin{1}.(fields{index});
        end
        varargout{1} = temp;
        % Only one property is allowed
    otherwise
        error('%s \n%s','Error using ==> get','Too many input arguments.');
end