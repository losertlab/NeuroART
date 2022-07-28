function h=addgrid(xy,y,c,s);
%add a y grid line on the current plot=================
%xy     0-xgrid,1-ygrid
%y		  positions for grid
%c		  corlor
%s		  line style
if nargin==3
   s=':';
end

if xy==1
   x=get(gca,'xlim');
   for i=1:length(y)
      h(i)=line(x,[1 1]*y(i),'linestyle',s,'color',c);
   end
   return
else
   h=plotxgrid(y,c,s);
end

function h=plotxgrid(x,c,s);
%add a x grid line on the current plot=================
y=get(gca,'ylim');
for i=1:length(x)
   h(i)=line([1 1]*x(i),y,'linestyle',s,'color',c);
end
return