clear
close all
clc

trans = @(x,y,z)([   1   0   0   x;
    0   1   0   y;
    0   0   1   z;
    0   0   0   1]);

% rotation about X
rotX = @(rx)([   1   0         0        0;
    0   cosd(rx)   -sind(rx) 0;
    0   sind(rx)   cosd(rx)  0;
    0   0         0        1]);

% rotation about Y
rotY = @(ry)([   cosd(ry)    0   sind(ry) 0;
    0          1   0       0;
    -sind(ry)   0   cosd(ry) 0;
    0          0   0       1]);

% rotation about Z
rotZ = @(rz)([   cosd(rz)    -sind(rz)    0 0;
    sind(rz)         cosd(rz)   0       0;
    0    0 1 0;
    0          0   0       1]);

% ext = 0.4;
d1 = 0.05;
d2 = 0.05;
d3 = 0.15;
d4 = 0.15;
% dx = 0.25;
% dy = 0.32;
% theta = 0;%30*pi/180;
% t=[0.2 0.3 -0.1]';
probing_angle = 30;

width = 1;
depth = .5;
head_rotation = 360;
probe_extension = .4;

x = linspace(0,width,11);
y = linspace(0,depth,6);
theta = linspace(0,head_rotation,19);
w = linspace(0,probe_extension, 5);

% x = linspace(0,width,3);
% y = linspace(0,depth,3);
% theta = linspace(0,0,1);
% w = linspace(0,0, 1);

comb = allcomb(theta,x,y,w);

% w = -t(3)/sind(probing_angle);
% offset = w*cosd(probing_angle);

% target in gantry frame
% Hgt = trans(-0.3,1.2,-.55); % say -0.5 for height above ground, and -0.1 for depth

% probe in shaft frame
% Hspe = trans(ext+d1,-d2, 0); % extended
% Hspr = trans(d1,-d2, 0); % retracted

% shaft in head frame
Hhs = trans(0,-d3,-d4)*rotX(90-probing_angle)*rotY(90);




% Hht = Hgh\Hgt;
%
% % probe tip in gantry frame
% Hgp = Hgh*Hhs*Hspr;
%
% % probe tip in head frame
% Hhp = Hhs*Hspr;

% gantry in cart frame
Hcg = trans(0,0,.25);


% head in gantry frame
% Hgh = zeros(4,4,length(comb));
for i = 1:size(comb,1)
    Hgh = trans(comb(i,2),comb(i,3),0)*rotZ(comb(i,1));
    Hsp = trans(comb(i,4)+d1,-d2, 0);
    Hhp = Hhs*Hsp;
    Hgp = Hgh*Hhp;
    Hcp(:,:,i) = Hcg*Hgp;
end

points = Hcp(1:3,4,:);
points2 = reshape(points,3,size(comb,1));%,3);
ax = plot3(points2(1,:),points2(2,:),points2(3,:),'.','markers',12);
% axis(ax,'equal')% % target in head frame
set(gca,'DataAspectRatio',[1 1 1])
xlabel('x')
ylabel('y')
zlabel('depth')
grid on
rectangle('Position',[0 0 width depth],'LineWidth',3)