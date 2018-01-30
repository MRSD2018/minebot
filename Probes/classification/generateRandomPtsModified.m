function [ points ] = generateRandomPts( r,cx,cy,npt,aoa,noise,target )

viz = false;

angles = linspace(-aoa/2,aoa/2,npt)';

% generate vector
if (viz)
    figure
    plot(0,0,'x')
    hold on; axis equal; grid on;
    rectangle('Position',[cx-r cy-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','black')
end

if target
    for i = 1:npt

        line = -r*noise:r/50:r*noise;
        line = line + r;

        vector.x = cx+line*sind(angles(i));
        vector.y = cy+line*cosd(angles(i));

        if (viz)
            plot(vector.x,vector.y)
        end

        randIndex = randi([1 length(line)]);

        points.x(i) = vector.x(randIndex);
        points.y(i) = vector.y(randIndex);
        points.target(i) = true;

    end
else
    for i = 1:npt

        line = -r*noise*5:r/50:r*noise*5;
        line = line + r;

        vector.x = cx+line*sind(angles(i));
        vector.y = cy+line*cosd(angles(i));

        if (viz)
            plot(vector.x,vector.y)
        end

        randIndex = randi([1 length(line)]);

        points.x(i) = vector.x(randIndex);
        points.y(i) = vector.y(randIndex);
        points.target(i) = randi([0 1]);

    end
end

if (viz)
    plot(points.x,points.y)
    hold off
end

end

