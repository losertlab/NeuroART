import numpy as np
import matplotlib.pyplot as plt
import csv
import argparse
from matplotlib.patches import Rectangle as rect
from PIL import Image
from cite_on import Detector
from time import time
from scipy.cluster.hierarchy import dendrogram, linkage, leaves_list

def draw_annotations(img, ann, gt=None,save=None):
    f,ax=plt.subplots(1,1,figsize=(15,15))
    #pic=(np.repeat(img[:,:,np.newaxis],3,axis=2)/255).astype(np.uint8)
    ax.imshow(img)
    for i in range(ann.shape[0]):
        ax.add_patch(rect((ann[i,0],ann[i,1]),ann[i,2]-ann[i,0],ann[i,3]-ann[i,1],linewidth=1, edgecolor='lime', facecolor='none'))
    if gt is not None:
        for i in range(gt.shape[0]):
            ax.add_patch(rect((gt[i,0],gt[i,1]),gt[i,2]-gt[i,0],gt[i,3]-gt[i,1],linewidth=1, edgecolor='magenta', facecolor='none'))
    if save is not None:
        plt.savefig(save,format='svg')
    plt.show()

def multiline_plot(x, save=None):
    plt.figure(figsize=(15,15))
    for i in range(x.shape[0]):
        plt.plot((x[i,:]/np.max(x[i,:]))+i)
    if save:
        plt.savefig(save,format='svg')
    plt.show()

def clustermap(a):
    f=plt.figure(figsize=(10,10))
    Z = linkage(a, 'average', 'euclidean')
    foo=leaves_list(Z)
    gs = f.add_gridspec(5,4, hspace=0) 
    ax0=plt.subplot(gs[0,:])
    ax1=plt.subplot(gs[1:,:])
    im=ax1.imshow(np.triu(a[:,foo][foo])+np.tril(a), cmap='seismic', clim=(-1,1), aspect='auto')
    ax0.axis('off')
    with plt.rc_context({'lines.linewidth': 0.5}):
        dendrogram(Z, truncate_mode='level', p=10, ax=ax0, no_labels=True, color_threshold=-np.inf, link_color_func=lambda x: 'k')

# Construct the argument parser

ap = argparse.ArgumentParser()

#Add the arguments to the parser
ap.add_argument("-x", "--numRows", type=int, required=True)
ap.add_argument("-y", "--numCols", type=int, required=True)
ap.add_argument("-n", "--numFrames", type=int, required=True)
ap.add_argument("-p", "--fPath", required=True)
ap.add_argument("-u", "--upscaling", type=float, required=True)
ap.add_argument("-t", "--threshold", type=float, required=True)

args = vars(ap.parse_args())

partial=time()
rows = args['numRows']
cols = args['numCols']
n_frames = args['numFrames']
ups = args['upscaling']
th = args['threshold']

fd = open(args['fPath'], 'rb')
#im=Image.open(r"C:\Users\dular\Documents\LosertLab\Kelson_FRA\greenchannel_2000_tif.tif")
img=np.ndarray((rows,cols,n_frames),dtype=np.uint16)
#for i in range(im.n_frames):
#    im.seek(i)
#   fr=np.array(im)
#    img[:,:,i]=fr.copy()

#fd = open(r"C:\Users\dular\Documents\LosertLab\Kelson_FRA\greenchannel_6000.raw", 'rb')
#fd = open(r"C:\Users\dular\Documents\LosertLab\Nick_Data\test1.raw", 'rb')
for i in range(n_frames):
    f= np.fromfile(fd, dtype=np.uint16,count=rows*cols)
    img[:,:,i]=f.reshape((rows, cols))

#plt.imshow(img[:,:,1000])

#print('Import took: '+str(time()-partial)+'s')
partial=time()
# median=fast_median(img)
median=np.median(img,axis=2)
#std = np.std(img,axis=2)
#std1 = np.reshape(std, [1,512*512])
#median = (std - np.min(std1))/(np.percentile(std1,99.5)- np.min(std1))

#for i in range(0,512):
#    for j in range(0,512):
#        if median[i][j]>1:
#            median[i][j]=1



#f,ax=plt.subplots(1,1,figsize=(15,15))
#ax.imshow(median)
#plt.show()

#print('Median took: '+str(time()-partial)+'s')
#plt.figure(figsize=(15,15))
#plt.imshow(median)

#ups = 2.6 # to make init_upscaling = upscaling
#th = 0.5

detector=Detector('wheighs/', gpu='0', init_shape=img.shape[:2], init_upscaling=ups)
partial=time()
boxes=detector.detect(median,threshold=th,upscaling=ups)
#print('No of cells: ' + str(boxes.shape[0]));

# name of csv file 
filename = r"cell_coordinates.csv"
    
# writing to csv file 
with open(filename, 'w', newline='') as csvfile:
    # creating a csv writer object 
    csvwriter = csv.writer(csvfile) 
        
    # writing the data rows
    for i in range(boxes.shape[0]):
        csvwriter.writerow([(boxes[i,0]+boxes[i,2])/2, (boxes[i,1]+boxes[i,3])/2])

print('Detection took: '+str(time()-partial)+'s')
#draw_annotations(median,boxes, save='detections.svg')

#im1 = Image.fromarray(median)
#im1 = im1.save("bla.jpg")

#img=np.ndarray((512,512,30),dtype=np.uint16)
#detector=Detector('wheighs/', gpu='0', init_shape=img.shape[:2], init_upscaling=2);
#print(np.min(std1),np.percentile(std1,95),np.max(std1))
#print(args);
print('Success');
