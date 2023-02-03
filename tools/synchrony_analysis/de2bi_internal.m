function b = de2bi_internal(d, n, p, msbFlag, protoType)
    % COMM.INTERNAL.UTILITIES.DE2BI Convert decimal numbers to binary
    % numbers.
    %
    % B = COMM.INTERNAL.UTILITIES.DE2BI(D, N, P, MSBFLAG, PROTOTYPE)
    % converts a nonnegative integer decimal vector D to a
    % binary matrix B. Each row of the binary matrix B corresponds to one
    % element of D.
    %
    % D must be a double precision column vector. D must contain finite
    % real nonnegative integer values.
    %
    % N defines how many digits (columns) are in the output. N must be
    % finite real integer scalar large enough to hold B for given P.
    % 
    % P defines which base to convert the decimal elements to. P must be a
    % finite real integer scalar greater than 1.
    %
    % MSBFLAG determines the output orientation. MSBFLAG has two possible
    % values, 'right-msb' and  'left-msb'.
    %
    % PROTOTYPE is a real scalar of the desired output data type.
    %
    % B is a matrix of size [length(D), N].
    %
    % This is an internal function which does not do any input validations.
    % It is users responsibility to ensure that the above conditions are
    % met.

    %   Copyright 2017 The MathWorks, Inc.
    
    %#codegen
    
    len_d = length(d);
    
    % Initial value.
    b = zeros(len_d, n);
    
    % Perform conversion.
    % Vectorized conversion for P=2 case
    if(p==2)
        if strcmp(msbFlag, 'right-msb')
            b(:) = de2biBase2RightMSB(d, n);
        else
            b(:) = de2biBase2LeftMSB(d, n);
        end
    else
        for i = 1 : len_d                   % Cycle through each element of the input vector/matrix.
            j = 1;
            tmp = d(i);
            while (j <= n) && (tmp > 0)     % Cycle through each digit.
                b(i, j) = rem(tmp, p);      % Determine current digit.
                tmp = floor(tmp/p);
                j = j + 1;
            end
        end
        % If a msbFlag is specified to flip the output such that the MSB is to the left.
        if strcmp(msbFlag, 'left-msb')
            b2 = b;
            b = b2(:,n:-1:1);
        end
    end
    
    b = cast(b, 'like', protoType);   % data type conversion
end
