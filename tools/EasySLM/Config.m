classdef Config
    
    properties (SetAccess = protected)
        screen; % Screen ID corresponding to slm
        res; % Resolution of the slm in pixel
        pitch; % Separation between two pixels from centers
        camera; % Camera ID for imaging
        exposure; % Camera exposure time
        framerate; % Camera framerate in Hz
        wl; % Wavelength in mm
    end
    
    methods (Static)
        
        function config = LCR2500()
            % Creates configuration for LC-R 2500 Holoeye SLM.
            config = Config('res',[768,1024], 'pitch', 19, 'exposure', -10.5, 'wl', 659);
        end 

        function config = BNS()
            % Creates configuration for BNS SLM.
            config = Config('res',[2304,2304], 'pitch', 10, 'wl', 1024); % dimensions multiplied by 1.5 to allow cropping while centering
        end
            
    end 
    
    methods
        
        function self = Config(varargin)
          % Class for storing parameters of SLM setup.
          %
          % Optional named parameters:
          %   - 'screen' int -- Screen device ID.
          %     Default: `2` 
          %   - 'camera' int -- Camera device ID.
          %     Default: `1`
          %   - 'res' [r1,r2]  -- Screen resolution in pixels.
          %     Default: `[100, 100]`
          %   - 'pitch' float -- Spacing of pixels.
          %     Default: `[10,10]`
          %   - 'exposure' float -- Camera exposure. Is a negative number,
          %     x, for which the exposure time in secs is calculated as 2^x.
          %     Default: `-2`
          %   - 'wl' float -- Wavelength of laser in nm.
          %     Default: `100`

            p = inputParser;
            p.addParameter('screen', 2);  
            p.addParameter('camera', 1);
            p.addParameter('res', [100,100]); % in pixels
            p.addParameter('pitch', 10); % in um
            p.addParameter('exposure', -2);  
            p.addParameter('wl', 100); % in nm
            p.parse(varargin{:});
            
            self.screen= p.Results.screen;
            self.camera= p.Results.camera;
            self.res= p.Results.res;
            self.pitch= p.Results.pitch*1e-3;
            self.exposure= p.Results.exposure;
            self.wl= p.Results.wl*1e-6; %in mm
            
            assert(isequal(size(self.res),[1,2]))
            if ~all(mod(self.res, 2) == 0)
                error('Only Even Resolutions Are Supported');
            end
        end 
        
        function cam = open_camera(self, varargin)
           %Creates a camera device for imaging and enables imaging.
           
           p = inputParser;
           p.addParameter('gain', 0);
           p.addParameter('gamma', 100); 
           p.addParameter('contrast', 100);
           p.addParameter('brightness', 255);
           p.parse(varargin{:});
            
           cam=  videoinput('winvideo', self.camera);
           src= getselectedsource(cam);
           src.ExposureMode = 'manual';
           src.ContrastMode = 'manual';
           src.GainMode = 'manual';
           src.Exposure= self.exposure;
           src.Gain= p.Results.gain;
           src.Contrast= p.Results.contrast;
           src.Gamma= p.Results.gamma;
           src.Brightness= p.Results.brightness;
       end
        
    end 
    
end 