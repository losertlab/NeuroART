function moveguiPlusRemote(Handle,Location)

set(Handle,'Units','pixels');
OP = get(Handle,'Position');
MP = get(0,'MonitorPosition');
SS = get(0,'ScreenSize');

switch lower(Location)
  case 'center';
    Position = [MP(1,3)/2-OP(3)/2,MP(1,4)/2-OP(4)/2-SS(2),OP(3:4)] ;
  case 'east';
    Position = [MP(1,3)-OP(3),MP(1,4)/2-OP(4)/2-SS(2),OP(3:4)] ;
  case 'west';
    Position = [10,MP(1,4)/2-OP(4)/2-SS(2),OP(3:4)] ;
  case 'north';
    Position = [MP(1,3)/2-OP(3)/2,MP(1,4)-OP(4)-SS(2),OP(3:4)] ;
  case 'south';
    Position = [MP(1,3)/2-OP(3)/2,100-SS(2),OP(3:4)] ;    
  otherwise
    
    error('Position not implemented.');
end
    
set(Handle,'Position',Position);