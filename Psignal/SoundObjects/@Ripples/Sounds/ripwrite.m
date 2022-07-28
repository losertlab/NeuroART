function  ripwrite(fname,a1am,a1rf,a1ph,a1rv);
% ripwrite(fname,a1am,a1rf,a1ph,a1rv);

nstim = size(a1am,2);

for abc = 1:nstim,

 str = num2str(abc);
 if length(str)==1, str = ['0' str]; end

 fid = fopen([fname '_' str '.lst'],'w')
 if ~iscell(a1am),
  fprintf(fid,'%2.4f %2.4f %4.4f %2.4f\n',[a1am(:,abc)';a1rf(:,abc)';a1ph(:,abc)';a1rv(:,abc)'])
 else
  fprintf(fid,'%2.4f %2.4f %4.4f %2.4f\n',[a1am{abc};a1rf{abc};a1ph{abc};a1rv{abc}])
 end

 fclose(fid)

end
