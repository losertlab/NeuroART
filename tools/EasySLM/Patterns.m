classdef Patterns

    methods (Static)
        
        function img = window(res, dt)
            % Creates a border around
            % the SLM screen.
            %
            % Parameters
            % -res, pixel resolution of image.
            % -dt, pixel thickness of border.
            
            assert(isequal(size(res),[1,2]));
            img= uint8(ones(res));
            img(dt:res(1)-dt,dt:res(2)-dt)= 0;
        end

        function img = crosshair(res, dt)
            % Creates a centered crosshair pattern.
            %
            % Parameters
            % -res, pixel resolution of image.
            % -dt, pixel thickness of crosshair lines.
            
            assert(isequal(size(res),[1,2]) && all(mod(res, 2)==0));
            assert(dt>=2 && mod(dt,2)==0);

            img= uint8(zeros(res));
            img((res(1)-dt)/2:(res(1)+dt)/2,:)= 255;
            img(:,(res(2)-dt)/2:(res(2)+dt)/2)= 255;
        end
        
        function img = spots(res, points, r)
            % Creates a spot pattern.
            %
            % Parameters
            % -res, pixel resolution of image.
            % -points, Nx2 array containing centers of each dot.
            % -r, radius of a dot.
            
            assert(size(points, 2) == 2);
            points(:, 1) = 0.5*(res(1)+1)*points(:, 1)+res(1)/2; 
            points(:, 2) = 0.5*(res(2)+1)*points(:, 2)+res(2)/2;
            
            P= points(1,:);
            img= otslm.simple.aperture(res, r, 'centre', [P(2),P(1)]);
            for i= 2:size(points,1)
                P= points(i,:);
                img =img|otslm.simple.aperture(res, r, 'centre', [P(2),P(1)]);
            end
        end

    end
        
end