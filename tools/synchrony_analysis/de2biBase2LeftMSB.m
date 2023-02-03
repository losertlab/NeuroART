function b = de2biBase2LeftMSB(d, n)
    % COMM.INTERNAL.UTILITIES.DE2BIBASE2LEFTMSB Convert decimal numbers to
    % binary numbers with Left-MSB orientation
    %
    % B = COMM.INTERNAL.UTILITIES.DE2BIBASE2LEFTMSB(D, N) converts a
    % nonnegative integer decimal vector D to a binary matrix B with
    % Left-MSB orientation; the first element in each row of B represents
    % the most significant bit. Each row of the binary matrix B corresponds
    % to one element of D.
    %
    % D must be a double or single precision column vector. D must contain
    % finite real nonnegative integer values.
    %
    % N defines how many digits (columns) are in the output. N must be
    % finite real integer scalar large enough to hold B
    %
    % B is a matrix of size [length(D), N].
    %
    % This is an internal function which does not do any input validations.
    % It is users responsibility to ensure that the above conditions are
    % met.

    %   Copyright 2017 The MathWorks, Inc.
    
    %#codegen

    b = rem(floor(d*pow2(1-n:0)),2);
end
