 classdef DHOT1 < SLM
    % Class for computing optical tweezers and then 
    % displaying them on SLM.
    
    properties (SetAccess = protected)
       tweezers= {}; % Cell array containing individual tweezer phase masks.
       positions= {}; % Cell array of tweezer positions (it is assumed each phase mask is centered).
       zoffsets= {}; % Cell array of tweezer displacements.
       N= 0; % Number of tweezers.
       f_eff; % Effective focal length of tweezer array in mm.
    end
    
    methods (Static)
        
        function phase = GS(components, alpha, N)
                % Performs Gerchberg-Saxon algorithm to combine optical tweezer
                % phase masks.
                %
                % Parameters
                % - components, individual phase masks to combine.
                % - alpha, adaptive additive factor. 1 corresponds to normal GS
                %   algorithm.
                % - N, number of iterations to perform.

                ntweezers= size(components,3);
                epsilon = zeros(ntweezers,N,'single');
                A = zeros(ntweezers,N,'single');
                A(:, 1) = ones(ntweezers, 1)/sqrt(ntweezers); % epsilon_j = alpha_j * exp(i* phi_j)
                epsilon(:,1) = A(:, 1);
                
                if isa(components, 'gpuArray')
                    A= gpuArray(A);
                    epsilon= gpuArray(epsilon);
                end
                
                e= exp(1i*components);
                E = sum(bsxfun(@times,e,reshape(epsilon(:,1),1,1,ntweezers)),3); % Eq. (1) of the dhot paper
                dA= size(components,1)*size(components,2);
                
                for i = 1:N % iterations in the GS algorithm, Eq. (3) of the dhot paper
                    % Compute phi^{(n)} from equation (2) and E.
                    phi_sum = (E./abs(E))./e;  
                    % Compute next epsilon values using equation (3) and phi.
                    epsilon(:, i) = reshape(sum(sum(phi_sum,1),2),[1,ntweezers])/dA;
                    % Replace amplitudes using equation (4).
                    A(:, i) = abs(epsilon(:, i));
                    A_prime = (1 - alpha)*A(:, 1) + alpha*A(:,1).^2./A(:, i);
                    epsilon(:,i) = A_prime.*exp(1i*angle(epsilon(:,i)));
                    % Compute E from equation (1) which is the early form of the phase mask
                    E = sum(bsxfun(@times,e,reshape(epsilon(:,i),1,1,ntweezers)),3); 
                end

                if isa(E, 'gpuArray')
                    E= gather(E);
                end

                phase = (angle(E) + pi);  
          end 
        
    end
    
    methods (Access = protected, Static)
        
        function param = parse_twzr_param(param, ntwzrs)
            % Parses a parameter(s) for a new tweezer.
            %
            % Parameters
            % - param, float/vector tweezer specific parameter(s).
            % - ntwzrs, integer number of tweezers.

            if isscalar(param)
                param=param*ones(ntwzrs,1);
            else 
                assert(isvector(param));
                assert(length(param) == ntwzrs);
            end 
        end
                
    end 

    methods
        
        function self = DHOT1(config, varargin)
          % Constructs a new SLM class instance.
          %
          % Parameters
          % - config, Config class instance that gives all 
          %   setup specific parameters.
          %
          % Optional named parameters:
          %   - 'f' float -- Effective focal length of setup. 
          %     Default: `250`

            p = inputParser;
            p.KeepUnmatched = true;
            p.addParameter('f', 0);
            p.parse(varargin{:});
            
            unmatched = [fieldnames(p.Unmatched).'; struct2cell(p.Unmatched).'];
            self = self@SLM(config, unmatched{:});
            self.f_eff= 250;
            self.f_eff= p.Results.f;
        end
        
        function set.f_eff(self, f)
            % Set method for effective focal length.
            %
            % Parameters
            % - f, new effective focal length.
            
            if f~=0
                self.f_eff=f;
            end
        end
        
        function self = add_tweezer(self, pos, phase, dz)
          % Specify optical tweezer array parameters.
          %
          % Parameters
          % - pos, array of tweezer positions.
          % - phase, arbitrary phase pattern. 
          % - dz, z displacement of tweezers.

            if size(phase,1) ~= self.config.res(1) || size(phase,2) ~= self.config.res(2)
                error('Dimension mismatch.');
            end

            assert(size(pos, 2) == 2); % check whether OTs have both x and y coordinates   
            pos(:, 1) = 0.5*(self.config.res(1))*(pos(:, 1))*self.config.pitch; 
            pos(:, 2) = 0.5*(self.config.res(2))*(pos(:, 2))*self.config.pitch;
            n= size(pos,1);
            
            dz= self.parse_twzr_param(dz,n);
            
            for i=1:n
                self.tweezers{self.N+i}= 2*pi*phase;
                self.positions{self.N+i}= pos(i,:);
                self.zoffsets{self.N+i}= dz(i);
            end
            self.N= self.N+n;
        end
        
        function self = add_vortex(self, pos, l, dz)
          % Specify optical tweezer array parameters.
          %
          % Parameters
          % - pos, array of tweezer positions.
          % - l, angular momentum of optical vortex.
          % - dz, z displacement of tweezers.

            assert(size(pos, 2) == 2); % check whether OTs have both x and y coordinates   
            pos(:, 1) = 0.5*(self.config.res(1))*(pos(:, 1))*self.config.pitch; 
            pos(:, 2) = 0.5*(self.config.res(2))*(pos(:, 2))*self.config.pitch;
            n= size(pos,1);
            
            l= self.parse_twzr_param(l,n);
            dz= self.parse_twzr_param(dz,n);
            theta = atan2(self.Y, self.X);
            
            for i=1:n
                self.tweezers{self.N+i}= theta .* l(i);
                self.positions{self.N+i}= pos(i,:);
                self.zoffsets{self.N+i}= dz(i);
            end
            self.N= self.N+n;
        end
        
      function self = add_axicon(self, pos, G, dz)
          % Specify optical tweezer array parameters.
          %
          % Parameters
          % - pos, array of tweezer positions.
          % - G, gradient of axicon pattern.
          % - dz, z displacement of tweezers.

            assert(size(pos, 2) == 2); % check whether OTs have both x and y coordinates   
            pos(:, 1) = 0.5*(self.config.res(1))*(pos(:, 1))*self.config.pitch; 
            pos(:, 2) = 0.5*(self.config.res(2))*(pos(:, 2))*self.config.pitch;
            n= size(pos,1);
            
            G= self.parse_twzr_param(G,n);
            dz= self.parse_twzr_param(dz,n);
            rho = sqrt(self.X.*self.X + self.Y.*self.Y); 
            
            for i=1:n
                self.tweezers{self.N+i}= -rho .* G(i);
                self.positions{self.N+i}= pos(i,:);
                self.zoffsets{self.N+i}= dz(i);
            end
            self.N= self.N+n;
        end
        
        function self = move_twzr(self, n, pos)
            % Moves a single tweezer to a new position.
            %
            % Parameters
            % - n, integer specifying tweezer.
            % - pos, [x,y] positon to move tweezer to.
            
            assert(length(pos) == 2);
            xy= self.config.pitch*[0.25*self.config.res(1)*pos(1),0.25*self.config.res(2)*pos(2)];
            self.positions{n}= xy;
        end 
        
        function phase = dhot(self, varargin) 
          % Calculates phase mask for optical tweezer array.
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
            
            dz= cell2mat(self.zoffsets);
            pos= transpose(cell2mat(cellfun(@(x)reshape(x,2,1),self.positions,'un',0)));
            twzrs= reshape(cell2mat(cellfun(@(x)reshape(x,self.config.res(1),self.config.res(2),[]) ... 
                ,self.tweezers,'un',0)),self.config.res(1),self.config.res(2),self.N);

            rho = sqrt(self.X.*self.X + self.Y.*self.Y); 
            trap = zeros(self.config.res(1),self.config.res(2),self.N);
            for i=1:self.N
                trap(:,:,i) = (2*pi).*self.xylens(pos(i,:),self.f_eff) + ...
                    (2*pi).*rho.^2*dz(i)/(self.config.wl*self.f_eff^2);
            end

            if p.Results.use_gpu
                components= gpuArray(trap+twzrs);
            else
                components= trap+twzrs;
            end

            phase = DHOT1.GS(components,p.Results.alpha,p.Results.N)./(2*pi); %normalize phase
        end
        
    end
end 