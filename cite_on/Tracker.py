import numpy as np
from .utils.compute_overlap import compute_overlap

class Tracker:
    """
    CITE-On  identity tracker used for online detection.

    if optional values are not specified defaults will be used
    ...

    Attributes
    ----------
    threshold : int, optional
        IoU threshold to consider for a positive identification
    frame_buffer : float, optional
        number of frames worth of space to allocate in system memory for identities positions
        
    Methods
    -------
    def update(detections, align_miss=False):
        Update the identities in the tracker memory with a new set of detections.
    def get_positions(filter_hits=0):
        Returns the current identities positions.
    def get_history():
        Returns all the past (and present) positions of each identity
    """
    def __init__(self, threshold=0.25, frame_buffer=1000):
        #data is stored in a matrix of shape (n_frames,n_cells, 6(params) ), where params are [xmin,ymin,xmax,ymax,score(0 if lost), hits]
        
        self.frame_buffer=frame_buffer
        self.data=np.full((frame_buffer,1,6),np.nan, np.double)
        self.threshold=threshold
        self.current_frame=0
        
    def update(self, detections, align_miss=False):
        """
        Update the identities in the tracker memory with a new set of detections.

        Parameters
        ----------
        detections : np.ndarray(ndims=2, dtype=np.float)
            an array of shape (n_boxes, 4) for each box having [xmin,ymin,xmax,ymax]
        align_miss : bool, optional
            if True aligns missed identities with mean shift of actively detected ids
        Returns
        -------
        None
        """
        #first thing to do is to increment counter and allocate more memory if needed
        self.current_frame+=1
        if self.current_frame >= self.data.shape[0]:
            new_chunk=np.full((self.frame_buffer,self.data.shape[1],6), np.nan, np.double)
            self.data=np.concatenate((self.data,new_chunk),axis=0)
        
        #if there are no cells make a cell for each detection
        if self.data.shape[1]==1:
            if len(detections):
                for bbox in detections:
                    new_cell=np.full((self.data.shape[0],1,6), np.nan, np.double)
                    new_cell[self.current_frame,0,:5]=bbox.copy()
                    new_cell[self.current_frame,0,5]=1
                    self.data=np.concatenate((self.data, new_cell), axis=1)
        #if there are no detections do a null update on all ids(maybe implement warnings here)
        elif not len(detections):
            for c in range(1,self.data.shape[1]):
                self.data[self.current_frame,c]=self.data[self.current_frame-1,c].copy()
                self.data[self.current_frame,c,4]=0
        #real case
        elif self.data.shape[1]>1 and len(detections):
            #retrive all cells positions at previous frame and compute overlaps
            positions=self.data[self.current_frame-1,1:,:4].copy()
            overlaps=compute_overlap(positions,detections[:,:4].astype(np.float64))
            #declaring indexes arrays
            positions_matched=[]
            detections_matched=[]
            #sort overlaps per positions and get maxima of detections
            sorted_idx=np.argsort(-overlaps,axis=1)#3
            max_overlap=np.argmax(overlaps,axis=0)#4
            #run throuth each cell position
            for i, idx in enumerate(sorted_idx):
                for j in idx:
                    if overlaps[i,j]>=self.threshold:
                        if i not in positions_matched and max_overlap[j]==i:
                            #if we have an above threshold iou and if it is max for the other cell its a match, update id position
                            positions_matched.append(i)
                            detections_matched.append(j)
                            self.data[self.current_frame,i+1,:5]=detections[j,:].copy()
                            self.data[self.current_frame,i+1,5]=self.data[self.current_frame-1,i+1,5]+1
                            break
                        else:
                            #otherwise keep on searching
                            continue
                    else:
                        # if we're under threshold stop searching and propagate previous position 
                        self.data[self.current_frame,i+1]=self.data[self.current_frame-1,i+1].copy()
                        self.data[self.current_frame,i+1,4]=0
                        break
            if align_miss:
                # get the average centroid shift of positive ids from the previous detection
                new_boxes=self.data[self.current_frame,1:,:4][positions_matched,:].copy()
                old_boxes=self.data[self.current_frame-1,1:,:4][positions_matched,:].copy()
                new_xc=(new_boxes[:,2]-new_boxes[:,0])/2+new_boxes[:,0]
                new_yc=(new_boxes[:,3]-new_boxes[:,1])/2+new_boxes[:,1]
                old_xc=(old_boxes[:,2]-old_boxes[:,0])/2+old_boxes[:,0]
                old_yc=(old_boxes[:,3]-old_boxes[:,1])/2+old_boxes[:,1]
                delta_xc=np.mean(new_xc-old_xc)
                delta_yc=np.mean(new_yc-old_yc)
                # get missed cells and update them
                positions_unmatched=np.setdiff1d(np.arange(positions.shape[0]), positions_matched, True)
                self.data[self.current_frame,1:,:4][positions_unmatched][:,[0,2]] += delta_xc
                self.data[self.current_frame,1:,:4][positions_unmatched][:,[1,3]] += delta_yc
            # get unmatched detections and make a new entry for them
            detections_unmatched=np.setdiff1d(np.arange(detections.shape[0]), detections_matched, True)   
            for i in detections_unmatched:
                new_cell=np.full((self.data.shape[0],1,6), np.nan, np.double)
                new_cell[self.current_frame,0,:5]=detections[i].copy()
                new_cell[self.current_frame,0,5]=1
                self.data=np.concatenate((self.data, new_cell), axis=1)
                
    def get_positions(self, filter_hits=0):
        """
        Returns the current identities positions, ordered by id.

        Parameters
        ----------
        filter_hits : np.ndarray(ndims=2, dtype=np.uint16)
            if specified only id with filter_hits of detections will be returned [xmin,ymin,xmax,ymax,score(0 if lost), hits]
        Returns
        -------
        np.ndarray(ndims=2, dtype=np.uint16)
            an array of shape (num_id, 6) ordered by id and for each one having[xmin,ymin,xmax,ymax,score(0 if lost), hits]
        """
        if self.data.shape[1]>1:
            out=self.data[self.current_frame,1:].copy()
            if filter_hits:
                return out[out[:,5]>=filter_hits]
        return self.data[self.current_frame,1:] if self.data.shape[1]>1 else []

    def get_history(self):
        """
        Returns all the past (and present) positions of each identity

        Parameters
        ----------
        None
        Returns
        -------
        np.ndarray(ndims=3, dtype=np.uint16)
            an array of shape (num_updates,num_id, 6) ordered by id and for each one having[xmin,ymin,xmax,ymax,score(0 if lost), hits]
        """
        return self.data[1:current_frame,1:]