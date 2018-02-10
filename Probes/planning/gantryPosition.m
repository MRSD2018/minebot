function [ gantry ] = gantryPosition( probe_pos, probe_angle, probe_length, gantry_yaw )

    gantry = [0; 0; 0];
    
    % compute gantry pos
    Hprobe = [1 0 0 probe_pos(1);
              0 1 0 probe_pos(2);
              0 0 1 probe_pos(3);
              0 0 0 1];
    
    Hyaw = [cosd(gantry_yaw), -sind(gantry_yaw), 0, 0;
            sind(gantry_yaw), cosd(gantry_yaw),  0, 0;
            0, 0, 1, 0;
            0, 0, 0, 1];

    Hxrot = [1 0 0 0;
             0 cosd(-probe_angle) cosd(-probe_angle) 0;
             0 sind(-probe_angle) cosd(-probe_angle) 0;
             0 0 0 1];

    Hd = [1 0 0 0;
          0 1 0 -probe_length;
          0 0 1 0;
          0 0 0 1];

    H = Hprobe*Hyaw*Hxrot*Hd*[0; 0; 0; 1];
    gantry = H(1:3);

end

