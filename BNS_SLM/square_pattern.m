function P = square_pattern(sidelength)
    if nargin == 0
        sidelength = 1;
    end
    P = sidelength*[1 1; 1 -1; -1 1; -1 -1]/2;
end
