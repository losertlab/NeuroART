function realTimeReadXML(rawfile,workingDir,exptId)

addpath(genpath('utilities')); 
%% File parameters

[filepath,name,ext] = fileparts(rawfile);
filepath = strcat(filepath,filesep); % The folder to which images are written
filename = strcat(name,ext); % 
workingDir = strcat(workingDir,filesep); % The working directory of the realTime App

cd(filepath)

%% Check whether the writing has started
j = 0;
while j<3000 % Time out after 30 seconds

    if isfile(filename)
        fprintf('File identified\n')
        break;
    end
    j = j + 1;
    pause(0.01)
    
end

if j == 3000
    fprintf('Did not find file in 20 seconds\n');
    return
end

disp('aaa');
XML = danParseXML('Experiment.xml');
disp('bbb')
exptVars = xmlVersionCheck(XML);

%% Save experiment variables to the working directory of the real time app

save([workingDir strcat('exptVars_',exptId,'.mat')],'exptVars');

