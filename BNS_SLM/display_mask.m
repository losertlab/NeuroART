function display_mask(mask, focal_length)
    if nargin == 2
        opts = lens_opts();
        opts.focal_length = focal_length;
        mask = mod(mask + lens(opts), 1);
    end

    f = figure('units', 'normalized', 'outerposition', [0 0 1 1]);
    colormap('gray');
    
    imagesc([0 1], [0 1], mask, [0 1]);
    xlim([0 1]);
    ylim([0 1]);
    
    h = gca;
    set(f, 'Toolbar', 'none');
    set(f, 'Menubar', 'none');
    set(f, 'CloseRequestFcn', @window_close);
    set(f, 'WindowKeyPressFcn', @window_key_press);
    set(h, 'Unit', 'normalized', 'Position', [0 0 1 1]);
    set(h, 'xtick', []);
    set(h, 'ytick', []);
    
    uiwait;

    function window_key_press(src, event)
        switch event.Key
          case 'return'
            delete(f);
        end
    end
    
    function window_close(src, event)
        should_display = false;
        delete(f);
    end
end
