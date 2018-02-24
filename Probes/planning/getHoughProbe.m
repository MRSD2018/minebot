    % find sparsest point in yaw angle range
    angle = [];
    for j = 1:length (contactPoints.x)
        angle(j) = atan2d(contactPoints.y(j) - est.y,contactPoints.x(j) - est.x);
    end
    angleSearchRange = (0:-1:-angleRange) - (180 - angleRange)/2;
    angle = sort(angle);
    angle = [min(angleSearchRange) angle  max(angleSearchRange)];
    for j = 1:length(angle)-1
        dist(j) = abs(angle(j) - angle(j+1));
    end
    [M,I] = max(dist);
    if I == 1
        newAngle = min(angleSearchRange);
    elseif I == length(dist)
        newAngle = max(angleSearchRange);
    else
        newAngle = angle(I) + dist(I)/2;
    end