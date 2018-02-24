function draw( f, n, gantry_pos, probe_pos, contactPoint,  i )
%DRAW Summary of this function goes here
%   Detailed explanation goes here
    %% Rendering
    children = get(gca, 'children');
    delete(children(1));
    
    title(sprintf('Probe Iteration #%d',i));
    if (~isempty(contactPoint))
        plot3(contactPoint(1),contactPoint(2),contactPoint(3),'r.', 'MarkerSize',25);
    end

    plot3([gantry_pos(1) probe_pos(1)], ...
          [gantry_pos(2) probe_pos(2)], ...
          [gantry_pos(3) probe_pos(3)], 'r-')
      
%     pause(0.75)
    
    % Capture the plot as an image 
    frame = getframe(f); 
    im = frame2im(frame); 
    [imind,cm] = rgb2ind(im,256); 
    % Write to the GIF File 
    filename = ['img ' num2str(n,'%d') '.png'];
    imwrite(imind,cm,filename,'png'); 


end

