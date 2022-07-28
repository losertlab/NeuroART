import os
import sys
import numpy as np
import cv2
from compute_overlap import *
from PIL import Image
from scipy.signal import butter,filtfilt,savgol_filter, find_peaks
from skimage.feature import register_translation

#this function compute all detection metrics given the gt boxes and the detected ones       
def compute_stats(ground_truth, detections, iou_threshold=0.5):
    #compute paiwise overlaps between boxes
    overlaps=compute_overlap(ground_truth.astype(np.float64),detections.astype(np.float64))
    matched_gt=[]
    matching_detections=[]
    aps=[]
    #descending sort overlap matrix row-wise in order to select maximum overlapping detection for each gt box
    sorted_idx=np.argsort(-overlaps,axis=1)
    #getting maximum overlapping gt box for each detected box; this is used as a control 
    max_overlaps=np.argmax(overlaps, axis=0)
    #running through all rows (ie. gt boxes) of sorted overlaps indexes 
    for i, idx in enumerate(sorted_idx):
            #running through all values in a row (detected boxes) 
            for j in idx:
                if overlaps[i,j]>=iou_threshold:
                    if i not in matched_gt and max_overlaps[j]==i:
                    #if above threshold and found a matching one save its index in the appropriate register and jump to next row 
                        matched_gt.append(i)
                        matching_detections.append(j)
                        aps.append(overlaps[i,j])
                        break
                    else:
                    #if already matched continue if there are any other box above threshold
                        continue
                #if below threshold jump to next row (no match)
                else:
                    aps.append(0)
                    break
    #compute unmatched indexes
    unmatched_detections=np.setdiff1d(np.arange(detections.shape[0]), matching_detections, True)
    unmatched_gt=np.setdiff1d(np.arange(ground_truth.shape[0]), matched_gt, True)
    #compute all stats    
    tp=len(matched_gt)
    fp=len(unmatched_detections)
    fn=len(unmatched_gt)
    
    precision=tp/(tp+fp) if tp+fp>0 else 0
    recall=tp/(tp+fn) if tp+fn>0 else 0
    f1=2*precision*recall/(precision+recall) if precision+recall>0 else 0
    map=np.mean(aps)
    stats={
        'precision':precision,
        'recall':recall,
        'f1':f1,
        'map':map,
        'true_positives':np.concatenate((np.array(matched_gt)[:,np.newaxis], np.array(matching_detections)[:,np.newaxis]), axis=1),
        'false_positives':np.array(unmatched_detections),
        'false_negatives':np.array(unmatched_gt)
    }
    return stats

#this function compute all detection metrics given the gt boxes and the detected ones       
def compute_3d_stats(ground_truth, detections, iou_threshold=0.5):
    #compute paiwise overlaps between boxes
    overlaps=compute_3d_overlap(ground_truth.astype(np.float64),detections.astype(np.float64))[...,0]
    matched_gt=[]
    matching_detections=[]
    aps=[]
    #descending sort overlap matrix row-wise in order to select maximum overlapping detection for each gt box
    sorted_idx=np.argsort(-overlaps,axis=1)
    #getting maximum overlapping gt box for each detected box; this is used as a control 
    max_overlaps=np.argmax(overlaps, axis=0)
    #running through all rows (ie. gt boxes) of sorted overlaps indexes 
    for i, idx in enumerate(sorted_idx):
            #running through all values in a row (detected boxes) 
            for j in idx:
                if overlaps[i,j]>=iou_threshold:
                    if i not in matched_gt and max_overlaps[j]==i:
                    #if above threshold and found a matching one save its index in the appropriate register and jump to next row 
                        matched_gt.append(i)
                        matching_detections.append(j)
                        aps.append(overlaps[i,j])
                        break
                    else:
                    #if already matched continue if there are any other box above threshold
                        continue
                #if below threshold jump to next row (no match)
                else:
                    aps.append(0)
                    break
    #compute unmatched indexes
    unmatched_detections=np.setdiff1d(np.arange(detections.shape[0]), matching_detections, True)
    unmatched_gt=np.setdiff1d(np.arange(ground_truth.shape[0]), matched_gt, True)
    #compute all stats    
    tp=len(matched_gt)
    fp=len(unmatched_detections)
    fn=len(unmatched_gt)
    
    precision=tp/(tp+fp)
    recall=tp/(tp+fn)
    f1=2*precision*recall/(precision+recall) if precision+recall>0 else 0
    map=np.mean(aps)
    stats={
        'precision':precision,
        'recall':recall,
        'f1':f1,
        'map':map,
        'true_positives':np.concatenate((np.array(matched_gt)[:,np.newaxis], np.array(matching_detections)[:,np.newaxis]), axis=1),
        'false_positives':np.array(unmatched_detections),
        'false_negatives':np.array(unmatched_gt)
    }
    return stats

