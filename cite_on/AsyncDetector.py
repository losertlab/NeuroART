import numpy as np
from .Detector import Detector
import cv2
from multiprocessing import Queue, Process, Lock

def target_function(model_path,gpu,in_q, out_q, l, multi, score, init_shape):
    with l:
        det=Detector(model_path,gpu,init_shape,multi)
    while True:
        if not in_q.empty():
            pic=in_q.get()
            boxes=det.detect(pic,score,multi)
            out_q.put(boxes)

class AsyncDetector:
    """
    CITE-On  asyncronous detector is a wrapper for the standard detector to make it asyncronous.

    ...

    Attributes
    ----------
    model_path : str
        path to the trained SavedModel folder
    upscaling : int, optional
        upscaling factor to apply to the image before detection
    threshold : int, optional
        score threshold applied to filter boxes
    gpu : int, optional
        gpu number to use
    init_shape : (int, int)
        shape of the image to prime the network with to allocate memory faster, requires init_upscaling
        
    Methods
    -------
    terminate():
        Terminates the worker process
    
    Properties
    -------
    in_queue : Queue
        Input queue for images to be detected
    out_queue : Queue
        Output queue for detections to be tracked and extracted
    startup_lock : Lock
        Lock held by the worker process while the network is initializing
    """
    def __init__(
        self,
        model_path,
        upscaling,
        threshold,
        gpu='0', 
        init_shape=None
    ):
        self.out_queue=Queue(maxsize=1)
        self.in_queue=Queue(maxsize=1)
        self.startup_lock=Lock()
        self.worker=Process(target=target_function,args=(model_path,gpu,self.in_queue,self.out_queue, self.startup_lock,upscaling, threshold, init_shape), daemon=True)
        with self.startup_lock:
            return
            
    def terminate(self):
        self.worker.terminate()