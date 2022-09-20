function output = pythonInit()
    % Set up python environment.
    if ismac
        [status, cmdout] = system("source ~/.bashrc; source ~/.bash_profile; which python");
        pyenv("Version", strtrim(cmdout));
    elseif ispc

    end
end