#this is the wrapper class for the retinanet object detector
class Detector:

    def __init__(self,retina_model_path,gpu='0', init_shape=None, init_multiplier=None):
        #on init, model gets lazy loaded so in order to avoid slow-downs specifying the operative size is needed 
        assert ((init_shape is None and init_multiplier is None) or (init_shape is not None and init_multiplier is not None)), 'If used, init parameters should both be specified'
        if init_shape is not None:
            assert (len(init_shape)==2), 'init_shape should have len = 2'
        import tensorflow as tf
        #these are the default sizes used during training, if you trained a model with different sizes please change these parameters 
        self.MIN_SIDE=800
        self.MAX_SIDE=1333
        #computing the init array of zeors
        if init_shape is not None and init_multiplier is not None:
            init_array,_=self._preprocess(np.zeros(init_shape, np.float32), init_multiplier)
        else:
            init_array=np.zeros((1,800,800,3), np.float32)
        #setting visible gpus for the model to run on
        if gpu:
            os.environ['CUDA_VISIBLE_DEVICES'] = str(gpu)
        else:
            os.environ['CUDA_VISIBLE_DEVICES'] = ''
        #creating tf graph and session
        self.retina_graph=tf.Graph()
        with self.retina_graph.as_default():
            self.retina_sess=tf.Session()
            with self.retina_sess.as_default():
                #loading model definition and weights from tf SavedModel format (thus avoiding the use of keras)
                tf.saved_model.loader.load(self.retina_sess, [tf.saved_model.tag_constants.SERVING],retina_model_path)
                #make a dry run to initialize model input shape
                _ = self.retina_sess.run(['filtered_detections/map/TensorArrayStack_1/TensorArrayGatherV3:0','filtered_detections/map/TensorArrayStack/TensorArrayGatherV3:0'], feed_dict={'input_1:0': init_array})
        
        print('AGOnIA up \'n running!')

    def _preprocess(self, img, multiplier):
        if len(img.shape)==2:
            #cast to float32 is required by tensorflow,
            image=img.astype(np.float32)
            #image is normalized, converted to rgb format and then the per-channel mean of imagenet dataset is subtracted(this is the dataset on which the network is scratch trained)
            image=cv2.normalize(image, None, 0, 255, cv2.NORM_MINMAX,-1)
            image=np.repeat(image[:,:,np.newaxis],3,axis=2)
            image[..., 0] -= 103.939
            image[..., 1] -= 116.779
            image[..., 2] -= 123.68
            #here the appropriate scale factor is computed given the default training sizes and the required multiplier
            (rows, cols, _) = image.shape
            smallest_side = min(rows, cols)
            scale = self.MIN_SIDE*multiplier / smallest_side
            largest_side = max(rows, cols)
            if largest_side * scale > self.MAX_SIDE*multiplier:
                scale = self.MAX_SIDE*multiplier / largest_side
            image = cv2.resize(image, None, fx=scale, fy=scale)
            #one azis is added in order to account for batching
            image=np.expand_dims(image, axis=0)
        else:
            raise ValueError('Unsupported image format')
        return image,scale
    
    def _decode(self, threshold, scale, boxes, scores, y_offset=0, x_offset=0):
        #boxes are computed on upscaled image so they are divided by scale factor and then ordered by score
        boxes /= scale
        indices = np.where(scores[0, :] > threshold)[0]
        scores = scores[0][indices]
        scores_sort = np.argsort(-scores)
        image_boxes      = boxes[0, indices[scores_sort], :]
        image_scores     = scores[scores_sort]
        #and returned as a single ndarray
        detection=np.concatenate([image_boxes, np.expand_dims(image_scores, axis=1)], axis=1)
        #in case of tiled detection boxes need an offset applied here
        if y_offset or x_offset:
            detection[:,[0,2]]+=x_offset
            detection[:,[1,3]]+=y_offset
        return detection
        
    def _nms(self, detections, threshold=0.2):
        #from https://www.pyimagesearch.com/2015/02/16/faster-non-maximum-suppression-python/ 
        x1 = detections[:, 0]
        y1 = detections[:, 1]
        x2 = detections[:, 2]
        y2 = detections[:, 3]
        scores = detections[:, 4]

        areas = (x2 - x1 + 1) * (y2 - y1 + 1)
        order = scores.argsort()[::-1]

        keep = []
        while order.size > 0:
            i = order[0]
            keep.append(i)
            xx1 = np.maximum(x1[i], x1[order[1:]])
            yy1 = np.maximum(y1[i], y1[order[1:]])
            xx2 = np.minimum(x2[i], x2[order[1:]])
            yy2 = np.minimum(y2[i], y2[order[1:]])

            w = np.maximum(0.0, xx2 - xx1 + 1)
            h = np.maximum(0.0, yy2 - yy1 + 1)
            inter = w * h
            ovr = inter / (areas[i] + areas[order[1:]] - inter)

            inds = np.where(ovr <= threshold)[0]
            order = order[inds + 1]

        return detections[keep]

    def detect(self, img, threshold=0.05, multiplier=1):
        #preprocess, run session and then decode
        image, scale = self._preprocess(img, multiplier)
        with self.retina_sess.as_default():
            scores, boxes = self.retina_sess.run(['filtered_detections/map/TensorArrayStack_1/TensorArrayGatherV3:0','filtered_detections/map/TensorArrayStack/TensorArrayGatherV3:0'], feed_dict={'input_1:0': image})
        detection= self._decode(threshold, scale, boxes, scores)
        return detection
    
    def tile_detect(self, img, n_tess=2, overlap=0.1,threshold=0.05, multiplier=1):
        #for tiled detection 
        assert (overlap<=1 and overlap>=0), 'Overlap must be within 0 and 1'
        assert (n_tess>1), 'Tessel count should be > 1'
        y_step=int(img.shape[0]/n_tess)
        y_size=int(img.shape[0]/(n_tess-overlap))
        x_step=int(img.shape[1]/n_tess)
        x_size=int(img.shape[1]/(n_tess-overlap))
        batch=[]
        sizes=[]
        for y in range(n_tess):
            y_off = y*y_step if y*y_step+y_size<=img.shape[0] else img.shape[0]-y_size 
            for x in range(n_tess):
                x_off = x*x_step if x*x_step+x_size<=img.shape[1] else img.shape[1]-x_size 
                image, scale =self._preprocess(img[y_off:y_off+y_size, x_off:x_off+x_size,...], multiplier)
                batch = np.concatenate((batch, image), axis=0) if len(batch) else image
                sizes.append((scale, y_off, x_off))
        scores, boxes = self.retina_sess.run(['filtered_detections/map/TensorArrayStack_1/TensorArrayGatherV3:0','filtered_detections/map/TensorArrayStack/TensorArrayGatherV3:0'], feed_dict={'input_1:0': batch})
        detections=np.zeros((1,5))
        for i in range(len(batch)):
            detection=self._decode(threshold, sizes[i][0], boxes[i][np.newaxis,...], scores[i][np.newaxis,...], sizes[i][1], sizes[i][2])
            detections=np.concatenate((detections,detection), axis=0)
        
        detections = detections[detections[:,-1]>threshold,:]
        detections = self._nms(detections)
        return detections

    def _get_session(self):
        return self.retina_sess

        
