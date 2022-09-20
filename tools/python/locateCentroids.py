import sys
sys.path.append(sys.argv[1]);
from complex_dynamics.image_analysis.locate_centroids import locate_centroids
print(locate_centroids());
