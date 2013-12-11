function Localization(x1, x2, x3, x4, y1, y2, y3, y4)

global userdata


y0_offset = 0;
x0_offset = 0; %   18.0224
x0 = 1024/2 - x0_offset;
y0 = 768/2 - y0_offset;


P0 = [0 0];
P1 = [0 14.5];
P2 = [11.655 8.741];
P3 = [0 -14.5];
P4 = [-10.563 2.483];
P_vert = norm(P3-P1); 


% QA = [492-x0 330-y0];
% QB = [473-x0 368-y0];
% QC = [567-x0 367-y0];
% QD = [513-x0 402-y0];

QA = [x1-x0 y1-y0]
QB = [x2-x0 y2-y0]
QC = [x3-x0 y3-y0]
QD = [x4-x0 y4-y0]

AB = norm(QB-QA);
AC = norm(QC-QA);
AD = norm(QD-QA);
BC = norm(QC-QB);
BD = norm(QD-QB);
CD = norm(QD-QC);

[max_dist, id_max] = max([AB AC AD BC BD CD]);
[min_dist, id_min] = min([AB AC AD BC BD CD]);

switch id_max 
    case 6
        switch id_min
            case 2
                Q1=QC; Q2=QA; Q3=QD; Q4=QB;                
            case 3
                Q1=QD; Q2=QA; Q3=QC; Q4=QB;                
            case 4
                Q1=QC; Q2=QB; Q3=QD; Q4=QA;
            case 5
                Q1=QD; Q2=QB; Q3=QC; Q4=QA;
            otherwise
                Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                disp('Not good stars');
        end    
    case 1
        switch id_min
            case 2
                Q1=QA; Q2=QC; Q3=QB; Q4=QD;                
            case 3
                Q1=QA; Q2=QD; Q3=QB; Q4=QC;                
            case 4
                Q1=QB; Q2=QC; Q3=QA; Q4=QD;
            case 5
                Q1=QB; Q2=QD; Q3=QA; Q4=QC;
            otherwise
                Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                disp('Not good stars');
        end
    case 2
        switch id_min
            case 1
                Q1=QA; Q2=QB; Q3=QC; Q4=QD;                
            case 3
                Q1=QA; Q2=QD; Q3=QC; Q4=QB;                
            case 4
                Q1=QC; Q2=QB; Q3=QA; Q4=QD;
            case 6
                Q1=QC; Q2=QD; Q3=QA; Q4=QB;
            otherwise
                Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                disp('Not good stars');
        end
    case 3
        switch id_min
            case 1
                Q1=QA; Q2=QB; Q3=QD; Q4=QC;                
            case 2
                Q1=QA; Q2=QC; Q3=QD; Q4=QB;                
            case 5
                Q1=QD; Q2=QB; Q3=QA; Q4=QC;
            case 6
                Q1=QD; Q2=QC; Q3=QA; Q4=QB;
            otherwise
                Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                disp('Not good stars');
        end
    case 4
        switch id_min
            case 1
                Q1=QB; Q2=QA; Q3=QC; Q4=QD;                
            case 2
                Q1=QC; Q2=QA; Q3=QB; Q4=QD;                
            case 5
                Q1=QB; Q2=QD; Q3=QC; Q4=QA;
            case 6
                Q1=QC; Q2=QD; Q3=QB; Q4=QA;
            otherwise
                Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                disp('Not good stars');
        end
    case 5
        switch id_min
            case 1
                Q1=QB; Q2=QA; Q3=QD; Q4=QC;                
            case 3
                Q1=QD; Q2=QA; Q3=QB; Q4=QC;                
            case 4
                Q1=QB; Q2=QC; Q3=QD; Q4=QA;
            case 6
                Q1=QD; Q2=QC; Q3=QB; Q4=QA;
            otherwise
                Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                disp('Not good stars');
        end
    
end

px_to_cm = P_vert/max_dist;

S1 = Q1 * px_to_cm;
S2 = Q2 * px_to_cm;
S3 = Q3 * px_to_cm;
S4 = Q4 * px_to_cm;

S0 = (S1 + S3)/2;
r = norm(S0);

 theta = atan2( (Q3(2) - Q1(2)), (Q3(1) - Q1(1))  );
 alpha = -atan2(S0(1),S0(2));
% 
%  phi = theta - alpha;

% phi = -pi/2 + atan2(S0(1),S0(2));
% phi(i) = atan2( (Q3(2) - Q1(2)), (Q3(1) - Q1(1))  );



x = -r*cos(theta - alpha);
y = -r*sin(theta - alpha);

userdata.x_robot(end+1) = x;
userdata.y_robot(end+1) = y;
userdata.u0(end+1) = x/px_to_cm;
userdata.v0(end+1) = y/px_to_cm;

% x = S0(1);
% y = S0(2);


% KK = [1 0  0;
%       0 1  10;
%       0 0  1];
%   o = [P0' P1' P2' P3' P4'; 1 1 1 1 1];
%   temp = K*KK*o;
%   temp(:,1) = temp(:,1)/temp(3,1);
%   temp(:,2) = temp(:,2)/temp(3,2);
%   temp(:,3) = temp(:,3)/temp(3,3);
%   temp(:,4) = temp(:,4)/temp(3,4);
%   
%   temp
%   
%   test = [512; 385.9140; 1];
% 
%   test2 = [837; 444; 1];
% 
% 
%      [x(i),y(i), dirx,diry] = the_NAMEEN_deal(uA,uB,uC,uD,vA,vB,vC,vD);
%     scale = 200;
%     dir0 = [0;1;0]*scale;
%     if (vC < vA)
%         roll(i)  = sol1(1,1);
%         pitch(i) = sol1(2,1);
%         yaw(i)   = sol1(3,1);
%         dir = Rotz(yaw(i))*dir0;
%      else
%         roll(i)  = sol2(1,1);
%         pitch(i) = sol2(2,1);
%         yaw(i)   = sol2(3,1);
%         dir = Rotz(yaw(i))*dir0;
%      end
%     dir = Rotz(yaw(i))*dir0;
%     set(h_dir,'xdata',[uD dir(1)+uD], 'ydata', [vD dir(2)+vD]);
set(userdata.h_plot, 'ydata', [Q1(1) Q2(1) Q3(1) Q4(1)],'xdata',[Q1(2) Q2(2) Q3(2) Q4(2)]);

    %set(h_dir,'ydata',[uD diry*scale+uD], 'xdata', [vD dirx*scale+vD]);
%     set(hc,'xdata',S0(1), 'ydata', S0(2));
% set(userdata.hc,'xdata',x, 'ydata', y);

scale = 20;
% set(userdata.h_dir,'xdata',[x sin(theta)*scale+x], 'ydata', [y cos(theta)*scale+y]);
     %set(h_roll,'xdata',1:i, 'ydata', phi(1:i));
     
%     set(h_pitch,'xdata',1:i, 'ydata', pitch(1:i));
%     set(h_yaw,'xdata',1:i, 'ydata', yaw(1:i));

    
    
    drawnow
end

