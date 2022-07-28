from os import path
from itertools import groupby
from operator import itemgetter
import numpy as np

class Dataset:
    def __init__(self,csv_path):
        self.csv_path=csv_path

        with open(csv_path,'r') as fd:
            text=fd.read()
            fd.close()
        data=[i.split(',') for i in text.split('\n')]
        sortkeyfn = itemgetter(0)
        data.sort(key=sortkeyfn)
        self.data = {}
        for key,valuesiter in groupby(data, key=sortkeyfn):
            self.data[key] = list(v[1:5] for v in valuesiter)
        self.file_names=list(self.data.keys())
        for i in self.file_names:
            self.data[i]=np.array(self.data[i], dtype=np.double)
    
    def __len__(self):
        return len(self.file_names)
        
    def get_gt(self, idx):
        return self.data[self.file_names[idx]]
        
    def get_image(self, idx):
        return cv2.imread(path.join(path.split(self.csv_path)[0], self.file_names[idx]))
  