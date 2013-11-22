% function [h, h_filtered, h_dir, h_dir_filtered] = plot_ring()
function plot_ring()
    Px = 230/2;
    Py = 120/2;
    Gx = 10;
    Gy = 50;
    
    arc_radius   = 35;
    arc_center_x = Px - arc_radius;
    arc_center_y = Py - arc_radius;
    
    
    
    hold on
    
    %% Plot the straight lines of the ring
    plot([          -Px               -Px],...
         [Py-arc_radius    -Py+arc_radius],'-')
    plot([-Px+arc_radius Px-arc_radius],...
         [            Py            Py],'-')
    plot([           Px             Px],...
         [Py-arc_radius -Py+arc_radius],'-')
    plot([Px-arc_radius -Px+arc_radius],...
         [          -Py            -Py],'-')
    
    %% Plot the arcs at the corner
    plot_arc( pi/2,    pi, -arc_center_x,   arc_center_y, arc_radius)
    plot_arc(    0,  pi/2,  arc_center_x,   arc_center_y, arc_radius)
    plot_arc(    0, -pi/2,  arc_center_x,  -arc_center_y, arc_radius)
    plot_arc(-pi/2,   -pi, -arc_center_x,  -arc_center_y, arc_radius)
    
    
    %% Plot the goals
    % Goal A
    plot([             -Px              -Px-Gx],...
         [   Py-arc_radius       Py-arc_radius],'-')
    plot([          -Px-Gx              -Px-Gx],...
         [   Py-arc_radius    Py-Gy-arc_radius],'-')
    plot([          -Px-Gx                 -Px],...
         [Py-Gy-arc_radius    Py-Gy-arc_radius],'-') 
     
    % Goal B
    plot([              Px               Px+Gx],...
         [   Py-arc_radius       Py-arc_radius],'-')
    plot([           Px+Gx               Px+Gx],...
         [   Py-arc_radius    Py-Gy-arc_radius],'-')
    plot([           Px+Gx                  Px],...
         [Py-Gy-arc_radius    Py-Gy-arc_radius],'-') 
%     plot([-Px+arc_radius Px-arc_radius],...
%          [            Py            Py],'-')
%     plot([           Px             Px],...
%          [Py-arc_radius -Py+arc_radius],'-')
%     plot([Px-arc_radius -Px+arc_radius],...
%          [          -Py            -Py],'-')
    
    
    %% Plot the laser at the center ot the ring
    plot(0,0,'r*')
    

    
    axis equal
    
end