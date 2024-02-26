
% Debugging SLM driver.

addpath('../');
%im= imread('+input\squiggle2.png');
%im = rgb2gray(im);

c= Config.LCR2500();
I= otslm.simple.gaussian(c.res, 150);  % Incident beam (gaussian)
slm= CGH( c, 'incident', I );
im= Patterns.crosshair(c.res,2);

pattern = slm.cgh(im, 'alpha', 0.5, 'use_gpu', false, 'N', 10);
%pattern = slm.multi_cgh(ims,'alpha', 0.5, 'use_gpu', true, 'N', 10);

lens= slm.zlens(190);
p = slm.apply_lens(pattern,lens);
%slm.open_camera();

start=-0.25;
step=0.05;
D= start;
for i=1:10
    lens= slm.xylens(-D,45);
    pp= slm.apply_lens(p,lens);
    slm.show(pp);
    pause(0.5);
    D= D+step;
    %pic= slm.capture();
    %filename= "C:\Users\Admin\Desktop\first_tests\crosshair\scattered\frame" + string(i) + ".png";
    %imwrite(pic,filename);
    %figure(i);
    %imagesc(pic);
end
