clc
clear
close all

r = 1;

c.x = .0;
c.y = -0;

npt = 4;
[ points ] = generateRandomPts( r,c.x,c.y,npt );
% [ points ] = generateFixedPts( r,c.x,c.y );
[ center_x,center_y ] = initCircle( points );

figure
plot(points.x,points.y,'.','MarkerSize',20)
hold on
plot(center_x,center_y,'.k','MarkerSize',20)
plot(c.x,c.y,'.c','MarkerSize',20)
rectangle('Position',[c.x-r c.y-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','c','LineStyle','--')
axis equal
grid on

ccalc.x = center_x;
ccalc.y = center_y;
% eps1 = 1e-5;
% eps2 = 1e-5;

epsinner = .1;
epsouter = 1e-10;

maxIter = 100;

% compute rHat (eq 5)
[ rHat ] = updateRadius( points,ccalc );
% compute J (eq 6)
% compute gradJ (eq 7)
[ J,dJdx,dJdy ] = computeCost( points,ccalc,rHat );

% if abs(J) < eps1 || sqrt(dJdx^2+dJdy^2)<eps2
%     break
% end

Jprev = J;
uprev = 0;
vprev = 0;
prevDJdx = dJdx; %dJdx or 0 (in code)
prevDJdy = dJdy; %dJdy or 0 (in code)
i = 1; % iterations

for i = 1:maxIter
    u = -dJdx;
    v = -dJdy;
    if i>1
        beta =(dJdx * (dJdx - prevDJdx) + dJdy * (dJdy - prevDJdy))/ (prevDJdx^2 + prevDJdy^2);
        u = u+beta*uprev;
        v = v+beta*vprev;
    end
    prevDJdx = dJdx;
    prevDJdy = dJdy;
    uprev    = u;
    vprev    = v;
    if i==1
        Jinner = J*10; % to make sure it enters inner loop
    end
    while (i<=maxIter && (abs(J-Jinner)/J)>epsinner)
        Jinner = J;
        lambda = newtonStep(u,v,rHat,points,ccalc);
        ccalc.x = ccalc.x+lambda*u;
        ccalc.y = ccalc.y+lambda*v;
        rHat = updateRadius(points,ccalc);
        [ J,dJdx,dJdy ] = computeCost( points,ccalc,rHat );
    end
%     fprintf('abs(J-Jprev)/J is %f\n',abs(J-Jprev)/J);
    if(abs(J-Jprev)/J)<epsouter
        break
    end
    Jprev = J;
    i = i+1;
end

plot(ccalc.x,ccalc.y,'.m','MarkerSize',20)
rectangle('Position',[ccalc.x-rHat ccalc.y-rHat 2*rHat 2*rHat],'Curvature',[1 1],'EdgeColor','m')

% fprintf('')
