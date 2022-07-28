function varargout = get (varargin)
switch nargin
    case 1
        fields = fieldnames(varargin{1});
        if nargout==0
            disp(sprintf('\t'));
        end
        if isobject(varargin{1}.(fields{end}))
            order = [length(fields) 1:length(fields)-1];
        else
            order = 1:length(fields);
        end
        for cnt1 = order
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
    otherwise
        error('%s \n%s','Error using ==> get','Too many input arguments.');
end