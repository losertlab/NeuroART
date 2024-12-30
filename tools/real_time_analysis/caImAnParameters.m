function caimanParams = caImAnParameters()
    % Prompt the user for CaImAn parameters
    prompt = {'Size of components to be found (um):', ...
              'Number of components to be found:', ...
              'Typical transient time (sec):', ...
              'Order of AR dynamics:', ...
              'Number of background components:', ...
              'Minimum SNR threshold:', ...
              'Merging threshold:', ...
              'Space correlation threshold:', ...
              'Threshold for CNN classifier:', ...
              'Frames per second:'};
    dlgtitle = 'CaImAn Parameters';
    dims = [1 50];
    definput = {'10', '150', '50', '2', '2', '2', '0.8', '0.3', '0.2', '15'};
    answer = inputdlg(prompt, dlgtitle, dims, definput);
    if ~isempty(answer)
        caimanParams.tau = str2double(answer{1});
        caimanParams.K = str2double(answer{2});
        caimanParams.decayTime = str2double(answer{3});
        caimanParams.pOrder = str2double(answer{4});
        caimanParams.nBackground = str2double(answer{5});
        caimanParams.minSnr = str2double(answer{6});
        caimanParams.mergeThr = str2double(answer{7});
        caimanParams.spaceThr = str2double(answer{8});
        caimanParams.cnnThr = str2double(answer{9});
        caimanParams.frameRate = str2double(answer{10});
    else
        disp('User cancelled parameter input.');
        caimanParams = [];
    end
end
