clc; clear; close all;

visualize = true;

% probes = 4:10;
% 
for outerLoop = 1%:length(probes)
%     
%     n = probes(outerLoop);
    
%% Perform simulation x times
for loop = 1

    % landmine simulation
    r = 7; % cm
    c.x = 0; c.y = 0;
    % probe simulation
    noise = 0.15; % 0.1 is reasonable assumption
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

    %% hough transform
    ccalc = houghCircle( pointsTrue, r );

    %% compute error
    circleFitError(loop) = fitError( pointsTrue, ccalc, r )/n; % normalized!
    groundTruthError(loop) = sqrt( (ccalc.x-c.x)^2+(ccalc.y-c.y)^2 ); 
    falseTrueRatio(loop) = length(pointsFalse) / length(pointsTrue);

end % inner loop

meanCircleFitError(outerLoop) = mean(circleFitError);
stdCircleFitError(outerLoop) = std(circleFitError);

meanGroundTruthError(outerLoop) = mean(groundTruthError);
stdGroundTruthError(outerLoop) = std(groundTruthError);

end % outer loop
% 
% if (loop > 1)
%     figure
%     subplot(2,1,1)
%     errorbar(probes,meanCircleFitError,stdCircleFitError)
%     title('Circle Fit Error vs # Probes')
%     hold on; grid on;
%     subplot(2,1,2)
%     errorbar(probes,meanGroundTruthError,stdGroundTruthError)
%     title('GT Error vs # Probes')
%     grid on; hold off;
% end

