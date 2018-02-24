% hough fit
est = houghCircle( contactPoints, mine.d/2 );
if (exist('houghViz'))
    delete(houghViz)
end
houghViz(1) = plot(est.x,est.y,'.m','MarkerSize',30);
houghViz(2) = rectangle('Position',[est.x-mine.d/2 est.y-mine.d/2 2*mine.d/2 2*mine.d/2],'Curvature',[1 1],'EdgeColor','m');
% error
circleFitError = fitError( contactPoints, est, mine.d/2 )/length(contactPoints) % normalized!
