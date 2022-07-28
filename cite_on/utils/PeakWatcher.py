import numpy as np
from scipy.signal import butter,filtfilt,savgol_filter, find_peaks
from multiprocessing import Pool, set_start_method

def _butter_lowpass_filter(data, cutoff, fs, order):
    normal_cutoff = cutoff / (0.5*fs)
    # Get the filter coefficients 
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

#this is the multiprocessing.Pool target function, it's defined outside the PeakWatcher wrapper class for multiprocessing scope issues
def _p_peakfinder(t, attention_window,sav_kwargs, but_kwargs, fp_kwargs):
    x=np.arange(len(t))
    fil=np.where(np.isfinite(t))
    sav=savgol_filter(t[fil], **sav_kwargs)
    but=_butter_lowpass_filter(sav, **but_kwargs)
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
        workers=46
    ):
        self.sav_kwargs=sav_kwargs if sav_kwargs is not None else {'window_length':15,'polyorder':2,'deriv':1}
        self.but_kwargs=sav_kwargs if but_kwargs is not None else {'cutoff':1, 'fs':30, 'order':3}
        self.fp_kwargs=fp_kwargs if fp_kwargs is not None else {'height':(3,None), 'prominence':7}
        self.attention_window=attention_window
        set_start_method('spawn', force=True)
        self.pool=Pool(workers)
        
    def watch(self, traces):
        flags=self.pool.starmap(_p_peakfinder, [(t, self.attention_window, self.sav_kwargs, self.but_kwargs, self.fp_kwargs) for t in traces.T] )
        return flags

