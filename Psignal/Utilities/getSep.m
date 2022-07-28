function Sep = getSep
if ~isempty(findstr('PCWIN',computer)) 
    Sep = '\'; 
else Sep = '/'; 
end
