function stimulatePrairieLink(targetCells, stimParams, dimX, dimY)

% Send stimulation coordinates to PrairieLink

% targetCells: x, y coordinates of the cells to be stimulated
% (first column -> x, second column -> y)

% subsetSize: number of cells that will be targeted simultaneously
% dimX, dimY: dimensions of each frame (e.g. dimX = 512, dimY = 512)

% Initially Created by Dulara De Zoysa, UMD on 1/25/2022
% Enhanced, commented and debugged by Aleks Smirnov, JHU/UMD on 1/26/2022

IPaddress = stimParams.IPaddress; % IP address of computer running PrairieView
CRLF = string([char(13) char(10)]);
sep = char(0x01); % separator char
% LaserName =  stimParams.LaserName ;% "Carbide 1023"

nTargets = size(targetCells,1); % Total number of cells to be stimulated
subsetSize = stimParams.setSize;

if nTargets>=subsetSize
    selection = randsample(nTargets,subsetSize); % select a random sample
    targetCells = targetCells(selection,:);
else
    disp('Size of the subset exceeds the total number of stimulation targets');
    return;
end

% Normalize
targetCells(:,1) = targetCells(:,1)/dimX;
targetCells(:,2) = targetCells(:,2)/dimY;

% Send commands to PrairieLink

% pl = actxGetRunningServer('PrairieLink64.Application');
% pl.Connect(''); % add IP address here
pl = tcpclient(IPaddress,1236);

pause(0.5);
txtCommand = ["-MarkPointsMetadata","True"];
write(pl,uint8(char(  join(txtCommand, sep) + CRLF )));

% Mark points parameters: .. Number of points in group, 3D? + more to come
 
txtCommand = ["-MarkAllPoints",string(subsetSize) ,stimParams.DStim];

for itr = 1:subsetSize
    txtCommand = [txtCommand, string(targetCells(itr,1)), string(targetCells(itr,2))];  
    if stimParams.DStim == "True"
        txtCommand = [txtCommand, string(stimParams.zOffset)];  
    end
end
 % Stimulation parameters: Duration(ms), Laser name, Laser power(tics),
 %                          Spirals?, Size(%/100), Revolutions, Trigger(name="Carbide 1023"), Wait time(ms)
  
% txtCommand = [txtCommand, string( stimParams.duration), LaserName, string(power), "True", "0.018", "10", "None"];
txtCommand = [txtCommand, string(stimParams.duration), stimParams.LaserName, string(stimParams.power), stimParams.isSpiral, string(stimParams.spiralSize), string(stimParams.spiralRevolutions), stimParams.trigger];

if stimParams.trigger ~= "None"
    txtCommand = [txtCommand,string(stimParams.triggerCount), string(stimParams.delay)];

end

disp(join(txtCommand,'_'));
  
write(pl,uint8(char(  join(txtCommand, sep)+CRLF ))); %pl.SendScriptCommands("-x");
pause(0.5); % need pause at least 500 ms to allow command to execute
resp = char(read(pl)); % read responces
    % disp(string(char(resp)));
    
 clear pl;

% for itr = 1:subsetSize
%     
%     % Parameters for MarkAllPoints (please double check)
%     % (point count, is 3D, x, y , z offset, Is Spiral, Spiral size, Spiral Revolutions,Trigger(None/PFI1/PFI8/TrigIn),
%     % Trigger count (only for PFI8), Delay (ms))
% 
%     txtCommand = ['-MarkAllPoints',string(subsetSize), stimParams.DStim, string(targetCells(itr,1)), string(targetCells(itr,2)), string(stimParams.zOffset), stimParams.isSpiral, string(stimParams.spiralSize), string(stimParams.spiralRevolutions), stimParams.trigger, string(stimParams.triggerCount), string(stimParams.delay)];
% 
%     pl.SendScriptCommands(strjoin(txtCommand));
%     
% end
end