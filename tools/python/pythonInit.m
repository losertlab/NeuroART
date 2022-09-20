function output = pythonInit()
    % Set up python environment.
    if ismac
        terminate(pyenv);
        [status, cmdout] = system("source ~/.bashrc; source ~/.bash_profile; which python");
        pyenv("Version", strtrim(cmdout));
    elseif ispc

    end
end