class Tracker:

    def __init__(self, threshold=0.25, frame_buffer=1000):
        #data is stored in a matrix of shape (n_frames,n_cells, 6(params) ), where params are [xmin,ymin,xmax,ymax,score(0 if lost), hits]
        
        self.frame_buffer=frame_buffer
        self.data=np.full((frame_buffer,1,6),np.nan, np.double)
        self.threshold=threshold
        self.current_frame=0
        
    def update(self, detections, align_miss=False):
        #this gets called every frame updating tracklets, first thing to do is to create a new frame in any case
        # new_frame=np.full((1,self.data.shape[1],6), np.nan, np.double)
        # self.data=np.concatenate((self.data,new_frame), axis=0)
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
                            positions_matched.append(i)
                            detections_matched.append(j)
                            self.data[self.current_frame,i+1,:5]=detections[j,:].copy()
                            self.data[self.current_frame,i+1,5]=self.data[self.current_frame-1,i+1,5]+1
                            break
                        else:
                            continue
                    else:
                        self.data[self.current_frame,i+1]=self.data[self.current_frame-1,i+1].copy()
                        self.data[self.current_frame,i+1,4]=0
                        break
            if align_miss:
                new_boxes=self.data[self.current_frame,1:,:4][positions_matched,:].copy()
                old_boxes=self.data[self.current_frame-1,1:,:4][positions_matched,:].copy()
                new_xc=(new_boxes[:,2]-new_boxes[:,0])/2+new_boxes[:,0]
                new_yc=(new_boxes[:,3]-new_boxes[:,1])/2+new_boxes[:,1]
                old_xc=(old_boxes[:,2]-old_boxes[:,0])/2+old_boxes[:,0]
                old_yc=(old_boxes[:,3]-old_boxes[:,1])/2+old_boxes[:,1]
                delta_xc=np.mean(new_xc-old_xc)
                delta_yc=np.mean(new_yc-old_yc)
                # print(new_xc.shape, new_yc.shape, old_xc.shape, old_yc.shape, delta_xc, delta_yc)
                positions_unmatched=np.setdiff1d(np.arange(positions.shape[0]), positions_matched, True)
                self.data[self.current_frame,1:,:4][positions_unmatched][:,[0,2]] += delta_xc
                self.data[self.current_frame,1:,:4][positions_unmatched][:,[1,3]] += delta_yc
            detections_unmatched=np.setdiff1d(np.arange(detections.shape[0]), detections_matched, True)   
            for i in detections_unmatched:
                new_cell=np.full((self.data.shape[0],1,6), np.nan, np.double)
                new_cell[self.current_frame,0,:5]=detections[i].copy()
                new_cell[self.current_frame,0,5]=1
                self.data=np.concatenate((self.data, new_cell), axis=1)
                
    def get_positions(self, filter_hits=0):
        if self.data.shape[1]>1:
            out=self.data[self.current_frame,1:].copy()
            if filter_hits:
                return out[out[:,5]>=filter_hits]
        return self.data[self.current_frame,1:] if self.data.shape[1]>1 else []

