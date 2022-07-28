import numpy as np
from multiprocessing import Pool, set_start_method

#this is the multiprocessing.Pool target function, it's defined outside the Extractor class due to multiprocessing scope issues  
def _p_extract(patch,mima):
    try:
        mi,ma=np.percentile(patch,mima)
        sel=patch[patch>=mi]
        if ma>mi:
            sel=sel[sel<=ma]
        return np.mean(sel)
    except Exception as e:
        return np.nan

class Extractor:
    """
    CITE-On parallelized trace extractor.
    
    if optional values are not specified defaults will be used
    ...

    Attributes
    ----------
    pmin : int, optional
        minimum percentile of pixel distribution for trace extraction
    pmax : int, optional
        maximum percentile of pixel distribution for trace extraction
    bg_corr : bool, optional
        if True background subtracting will be performed
    workers : int, optional
        number of worker processes to generate for trace extraction
    frame_buffer : float, optional
        number of frames worth of space to allocate in system memory for extracted traces   
        
    Methods
    -------
    def extract(frame, boxes):
        Extract the functional trace from a given set of boxes
    def get_traces(tail=0):
        Returns the funtional traces.
    """
    def __init__(self, pmin=80, pmax=95, bg_corr=True, workers=12, frame_buffer=1000):
        
        self.data=np.full((frame_buffer,1),np.nan,np.double)
        self.frame_buffer=frame_buffer
        self.mima=(pmin,pmax)
        self.pool=Pool(workers)
        self.bg_corr=bg_corr
        self.current_frame=0
        set_start_method('spawn', force=True)
        
    def extract(self, frame, boxes):
        """
        Extract the funtional traces from a set of ordered boxes.

        Parameters
        ----------
        frame : np.ndarray(ndims=2, dtype=np.uint16)
            frame to extract the functional trace from
        boxes : np.ndarray(ndims=2, dtype=np.float)
            set of ordered boxes in the form Detector or Tracker produce 
        Returns
        -------
        None
        """
        self.current_frame+=1
        if self.current_frame >= self.data.shape[0]:
            new_chunk=np.full((self.frame_buffer,self.data.shape[1]), np.nan, np.double)
            self.data=np.concatenate((self.data,new_chunk),axis=0)
        
        if len(boxes):
            # if we have new ids enlarge the data array
            if len(boxes)-self.data.shape[1]+1>0:
                new_cells=np.full((self.data.shape[0],len(boxes)-self.data.shape[1]+1), np.nan, np.double)
                self.data=np.concatenate((self.data, new_cells), axis=1)
            # save pixels from every single cell in an array while masking a copy of the frame with nans
            patches=[]
            bg_frame=frame.astype(np.double, copy=True)
            for b in boxes:
                bg_frame[int(b[1]):int(b[3]),int(b[0]):int(b[2])]=np.nan
                patches.append(frame[int(b[1]):int(b[3]),int(b[0]):int(b[2])].flatten())
            #let the pool of workers run throu each single set of pixet to retrieve the trace point and store it 
            traces=self.pool.starmap(_p_extract,[(p,self.mima) for p in patches])
            self.data[self.current_frame,1:]=traces 
            # the background subtraction is applied by averaging the masked frame excluding nans and subtracting that value 
            if self.bg_corr:
                #self.data[-1,1:]-=np.nanmean(bg_frame)
                self.data[self.current_frame,0]-=np.nanmean(bg_frame)       

    def get_traces(self, tail=0):
        """
        Returns the funtional traces.

        If tail is specified, only the last n_tail of the traces will be returned

        Parameters
        ----------
        tail : int, optional
            is specified, only the last n_tail of the traces will be returned
        
        Returns
        -------
        np.ndarray(ndims=2, dtype=np.uint16)
            an array of shape (num_frames/tail,num_boxes) with the functional trace for each box
        """
        if tail>0 and tail>self.data.shape[0]:
            return self.data[int(-tail):self.current_frame,1:]
        else:
            return self.data[1:self.current_frame,1:]
