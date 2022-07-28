import numpy as np
from PIL import Image
import os

class TSeries:
    """
    This class emulates frames coming from the microscobe by teading tiff files one frame at a time and computing different projections

    Attributes
    ----------
    path : str
        path to the tiff stack
    buffer : int, optional
        if specified, will produce the projection specified in kind 
    kind : str, optional
        kind of projection to compute, can be 'step' for making a projection every buffer frames or 'ring' that will make a rolling projection updating every frame
    projection : function, optional
        projection function to use
    init_shape : (int, int)
        shape of the image to prime the network with to allocate memory faster, requires init_upscaling
        
    Methods
    -------
    read():
        Returns a new frame and a new projection, if is the case
    set_index(index):
        Sets the inter frame counter
    Raises
    -------
    Indexerror
        If the stack is over
    """
    def __init__(self,path,buffer=None, kind='step', projection=np.mean):
        assert (buffer is None or buffer>1), 'buffer should be None or >1'
        assert (kind=='step' or kind=='ring'), 'buffer kind should be "step" or "ring"'
        if os.path.isfile(path):
            self.fd=Image.open(path)
            self.idx=0
            self.buf_len=0
            self.type=kind
            self.projection=projection
            if buffer:
                self.buf_len=buffer
                self.buf_counter=0
                self.buffer=np.empty((self.fd.size[1],self.fd.size[0],self.buf_len), np.float32)
            #print('T-Series has been successfully loaded!')
        else:
            raise FileNotFoundError(path + ' doesn\'t exist!')
    
    def __len__(self):
        return self.fd.n_frames
    
    def __getitem__(self,index):
        if index>=0 and index<self.fd.n_frames:
            self.fd.seek(index)
            retval=np.array(self.fd).astype(np.float32)
            self.fd.seek(self.idx)
            return retval
        else:
            raise IndexError('Index '+str(index)+' out of range of '+str(self.fd.n_frames))
    
    def set_index(self,index):
        if index>=0 and index<self.fd.n_frames:
            self.idx=index
        else:
            raise IndexError('Index '+str(index)+' out of range of '+str(self.fd.n_frames))
    
    def read(self):
        """
        Returns a new frame and a new projection, if is the case

        Parameters
        ----------
        None
        Returns
        -------
        np.ndarray(ndims=2, dtype=np.uint16) or ( np.ndarray(ndims=2, dtype=np.uint16),  np.ndarray(ndims=2, dtype=np.uint16))
            if buffer=0 returns just a single array with the frame, if buffer>0 returns 2 arrays the first is the new frame the sencon is the projection, if available, otherwise a None 
        """
        if self.idx < self.fd.n_frames:
            self.fd.seek(self.idx)
            retval=np.array(self.fd).astype(np.float32)
            self.idx += 1
            if self.buf_len:
                self.buffer[...,self.buf_counter]=retval
                self.buf_counter+=1
                if self.buf_counter>=self.buf_len:
                    if self.type=='step':
                        self.buf_counter=0
                        return retval, self.projection(self.buffer, axis=2, dtype=np.float32)
                    else:
                        mean=self.projection(self.buffer, axis=2, dtype=np.float32)
                        self.buffer=np.roll(self.buffer,-1,2)
                        self.buf_counter-=1
                        return retval, mean
                else:
                    if self.type=='step':
                        return retval, None
                    else:
                        return retval, None
            else:
                return retval
        else:
            raise IndexError('T-series is over, catch this error please!')
  