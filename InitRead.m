function frames = InitRead(initBatchSize, IPaddress)
% can remove mode now since if initBatchSize not None, then init else
% acquisition

%in RT there is information about upper limit. 

% first test 750 + 200000 frames in raw file
    if ~exist('IPaddress','var')
         % third parameter does not exist, so default it to something
         IPaddress= "10.99.5.51"
    end
    
    if ~exist('initBatchSize','var')
         % third parameter does not exist, so default it to something
         initBatchSize = 750;
    end

    try
        pl = actxGetRunningServer('PrairieLink64.Application')
        % no crash so probably succeeded to connect to a running server
    catch
        % Never mind - continue normally to start the COM server and connect to it
        pl = actxserver('PrairieLink64.Application')
    end

    %connect to Prairie View
    %pl.Connect(IPaddress) %, passwd)
    pl.Connect()
    %number of channels
    n_channels = 1

    activeMode = pl.GetState('activeMode')
    if strcmp(activeMode,'Galvo'), flip = false;
    else flip=true; end

    %Obtain info to make sense of incoming stream
    samplesPerPixel = pl.SamplesPerPixel()
    pixelsPerLine = pl.PixelsPerLine()
    linesPerFrame = pl.LinesPerFrame()
    totalSamplesPerFrame = samplesPerPixel*pixelsPerLine*linesPerFrame*n_channels;

    %Check that connection has been succesful
    if totalSamplesPerFrame==0 , disp("Error linking to Prairie View !!!"); delete(pl);return;
    end

    %limit buffer size to approximately one frame so that matlab is not
    %overwhelmed by incoming stream of data and drops frames
    pl.SendScriptCommands('-lbs true 35');
    %Add a buffer size of 5 frames so that in case matlab cannot keep up then
    %frames are kept in the buffer
    pl.SendScriptCommands("-srd true 5");

    % initialize state variables
    running = 0;
    started = 0;

    %Initialize buffer
    buffer = [];
    % number of samples read per iter
    numSamplesRead = 0;
    SamplesRead = [];

    %frames counters
    framesCounter  = 0;
    frameNum = 0;

    %Variables used for displaying message when data is dropped
    droppedData = [];
    msg = [];

    %number of frames grabbed at each iter, used to process frames
    numWholeFramesGrabbed = 0;

    %counter of loops
    loopCounter = 0;

    frame_size = pixelsPerLine * linesPerFrame * samplesPerPixel;
    
    ch1_frame = zeros(1,frame_size, 'int16');
    img_ch1 = zeros(linesPerFrame, pixelsPerLine, initBatchSize, 'int16');

    if n_channels == 2
        ch2_frame = zeros(1,frame_size, 'int16');
        img_ch2 = zeros(linesPerFrame, pixelsPerLine, initBatchSize, 'int16');
    end  

    % flush buffer
    flushing = 1;
    while flushing
        [~, numSamplesRead] = pl.ReadRawDataStream(1);
        if numSamplesRead == 0
            flushing = 0;
            running = 1;
        end
    end

%   Modify here if intend to include option for live scanning
    pl.SendScriptCommands("-ts");


    %get data
    while running

        %read raw data stream
        [data, numSamplesRead] = pl.ReadRawDataStream(1);

        % append new data to any remaining old data
        buffer = [buffer data(1:numSamplesRead)];
        %append info about samples read at each iteration
        SamplesRead  = [SamplesRead numSamplesRead];

        % extract full frames
        numWholeFramesGrabbed = floor(length(buffer)/totalSamplesPerFrame);
        %obtain data to Process
        toProcess = buffer(1:(numWholeFramesGrabbed)*totalSamplesPerFrame);

        %divide into processing branch for the two channels or keep only one if
        %only one channel
        if n_channels == 2
            ch1_toProcess = toProcess(1:2:end);
            ch2_toProcess = toProcess(2:2:end);
        else
            ch1_toProcess = toProcess;
        end

        % clear data from buffer
        buffer = buffer((numWholeFramesGrabbed*totalSamplesPerFrame)+1:end);

        if numWholeFramesGrabbed > 0
            for i = 1:numWholeFramesGrabbed
                if started == 0
                    started = 1;
                end
                if (i + framesCounter) > initBatchSize 
                    break;
                end
                %creates variable containing all samples for frame in ch1
                ch1_frame(1:frame_size) = ch1_toProcess((((i-1)*(frame_size)+1):(i*(frame_size))));
                %processes frame to average img
                img_ch1(:,:,i) = process_frame_mex(ch1_frame,  pixelsPerLine, linesPerFrame, samplesPerPixel, flip);
                %if using two channels repeats same steps as above
                if n_channels == 2
                    ch2_frame(1:frame_size) = ch2_toProcess((((i-1)*(frame_size)+1):(i*(frame_size))));
                    img_ch2(:,:,i) = process_frame_mex(ch2_frame,  pixelsPerLine, linesPerFrame, samplesPerPixel, flip);
                end
                % increment frame counter
                frameNum = frameNum + n_channels;
            end
        end

        % increment counters
        framesCounter = framesCounter + (numWholeFramesGrabbed*n_channels);

        % test for dropped data
        if pl.DroppedData()
            warning(['\n!!! DROPPED DATA AT FRAME ' num2str(framesCounter) ' !!!\n']) 
        end

        loopCounter = loopCounter + 1;
        
        if framesCounter >= initBatchSize
            running = 0;
        end
        pause(0.01)
    end
    
    %Return array of frames
    frames = img_ch1
    if n_channels == 2
       frames = {img_ch1, img_ch2}
    end
    % clean up
    pl.SendScriptCommands("-x"); % Exit TCP/IP communication
    pl.Disconnect();
    delete(pl);
end