function vid = record(filename, varargin)
    p = inputParser;
    p.addParameter('path', pwd); 
    p.addParameter('framerate', 1);
    p.parse(varargin{:});

    vid= VideoWriter(fullfile(p.Results.path,filename));
    vid.FrameRate= p.Results.framerate;
end