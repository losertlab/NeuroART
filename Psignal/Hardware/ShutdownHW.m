function ShutdownHW(HW)
% Bring all DAQ channels to low, and delete DAQ ojects
if (nargin<1)
    HW=[];
end
%If HWSetup exists, open it and close all devices
fclose ('all');
if ~isempty(HW)
    devices = fieldnames(HW);
    for cnt1 = 1:length(devices)
        if ~isempty(HW.(devices{cnt1})) && isobject(HW.(devices{cnt1}))
            try stop(HW.(devices{cnt1} ))
            catch
            end
            try delete(HW.(devices{cnt1}))
            catch
            end
            try
                clear HW.(devices{cnt1}
            catch
            end
        end
    end
end
devices = instrfindall;
if ~isempty(devices)
    for cnt1 = 1:length(devices)
        delete(devices(cnt1));
    end
end
clear HW;
