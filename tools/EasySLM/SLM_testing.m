
% Debugging SLM driver.
addpath('../');

c= Config.LCR2500();
%cam= c.open_camera();
%start(cam);
I= otslm.simple.gaussian(c.res, 150);  % Incident beam (gaussian)

slm= CGH( c, 'incident', I );
im = otslm.simple.aperture(c.res, 40, 'shape', 'circle');

pattern = slm.cgh(im, 'alpha', 0.5, 'use_gpu', true, 'N', 10);
%pattern = slm.multi_cgh(ims,'alpha', 0.5, 'use_gpu', true, 'N', 10);

lens= slm.virtual_lens([0,-0.01],195);
p = slm.apply_lens(pattern,lens);
slm.show(p);
%img= capture(cam);
%figure();
%imshow(img);
%stop(cam);
%delete(cam);

%frames= {p, slm.apply_lens(p,slm.ylens(10)), slm.apply_lens(p,slm.ylens(8)), ... 
%    slm.apply_lens(p,slm.xlens(10)), slm.apply_lens(p,slm.xylens(10,-45)), slm.apply_lens(p,slm.xlens(-8))};
%slm.play(frames,'fps',1);

function phase = rmask(phase,r,n)
    sz= size(phase);
    I= ones(r,r);
    for i=1:n
        mask= rand(sz(1)/r,sz(2)/r);
        mask= kron(mask,I);
        phase= SLM.apply_lens(phase,mask);
    end
end
