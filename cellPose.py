from cellpose import models, io, plot
import numpy as np
import pandas as pd
from scipy.ndimage import center_of_mass
from scipy.io import loadmat
import tifffile
import argparse


def segment(calcium_image):
    CHANNELS = [0, 0]
    model = models.Cellpose(gpu=True, model_type='cyto3')
    masks, flows, styles, diams = model.eval([calcium_image.mean(axis=0)], diameter=0, channels=CHANNELS, channel_axis=2)
    return masks[0]

# Construct the argument parser

ap = argparse.ArgumentParser()

#Add the arguments to the parser

ap.add_argument("-p", "--fPath", required=True) # file path
ap.add_argument("-n", "--nFrames", type=int, required=True) # number of frames in the initial batch

args = vars(ap.parse_args())

n_frames = args['nFrames']
image_path = args['fPath']

#image_path = r'C:\Users\dular\Documents\Barbara\data\2024-08-29_GL261_15 HRS WH_Cal590\multipage tiff\WT\2024-08-29_GL261.mvd2 - GL261 WT WH - C=1-1.tif'  # Update this path based on your file path
#calcium_image = tifffile.imread(image_path)

# Read only the first n frames
with tifffile.TiffFile(image_path) as tif:
    # Select the first n frames using slices
    calcium_image = tif.asarray(key=range(n_frames))


# Load the calcium_image from .mat file
# calcium_data = loadmat('calcium_image.mat')
# calcium_image = calcium_data['IMG']  # Assuming the variable is named 'IMG'

masks = segment(calcium_image)

labels = np.unique(masks)  # Get unique labels for masks
labels = labels[labels > 0]  # Exclude background (label 0)

centroids = []
for label in labels:
    centroid = center_of_mass(masks == label)
    centroids.append(centroid)  # Each centroid is a tuple (y, x)

centroids_df = pd.DataFrame(centroids, columns=['Y', 'X'])
centroids_df.to_csv('centroids.csv', index=False)  # Save to CSV
