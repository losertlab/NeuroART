classdef SLM < otslm.utils.ScreenDevice
    % Class for displaying images to SLM.
    % Dependent on ScreenDevice class from OTSLM.

    properties (SetAccess = protected)
        config Config; % Config instance that stores setup parameters.
        roi; % Region of interest. Can be a region of the total screen.
        grayscale; % Grayscale value to scale phase pattern to.
        X; Y; % Meshgrid of SLM screen.
    end
    
    methods (Static)
                
        function pattern = apply_lens(phase,lens)
            % Applies virtual lens to phase pattern(s).
            %
            % Parameters
            % - phase, a single 2D matrix or cell array of 2D matrices
            %   giving phase masks to apply virtual lens to.
            % - lens, 2D matrix giving len's phase maks 
            %
            
            if iscell(phase)
                pattern= cell(1,size(phase,2));
                for i=1:size(phase,2)
                   pattern{i}= mod(phase{i}+lens,1); 
                end
            else
                pattern= mod(phase + lens, 1);
            end
        end
        
        function phase = aberration_correction(phase,abrs)
            % Performs aberration correction with Zernike polynomials.
            % For integer n, only every other integer, m, from -n to n is
            % considered.
            %
            % Parameters
            % - phase, phase pattern to apply corrections to.
            % - abrs, a string or array of strings corresponding to
            %   the aberrations to subtract. 

            if isstring(abrs)
                phase= phase - aberration(size(phase),abrs);
                disp(min(min(phase)));
            else 
                for i=1:length(abrs)
                   phase= phase - aberration(size(phase),abrs(i)); 
                end
            end
            %{
            n=1; m=-1;
            for i=1:length(Z)
               phase= phase - Z(i)*otslm.simple.zernike(size(phase),n,m);
               m= m+2;
               if m==n
                  n= n+1;
                  m= -n;
               end
            end
            %}
        end

    end
    
    methods (Access = protected)
        
        function [X,Y] = meshgrid(self)
            % Creates a mesh for the slm screen.
            linx= linspace(0, self.config.res(2)-1, self.config.res(2))+0.5;
            liny= linspace(0, self.config.res(1)-1, self.config.res(1))+0.5;
            [X,Y]= meshgrid(linx,liny);
            
            x0= (self.config.res(2)+1)/2;
            y0= (self.config.res(1)+1)/2;
            X= X-x0;
            Y= Y-y0;
        end
                
        function img = resize(self, im)
           % Resizes a image to be square if screen resolution isn't.
           %
           % Parameters
           % - im, 2D matrix to be resized.
           %

            sz= size(im);
            m= max(self.config.res);
            if (self.config.res(1)~=self.config.res(2)) && ~(sz(1)==m && sz(2)==m) 
                r = max(self.config.res);
                img= zeros(r,r);
                if self.config.res(1)>self.config.res(2)
                    img(:,(self.config.res(1)-self.config.res(2))/2+1: ...
                    (self.config.res(1)-self.config.res(2))/2+self.config.res(2)) = im;
                elseif self.config.res(2)>self.config.res(1)
                   img((self.config.res(2)-self.config.res(1))/2+1: ...
                        (self.config.res(2)-self.config.res(1))/2+self.config.res(1),:) = im;
                end
            else
                img = im;
            end 
        end 
        
        function im = crop(self, im)
           % Crops an image to the size of the screen.
           %
           % Parameters
           % - im, 2D matrix to be cropped.
           %

            if size(im,1)>self.config.res(1)
                diff= (size(im,1)-self.config.res(1))/2;
                im = im(diff+1:diff+self.config.res(1),:);
            end
            if size(im,2)>self.config.res(2)
                diff= (size(im,2)-self.config.res(2))/2;
                im = im(:,diff+1:diff+self.config.res(2));
            end
        end 
        
        function pattern = mask(self, phase)
           % Turn a phase pattern into a proper grayscale RGB phase mask.
           %
           % Parameters
           % - phase, a single 2D matrix or cell array of 2D matrices
           %   to be turned into grayscaled images.
           %

            if iscell(phase)
                pattern= cell(1,size(phase,2));
                for i=1:size(phase,2)
                   phi= uint8( round(phase{i}.*self.grayscale) );
                   pattern{i}= repmat(phi,[1,1,3]);
                end
            else
                phase= uint8( round(phase.*self.grayscale) );
                pattern= repmat(phase,[1,1,3]);
            end 
        end
        
    end 

    methods

        function self = SLM(config, varargin)
          % Constructs a new SLM class instance.
          %
          % Parameters
          % - config, Config class instance that gives all 
          %   setup specific parameters.
          %
          % Optional named parameters:
          %     Default: `ones(res)` (uniform intensity)
          %   - 'offset' [r,c]  -- Offset within the window.  Negative
          %     values are offset from the top of the screen.
          %     Default: `[0, 0]`
          %   - 'roi' [x,y] -- Region of interest resolution.
          %     Default: `Config.res`
          %   - 'grayscale' int -- Grayscale value to scale phase pattern to.
          %     Default: `255`

            p = inputParser;
            p.addParameter('offset', [0, 0]);
            p.addParameter('roi', config.res);
            p.addParameter('grayscale', 256);
            p.parse(varargin{:});
            
            self = self@otslm.utils.ScreenDevice(config.screen, 'size', config.res, ...
              'pattern_type', 'phase', 'fullscreen', true, 'prescaledPatterns', false, ...
              'offset', p.Results.offset);
          
            self.config= config; 
            self.grayscale= p.Results.grayscale;
            [self.X,self.Y]= self.meshgrid();
        end
        
        %{
        function set.roi(self, roi)
            if isequal(size(roi),[1,2])
                if all(mod(roi, 2) == 0)
                   if roi(1)<=self.config.res(1) && roi(2)<=self.config.res(2)
                       
                   end
                end
            end
        end
        %}
        
        function set.grayscale(self, grayscale)
           % Set method for grayscale of images.
           %
           % Parameters
           % - grayscale, a positive integer between 0 and 255. 

           if grayscale>=0 && grayscale<=255
               self.grayscale= self.grayscale;
           else
               self.grayscale= 255;
           end
       end 
       
       function self = show(self, phase, varargin) 
            % Displays phase pattern on SLM.
            %
            % Parameter
            % - phase, 2D matrix giving phase mask. Phases should 
            % be normalized to be within 0-1 corresponding to 0-2pi.
            %
            % Optional named parameters:
            %   - 't' float -- Time to display image. If negative, the
            %   image will remain until closed.
            %   Default: `-1`
            
            p = inputParser;
            p.addParameter('t', -1);
            p.parse(varargin{:});
            
            pattern= self.mask(phase);
            self.showRaw('pattern',pattern);
            if p.Results.t >= 0
                pause(p.Results.t);
                self.close();
            end
        end

        function self = play(self, frames, varargin)
            % Displays a movie of phase patterns on SLM.
            %
            % Parameter
            % - frames, Cell array of 2D matrices corresponding to invidividual frames. 
            % Phases should be normalized to be within 0-1 corresponding to 0-2pi.
            %
            % Optional named parameters:
            %   - 'fps' float -- Framerate to use.
            %   Default: `1`
            
            p = inputParser;
            p.addParameter('fps', 1);
            p.parse(varargin{:}); 
            
            N= size(frames,2);
            frames= self.mask(frames);
            F = struct('cdata', {}, 'colormap', {});
            for i= 1:N
              F(i) = im2frame(frames{i});
            end
        
            self.showRaw('pattern',F,'framerate',p.Results.fps);
            pause(1/p.Results.fps);
            self.close();
        end 
        
        function lens = virtual_lens(self, pos, f)
            % Creates an overall virtual lens for moving beam in xy-plane and 
            % focusing it along the z-axis.
            % 
            % Parameters
            % - pos, normalized position of a translated point. 
            % - f, Focal length in mm. Choose to be ~distance to image plane.
            
            zlens= self.zlens(f);
            xylens= self.xylens(pos, f);
            lens= zlens+xylens;
        end
        
        function lens = zlens(self,f)
            % Creates virtual lens for focusing image along z-axis.
            % 
            % Parameters
            % - f, Focal length in mm. Choose to be ~distance to image plane. 
            
            if f>0
                lens = (sqrt(f.^2 + self.config.pitch^2*(self.Y.^2 + self.X.^2)) - abs(f))/self.config.wl;
            else
                lens= zeros(self.config.res);
            end
        end
        
        function lens = xylens(self, pos, f)
            % Creates virtual lens for moving beam in xy-plane.
            % 
            % Parameters
            % - pos, normalized position of a translated point. 
            % - f, Focal length in mm. Should match the focal len of a
            % virtual/real imaging lens. 
            
            if f ~= 0
                pos(1) = 0.25*self.config.res(1)*pos(1); 
                pos(2) = 0.25*self.config.res(2)*pos(2);
                lens = self.config.pitch^2*(self.X*pos(1) + self.Y*pos(2))/(self.config.wl*f);
            else
                lens= zeros(self.config.res);
            end
        end 

    end
end 