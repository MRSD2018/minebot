
search = [ - sind(gantry.yaw)*maxForwardSearch; ...
   + cosd(gantry.yaw)*maxForwardSearch; ...
   0];

gantry.pos = gantryPosition( probe.pos, probe.angle, probe.length, gantry.yaw );
plot3([gantry.pos(1) probe.pos(1)], [gantry.pos(2) probe.pos(2)], [gantry.pos(3) probe.pos(3)], 'r-');

%% begin loop
contact = [];
while(isempty(contact))
   
    %% compute gantry pos
    gantry.pos = gantryPosition( probe.pos, probe.angle, probe.length, gantry.yaw );

    %% find contact point
    contact = intersection( gantry.pos, probe.pos, mine.pos, mine.d, mine.h);
    contact = normrnd(contact,noiseFactor);
    
    %% render
    draw( f, n, gantry.pos, probe.pos, contact,  i );
    n = n + 1;
    
    %% update gantry position
    probe.pos = probe.pos + search;
    gantry.pos = gantryPosition( probe.pos, probe.angle, probe.length, gantry.yaw );

    i = i + 1;
end

contactPoints.x = [contactPoints.x contact(1)];
contactPoints.y = [contactPoints.y contact(2)];
contactPoints.z = [contactPoints.z contact(3)];

children = get(gca, 'children');
delete(children(1));