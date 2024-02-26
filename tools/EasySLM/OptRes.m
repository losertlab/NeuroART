classdef OptRes < CGH
    
    properties (SetAccess = protected)
        input; % Indices corresponding to input state.
        reservoir; % Indices corresponding to reservoir state.
        bias; % Indices corresponding to bias.
    end
    
    methods (Static)
       
        function output = sample_res_state(res_state)
           % Need a function to sample/parse output speckle pattern. 
        end
        
    end
    
    methods (Access = protected)
        
        function input = input_state(self)
           % Need a function to find phase mask of input.  
        end
       
        function output = reservoir_state(self)
            % Need a function to sample/parse output speckle pattern. 
        end
        
        function phase = compile_phase_mask(self, input, res_state)
            % Need a function to combine the 3 regions. 
        end
        
    end
    
    methods 
        
        function self = OptRes(self, config, sz, varargin)
            % Need to divide the SLM into 3 regions (input, reservoir,
            % bias)
            
            self = self@SLM(config, varargin{:});
        end
        
    end
    
end