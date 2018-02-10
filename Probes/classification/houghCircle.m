function [ ccalc ] = houghCircle( pointsTrue, r )
    
    visualize = true;

    %% draw circles in parameter space
    intersection = [];
    for i = 1:length(pointsTrue.x)
%         rectangle('Position',[pointsTrue.x(i)-r pointsTrue.y(i)-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','black')
        for j = 1:i
            [xout,yout] = circcirc(pointsTrue.x(i),pointsTrue.y(i),r,pointsTrue.x(j),pointsTrue.y(j),r);
            for k = 1:length(xout)
                if ( ~isnan(xout) ) 
                    intersection = [intersection; xout(k), yout(k)];
                end
            end
        end
    end

%     plot(intersection(:,1),intersection(:,2),'xm')
    
    %% remove points (bounding box)
    intersection = unique(intersection,'rows');
    x = [];
    for i = 1:length(intersection)
        if intersection(i,2) < max(pointsTrue.y) ...
                && intersection(i,1) < max(pointsTrue.x) ...
                && intersection(i,1) > min(pointsTrue.x)
           x = [x; intersection(i,:)];
        end
    end

    %% K-means
    clusters = 2;
    [idx,C] = kmeans(x,clusters);
    strongestCentre = C(mode(idx),:);
    
    if (visualize)
        plot(x(:,1),x(:,2),'xm')
        plot(C(:,1),C(:,2),'.m','MarkerSize',15);
        plot(strongestCentre(1),strongestCentre(2),'.m','MarkerSize',30);
        rectangle('Position',[strongestCentre(1)-r strongestCentre(2)-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','m')
    end
    
    ccalc.x = strongestCentre(1);
    ccalc.y = strongestCentre(2);
    
end

