%Made by Samira for calibrating the lens function
%Generating multiple phase masks for Vmode=30 with lens function -580:620
opts=dhot_opts();
opts.vortex_modes=30; 
pathname='C:\Users\Sauron\Desktop\BNS SLM\dhot-master_samira';
for i=600; %580:1:620
    for j=-0.5:0.25:0.5
   opts.lens=i*-1;
   mask=dhot([0+j 0.25], opts);
   foldername=['lens-' num2str(i) 'v30shifted_2pi'];
   cd(pathname);
  % mkdir(foldername)
   imwrite(mask, [pathname '\' foldername '\' 'lensplus' num2str(i) 'V30_y0.25x' num2str(j) '_2pi.tif'])
   cd([pathname '\dhot-master'])
    end
end
