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

%% compute centre
% [ center_x,center_y ] = initCircle( points );
% ccalc.x = center_x; ccalc.y = center_y;
% plot(ccalc.x,ccalc.y,'.m','MarkerSize',20)
% rectangle('Position',[ccalc.x-r ccalc.y-r 2*r 2*r],'Curvature',[1 1],'EdgeColor','m')
% 
%% circle estimator w/ unknown radius
% 
% radiusKnown = true;
% 
% % eps1 = 1e-5;
% % eps2 = 1e-5;
% epsinner = .1;
% epsouter = 1e-10;
% 
% maxIter = 100;
% 
% % compute rHat (eq 5)
% if radiusKnown 
%     rHat = r;
% else    
%     [ rHat ] = updateRadius( pointsTrue,ccalc );
% end
% % compute J (eq 6)
% % compute gradJ (eq 7)
% [ J,dJdx,dJdy ] = computeCost( pointsTrue,ccalc,rHat );
% 
% % if abs(J) < eps1 || sqrt(dJdx^2+dJdy^2)<eps2
% %     break
% % end
% 
% Jprev = J;
% uprev = 0;
% vprev = 0;
% prevDJdx = dJdx; %dJdx or 0 (in code)
% prevDJdy = dJdy; %dJdy or 0 (in code)
% i = 1; % iterations
% 
% for i = 1:maxIter
%     u = -dJdx;
%     v = -dJdy;
%     if i>1
%         beta =(dJdx * (dJdx - prevDJdx) + dJdy * (dJdy - prevDJdy))/ (prevDJdx^2 + prevDJdy^2);
%         u = u+beta*uprev;
%         v = v+beta*vprev;
%     end
%     prevDJdx = dJdx;
%     prevDJdy = dJdy;
%     uprev    = u;
%     vprev    = v;
%     if i==1
%         Jinner = J*10; % to make sure it enters inner loop
%     end
%     while (i<=maxIter && (abs(J-Jinner)/J)>epsinner)
%         Jinner = J;
%         lambda = newtonStep(u,v,rHat,pointsTrue,ccalc);
%         ccalc.x = ccalc.x+lambda*u;
%         ccalc.y = ccalc.y+lambda*v;
% %         if ~radiusKnown
%             rHat = updateRadius(pointsTrue,ccalc);
% %         end
%         [ J,dJdx,dJdy ] = computeCost( pointsTrue,ccalc,rHat );
%     end
% %     fprintf('abs(J-Jprev)/J is %f\n',abs(J-Jprev)/J);
%     if(abs(J-Jprev)/J)<epsouter
%         break
%     end
%     Jprev = J;
%     i = i+1;

% end

% %% plot results
% if viz
%     plot(ccalc.x,ccalc.y,'.g','MarkerSize',20)
%     rectangle('Position',[ccalc.x-rHat ccalc.y-rHat 2*rHat 2*rHat],'Curvature',[1 1],'EdgeColor','g')
% end
