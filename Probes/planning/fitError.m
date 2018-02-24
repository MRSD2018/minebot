function [ circleFitError ] = fitError( pointsTrue, ccalc, r )

    % compute error metric...
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

end

