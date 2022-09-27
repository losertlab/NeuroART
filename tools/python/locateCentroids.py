import sys
sys.path.append(sys.argv[1]);
from image_analysis import locate_centroids
print(locate_centroids());
