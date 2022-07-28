from . import compute_overlap
import numpy as np

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
