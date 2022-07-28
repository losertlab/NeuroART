function o = ObjUpdate (o)
% ObjUpdate runs ObjUpdate for Primary and Probe objects
pri = get(o,'PrimaryHandle');
pro = get(o,'ProbeHandle');
if ~isempty(pro)
    o=set(o,'ProbeClass',class(pro));
end
if ~isempty(pri)
    o=set(o,'PrimaryClass',class(pri));
elseif isempty(pri)
    o=set(o,'TrialIndices',[]);
end







