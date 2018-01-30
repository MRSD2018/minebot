%% DEPRICATED

%% Spline

% x = points.x;
% y = points.y;
% xq = min(x):1:max(x);
% 
% yq = interp1(x,y,xq,'spline');
% plot(xq,yq,':.');
% axis equal
% grid on

%% Bezier
% p = [transpose(points.x) transpose(points.y)]
% for    i=0:1:n
% sigma(i+1)=factorial(n)/(factorial(i)*factorial(n-i));  % for calculating (x!/(y!(x-y)!)) values 
% end
% l=[];
% UB=[];
% for u=0:0.002:1
% for d=1:n
% UB(d)=sigma(d)*((1-u)^(n-d))*(u^(d-1));
% end
% l=cat(1,l,UB);                                      %catenation 
% end
% P=l*p;
% line(P(:,1),P(:,2))


%% hough
% img = full( sparse(round(points.x)+r*2,round(points.y)+r*2,true) );
% img = full( sparse(round(xq)+r*2,round(yq)+r*2,true) );
% figure
% imshow(img)
% hold on
% 
% h = hough(img);
% figure
% imshow(h)
% 
% [centers, radii, metric] = imfindcircles(img, r)
% viscircles(centers, radii,'EdgeColor','b');
% hold off