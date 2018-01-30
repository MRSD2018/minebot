clc; clear; close all;

viz = true;  

for loop = 1:10

    %% ground truth circle
    % landmine simulation
    r = 9; % cm
    c.x = 0; c.y = 0;
    % probe simulation
    noise = 0.2; % 0.1 is reasonable assumption
    aoa = 180; % probe angle of attack in degrees
    n = 9; % number of points
    [ points ] = generateRandomPtsModified( r,c.x,c.y,n,aoa,noise,true );

    % get positive subset
    counter = 1;
    for i = 1:length(points.target)
       if points.target(i)
           pointsTrue.x(counter) = points.x(i);
           pointsTrue.y(counter) = points.y(i);
           counter = counter + 1;
       end
    end

    % plot GT with simulated points
    if viz
        figure; 
        hold on; axis equal; grid on;

        for i = 1:length(points.target)
            if points.target(i)
                plot(points.x(i),points.y(i),'.green','MarkerSize',20)
            else
                plot(points.x(i),points.y(i),'.red','MarkerSize',20)
            end
        end

        plot(c.x,c.y,'.blue','MarkerSize',20)
        rectangle('Position',[c.x-r c.y-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','blue')
    end

    %% hough classifier

    % draw circles in parameter space
    intersection = [];
    for i = 1:length(points.x)
    %     rectangle('Position',[points.x(i)-r points.y(i)-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','black')
        for j = 1:i
            [xout,yout] = circcirc(points.x(i),points.y(i),r,points.x(j),points.y(j),r);
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
        if intersection(i,2) < max(points.y) ...
                && intersection(i,1) < max(points.x) ...
                && intersection(i,1) > min(points.x)
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
    for i = 1:length(points.x)
        coefficients = polyfit([points.x(i), ccalc.x], [points.y(i), ccalc.y], 1);
        slope = coefficients (1);
        intercept = coefficients (2);

        % find line intersection with new circle
        [xcross,ycross] = linecirc(slope,intercept,ccalc.x,ccalc.y,r);

        % find closest point and computer squared error  
        for j = 1:length(xcross)
            dist(j) = sqrt( (points.x(i)-xcross(j))^2+(points.y(i)-ycross(j))^2 ); 
        end    
        circleFitError = circleFitError + min(dist)^2;
    end

    circleFitError(loop) = circleFitError/n; % normalise by number of points becuase this is ongoing!
%     radiusError(loop) = abs(r-rHat);
    trueFalseRatio(loop) = length(pointsTrue) / ...
                           length(points.target(points.target == false));

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







%% compute centre
% [ center_x,center_y ] = initCircle( points );
% ccalc.x = center_x; ccalc.y = center_y;
% plot(ccalc.x,ccalc.y,'.m','MarkerSize',20)
% rectangle('Position',[ccalc.x-r ccalc.y-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','m')
% 
%% circle estimator w/ unknown radius
% 
% radiusKnown = true;
% 
% % eps1 = 1e-5;
% % eps2 = 1e-5;
% epsinner = .1;
% epsouter = 1e-10;
% 
% maxIter = 100;
% 
% % compute rHat (eq 5)
% if radiusKnown 
%     rHat = r;
% else    
%     [ rHat ] = updateRadius( pointsTrue,ccalc );
% end
% % compute J (eq 6)
% % compute gradJ (eq 7)
% [ J,dJdx,dJdy ] = computeCost( pointsTrue,ccalc,rHat );
% 
% % if abs(J) < eps1 || sqrt(dJdx^2+dJdy^2)<eps2
% %     break
% % end
% 
% Jprev = J;
% uprev = 0;
% vprev = 0;
% prevDJdx = dJdx; %dJdx or 0 (in code)
% prevDJdy = dJdy; %dJdy or 0 (in code)
% i = 1; % iterations
% 
% for i = 1:maxIter
%     u = -dJdx;
%     v = -dJdy;
%     if i>1
%         beta =(dJdx * (dJdx - prevDJdx) + dJdy * (dJdy - prevDJdy))/ (prevDJdx^2 + prevDJdy^2);
%         u = u+beta*uprev;
%         v = v+beta*vprev;
%     end
%     prevDJdx = dJdx;
%     prevDJdy = dJdy;
%     uprev    = u;
%     vprev    = v;
%     if i==1
%         Jinner = J*10; % to make sure it enters inner loop
%     end
%     while (i<=maxIter && (abs(J-Jinner)/J)>epsinner)
%         Jinner = J;
%         lambda = newtonStep(u,v,rHat,pointsTrue,ccalc);
%         ccalc.x = ccalc.x+lambda*u;
%         ccalc.y = ccalc.y+lambda*v;
% %         if ~radiusKnown
%             rHat = updateRadius(pointsTrue,ccalc);
% %         end
%         [ J,dJdx,dJdy ] = computeCost( pointsTrue,ccalc,rHat );
%     end
% %     fprintf('abs(J-Jprev)/J is %f\n',abs(J-Jprev)/J);
%     if(abs(J-Jprev)/J)<epsouter
%         break
%     end
%     Jprev = J;
%     i = i+1;

% end

% %% plot results
% if viz
%     plot(ccalc.x,ccalc.y,'.g','MarkerSize',20)
%     rectangle('Position',[ccalc.x-rHat ccalc.y-rHat 2*rHat 2*rHat],'Curvature',[1 1],'EdgeColor','g')
% end


