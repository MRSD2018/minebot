function [ rHat ] = updateRadius( points,center )
rHat = 0;
for i = 1:length(points.x)
    dx = points.x(i) - center.x;
    dy = points.y(i) - center.y;
    rHat = rHat + sqrt(dx^2 + dy^2);
end
rHat = rHat/ length(points.x);
end

