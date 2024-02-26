
% Debugging SLM driver.

addpath('../');

sz = [768,1024];      % Size of pattern

% incident = [];        % Incident beam (use default in visualize)
%incident = ones(sz); %otslm.simple.gaussian(sz, 150);  % Incident beam (gaussian)
%im = otslm.simple.aperture(sz, sz(1)/50, 'shape', 'circle');

c= Config.LCR2500();
I= otslm.simple.gaussian(c.res, 150);  % Incident beam (gaussian)
slm= CGH( c, 'incident', I );
im = otslm.simple.aperture(c.res, 40, 'shape', 'square');
sqr = slm.cgh(im, 'alpha', 0.5, 'use_gpu', true, 'N', 10);
im = otslm.simple.aperture(c.res, 40);
cir = slm.cgh(im, 'alpha', 0.5, 'use_gpu', true, 'N', 10);

slm= DHOT(c);
slm.add_vortex([-0.05,0.2],15,0.002);
slm.add_tweezer([0,-0.2],sqr,0);
slm.add_axicon([0.4,.01],-1/5,0);
pattern = slm.dhot('alpha',0.5);

f=173;

lens= slm.virtual_lens([0,0],f);
p = slm.apply_lens(pattern,lens);
slm.show(p);

cam= c.open_camera();
vid= record('dhot2.avi','framerate',5);

start(cam);
open(vid);

for i=1:99
   img= capture(cam);
   if i<21
      f= f+1; 
   end
   lens= slm.virtual_lens([0,0],f);
   p = slm.apply_lens(pattern,lens);
   slm.show(p);
   writeVideo(vid,im2frame(img));
end

stop(cam);
close(vid);
delete(cam);
delete(vid);
