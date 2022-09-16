function result = condEval(obj, key, default)
    if isfield(obj, key)
        result = obj.(key);
    else
        result = default;
    end
end