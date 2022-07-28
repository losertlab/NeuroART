RegImg = randi(3000,512,512,1000,'uint16');
frameRate = 30;
pos_lst = [85 55;170 45;66 276;345 270; 255 155];
r_pixels = 10;
winsize = 300;
percentBaselineSub = 50;
neuropilSubPercent = 70;
computeDFF_new_coder(RegImg,frameRate,pos_lst,r_pixels,winsize,percentBaselineSub,neuropilSubPercent);