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

def create_image():
    width, height = 800, 500
    background_color = 0  # Black --> set to 0 
    spot_color = 255  # White

    # Initialize the image with the background color
    image = np.ones((height, width), dtype=np.uint8)*spot_color;


    # Draw rectangles in grayscale (0, 40, 80, 120, 160, 200, 240, 255)

    #grayscale = [0, 40, 80, 120, 160, 200, 240, 255]

    #for itr in range(8):
        #for x in range(100*itr,100*(itr+1)):
                #for y in range(height):
                        #image[y][x] = grayscale[itr];

    # Display the image for debugging
    #plt.imshow(image, cmap='gray', vmin=0, vmax=255)
    #plt.axis('off')  # Hide axis labels and ticks
    #plt.show()

    return image


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


blankFrame = convert(np.zeros((500, 800), dtype=np.uint8)) # all zeros displayed in between stimulations

# First group only, simultaneous stimulation
frame1 = convert(create_image())
delay = 20 # 20ms Delay
T = 2000 # 2 sec
#t = 1000 # ms
niter = 10
        
for itr in range(niter):
        send(sock,width,height,frame1)
        if delay > 0:
                time.sleep(delay/1000)
        send(sock,width,height,blankFrame) # no stimulation in between two iterations        
        time.sleep((T-delay)/1000)
                

sock.close()


