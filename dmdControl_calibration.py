import socket
import numpy as np
import time
import json
import sys
#import matplotlib.pyplot as plt

def convert(img8):
        img8 = ((img8 > 127) * np.ones(img8.shape)).astype(np.uint8)
        s=img8.shape
        a=img8.reshape(s[0]*s[1]//8, 8)
        a2 = np.ones(a.shape)
        for i in range(7):
            a2[:, i] = 1 << (7 - i)
        a = a*a2
        return a.sum(axis=1).astype(np.uint8)

def send(sock,width, height,img):
        sock.sendall(np.uint32(2))
        sock.sendall(np.uint32(width))
        sock.sendall(np.uint32(height))
        sock.sendall(img)

def create_image(matrix1,matrix2, radius,background):


    width, height = 800, 500
    spot_color = 255  # White


    # Initialize the image with all zeros
    image = np.zeros((height, width), dtype=np.uint8)

    if background > 0:

            # Calculate the number of elements to be set to 1
            num_elements = image.size
            num_ones = int(background/100 * num_elements)

            # Randomly select indices to assign ones
            indices = np.random.choice(num_elements, num_ones, replace=False)

            # Assign ones to the selected indices
            np.put(image, indices, 255)

    # Function to check if a point is within the circle
    def in_circle(center_x, center_y, x, y, radius):
        # return (x - center_x) ** 2 + (y - center_y) ** 2 <= radius ** 2
        rx2 = (radius*1.6) ** 2
        ry2 = radius ** 2
        return ((x - center_x) ** 2)/rx2 + ((y - center_y) ** 2)/ry2 <= 1 # accounting for unequal pixel lengths in x and y

        
    # Draw white spots for each coordinate in the matrix
    # matrix1: coordinates of the cells that needs to be stimulated (group 1)
    # matrix2: coordinates of the cells that needs to be excluded from the background stimulation (group 2)

    for center_x, center_y in matrix1:
        for x in range(max(int(center_x - 1.6*radius), 0), min(int(center_x + 1.6*radius) + 1, width)):
            for y in range(max(int(center_y - radius), 0), min(int(center_y + radius) + 1, height)):
                if in_circle(center_x, center_y, x, y, radius):
                    image[y][x] = spot_color

    
    for center_x, center_y in matrix2:
        for x in range(max(int(center_x - 1.6*radius), 0), min(int(center_x + 1.6*radius) + 1, width)):
            for y in range(max(int(center_y - radius), 0), min(int(center_y + radius) + 1, height)):
                if in_circle(center_x, center_y, x, y, radius):
                    image[y][x] = 0



    #for center_x, center_y in matrix1:
    #    for x in range(max(int(center_x - radius), 0), min(int(center_x + radius) + 1, width)):
    #        for y in range(max(int(center_y - radius), 0), min(int(center_y + radius) + 1, height)):
    #            if in_circle(center_x, center_y, x, y, radius):
    #                image[y][x] = spot_color

    
    #for center_x, center_y in matrix2:
    #    for x in range(max(int(center_x - radius), 0), min(int(center_x + radius) + 1, width)):
    #        for y in range(max(int(center_y - radius), 0), min(int(center_y + radius) + 1, height)):
    #            if in_circle(center_x, center_y, x, y, radius):
    #                image[y][x] = 0
              

    # Display the image for debugging
    #plt.imshow(image, cmap='gray', vmin=0, vmax=255)
    #plt.axis('off')  # Hide axis labels and ticks
    #plt.show()

    return image


json_matrix1 = sys.argv[1]; # group 1 cell coordinates
json_matrix2 = sys.argv[2]; # group 2 cell coordinates
nTargets = int(sys.argv[3]); # number of stim. targets
radius = int(sys.argv[4]); # radius of stimulation spots in pixels
t = int(sys.argv[5]); # pulse duration (in ms)
T = int(sys.argv[6]); # interval between stimulation pulses (in ms)
niter = int(sys.argv[7]); # number of iterations
testMode = int(sys.argv[8]); # testMode = 1, in test mode, otherwise 0
background = int(sys.argv[9]); # background intensity of the DMD pattern. Range: 0-255 (default: 0)
stimGroup = int(sys.argv[10]); # id of the group that needs to be stimulated if the test mode is selected

# deserialize the JSON strings back to python

cells_1 = json.loads(json_matrix1) # cell group 1
cells_2 = json.loads(json_matrix2) # cell group 2

if not isinstance(cells_1[0],list): # handle the case where there is only one cell in this group
        cells_1 = [cells_1]

host = "localhost"
port = 2222
height = 500
width = 800
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
remote_ip = socket.gethostbyname( host )


try:
    sock.connect((remote_ip, port))
except socket.timeout:
    print("Polygon server disconnected.")

#frame1 = convert(255*np.random.random_sample((height, width)))
#frame2 = convert(255*np.zeros((500, 800), dtype=np.uint8))

blankFrame = convert(np.zeros((500, 800), dtype=np.uint8)) # all zeros displayed in between stimulations

if len(cells_2) > 0:
        # Create images for both groups
        frame1 = convert(create_image(cells_1,cells_2,radius,background))

        if not isinstance(cells_2[0],list): # handle the case where there is only one cell in this group
                cells_2 = [cells_2]
        frame2 = convert(create_image(cells_2,cells_1,radius,background))
        
        delay = max(0,t-10) # minimum time required to switch between patterns is 10ms for this DMD

        if testMode > 0:
                for itr in range(niter):
                        if (stimGroup == 1):
                                send(sock,width,height,frame1) # Group 1 only
                        else:
                                send(sock,width,height,frame2) # Group 2 only
                                
                        if delay > 0:
                                time.sleep(delay/1000)
                        send(sock,width,height,blankFrame) # no stimulation in between two iterations        
                        time.sleep((T-t)/1000)
                        
        else:           
                for itr in range(niter):
                        send(sock,width,height,frame1)
                        if delay > 0:
                                time.sleep(delay/1000)
                        send(sock,width,height,frame2)
                        if delay > 0:
                                time.sleep(delay/1000)
                        send(sock,width,height,blankFrame) # no stimulation in between two iterations        
                        time.sleep((T-2*t)/1000)

elif nTargets == 1:
        # First group only, simultaneous stimulation
        frame1 = convert(create_image(cells_1,cells_2,radius,background))
        delay = max(0,t-10) # minimum time required to switch between patterns is 10ms for this DMD
        
        for itr in range(niter):
                send(sock,width,height,frame1)
                if delay > 0:
                        time.sleep(delay/1000)
                send(sock,width,height,blankFrame) # no stimulation in between two iterations        
                time.sleep((T-t)/1000)
                
else:
        delay = max(0,t-15) # minimum time required to switch between patterns is 10ms for this DMD + 5ms for image generation
        # First group only, sequential stimulation
        
        for itr in range(niter):
                for xc,yc in cells_1:
                        frame1 = convert(create_image([[xc,yc]],cells_2,radius,background))
                        send(sock,width,height,frame1)
                        if delay > 0:
                                time.sleep(delay/1000)
                        
                send(sock,width,height,blankFrame) # no stimulation in between two iterations
                time.sleep(max(0,T-nTargets*t)/1000)
        

#for itr in range(4):       
#       t0 = time.time()
#        send(sock,width,height,frame2)
#       print(time.time()-t0, flush=True)
#        t0 = time.time()
#        send(sock,width,height,frame1)
#        print(time.time()-t0, flush=True)
#        t0 = time.time()
#        send(sock,width,height,frame2)
#        print(time.time()-t0, flush=True)
#        time.sleep(1)

sock.close()


