function max_std_image = stdImg_window(regImg,window_size)

% Assuming 'data' is your 512x512x601 matrix
data = regImg;  % Replace with your actual data variable

% Initialize variables
[num_rows, num_cols, num_frames] = size(data);
% window_size = 20;
num_windows = num_frames - window_size + 1;

% Initialize max std image
max_std_image = zeros(num_rows, num_cols);

% Loop through each window of 30 frames
for i = 1:num_windows
    % Extract the current 30-frame segment
    current_segment = data(:, :, i:i+window_size-1);
    
    % Calculate standard deviation across the third dimension (frames)
    std_image = std(double(current_segment), 0, 3);
    
    % Update max std image with the maximum standard deviation values
    max_std_image = max(max_std_image, std_image);
end

% max_std_image now contains the maximum standard deviation image
% imshow(max_std_image, []);  % Display the max std image
% colorbar;                   % Add a colorbar to visualize the intensity

end