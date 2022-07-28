alpha = 6;
J1 = dre(mat2gray(imread('s1.tif')), 2, alpha);
J2 = dre(mat2gray(imread('s2.tif')), 2, alpha);
J3 = dre(mat2gray(imread('s3.tif')), 2, alpha);
J4 = dre(mat2gray(imread('s4.tif')), 2, alpha);
J = mat2gray(J1 + J2 + J3 + J4);
imshow(J);
imwrite(J, 'Spattern.tif');
