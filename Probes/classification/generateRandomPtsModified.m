function [ pointsTrue, pointsFalse ] = generateRandomPts( r,cx,cy,npt,aoa,noise,target )

    visualize = false;

    if (visualize)
        figure
        plot(0,0,'x')
        hold on; axis equal; grid on;
        rectangle('Position',[cx-r cy-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','black')
    end
    
    angles = linspace(-aoa/2,aoa/2,npt)'; % get probe angles

    if target
        for i = 1:npt

            line = -r*noise:r/50:r*noise;
            line = line + r;

            vector.x = cx+line*sind(angles(i));
            vector.y = cy+line*cosd(angles(i));

            if (visualize)
                plot(vector.x,vector.y)
            end

            randIndex = randi([1 length(line)]);

            points.x(i) = vector.x(randIndex);
            points.y(i) = vector.y(randIndex);
            points.target(i) = true;

        end
    else
        for i = 1:npt

            line = -r*noise*3:r/50:r*noise*3;
            line = line + r;

            vector.x = cx+line*sind(angles(i));
            vector.y = cy+line*cosd(angles(i));

            if (visualize)
                plot(vector.x,vector.y)
            end

            randIndex = randi([1 length(line)]);

            points.x(i) = vector.x(randIndex);
            points.y(i) = vector.y(randIndex);
            
            points.target(i) = 0;
            if (sum(points.target) < 3)
                points.target(i) = 1;
            else
                points.target(i) = randi([0 1]);
            end

        end
    end

    % separate data
    trueCounter = 1; falseCounter = 1;
    pointsTrue = []; pointsFalse = [];
    for i = 1:length(points.target)
        if points.target(i)
           pointsTrue.x(trueCounter) = points.x(i);
           pointsTrue.y(trueCounter) = points.y(i);
           trueCounter = trueCounter + 1;
        else
           pointsFalse.x(trueCounter) = points.x(i);
           pointsFalse.y(trueCounter) = points.y(i);
           falseCounter = falseCounter + 1; 
        end
    end

    if (visualize)
        plot(points.x,points.y)
        hold off
    end

end

