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
        
   
fig2 = figure(2);
set(fig2,'Position',[0 400 750 600])
hold on
        plot_ring;
         %% Plot the robot at the center of the ring
         userdata.team.robot1.plot = plot(0,0,'ro','MarkerSize',8);
         userdata.team.robot1.plot_dir = plot(0,0,'r--','MarkerSize',8);
         userdata.team.robot2.plot = plot(0,0,'go','MarkerSize',8);
         userdata.team.robot2.plot_dir = plot(0,0,'g--','MarkerSize',8);
         userdata.team.robot3.plot = plot(0,0,'bo','MarkerSize',8);
         userdata.team.robot3.plot_dir = plot(0,0,'b--','MarkerSize',8);
         
    userdata.enemies.robot1.plot = plot(0,0,'rx','MarkerSize',8);
    userdata.enemies.robot2.plot = plot(0,0,'gx','MarkerSize',8);
    userdata.enemies.robot3.plot = plot(0,0,'bx','MarkerSize',8);
    
    hold off
    %xlim([-230/2-10 230/2+10])
    xlim([-140 140])
    ylim([-80 80])
    legend([userdata.enemies.robot1.plot userdata.enemies.robot2.plot userdata.enemies.robot3.plot userdata.team.robot1.plot userdata.team.robot1.plot_dir userdata.team.robot2.plot userdata.team.robot2.plot_dir userdata.team.robot3.plot userdata.team.robot3.plot_dir],...
            'Enemy 1','Enemy 2', 'Enemy 3', 'Robot 1', 'Direction 1', 'Robot 2', 'Direction 2',  'Robot 3', 'Direction 3')
    xlabel('x  (mm)')
    ylabel('y  (mm)')
    
    fig3 = figure(3);
    set(fig3,'Position',[770 400 500 600])
    text_x = 10;
        text_y = 500;
        y_space = 40;
        text_size = 12;
        h.robot1.var0 = uicontrol('Style', 'text', 'String', 'ROBOT 1','Fontsize',text_size,...
            'Position', [180 text_y+y_space 80 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var1 = uicontrol('Style', 'text', 'String', 'Status GO_TO_GOAL','Fontsize',text_size,...
            'Position', [text_x text_y 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var2 = uicontrol('Style', 'text', 'String', '-','Fontsize',text_size,...
            'Position', [text_x text_y-y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var3 = uicontrol('Style', 'text', 'String', '-','Fontsize',text_size,...
            'Position', [text_x text_y-2*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var4 = uicontrol('Style', 'text', 'String', 'Angle to GOAL','Fontsize',text_size,...
            'Position', [text_x text_y-3*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var5 = uicontrol('Style', 'text', 'String', 'Distance to GOAL','Fontsize',text_size,...
            'Position', [text_x text_y-4*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var6 = uicontrol('Style', 'text', 'String', 'Orientation','Fontsize',text_size,...
            'Position', [text_x text_y-5*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var7 = uicontrol('Style', 'text', 'String', 'Commands Var','Fontsize',text_size,...
            'Position', [text_x text_y-6*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var8 = uicontrol('Style', 'text', 'String', 'Diff Theta','Fontsize',text_size,...
            'Position', [text_x text_y-7*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var9 = uicontrol('Style', 'text', 'String', '-','Fontsize',text_size,...
            'Position', [text_x text_y-8*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var10 = uicontrol('Style', 'text', 'String', '-','Fontsize',text_size,...
            'Position', [text_x text_y-9*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var11 = uicontrol('Style', 'text', 'String', '-','Fontsize',text_size,...
            'Position', [text_x text_y-10*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
        h.robot1.var12 = uicontrol('Style', 'text', 'String', '-','Fontsize',text_size,...
            'Position', [text_x text_y-11*y_space 180 20],'BackgroundColor',userdata.defaultBackground);
       
        
        
        
        text_x_value = 170;
        userdata.h.robot1.var1_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var2_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var3_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-2*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var4_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-3*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var5_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-4*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var6_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-5*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var7_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-6*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var8_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-7*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var9_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-8*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var10_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-9*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var11_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-10*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot1.var12_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-11*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
       
    text_x_value = 250;
        h.robot2.var0 = uicontrol('Style', 'text', 'String', 'ROBOT 2','Fontsize',text_size,...
            'Position', [260 text_y+y_space 80 20],'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var1_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var2_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var3_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-2*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var4_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-3*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var5_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-4*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var6_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-5*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var7_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-6*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var8_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-7*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var9_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-8*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var10_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-9*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var11_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-10*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot2.var12_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-11*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
       
        text_x_value = 330;
        h.robot3.var0 = uicontrol('Style', 'text', 'String', 'ROBOT 3','Fontsize',text_size,...
            'Position', [340 text_y+y_space 80 20],'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var1_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var2_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var3_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-2*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var4_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-3*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var5_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-4*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var6_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-5*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var7_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-6*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var8_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-7*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var9_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-8*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var10_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-9*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var11_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-10*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
        userdata.h.robot3.var12_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',text_size,...
           'Position', [text_x+text_x_value text_y-11*y_space 80 20], 'BackgroundColor',userdata.defaultBackground);
       
      
       
    
    
        % Infinite loop to plot the data
        i=1;
        userdata.flag_plot_sensors = 1;
        userdata.enemies.flag = 0;
        
        userdata.team.robot1.x = [];
        userdata.team.robot1.y = [];
        userdata.team.robot1.yaw = [];
        userdata.enemies.robot1.x = [];
        userdata.enemies.robot1.y = [];
        userdata.enemies.robot2.x = [];
        userdata.enemies.robot2.y = [];
        userdata.enemies.robot3.x = [];
        userdata.enemies.robot3.y = [];
        
        
        init_plot_sensors_robot1();
        init_plot_sensors_robot2();
        init_plot_sensors_robot3();
        
        
        tic
        while (userdata.flag_plot_sensors)
            userdata.time(i) = toc; % Grab the time passed
            
            % ASK ROBOT 1 FOR EACH VARIABLES
            fwrite(userdata.handle, 1);
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            while(nBytes==0)
                nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            end
            get_robot1_variables(i);  
            
            fwrite(userdata.handle, 4);
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            while(nBytes==0)
                nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            end
            get_robot1_ADC(i);
            
%             
%             % ASK ROBOT 2 FOR EACH VARIABLES
            fwrite(userdata.handle, 2);
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            while(nBytes==0)
                nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            end
            get_robot2_variables(i); 
%             
            fwrite(userdata.handle, 5);
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            while(nBytes==0)
                nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            end
            get_robot2_ADC(i);
%             
%             
            
            % ASK ROBOT 3 FOR EACH VARIABLES
            fwrite(userdata.handle, 3);
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            while(nBytes==0)
                nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            end
            get_robot3_variables(i); 
            
            fwrite(userdata.handle, 6);
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            while(nBytes==0)
                nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            end
            get_robot3_ADC(i);
            
%                 if (userdata.enemies.flag)
%                 % Grab data from M2 until line terminator
%                 % Plot the data
%                 % Same for all below
%                 data = fgetl(userdata.handle);
%                 userdata.enemies.robot1.x(i) = str2double(data);
%                 data = fgetl(userdata.handle);
%                 userdata.enemies.robot1.y(i) = str2double(data);
%                 set(userdata.enemies.robot1.plot,...
%                     'xdata',userdata.enemies.robot1.x(i),...
%                     'ydata',userdata.enemies.robot1.y(i))
%                 
%                 
%                 data = fgetl(userdata.handle);
%                 userdata.enemies.robot2.x(i) = str2double(data);
%                 data = fgetl(userdata.handle);
%                 userdata.enemies.robot2.y(i) = str2double(data);
%                 set(userdata.enemies.robot2.plot,...
%                     'xdata',userdata.enemies.robot2.x(i),...
%                     'ydata',userdata.enemies.robot2.y(i))
%                 
%                 
%                 data = fgetl(userdata.handle);
%                 userdata.enemies.robot3.x(i) = str2double(data);
%                 data = fgetl(userdata.handle);
%                 userdata.enemies.robot3.y(i) = str2double(data);
%                 set(userdata.enemies.robot3.plot,...
%                     'xdata',userdata.enemies.robot3.x(i),...
%                     'ydata',userdata.enemies.robot3.y(i))
%                
                
                drawnow;
                i = i+1;
               
        end
    end



