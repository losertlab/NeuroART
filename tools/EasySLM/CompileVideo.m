

path= '';
vid= VideoWriter(fullfile(path,'video.avi'));
vid.FrameRate=10;
open(vid);

N= 231;
for i=1:N
   filename= "frame" + string(i-1) + ".png";
   disp(i);
   img= imread(fullfile(path,filename));
   frame= im2frame(img);
   writeVideo(vid,frame);
end
close(vid);