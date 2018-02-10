
search = [ - sind(gantry.yaw)*maxForwardSearch; ...
   + cosd(gantry.yaw)*maxForwardSearch; ...
   0];

gantry.pos = gantryPosition( probe.pos, probe.angle, probe.length, gantry.yaw );
plot3([gantry.pos(1) probe.pos(1)], [gantry.pos(2) probe.pos(2)], [gantry.pos(3) probe.pos(3)], 'r-');

%% begin loop
contactPoint = [];
while(isempty(contactPoint))
   
    %% compute gantry pos
    gantry.pos = gantryPosition( probe.pos, probe.angle, probe.length, gantry.yaw );

    %% find contact point
    contactPoint = intersection( gantry.pos, probe.pos, mine.pos, mine.d, mine.h);
    
    %% render
    draw( gantry.pos, probe.pos, contactPoint,  i );

    %% update gantry position
    probe.pos = probe.pos + search;
    gantry.pos = gantryPosition( probe.pos, probe.angle, probe.length, gantry.yaw );

    i = i + 1;
end

children = get(gca, 'children');
delete(children(1));