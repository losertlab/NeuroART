import numpy as np
import cv2
from skimage.feature import register_translation  

class MotionCorrection():

    def __init__(self,pix_precision):
        self.pix_precision = pix_precision
        

    def motion_corr(self,im_stream,register_stack=False,ref_image=None,ref_frame=None):
        if (ref_frame != None and ref_image.all() != None):
            raise Exception('ref_frame or ref_image must be None')
                    
        cols,rows,T = im_stream.shape

        if(ref_image.all()!=None):
            pass
        elif(ref_frame!=None):
            ref_image = im_stream[:,:,ref_frame]
        else:
            ref_image = im_stream[:,:,0]

        shift_vec = np.empty((T,2),dtype=np.float64)
        for i in range(T):
            shift_vec[i,:],_,_ = register_translation(ref_image, im_stream[:,:,i],upsample_factor=self.pix_precision)
            if register_stack:
                M = np.float32([[1,0,shift_vec[i,1]],[0,1,shift_vec[i,0]]])
                im_stream[:,:,i] = cv2.warpAffine(im_stream[:,:,i],M,(cols,rows))

        return shift_vec
        
        
        
        
        
