 function img = capture(cam, varargin)
    % Takes a single frame.
    p = inputParser;
    p.addParameter('mirror', 1);
    p.addParameter('filename', 0);
    p.addParameter('path', pwd);
    p.parse(varargin{:});

    wait(cam);
    img= getsnapshot(cam);
    if p.Results.mirror
       img= rot90(img,2);
    end

    if isstring(p.Results.filename)
        path= fullfile(p.Results.path,p.Results.filename);
        imwrite(img,path);
    end
end