function centroids = locateCentroids(complexDynamicsPath)
    % Locates centroids from image.
    [status, cmdout] = system("" + ...
        "source ~/.bashrc;" + ...
        "source ~/.bash_profile;" + ...
        "which python;" + ...
        "python tools/python/locateCentroids.py " + complexDynamicsPath ...
        ...
        );
    disp(cmdout);
    disp(complexDynamicsPath);
end