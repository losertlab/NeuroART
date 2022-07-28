function Response = IOResponseRead(HW)
global globalparams
fs = globalparams.HWparams.fsAI;
% This function reads the Response signal from the daq card.
if strcmpi(HW.params.DAQ,'ni')
    Response = HW.DIO.inputSingleScan;
    Response = Response(1);
elseif strcmpi(HW.params.DAQ,'audio')
    Response=0;
    [ResponseData ResponseNames] = IOReadAIData(HW);
    if length(ResponseData)>0
        Response = max(0,diff(round(ResponseData(end-(fs.*.01):end))));
        Response= ~isempty(find(Response,1));
    end
end
