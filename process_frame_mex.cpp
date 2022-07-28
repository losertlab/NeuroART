#include "mex.h"


void processFrame(signed short *samples, int pixelsPerLine, int linesPerFrame,
                  int samplesPerPixel, bool flip, signed short *frame)
{
    /* This function processes the raw samples received via Praire Link 
       correcsponsing to a frame into the actual frame of size 
       linesPerFrame x pixelsPerLine. Data must be transposed, so actually first read 
       columns and then rows
       Input:
       -*samples(signed short *p): pointer to array of samples. Array must be 1-dimensional and must have
       length (pixelsPerLine*linesPerFrame*samplesPerPixel)
       -pixelsPerLine(int) number of columns in a frame
       -linesPerFrame(int): number of rows in a frame
       -samplesPerPixel(int): number of rows in a frame
       -flip(bool): variable to determine if even lines must be flipped.
       Output:
       -frame(signed short **p): output frame being constructed   
    */
    double sampleVal = 0;
    double sumSamples = 0;

    signed short offset= 8000;

    /*frame = new signed short*[linesPerFrame];
    for(int i=0; i<linesPerFrame; i++)
    {
        frame[i] = new signed short[pixelsPerLine];
    }*/
 
    for(int i=0; i<linesPerFrame; i++) 
    {    
        for (int j=0; j<pixelsPerLine; j++)  
        {    
            sampleVal = 0;
            sumSamples = 0;
            for (int k=0; k<samplesPerPixel; k++) 
            {
                sampleVal = samples[k + (j*samplesPerPixel) + (i*linesPerFrame*samplesPerPixel)];
                sampleVal -= offset;
                sumSamples += sampleVal;
            }
            if(flip)
            {
                if ((i % 2) == 1)
                {
                    //frame[i][(pixelsPerLine-1) - j] = sumSamples/samplesPerPixel;
                    frame[(i*linesPerFrame) + (pixelsPerLine - 1 - j)] = sumSamples/samplesPerPixel;
                }
                else
                {
                    frame[(i*linesPerFrame) + j] = sumSamples/samplesPerPixel;
                }
            }
            else
            {
                frame[(i*linesPerFrame) + j] = sumSamples/samplesPerPixel;
            }
        }
    }

    return;
}


void mexFunction(int nlhs, mxArray *plhs[],  // plhs: array of mxArray pointers to output 
                 int nrhs, const mxArray *prhs[])    // prhs: array of mxArray pointers to input
{
    int pixelsPerLine;
    int linesPerFrame;
    int samplesPerPixel;
    int flip;
    signed short *samples;  
    signed short *frame;
    
    samples = (signed short *)mxGetData(prhs[0]);
    pixelsPerLine   = mxGetScalar(prhs[1]);
    linesPerFrame   = mxGetScalar(prhs[2]);
    samplesPerPixel = mxGetScalar(prhs[3]);
    flip    = mxGetScalar(prhs[4]);
    // outputframe
    plhs[0] = mxCreateNumericMatrix(linesPerFrame, pixelsPerLine, mxINT16_CLASS, mxREAL);
    
    // get a pointer to the real data in the output matrix
    frame = (signed short *)mxGetData(plhs[0]);
    
    // call the computational routine
    processFrame(samples, pixelsPerLine, linesPerFrame,  samplesPerPixel, flip, frame);
}