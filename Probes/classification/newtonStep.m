function [ stepLength ] = newtonStep( u,v,rHat,points,center )
%NEWTONSTEP Summary of this function goes here
%   Detailed explanation goes here

sum1 = 0;
sum2 = 0;
sumFac = 0;
sumFac2R = 0;
for i = 1:length(points.x)
    dx     = center.x - points.x(i);
    dy     = center.y - points.y(i);
    di     = sqrt(dx^2 + dy^2);
    coeff1 = (dx * u + dy * v) /  di;
    coeff2 = di - rHat;
    sum1         = sum1+ coeff1 * coeff2;
    sum2         = sum2+ coeff2 / di;
    sumFac       = sumFac+ coeff1;
    sumFac2R     = sumFac2R+ coeff1 ^2 / di;
end
stepLength = -sum1 / ((u^2 + v^2) * sum2 - sumFac ^2 / length(points.x)+ rHat * sumFac2R);

end

