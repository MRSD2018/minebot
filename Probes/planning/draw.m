function draw( gantry_pos, probe_pos, contactPoint,  i )
%DRAW Summary of this function goes here
%   Detailed explanation goes here
    %% Rendering
    children = get(gca, 'children');
    delete(children(1));
    
    title(sprintf('probe iteration %d',i));
    if (~isempty(contactPoint))
        plot3(contactPoint(1),contactPoint(2),contactPoint(3),'r.', 'MarkerSize',25);
    end

    plot3([gantry_pos(1) probe_pos(1)], ...
          [gantry_pos(2) probe_pos(2)], ...
          [gantry_pos(3) probe_pos(3)], 'r-')
      
    pause(0.5)

end

