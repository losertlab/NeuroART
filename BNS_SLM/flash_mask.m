function flash_mask(mask, interval, duration)
    assert(duration <= interval);

    f = figure('units', 'normalized');
    colormap('gray');

    blank_mask = zeros(size(mask), 'like', mask);

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

    should_display = true;
    while should_display
        pause(duration);
        if should_display
            imagesc([0 1], [0 1], blank_mask, [0 1]);
            h = gca;
            set(h, 'Unit', 'normalized', 'Position', [0 0 1 1]);
            set(h, 'xtick', []);
            set(h, 'ytick', []);
        end
        pause(interval - duration);
        if should_display
            imagesc([0 1], [0 1], mask, [0 1]);
            h = gca;
            set(h, 'Unit', 'normalized', 'Position', [0 0 1 1]);
            set(h, 'xtick', []);
            set(h, 'ytick', []);
        end
    end

    function window_key_press(src, event)
        switch event.Key
          case 'return'
            should_display = false;
            close(f);
        end
    end
    
    function window_close(src, event)
        should_display = false;
        delete(f);
    end
end
