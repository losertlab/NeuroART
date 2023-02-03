function Z_nm = zernike(n,m,theta,rho)

% rho, theta --> dimensions = slm_dims, passed from dhot.m

R_m = factorial(n)*(rho.^n)./((factorial(n/2 + abs(m)/2))*factorial(n/2 - abs(m)/2)); % s = 0

if (n/2 - abs(m)/2)>0  
    for s = 1:(n/2 - abs(m)/2)
        R_m = R_m + ((-1)^s) *factorial(n-s)*(rho.^(n-2*s))./((factorial(n/2 + abs(m)/2 - s))*factorial(n/2 - abs(m)/2 -s)*factorial(s)); 
    end    
end

if m < 0
    Z_nm = -1*sqrt(2*(n+1)).*R_m.*sin(m*theta);
elseif m == 0
    Z_nm = sqrt(n+1).*R_m.*cos(m*theta);
else
    Z_nm = sqrt(2*(n+1)).*R_m.*cos(m*theta);      
end

end