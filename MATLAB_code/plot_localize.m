%% Localize function
    function localize(src,evnt)
        clc
        x0 = 1024/2;
        y0 = 768/2;
        
        figure;
        hold on
        userdata.h_plot = plot(x0,y0,'ro');
        hold off
        xlim([-x0 x0])
        ylim([-y0 y0])
        set(gca,'YDir','reverse');
        

        figure;
        hold on
        userdata.h_yaw = plot(0,0,'b-');
        hold off
        xlabel('time  (sec)');
        ylabel('yaw  (radians)');
        legend('Yaw');
        
        [userdata.hc, userdata.h_dir] = plot_ring;
        
        % Infinite loop to plot the data
        i=1;
        userdata.flag_plot_sensors = 1;
        userdata.x_robot = [];
        userdata.y_robot = [];
        userdata.u0 = [];
        userdata.v0 = [];
        
        tic
        while (userdata.flag_plot_sensors)
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            if(nBytes~=0)
                userdata.time(i) = toc; % Grab the time passed
                
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
                
                Localization(userdata.x1(i), userdata.x2(i), userdata.x3(i), userdata.x4(i),...
                             userdata.y1(i), userdata.y2(i), userdata.y3(i), userdata.y4(i));
                
                i=i+1;
            else
                % Send M2 the instruction to send sensors data
                fwrite(userdata.handle, 4);
            end
        end
    end


