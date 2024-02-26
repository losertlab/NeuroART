
r=20;
addpath('../');
c= Config.LCR2500();
I= otslm.simple.gaussian([1024,1024], 150);  % Incident beam (gaussian)

grid_points= [0,0;-0.15,0;0.15,0;0,0.15;0,-0.15;-0.15,-0.15;0.15,0.15;-0.15,0.15;0.15,-0.15];
N= 1:length(grid_points);

%{
nn=0;
for k=1:length(grid_points)
   nn= nn + length(nchoosek(N,k)); 
end

inputs= zeros(c.res(1),c.res(2),nn);
%}

cam= c.open_camera();
vid= record('test.avi','framerate',1);

start(cam);
open(vid);

lens= slm.virtual_lens([0,0],195);

for k=1:6
    C= nchoosek(N,k);
    radii= zeros(length(k),1)+r;
    for i=1:size(C,1)
       input= Patterns.spots(c.res,grid_points(C(i,:),:),r);
       pattern = slm.cgh(input, 'alpha', 0.5, 'use_gpu', true, 'N', 10);
       p = slm.apply_lens(pattern,lens);
       slm.show(p);
       pause(0.15);
       img= capture(cam);
       img= flipud(img);
       writeVideo(vid,im2frame(img));
       %inputs(:,:,n)= input;
    end
end

close(vid)
