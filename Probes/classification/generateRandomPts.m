function [ points ] = generateRandomPts( r,cx,cy,npt )
%GENERATERANDOMPTS Summary of this function goes here
%   Detailed explanation goes here

% th = 360;
th = linspace(-75,75,npt+1)';
dev_dist = r/3;

% angles = rand(npt,1)*th;
angles = th(1:npt);
% angles = th(1:npt)';

pert = rand(npt,1)*dev_dist;

points.x = cx+r.*cosd(angles)+pert.*cosd(angles);
points.y = cy+r.*sind(angles)+pert.*cosd(angles);


end