class ZTracker:

    def __init__(self, max_age, threshold=0.25):
        #data is stored in a matrix of shapa (n_frames,n_cells, 7(params) ), where params are [xmin,ymin,xmax,ymax,score(0 if lost), age, id]
        self.data=np.full((1,1,7),np.nan, np.double)
        self.threshold=threshold
        self.old_data=[]
        self.max_age=max_age
        self.id_count=0
        
    def update(self, detections, align_miss=False):
        #this gets called every frame updating tracklets, first thing to do is to create a new frame in any case
        new_frame=np.full((1,self.data.shape[1],7), np.nan, np.double)
        self.data=np.concatenate((self.data,new_frame), axis=0)
        
        #if there are no cells make a cell for each detection
        if self.data.shape[1]==1:
            if len(detections):
                for bbox in detections:
                    new_cell=np.full((self.data.shape[0],1,7), np.nan, np.double)
                    new_cell[-1,0,:5]=bbox.copy()
                    new_cell[-1,0,5]=1
                    self.id_count=self.id_count+1
                    new_cell[-1,0,6]=self.id_count
                    self.data=np.concatenate((self.data, new_cell), axis=1)
        #if there are no detections do a null update on all ids(maybe implement warnings here)
        elif not len(detections):
            for c in range(1,self.data.shape[1]):
                self.data[-1,c]=self.data[-2,c].copy()
                self.data[-1,c,4]=0
        #real case
        elif self.data.shape[1]>1 and len(detections):
            #retrive all cells positions at previous frame and compute overlaps
            positions=self.data[-2,1:,:4].copy()
            overlaps=compute_overlap(positions,detections[:,:4].astype(np.float64))
            #declaring indexes arrays
            positions_matched=[]
            detections_matched=[]
            old_positions=[]
            #sort overlaps per positions and get maxima of detections
            sorted_idx=np.argsort(-overlaps,axis=1)#3
            max_overlap=np.argmax(overlaps,axis=0)#4
            #run throuth each cell position
            for i, idx in enumerate(sorted_idx):
                for j in idx:
                    if overlaps[i,j]>=self.threshold:
                        if i not in positions_matched and max_overlap[j]==i:
                            positions_matched.append(i)
                            detections_matched.append(j)
                            self.data[-1,i+1,:5]=detections[j,:].copy()
                            self.data[-1,i+1,5]=self.data[-2,i+1,5]+1
                            self.data[-1,i+1,6]=self.data[-2,i+1,6]
                            if self.data[-1,i+1,5]>self.max_age:
                                old_positions.append(i)
                            break
                        else:
                            continue
                    else:
                        self.data[-1,i+1]=self.data[-2,i+1].copy()
                        self.data[-1,i+1,4]=0
                        self.data[-1,i+1,5]=self.data[-2,i+1,5]+1
                        self.data[-1,i+1,6]=self.data[-2,i+1,6]
                        if self.data[-1,i+1,5]>self.max_age:
                            old_positions.append(i)
                        break
            if align_miss:
                new_boxes=self.data[-1,1:,:4][positions_matched,:].copy()
                old_boxes=self.data[-2,1:,:4][positions_matched,:].copy()
                new_xc=(new_boxes[:,2]-new_boxes[:,0])/2+new_boxes[:,0]
                new_yc=(new_boxes[:,3]-new_boxes[:,1])/2+new_boxes[:,1]
                old_xc=(old_boxes[:,2]-old_boxes[:,0])/2+old_boxes[:,0]
                old_yc=(old_boxes[:,3]-old_boxes[:,1])/2+old_boxes[:,1]
                delta_xc=np.mean(new_xc-old_xc)
                delta_yc=np.mean(new_yc-old_yc)
                # print(new_xc.shape, new_yc.shape, old_xc.shape, old_yc.shape, delta_xc, delta_yc)
                positions_unmatched=np.setdiff1d(np.arange(positions.shape[0]), positions_matched, True)
                self.data[-1,1:,:4][positions_unmatched][:,[0,2]] += delta_xc
                self.data[-1,1:,:4][positions_unmatched][:,[1,3]] += delta_yc
            detections_unmatched=np.setdiff1d(np.arange(detections.shape[0]), detections_matched, True)   
            
            for i,ind in enumerate(old_positions):
                self.old_data.append(self.data[:,ind-i,:].copy())
                self.data=np.delete(self.data,ind-i,axis=1)
                
                
            for i in detections_unmatched:
                new_cell=np.full((self.data.shape[0],1,7), np.nan, np.double)
                new_cell[-1,0,:5]=detections[i].copy()
                new_cell[-1,0,5]=1
                self.id_count=self.id_count+1
                new_cell[-1,0,6]=self.id_count
                self.data=np.concatenate((self.data, new_cell), axis=1)
                
    def get_positions(self):
        return self.data[-1,1:] if self.data.shape[1]>1 else []
    
    def get_history(self):
        current_size=self.data.shape[0]
        padded=np.empty((self.data.shape[0],len(self.old_data),7))
        for i in range(len(self.old_data)):
            padded[:,i,:]=np.pad(self.old_data[i],((0,current_size-self.old_data[i].shape[0]),(0,0)),mode='constant',constant_values=np.nan)
        return np.concatenate([padded,self.data], axis=1)[1:,1:,:]
    
    def get_3d_bboxes(self):
        history=self.get_history()
        bboxes=[]
        for i in range(history.shape[1]):
            cell=history[:,i,:].copy()
            xmin=np.nanmin(cell[:,0])
            ymin=np.nanmin(cell[:,1])
            xmax=np.nanmin(cell[:,2])
            ymax=np.nanmin(cell[:,3])
            zmin=np.where(cell[:,4]>0)[0].min()
            zmax=np.where(cell[:,4]>0)[0].max()
            if zmax==zmin:
                zmax=zmin+1
            bboxes.append([xmin,ymin,zmin,xmax,ymax,zmax])
        return np.array(bboxes)
    
