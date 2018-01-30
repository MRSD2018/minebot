clc; clear; close all;

visualize = true;

%% Perform simulation x times
for loop = 1

    % landmine simulation
    r = 9; % cm
    c.x = 0; c.y = 0;
    % probe simulation
    noise = 0.2; % 0.1 is reasonable assumption
    aoa = 180; % probe angle of attack in degrees
    n = 9; % number of points
    [ pointsTrue, pointsFalse ] = generateRandomPtsModified( r,c.x,c.y,n,aoa,noise,true );

    % plot Ground Truth with simulated points
    if visualize
        figure; 
        hold on; axis equal; grid on;
        plot(pointsTrue.x,pointsTrue.y,'.green','MarkerSize',20)
        if (~isempty(pointsFalse))
            plot(pointsFalse.x,pointsFalse.y,'.red','MarkerSize',20)
        end
        plot(c.x,c.y,'.blue','MarkerSize',20)
        rectangle('Position',[c.x-r c.y-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','blue')
    end

    %% hough classifier

    % draw circles in parameter space
    intersection = [];
    for i = 1:length(pointsTrue.x)
    %     rectangle('Position',[points.x(i)-r points.y(i)-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','black')
        for j = 1:i
            [xout,yout] = circcirc(pointsTrue.x(i),pointsTrue.y(i),r,pointsTrue.x(j),pointsTrue.y(j),r);
            for k = 1:length(xout)
                if ( ~isnan(xout) ) 
                    intersection = [intersection; xout(k), yout(k)];
                end
            end
        end
    end

    % remove points
    intersection = unique(intersection,'rows');

    x = [];
    for i = 1:length(intersection)
        if intersection(i,2) < max(pointsTrue.y) ...
                && intersection(i,1) < max(pointsTrue.x) ...
                && intersection(i,1) > min(pointsTrue.x)
           x = [x; intersection(i,:)];
        end
    end

    plot(x(:,1),x(:,2),'xm')

    % K-means
    clusters = 3;
    [idx,C,sumd,D] = kmeans(x,clusters);
    plot(C(:,1),C(:,2),'.m','MarkerSize',15);
    strongestCentre = C(mode(idx),:);
    plot(strongestCentre(1),strongestCentre(2),'.m','MarkerSize',30);
    rectangle('Position',[strongestCentre(1)-r strongestCentre(2)-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','m')

    ccalc.x = strongestCentre(1);
    ccalc.y = strongestCentre(2);

    %% compute error metric
    % find line between point and circle centre
    circleFitError = 0;
    for i = 1:length(pointsTrue.x)
        coefficients = polyfit([pointsTrue.x(i), ccalc.x], [pointsTrue.y(i), ccalc.y], 1);
        slope = coefficients (1);
        intercept = coefficients (2);

        % find line intersection with new circle
        [xcross,ycross] = linecirc(slope,intercept,ccalc.x,ccalc.y,r);

        % find closest point and computer squared error  
        for j = 1:length(xcross)
            dist(j) = sqrt( (pointsTrue.x(i)-xcross(j))^2+(pointsTrue.y(i)-ycross(j))^2 ); 
        end    
        circleFitError = circleFitError + min(dist)^2;
    end

    circleFitError(loop) = circleFitError/n; % normalized!
    falseTrueRatio(loop) = length(pointsFalse) / length(pointsTrue);

end

histBins = 100;

figure
subplot(2,1,1)
histogram(circleFitError,histBins)
title('Circle Fit Error')
hold on; grid on;
% subplot(2,1,2)
% histogram(radiusError,histBins)
% title('Radius Estimate Error')
% grid on; hold off;

