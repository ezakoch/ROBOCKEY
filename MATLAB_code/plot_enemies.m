function plot_enemies(src,evnt)

global userdata

%% PLOT ENEMIES function

%         x0 = 1024/2;
%         y0 = 768/2;
%         
%         figure;
%         hold on
%         userdata.h_plot = plot(x0,y0,'ro');
%         hold off
%         xlim([-x0 x0])
%         ylim([-y0 y0])
%         set(gca,'YDir','reverse');
%         
% 
%         figure;
%         hold on
%         userdata.h_yaw = plot(0,0,'b-');
%         hold off
%         xlabel('time  (sec)');
%         ylabel('yaw  (radians)');
%         legend('Yaw');
        
   
figure(2);
hold on
        plot_ring;
         %% Plot the robot at the center of the ring
    userdata.enemies.robot1.plot = plot(0,0,'rx','MarkerSize',8);
    userdata.enemies.robot2.plot = plot(0,0,'go','MarkerSize',8);
    userdata.enemies.robot3.plot = plot(0,0,'bo','MarkerSize',8);
    
    hold off
    %xlim([-230/2-10 230/2+10])
    xlim([-140 140])
    ylim([-80 80])
    legend([userdata.enemies.robot1.plot userdata.enemies.robot2.plot userdata.enemies.robot3.plot],...
            'Enemy 1','Enemy 2', 'Enemy 3')
    xlabel('x  (mm)')
    ylabel('y  (mm)')
        % Infinite loop to plot the data
        i=1;
        userdata.flag_plot_sensors = 1;
        userdata.enemies.robot1.x = [];
        userdata.enemies.robot1.y = [];
        userdata.enemies.robot2.x = [];
        userdata.enemies.robot2.y = [];
        userdata.enemies.robot3.x = [];
        userdata.enemies.robot3.y = [];
        

        while (userdata.flag_plot_sensors)
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            if(nBytes~=0)
                
                % Grab data from M2 until line terminator
                % Plot the data
                % Same for all below
                data = fgetl(userdata.handle);
                userdata.enemies.robot1.x(i) = str2double(data);
                data = fgetl(userdata.handle);
                userdata.enemies.robot1.y(i) = str2double(data);
                set(userdata.enemies.robot1.plot,...
                    'xdata',userdata.enemies.robot1.x(i),...
                    'ydata',userdata.enemies.robot1.y(i))
                
                
                data = fgetl(userdata.handle);
                userdata.enemies.robot2.x(i) = str2double(data);
                data = fgetl(userdata.handle);
                userdata.enemies.robot2.y(i) = str2double(data);
                set(userdata.enemies.robot2.plot,...
                    'xdata',userdata.enemies.robot2.x(i),...
                    'ydata',userdata.enemies.robot2.y(i))
                
                
                data = fgetl(userdata.handle);
                userdata.enemies.robot3.x(i) = str2double(data);
                data = fgetl(userdata.handle);
                userdata.enemies.robot3.y(i) = str2double(data);
                set(userdata.enemies.robot3.plot,...
                    'xdata',userdata.enemies.robot3.x(i),...
                    'ydata',userdata.enemies.robot3.y(i))
                
                i=i+1;
            else
                % Send M2 the instruction to send enemies data
                fwrite(userdata.handle, 4);
            end
        end
    end



