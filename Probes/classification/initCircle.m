function [ center_x,center_y ] = initCircle( points )
%FIRSTGUESS Summary of this function goes here
%   Detailed explanation goes here
sigX = 0;
sigY = 0;
q = 0;

n = length(points.x);

% eps = 1e-10;

for i = 1:n-2
    for j = i+1:n-1
        for k = j+1:n
            %             if abs(delta)>eps
            [cc_x,cc_y]=circumcenter(points.x(i),points.y(i),points.x(j),points.y(j),points.x(k),points.y(k));
            sigX = sigX + cc_x;
            sigY = sigY + cc_y;
            q = q+1;
            %             end
        end
    end
end
if q==0
    disp('All points aligned')
end
center = [sigX/q, sigY/q];
center_x = center(1);
center_y = center(2);
end

