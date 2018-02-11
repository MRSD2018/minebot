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

surf(X,Y,Z,'FaceAlpha',0.25)
plot3([0 0], [0 3],[0 0], 'g-', 'LineWidth', 2);  %x-axis
plot3([0 3], [0 0],[0 0], 'r-', 'LineWidth', 2);  %y-axis

%% simulate metal detector input
md.sigma_x = mine.d/5; % Not so sure about this?
md.sigma_y = md.sigma_x;
md.u_x = normrnd(mine.pos(1),md.sigma_x);
md.u_y = normrnd(mine.pos(2),md.sigma_y);
plot3(md.u_x, md.u_x, 0, 'gx', 'MarkerSize', 20);

%% gantry
% PARAMS
gantry.pos = [0; 0; 0];
% END PARAMS

%% probe
% PARAMS
probe.angle = 30; % deg
probe.length = 20;
% END PARAMS

%%%%%%%%%%%%%%%%
%% SIMULATION %%
%%%%%%%%%%%%%%%%

% search parameters
SearchSF = 1;
maxForwardSearch = cosd(probe.angle)*mine.h*(1/SearchSF);
angleRange = 130; % degrees
noiseFactor = 0.5;

%% INITIAL SEARCH
% set position/yaw
probe.pos = [md.u_x; md.u_y - mine.d*SearchSF; -15];
gantry.yaw = 0;

contactPoints.x = []; contactPoints.y = []; contactPoints.z = [];
i = 1; 
probeSequence;

%% INFO MAX SEARCH
% find extrema of possible mine centres
th = linspace( pi/2, -pi/2, 3);
x = mine.d/2*sin(th) + contact(1);
y = mine.d/2*cos(th) + contact(2);
z = zeros(1,length(x)) + contact(3);
% plot3(x,y,z,'bx');

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

%% BEGIN HOUGH BASED SEARCH
az = 0; el = 90; % change view
view(az, el);

houghCalculation;

while(length(contactPoints.x) < 5 || circleFitError > 2)

    getHoughProbe;
    probe.pos = [cosd(newAngle)*mine.d/2*0.9; sind(newAngle)*mine.d/2*0.9; mean(contactPoints.z)];
    gantry.yaw = newAngle + 90;
    probeSequence; % perform probe!
    
    houghCalculation;
    
end

