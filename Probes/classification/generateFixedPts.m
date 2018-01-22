function [ points ] = generateFixedPts( r,cx,cy )
%GENERATERANDOMPTS Summary of this function goes here
%   Detailed explanation goes here
npt = 8;

th = linspace(0,360,npt+1);
dev_dist = r/10;

angles = th(1:npt);

pert = 0.5*dev_dist;

points.x = cx+r.*cosd(angles)+pert.*cosd(angles);
points.y = cy+r.*sind(angles)+pert.*cosd(angles);

end

