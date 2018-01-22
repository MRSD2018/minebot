function [ J,dJdx,dJdy ] = computeCost( points,center,rHat )
%COMPUTECOST Summary of this function goes here
%   Detailed explanation goes here

J    = 0;
dJdx = 0;
dJdy = 0;

for i = 1:length(points.x)
    dx = points.x(i) - center.x;
    dy = points.y(i) - center.y;
    di = sqrt(dx ^2 + dy ^2);
    dr    = di - rHat;
    ratio = dr / di;
    J    = J + dr * (di + rHat);
    dJdx = dJdx+ dx * ratio;
    dJdy = dJdy+ dy * ratio;
end
dJdx = dJdx*2.0;
dJdy = dJdy*2.0;
end