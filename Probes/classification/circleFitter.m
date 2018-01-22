clc
clear
close all

r = 1;

c.x = .0;
c.y = -0;

[ points ] = generateRandomPts( r,c.x,c.y );
[ center_x,center_y ] = initCircle( points );

figure
plot(points.x,points.y,'.','MarkerSize',20)
hold on
plot(center_x,center_y,'.k','MarkerSize',20)
plot(0,0,'.g','MarkerSize',20)
rectangle('Position',[c.x-r c.y-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','r')
axis equal