classdef CGH < SLM
    % Class for computing CGH images and then 
    % displaying them on SLM.
    
    properties (SetAccess = protected)
        incident; % 2D incident intensity matrix
        padding; % Padding to add to images when performing FFTs.
    end
    
    methods (Static)
        
        function phase = GS(incident, target, alpha, N)
            % Performs Gerchberg-Saxon algorithm.
            %
            % Parameters
            % - incident, 2D matrix (can be gpuArray) giving incident
            %   intensity.
            % - target, 2D matrix (can be gpuArray) for which phase mask
            %   will be calculated.
            % - alpha, adaptive additive factor. 1 results in standard
            %   GS algorithm.
            % - N, integer number of iteratons to perform.

            if isa(target, 'gpuArray')
               if ~isa(incident, 'gpuArray')
                    incident= gpuArray(incident);
               end
            elseif isa(incident, 'gpuArray')
                if ~isa(target, 'gpuArray')
                    target= gpuArray(target);
                end
            end 

            target=double(target);
            A = (ifft2(fftshift(target))).*numel(target);
            for i=1:N
                B = abs(incident) .* exp(1i*angle(A));
                C = fftshift(fft2((B)))./numel(B);
                
                T= alpha.*abs(target) ...
                    + (1 - alpha).*abs(C);
                
                D = T .* exp(1i*angle(C));
                A = (ifft2(fftshift(D))).*numel(D);
            end
            
            if isa(A, 'gpuArray')
                A= gather(A);
            end

            phase= angle(A)+pi;
        end
        
    end 
    
    methods (Access = protected) 
           
        function img = pad(self, im)
           % Adds padding to an image.
           %
           % Parameters
           % - im, 2D matrix to be padded.
           %
            res= size(im) + 2*self.padding;
            img= zeros(res);
            img(self.padding+1:res(1)-self.padding, ...
                self.padding+1:res(2)-self.padding) = im;
        end 
        
    end

    methods
        
       function self = CGH(config, varargin)
          % Constructs a new SLM class instance.
          %
          % Parameters
          % - config, Config class instance that gives all 
          %   setup specific parameters.
          %
          % Optional named parameters:
          %   - 'incident' matrix -- Matrix matching screen resolution that
          %     gives incident intensity pattern. 
          %     Default: `ones(res)` (uniform intensity)
          %   - 'padding' int -- Padding to add to all images when
          %      performing calculations. 
          %     Default: `100`

            p = inputParser;
            p.KeepUnmatched = true;
            p.addParameter('incident', ones(config.res));
            p.addParameter('padding', 0);
            p.parse(varargin{:});
            
            unmatched = [fieldnames(p.Unmatched).'; struct2cell(p.Unmatched).'];
            self = self@SLM(config, unmatched{:});
            
            self.incident= p.Results.incident;
            self.padding= p.Results.padding;
            
       end
        
       function set.incident(self, incident)
            % Set method for incident illumination.
            %
            % Parameters
            % - incident, matrix matching screen resolution that
            %   gives incident intensity pattern. 
            
            d= size(incident);
            if length(d) ~=2
                error('Expecting 2D Intensity Pattern');
            end
            if d(1) ~= self.config.res(1) || d(2) ~= self.config.res(2)
                if ~(d(1)==max(self.config.res) || d(2)==max(self.config.res))
                    error('Dimension mismatch.');
                end
            end
            
            self.incident= incident;
       end
        
       function set.padding(self, padding)
           % Set method for calculation pixel padding.
           %
           % Parameters
           % - padding, a positive integer number of pixels to add
           %   to all images during calculations. 

           if padding>=0
               self.padding= padding;
           else
               error('Expecting positive integer padding.');
           end
       end 
       
      function phase = cgh(self, target, varargin)
          % Calculates phase mask for a target image.
          %
          % Parameters
          % - target, 2D matrix for which phase pattern will be calculated.
          %
          % Optional named parameters:
          %   - 'alpha' int -- Adaptive additive factor. 1 for 
          %     normal GS algorithm.
          %     Default: `1` 
          %   - 'N' int  -- Number of iterations to perfrom GS algorithm.
          %     Default: `10`
          %   - 'use_gpu' bool -- Whether to use gpu to calculate phase mask. 
          %     Default: `false`

            p = inputParser;
            p.addParameter('alpha', 1);
            p.addParameter('N', 10);
            p.addParameter('use_gpu', false);
            p.parse(varargin{:});
            
            d= size(target);
            if length(d) ~=2
                error('Expecting 2D Target');
            end 
            if d(1) ~= self.config.res(1) || d(2) ~= self.config.res(2)
                error('Dimension mismatch.');
            end
            
            target= self.pad(self.resize(target));
            I= self.pad(self.resize(self.incident));
            if p.Results.use_gpu
                target= gpuArray(target);
                I= gpuArray(I);
            end

            phase = CGH.GS(I,target,p.Results.alpha,p.Results.N)./(2*pi); %normalize phase
            phase= self.crop(phase);
      end 

        
      function phases = multi_cgh(self, targets, varargin)
          % Calculates phase masks in parallel for multiple targets.
          %
          % Parameters
          % - target, cell array of 2D matrices for which phase patterns will be calculated.
          %
          % Optional named parameters:
          %   - 'alpha' int -- Adaptive additive factor. 1 for 
          %     normal GS algorithm.
          %     Default: `1` 
          %   - 'N' int  -- Number of iterations to perfrom GS algorithm.
          %     Default: `10`
          %   - 'use_gpu' bool -- Whether to use gpu to calculate phase mask. 
          %     Default: `false`

            p = inputParser;
            p.addParameter('alpha', 1);
            p.addParameter('N', 10);
            p.addParameter('use_gpu', false);
            p.parse(varargin{:});

            for i= 1:size(targets,2)
                d= size(targets{i});
                if length(d) ~=2
                    error('Expecting 2D Target');
                end 
                if d(1) ~= self.config.res(1) || d(2) ~= self.config.res(2)
                    error('Dimension mismatch.');
                end
            end 

            use_gpu= p.Results.use_gpu;
            N= p.Results.N;
            alpha= p.Results.alpha;
            I= self.incident;
            phases= cell(1,size(targets,2));
            parfor i= 1:size(targets,2)
                if use_gpu
                    target= gpuArray(targets{i});
                else 
                    target= targets{i};
                end 

                phases{i}= (CGH.GS(I,target,alpha,N) + pi)./(2*pi); %normalize phase
            end
      end 
      
    end
end 