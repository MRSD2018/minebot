function [ contactPoint ] = intersection( gantry_pos, probe_pos, mine_pos, mine_d, mine_h)
%INTERSECTION Summary of this function goes here
%   Detailed explanation goes here

contactPoint = [];

probeDist = sqrt( (probe_pos(1)-mine_pos(1))^2 + (probe_pos(2)-mine_pos(2))^2 ); % projected to X/Y plane
if ( probeDist < mine_d/2 && ( probe_pos(3) >= mine_pos(3) && probe_pos(3) <= (mine_pos(3)+mine_h)) )
   
%     disp('intersection!')
    
    numPoints = 1000;
    xPoints = linspace(gantry_pos(1), probe_pos(1), numPoints);
    yPoints = linspace(gantry_pos(2), probe_pos(2), numPoints);
    zPoints = linspace(gantry_pos(3), probe_pos(3), numPoints);
    
    i = numPoints;
    error = -1;
    while (error < 0)
        
        probeDist = sqrt( (xPoints(i)-mine_pos(1))^2 + (yPoints(i)-mine_pos(2))^2 ); % projected to X/Y plane
        error = probeDist - mine_d/2;
        i = i-1;
    end
    
    contactPoint = [xPoints(i); yPoints(i); zPoints(i)];
end

end