class TSeries:
    
    def __init__(self,path,buffer=None, type='step', projection=np.mean):
        assert (buffer is None or buffer>1), 'buffer should be None or >1'
        assert (type=='step' or type=='ring'), 'buffer type should be "step" or "ring"'
        if os.path.isfile(path):
            self.fd=Image.open(path)
            self.idx=0
            self.buf_len=0
            self.type=type
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
                        return mean
                else:
                    if self.type=='step':
                        return retval, None
                    else:
                        return None
            else:
                return retval
        else:
            raise IndexError('T-series is over, catch this error please!')
            

#this is the multiprocessing.Pool target function, it's defined outside the Extractor wrapper class for multiprocessing scope issues  
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

    def __init__(self, pmin=80, pmax=95, bg_corr=True, workers=12, frame_buffer=10000):
        from multiprocessing import Pool, set_start_method
        set_start_method('spawn', force=True)
        self.frame_buffer=frame_buffer
        self.data=np.full((frame_buffer,1),np.nan,np.double)
        self.mima=(pmin,pmax)
        self.pool=Pool(workers)
        self.bg_corr=bg_corr
        self.current_frame=0
        
    def extract(self, frame, boxes):
        self.current_frame+=1
        if self.current_frame >= self.data.shape[0]:
            new_chunk=np.full((self.frame_buffer,self.data.shape[1]), np.nan, np.double)
            self.data=np.concatenate((self.data,new_chunk),axis=0)
        if len(boxes):
            if len(boxes)-self.data.shape[1]+1>0:
                new_cells=np.full((self.data.shape[0],len(boxes)-self.data.shape[1]+1), np.nan, np.double)
                self.data=np.concatenate((self.data, new_cells), axis=1)
            patches=[]
            bg_frame=frame.astype(np.double, copy=True)
            for b in boxes:
                bg_frame[int(b[1]):int(b[3]+1),int(b[0]):int(b[2]+1)]=np.nan
                patches.append(frame[int(b[1]):int(b[3]+1),int(b[0]):int(b[2]+1)].flatten())
            traces=self.pool.starmap(_p_extract,[(p,self.mima) for p in patches])
            self.data[self.current_frame,1:]=traces 
            if self.bg_corr:
                #self.data[-1,1:]-=np.nanmean(bg_frame)
                self.data[self.current_frame,0]-=np.nanmean(bg_frame)       

    def get_traces(self, tail=0):
        if tail>0 and tail>self.data.shape[0]:
            return self.data[int(-tail):self.current_frame,1:]
        else:
            return self.data[1:self.current_frame,1:]

