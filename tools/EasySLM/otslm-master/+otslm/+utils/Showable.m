classdef (Abstract) Showable < handle
% Represents devices that can display a pattern.
% Inherits from :class:`handle`.
%
% Methods (abstract):
%   - showRaw(pattern)  --  Display the pattern on the device.  The pattern
%     is raw values from the device valueRange (i.e. colour mapping
%     should already have been applied).
%
% Methods:
%   - show(pattern)     --  Display the pattern on the device.  The pattern
%     type is determined from the patternType property.
%
%   - showComplex(pattern) -- Display a complex pattern.  The default
%     behaviour is to call show after converting the pattern
%     to the patternType of the device.  Conversion is done by calling
%     otslm.tools.finalize with for amplitude, phase target.
%
%   - showIndexed(pattern) -- Display a pattern with integers describing
%     entries in the lookup table.
%
%   - view(pattern)        -- Calculate the raw pattern.
%   - viewComplex(pattern) -- Calculate the raw pattern from complex
%   - viewIndexed(pattern) -- Calculate the raw pattern from indexed
%
%   - valueRangeNumel() --  Total number of values device can display
%
% Properties (abstract):
%   - valueRange      --  Values that the device patterns can contain.
%     This should be a 1-d array, or cell array of 1-d arrays for
%     each dimension of the raw pattern.
%
%   - patternType     --  Type of pattern, can be one of:
%    - 'phase'        --   Real pattern in range [0, 1]
%    - 'amplitude'    --   Real pattern in range [0, 1]
%    - 'complex'      --   Complex pattern, abs(value) <= 1
%
%   - size            --  Size of the device [rows, columns]
%   - lookupTable     --  Lookup table for show -> raw mapping
%
% This is the interface that utility functions which request an
% image from the experiment/simulation use.  For declaring a new
% display device, you should inherit from this class and define
% the abstract methods and properties described above.
% You can also override the other methods if needed.
%
% See also Showable and otslm.utils.ScreenDevice.

