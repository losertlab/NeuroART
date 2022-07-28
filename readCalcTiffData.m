function data = readCalcTiffData(filename)
    warning('off','all') % Suppress all the tiff warnings
    tstack  = Tiff(filename);
    [I,J] = size(tstack.read());
    K = length(imfinfo(filename));
    data = zeros(I,J,1500);
    data(:,:,1)  = tstack.read();
    for n = 2:1500
        tstack.nextDirectory()
        data(:,:,n) = tstack.read();
        n
    end
    warning('on','all')
%     calcData = zeros([size(data,1) size(data,2) floor(size(data,3)/2)]);
%     j=1;
%     for i=1:2:size(data,3)
%         calcData(:,:,j)=data(:,:,i);
%         j=j+1;
%     end
end