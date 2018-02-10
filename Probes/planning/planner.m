clear all; close all; clc;

%% generate target 
% PARAMS
mine.d = 10;
mine.pos = [0 0 -15];
mine.h = 5;
% END PARAMS

%% render
f = figure; movegui(f,'northwest');
axis equal; grid on; hold on; 

az = 45; el = 45;
view(az, el);

[X,Y,Z] = cylinder(mine.d/2,30);
X = X + mine.pos(1);
Y = Y + mine.pos(2);
Z = Z*mine.h + mine.pos(3);

% surf(X,Y,Z,'FaceAlpha',0.25)
% plot3([0 0], [0 5],[0 0], 'r-', 'LineWidth', 2);  %x-axis
% plot3([0 5], [0 0],[0 0], 'g-', 'LineWidth', 2);  %y-axis

%% simulate metal detector input
md.u_x = mine.pos(1);
md.u_y = mine.pos(2); 
md.sigma_x = mine.d; % Not so sure about this?
md.sigma_y = md.sigma_x;

plot3(md.u_x, md.u_x, 0, 'gx', 'MarkerSize', 20);
rectangle('Position',[md.u_x-md.sigma_x md.u_y-md.sigma_y 2*md.sigma_x 2*md.sigma_y],'Curvature',[1 1],'EdgeColor','g')

%% gantry
% PARAMS
gantry.pos = [0; 0; 0];
% END PARAMS

%% probe
% PARAMS
probe.angle = 30; % deg
probe.length = 50;
% END PARAMS

%%%%%%%%%%%%%%%%
%% SIMULATION %%
%%%%%%%%%%%%%%%%

%% INITIAL SEARCH

% search parameters
SearchSF = 1.5;
maxForwardSearch = cosd(probe.angle)*mine.h*(1/SearchSF);

% set position/yaw
probe.pos = [md.u_x; md.u_y - md.sigma_y*SearchSF; -15];
gantry.yaw = 0;

i = 1; probeSequence;

%% INFO MAX SERACH

% find extrema of possible mine centres
th = linspace( pi/2, -pi/2, 3);
x = mine.d/2*sin(th) + contactPoint(1);
y = mine.d/2*cos(th) + contactPoint(2);
z = zeros(1,length(x)) + contactPoint(3);
plot3(x,y,z,'bx');

if mine.pos(2) <= 0
    worst_case.x = x(1); % go for right point
    worst_case.y = y(1);
    gantry.yaw = 45;
else
    worst_case.x = x(3); % go for left point
    worst_case.y = y(3);
    gantry.yaw = -45; 
end

probe.pos = [ worst_case.x + sind(gantry.yaw) * mine.d/2*SearchSF; ...
              worst_case.y - cosd(gantry.yaw) * mine.d/2*SearchSF; ...
              -15];
probeSequence; % perform probe!

%% Begin hough-based search







