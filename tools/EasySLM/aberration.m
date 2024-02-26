function Z = aberration(sz, type)
    if type=="vtilt"
        m=-1; n=1;
    elseif type=="htilt"
        m=1; n=1;
    elseif type=="obl_astigmatism"
        m=-2; n=2;
    elseif type=="defocus"
        m=0; n=2;
    elseif type=="astigmatism"
        m=2; n=2;
    elseif type=="vtrefoil"
        m=-3; n=3;
    elseif type=="vcoma"
        m=-1; n=3;
    elseif type=="hcoma"
        m=1; n=3;
    elseif type=="obl_trefoil"
        m=3; n=3;
    elseif type=="obl_quadrafoil"
        m=4; n=4;
    elseif type=="obl_astigmatism2"
        m=-2; n=4;
    elseif type=="spherical"
        m=0; n=4;
    elseif type=="astigmatism2"
        m=2; n=4;
    elseif type=="vquadrafoil"
        m=4; n=4;
    else
        m=0; n=0;
    end
    Z= otslm.simple.zernike(sz,m,n);
end