% Copyright 2018 Isaac Lenton
% This file is part of OTSLM, see LICENSE.md for information about
% using/distributing this file.

  methods (Abstract)
    showRaw(obj, pattern)     % Method to show raw pattern
  end

  methods
    function slm = Showable(varargin)
      % Constructor for Showable objects, provides options for default vals
      %
      % Optional named arguments
      %   - prescaledPatterns   bool  -- Default value for prescaled argument
      %     in show.  Default: false.
      
      p = inputParser;
      p.addParameter('prescaledPatterns', false);
      p.parse(varargin{:});
      
      slm.prescaledPatterns = p.Results.prescaledPatterns;
    end
    
    function im = view(slm, pattern)
      % Generate the raw pattern that is displayed on the device
      %
      % im = slm.view(pattern) apply the modulo (if phase device)
      % and apply the lookup table.  Any nans remaining in the image
      % are replaced by the first value in the lookup table.

      % Determine if we should apply a modulo
      switch slm.patternType
        case 'amplitude'
          modulo = 'none';
        case 'phase'
          modulo = 1;
        case 'complex'
          % Nothing to do
        otherwise
          error('Unknown pattern type for class');
      end
      
      % Convert the pattern to a raw pattern
      im = otslm.tools.finalize(pattern, 'colormap', slm.lookupTable, ...
        'modulo', modulo);

        % Remove NANs, replace with first value from lookupTable
        if any(isnan(im(:)))
          if ~isempty(slm.lookupTable)
            warning('Replacing nans with first value from lookup table');
            im(isnan(im)) = repmat(slm.lookupTable.value(1, :), ...
                  [sum(sum(isnan(im(:, :, 1)))), 1]);
          else
            warning('Leaving nans in pattern');
          end
        end
    end

    function im = viewComplex(slm, pattern)
      % Convert the complex pattern to a raw pattern

      % Split the pattern
      phase = angle(pattern);
      amplitude = abs(pattern);

      % Convert the pattern
      switch slm.patternType
        case 'amplitude'
          pattern = otslm.tools.finalize(phase, 'amplitude', amplitude, ...
              'device', 'dmd', 'rpack', 'none', 'colormap', 'gray');
        case 'phase'
          pattern = otslm.tools.finalize(phase, 'amplitude', amplitude, ...
              'device', 'slm', 'rpack', 'none', 'colormap', 'gray');
        case 'complex'
          % Nothing to do
        otherwise
          error('Unknown pattern type for class');
      end
      
      % Generate the raw pattern
      im = slm.view(pattern);
    end
    
    function rawpattern = viewIndexed(slm, pattern)
      % Convert the indexed pattern to a raw pattern

      assert(min(pattern(:)) >= 1 ...
          && max(pattern(:)) <= slm.valueRangeNumel(), ...
          'Indicies must be between 1 and valueRangeNumel');

      % Get lookup table for linear indexes
      valueTable = slm.linearValueRange('structured', true);

      % Generate the raw pattern
      rawpattern = zeros([slm.size, length(slm.valueRange)]);
      for ii = 1:length(slm.valueRange)
        layer = valueTable(ii, pattern(:));
        rawpattern(:, :, ii) = reshape(layer, slm.size);
      end
    end

    function show(obj, varargin)
      % Method to show device type pattern
      %
      % slm.show(...) with no pattern opens the window with an empty
      % pattern.
      %
      % slm.show(pattern, ...) displays the pattern after applying the
      % color map.  For phase based devices, the pattern should not be
      % scalled by 2*pi (i.e. mod(pattern, 1)*2*pi should give the angle).
      %
      % Optional named arguments:
      %   -- prescaled  bool -- If the pattern is already scalled by 2*pi.
      %      Default: false.
      %
      % Additional arguments are passed to showRaw.
      %
      % See also view, showRaw, showComplex.

      p = inputParser;
      p.KeepUnmatched = true;
      p.addOptional('pattern', []);
      p.addParameter('prescaled', obj.prescaledPatterns);
      p.parse(varargin{:});
      
      if isempty(p.Results.pattern)
        obj.showRaw(varargin{:});
      else
        pattern = p.Results.pattern;
        if p.Results.prescaled
          pattern = pattern ./ (2*pi);
        end
        
        pattern = obj.view(pattern);
        obj.showRaw(pattern, varargin{2:end})
      end
    end

    function showComplex(obj, pattern, varargin)
      % Default function to display a complex pattern on a device
      %
      % slm.showComplex(pattern, ...)
      % Additional arguments are passed to showRaw.

      % Split the pattern
      phase = angle(pattern);
      amplitude = abs(pattern);

      % Convert the pattern
      switch obj.patternType
        case 'amplitude'
          pattern = otslm.tools.finalize(phase, 'amplitude', amplitude, ...
              'device', 'dmd', 'rpack', 'none', 'colormap', 'gray');
        case 'phase'
          pattern = otslm.tools.finalize(phase, 'amplitude', amplitude, ...
              'device', 'slm', 'rpack', 'none', 'colormap', 'gray');
        case 'complex'
          % Nothing to do
        otherwise
          error('Unknown pattern type for class');
      end

      % Call the show method to display the function
      obj.show(pattern, varargin{:});
    end

    function showIndexed(slm, pattern, varargin)
      % Display a pattern described by linear indexes on the device
      %
      % slm.showIndexed(pattern, ...)
      % Additional arguments are passed to showRaw.
      rawpattern = slm.viewIndexed(pattern);
      slm.showRaw(rawpattern, varargin{:});
    end

    function valueRangeSz = valueRangeSize(obj, idx)
      % Calculate the size of the lookup table
      valueRangeSz = zeros([1, length(obj.valueRange)]);
      for ii = 1:length(valueRangeSz)
        valueRangeSz(ii) = length(obj.valueRange{ii});
      end
      
      if nargin == 2
        valueRangeSz = valueRangeSz(idx);
      end
    end
    
    function num = valueRangeNumel(obj)
      % Calculate the total number of values the device can display
      num = prod(obj.valueRangeSize());
    end
    
    function values = linearValueRange(obj, varargin)
      % Generate an array of all possible device value combinations
      %
      % linearValueRange('structured', true) generates a table with
      % as many rows as valueRange has cells.
      %
      % lienarValueRange() generates a table with a single column.
      % values in each column of valueRange must be column unique.
      
      p = inputParser;
      p.addParameter('structured', false);
      p.parse(varargin{:});
      
      valueRangeOrder = obj.linear_order;
      if isempty(valueRangeOrder)
        valueRangeOrder = 1:length(obj.valueRange);
      end
      
      valueRangeSz = obj.valueRangeSize();
      
      if p.Results.structured
        values = zeros([length(obj.valueRange), obj.valueRangeNumel()]);
        
        % Generate values for each column
        for ii = 1:length(obj.valueRange)

          % Get the value range column in row form
          data = obj.valueRange{valueRangeOrder(ii)}(:).';

          % Repeate the values for every remaining column
          if ii+1 <= length(obj.valueRange)
            data = repmat(data, [prod(valueRangeSz(valueRangeOrder(ii+1:end))), 1]);
          end

          % Convert to column form
          data = reshape(data, [numel(data), 1]);

          % Repeate the values for all previous columns
          data = repmat(data, [prod(valueRangeSz(valueRangeOrder(1:ii-1))), 1]);

          % Store the column
          values(valueRangeOrder(ii), :) = data;

        end
      else
        values = zeros(obj.valueRangeNumel(), 1);
        
        % Calculate range and min values
        minvalues = zeros(length(obj.valueRange), 1);
        maxvalues = zeros(length(obj.valueRange), 1);
        for ii = 1:length(obj.valueRange)
          minvalues(ii) = min(obj.valueRange{ii});
          maxvalues(ii) = max(obj.valueRange{ii}) - minvalues(ii);
          
          if size(unique(obj.valueRange{ii})) ~= size(obj.valueRange{ii})
            error('columns of valueRange must have no repetitions');
          end
        end
        
        % Get a modified value table with normalized values
        mvalueRange = obj.valueRange;
        for ii = 1:length(mvalueRange)
          mvalueRange{ii} = mvalueRange{ii} - minvalues(ii);
        end
        
        for ii = 1:length(mvalueRange)
          
          % Get the value range column in row form
          data = mvalueRange{valueRangeOrder(ii)}(:).';

          % Repeate the values for every remaining column
          if ii+1 <= length(mvalueRange)
            data = repmat(data, [prod(valueRangeSz(valueRangeOrder(ii+1:end))), 1]);
          end

          % Convert to column form
          data = reshape(data, [numel(data), 1]);

          % Repeate the values for all previous columns
          data = repmat(data, [prod(valueRangeSz(valueRangeOrder(1:ii-1))), 1]);

          % Store the result
          values(:) = values(:) + data*prod(maxvalues(valueRangeOrder(1:ii-1)));
        end
      end
    end
  end
  
  properties (SetAccess=protected)
    linear_order = []; % Significance of valueRange columns
  end
  
  properties
    prescaledPatterns; % Default prescaled argument for slm.show
  end
  
  properties (Abstract)
    size              % Size of the device
  end

  % TODO: These should all probably be SetAccess=public
  % and they should have set methods which verify the type
  properties (Abstract, SetAccess=protected)
    valueRange        % Range of values for raw pattern
    lookupTable       % Lookup table for raw values
    patternType       % Type of pattern show() expects
  end

end
