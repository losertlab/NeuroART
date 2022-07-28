function Y = dre(X, type, alpha)
    assert(min(X(:)) >= 0);
    assert(max(X(:)) <= 1);
    if nargin < 2
        type = 1;
    end
    assert(type == 1 || type == 2);
    if type == 1
        if nargin < 3
            alpha = exp(1);
        end
        Y = g(f(X, alpha));
    elseif type == 2
        if nargin < 3
            alpha = sqrt(2);
        end
        Y = h(f(X, alpha));
    end
end

function Y = f(X, alpha)
    Y = (X.^alpha)./(1 - X);
end

function Y = g(X)
    Y = (2/pi)*atan((pi/2)*X);
end

function Y = h(X)
    Y = 1 - 2./(1 + exp(X));
end
