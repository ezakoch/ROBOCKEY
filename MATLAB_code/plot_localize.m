%% Localize function
    function plot_localize(src,evnt)
    
    y0_offset = 0;
x0_offset = 0;

x0 = 1024/2 - x0_offset;
y0 = 768/2 - y0_offset;

%     figure;
% hold on
% h = plot(1024/2,768/2,'ro');
% hold off
% xlim([-x0 x0])
% ylim([-y0 y0])
% set(gca,'YDir','reverse');
    
    
    global userdata
    
        clc
        x0 = 1024/2;
        y0 = 768/2;
        
        fig4 = figure(4);
        hold on
        stars = plot(0,0,'ro');
        hold off
        xlim([-x0 x0])
        ylim([-y0 y0])
        set(gca,'YDir','reverse');
        

%         figure;
%         hold on
%         userdata.h_yaw = plot(0,0,'b-');
%         hold off
%         xlabel('time  (sec)');
%         ylabel('yaw  (radians)');
%         legend('Yaw');
        
%         [userdata.hc, userdata.h_dir] = plot_ring;
        
        % Infinite loop to plot the data
        i=1;
        userdata.flag_plot_sensors = 1;
        userdata.x_robot = [];
        userdata.y_robot = [];
        userdata.u0 = [];
        userdata.v0 = [];
        
        tic
        while (userdata.flag_plot_sensors)
            % ASK ROBOT 1 FOR EACH VARIABLES
            fwrite(userdata.handle, 9);
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            while(nBytes==0)
                nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            end
            
                
                % Grab data from M2 until line terminator
                % Plot the data
                % Same for all below
                data = fgetl(userdata.handle);
                userdata.x1(i) = str2double(data);
              
                data = fgetl(userdata.handle);
                userdata.x2(i) = str2double(data);
                
                data = fgetl(userdata.handle);
                userdata.x3(i) = str2double(data);
                
                data = fgetl(userdata.handle);
                userdata.x4(i) = str2double(data);
                
                data = fgetl(userdata.handle);
                userdata.y1(i) = str2double(data);
                
                data = fgetl(userdata.handle);
                userdata.y2(i) = str2double(data);
                
                data = fgetl(userdata.handle);
                userdata.y3(i) = str2double(data);
                
                data = fgetl(userdata.handle);
                userdata.y4(i) = str2double(data);
                
%                 userdata.x1(i)
                
%                 Localization(userdata.x1(i), userdata.x2(i), userdata.x3(i), userdata.x4(i),...
%                            userdata.y1(i), userdata.y2(i), userdata.y3(i), userdata.y4(i));
                
        Q1 = [userdata.x1(i)-x0 userdata.y1(i)-y0]
        Q2 = [userdata.x2(i)-x0 userdata.y2(i)-y0]
        Q3 = [userdata.x3(i)-x0 userdata.y3(i)-y0]
        Q4 = [userdata.x4(i)-x0 userdata.y4(i)-y0]

        set( stars, 'xdata', [Q1(1) Q2(1) Q3(1) Q4(1)],'ydata',[Q1(2) Q2(2) Q3(2) Q4(2)]);
    drawnow;
                i=i+1;

        end
    end


