
% x range: [-0.6, 0.55] or [150,850]
% y range: [-0.35, 0.3] or [200,600]

addpath('../');
c= Config.LCR2500();
I= otslm.simple.gaussian([1024,1024], 150);  % Incident beam (gaussian)

slm= CGH( c, 'incident', I);

x0= -0.55;
y0= -0.25;
xmax= 0.5;
ymax= 0.3;

spacing= 0.05;
jsteps= (xmax-x0)/spacing;
isteps= (ymax-y0)/spacing;

img= otslm.simple.aperture(c.res, 20, 'shape', 'square');
pattern = slm.cgh(img, 'alpha', 0.5, 'use_gpu', true, 'N', 10);
N=0;
x=x0;
y=y0;

cam= c.open_camera();
vid= record('test.avi','framerate',5);

start(cam);
open(vid);

for i=1:isteps
    for j=1:jsteps
        lens= slm.virtual_lens([x,y],193);
        p = slm.apply_lens(pattern,lens);
    	slm.show(p);
        pause(0.5);
        img= capture(cam);
        img= flipud(img);
        writeVideo(vid,im2frame(img));
        x= x+spacing;
        N= N+1;
    end
    y= y+spacing;   
    x=x0;
end

stop(cam);
close(vid);
delete(cam);
delete(vid);


