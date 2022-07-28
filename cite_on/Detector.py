import os
import numpy as np
import cv2

#this is the wrapper class for the retinanet object detector
class Detector:
    """
    CITE-On detector class is a wrapper class for the retinanet object detector.

    ...

    Attributes
    ----------
    model_path : str
        path to the trained SavedModel folder
    gpu : int, optional
        gpu number to use
    init_shape : (int, int)
        shape of the image to prime the network with to allocate memory faster, requires init_upscaling
    init_upscaling : float, optional
        upscaling factor of the image to prime the network with to allocate memory faster, requires init_shape
        
    Methods
    -------
    detect(img, threshold=0.05, upscaling=1):
        Runs the network to return bounding boxes.
    def tile_detect(img, n_tess=2, overlap=0.1,threshold=0.05, upscaling=1, batch_size=16):
        Runs the network on image tiles to return bounding boxes for the whole image.
    """
    
    def __init__(self,model_path,gpu=None, init_shape=None, init_upscaling=None):
        #on init, model gets lazy loaded so in order to avoid slow-downs specifying the operative size is needed 
        assert ((init_shape is None and init_upscaling is None) or (init_shape is not None and init_upscaling is not None)), 'If used, init parameters should both be specified'
        if init_shape is not None:
            assert (len(init_shape)==2), 'init_shape should have len = 2'
        #these are the default sizes used during training, if you trained a model with different sizes please change these parameters 
        import tensorflow as tf
        self.MIN_SIDE=800
        self.MAX_SIDE=1333
        #computing the init array of zeors
        if init_shape is not None and init_upscaling is not None:
            init_array,_=self._preprocess(np.zeros(init_shape, np.float32), init_upscaling)
        else:
            init_array=np.zeros((1,800,800,3), np.float32)
        #setting visible gpus for the model to run on
        if gpu is not None:
            os.environ['CUDA_VISIBLE_DEVICES'] = str(gpu)
        else:
            os.environ['CUDA_VISIBLE_DEVICES'] = ''
        #creating tf graph and session
        self.retina_graph=tf.Graph()
        with self.retina_graph.as_default():
            self.retina_sess=tf.Session()
            with self.retina_sess.as_default():
                #loading model definition and weights from tf SavedModel format (thus avoiding the use of keras)
                tf.saved_model.loader.load(self.retina_sess, [tf.saved_model.tag_constants.SERVING],model_path)
                #make a dry run to initialize model input shape
                _ = self.retina_sess.run(['filtered_detections/map/TensorArrayStack_1/TensorArrayGatherV3:0','filtered_detections/map/TensorArrayStack/TensorArrayGatherV3:0'], feed_dict={'input_1:0': init_array})
        
        print('AGOnIA up \'n running!')

    def _preprocess(self, img, upscaling):
        if len(img.shape)==2:
            #cast to float32 is required by tensorflow,
            image=img.astype(np.float32)
            #image is normalized, converted to rgb format and then the per-channel mean of imagenet dataset is subtracted(this is the dataset on which the network is scratch trained)
            image=cv2.normalize(image, None, 0, 255, cv2.NORM_MINMAX,-1)
            image=np.repeat(image[:,:,np.newaxis],3,axis=2)
            image[..., 0] -= 103.939
            image[..., 1] -= 116.779
            image[..., 2] -= 123.68
            #here the appropriate scale factor is computed given the default training sizes and the required upscaling
            (rows, cols, _) = image.shape
            smallest_side = min(rows, cols)
            scale = self.MIN_SIDE*upscaling / smallest_side
            largest_side = max(rows, cols)
            if largest_side * scale > self.MAX_SIDE*upscaling:
                scale = self.MAX_SIDE*upscaling / largest_side
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

    def detect(self, img, threshold=0.05, upscaling=1):
        """
        Runs the network to return bounding boxes.

        If not specified, optional values will be defaulted

        Parameters
        ----------
        img : np.ndarray(ndims=2, dtype=np.uint16)
            image to run the detector on
        threshold : int, optional
            score threshold applied to filter boxes
        upscaling : int, optional
            upscaling factor to apply to the image before detection
        Returns
        -------
        np.ndarray(ndims=2, dtype=np.uint16)
            an array of shape (num_boxes,5), where each box is represented as [xmin,ymin,xmax,ymax,score]
        """
        image, scale = self._preprocess(img, upscaling)
        with self.retina_sess.as_default():
            scores, boxes = self.retina_sess.run(['filtered_detections/map/TensorArrayStack_1/TensorArrayGatherV3:0','filtered_detections/map/TensorArrayStack/TensorArrayGatherV3:0'], feed_dict={'input_1:0': image})
        detection= self._decode(threshold, scale, boxes, scores)
        return detection
    
    def tile_detect(self, img, n_tess=2, overlap=0.1, batch_size=16, threshold=0.05, upscaling=1):
        """
        Runs the network on image tiles to return bounding boxes for the whole image.
        
        If not specified, optional values will be defaulted

        Parameters
        ----------
        img : np.ndarray(ndims=2, dtype=np.uint16)
            image to run the detector on
        n_tess : int, optional
            number of tiles per side to tile the fov with
        overlap : int, optional
            overelap between tiles, from 0 to 1
        batch_size : int, optional
            max batch of images to be sent to the gpu
        threshold : int, optional
            score threshold applied to filter boxes
        upscaling : int, optional
            upscaling factor to apply to the image before detection
        Returns
        -------
        np.ndarray(ndims=2, dtype=np.float)
            an array of shape (num_boxes,5), where each box is represented as [xmin,ymin,xmax,ymax,score]
        """
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
                image, scale =self._preprocess(img[y_off:y_off+y_size, x_off:x_off+x_size,...], upscaling)
                batch = np.concatenate((batch, image), axis=0) if len(batch) else image
                sizes.append((scale, y_off, x_off))
        scores = []
        boxes = []
        for i in range(0,len(batch),batch_size):
            score, box = self.retina_sess.run(['filtered_detections/map/TensorArrayStack_1/TensorArrayGatherV3:0','filtered_detections/map/TensorArrayStack/TensorArrayGatherV3:0'], feed_dict={'input_1:0': batch[i:min(i+batch_size, len(batch))]})
            scores.append(score)
            boxes.append(box)
        scores=np.concatenate(scores,axis=0)
        boxes=np.concatenate(boxes,axis=0)
        detections=np.zeros((1,5))
        for i in range(len(batch)):
            detection=self._decode(threshold, sizes[i][0], boxes[i][np.newaxis,...], scores[i][np.newaxis,...], sizes[i][1], sizes[i][2])
            detections=np.concatenate((detections,detection), axis=0)
        
        detections = detections[detections[:,-1]>threshold,:]
        detections = self._nms(detections)
        return detections

    
