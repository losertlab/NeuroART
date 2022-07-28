function [P, neuronXY] = run_dhot(pmt_img, cameraToSlm, cameraToPmt, format, m, n, rad)
    assert(nargin >= 3);

    if nargin < 4
        format = 'uint16';
    end
    if nargin < 5
        m = 512;
    end
    if nargin < 6
        n = 512;
    end
    if nargin < 7
        rad = 0.01;
    end
    
    [~, ~, ext] = fileparts(pmt_img);
    if strcmp(ext, '.tif')
        img = imread(pmt_img);
    elseif strcmp(ext, '.raw')
        mapped = memmapfile(pmt_img, 'Format', format);
        img = reshape(mapped.data, m, n);
    else
        error(fprintf('unsupported file type: %s', pmt_img));
    end
    img = mat2gray(img);

    P = [];

    f = figure('units', 'normalized');
    hold on;

    colormap('gray');

    imagesc([0 1], [0 1], img);
    hold on;
    xlim([0 1]);
    ylim([0 1]);
    
    draw_limit_box(m, n, cameraToSlm, cameraToPmt);

    h = gca;
    set(f, 'Toolbar', 'none');
    set(f, 'Menubar', 'none');
    set(f, 'WindowButtonDownFcn', @window_button_down);
    set(f, 'WindowKeyPressFcn', @window_key_press);
    set(h, 'Unit', 'normalized', 'Position', [0 0 1 1]);
    set(h, 'xtick', []);
    set(h, 'ytick', []);
    uiwait(f);

    % figure;
    % hold on;
    % imagesc([0 1], [0 1], img);
    % xlim([0 1]);
    % ylim([0 1]);
    % scatter(P(:, 1), P(:, 2), 15*rad*m, 'red');
    
    % Save (x, y) coordinates of the neurons selected in the
    % average image.
    neuronXY = [m*P(:, 1) n*P(:, 2)];
    
    P = transform_points_to_slm_space(P, m, n, cameraToSlm, cameraToPmt);
    figure;
    scatter(P(:, 1), P(:, 2));
    xlim([0 1]);
    ylim([0 1]);
    uiwait;

    function window_button_down(src, event)
        pos = get(src, 'CurrentPoint');
        P = [P; pos];
        draw_circle(pos(1), pos(2), rad);
    end

    function window_key_press(src, event)
        switch event.Key
          case 'return'
            close(f);
        end
    end
end

function draw_circle(x0, y0, r)
    nsegs = 20;
    Theta = linspace(0, 2*pi, nsegs + 1);
    for k = 1:nsegs
        theta = Theta(k:k+1);
        x = x0 + r*cos(theta);
        y = y0 + r*sin(theta);
        line(x, y, 'Color', 'red');
    end
end

function draw_limit_box(m, n, cameraToSlm, cameraToPmt)
    Q = [0 0; 1 0; 1 1; 0 1; 0 0].*(ones(5, 1)*[m n]);
    
    tform1 = affine2d(cameraToSlm.tdata.T);
    tform2 = affine2d(cameraToPmt.tdata.T);
    
    [p, q] = transformPointsForward(affine2d(cameraToSlm.tdata.T), Q(:, 1), Q(:, 2));
    [p, q] = transformPointsInverse(affine2d(cameraToPmt.tdata.T), p, q);

    p = p/n;
    q = q/m;
    plot(p, q);
end

function P = transform_points_to_slm_space(P, m, n, cameraToSlm, cameraToPmt)
    T1 = affine2d(cameraToPmt.tdata.T);
    T2 = affine2d(cameraToSlm.tdata.T);
    p = n*P(:, 1);
    q = m*P(:, 2);
    [p, q] = transformPointsForward(T1, p, q);
    [p, q] = transformPointsInverse(T2, p, q);
    p = p/n;
    q = q/m;
    P = [p q];
end