#this is the multiprocessing.Pool target function, it's defined outside the PeakWatcher wrapper class for multiprocessing scope issues
# def _p_peakfinder(t, attention_window,sav_kwargs, but_kwargs, fp_kwargs):
#     try:
#         x=np.arange(len(t))
#         fil=np.where(np.isfinite(t))
#         sav=savgol_filter(t[fil], **sav_kwargs)
#         but=butter_lowpass_filter(sav, **but_kwargs)
#         peaks,_ =find_peaks(but,**fp_kwargs)
# #         return True if len(x[fil][peaks]) and x[fil][peaks][-1]>len(x)-attention_window else False
#         # return x[fil][peaks]
#         return len(peaks)
#     except:
#         return []
def butter_lowpass_filter(data, cutoff, fs, order):
    normal_cutoff = cutoff / (0.5*fs)
    # Get the filter coefficients 
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

def _p_peakfinder(t, attention_window,sav_kwargs, but_kwargs, fp_kwargs):
    x=np.arange(len(t))
    fil=np.where(np.isfinite(t))
    sav=savgol_filter(t[fil], **sav_kwargs)
    but=butter_lowpass_filter(sav, **but_kwargs)
    peaks,_ =find_peaks(but,**fp_kwargs)
#         return True if len(x[fil][peaks]) and x[fil][peaks][-1]>len(x)-attention_window else False
    # return x[fil][peaks]
    return len(peaks)

class PeakWatcher:
    
    def __init__(
        self,
        sav_kwargs=None,
        but_kwargs=None,
        fp_kwargs=None,
        attention_window=10,
        workers=12
    ):
        self.sav_kwargs=sav_kwargs if sav_kwargs is not None else {'window_length':15,'polyorder':2,'deriv':1}
        self.but_kwargs=sav_kwargs if but_kwargs is not None else {'cutoff':1, 'fs':30, 'order':3}
        self.fp_kwargs=fp_kwargs if fp_kwargs is not None else {'height':(3,None), 'prominence':7}
        self.attention_window=attention_window
        from multiprocessing import Pool, set_start_method
        set_start_method('spawn', force=True)
        self.pool=Pool(workers)
        
    def watch(self, traces):
        flags=self.pool.starmap(_p_peakfinder, [(t, self.attention_window, self.sav_kwargs, self.but_kwargs, self.fp_kwargs) for t in traces.T] )
        return flags


class Dataset:
    def __init__(self,csv_path):
        self.csv_path=csv_path
        from itertools import groupby
        from operator import itemgetter
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
        return cv2.imread(os.path.join(os.path.split(self.csv_path)[0], self.file_names[idx]))
        
class MotionCorrection():

    def __init__(self,pix_precision):
        self.pix_precision = pix_precision
        

    def motion_corr(self,im_stream,ref_image=None,ref_frame=None):
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
            # M = np.float32([[1,0,shift_vec[i,1]],[0,1,shift_vec[i,0]]])
            # im_stream[i,:,:] = cv.warpAffine(im_stream[i,:,:],M,(cols,rows))

        return shift_vec
        
        
        
        
        
