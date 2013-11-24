%% PLOT the SENSORS data
    function plot_sensors(src,evnt)
    
    global userdata
    
        % Initialiaze figures
        fig3 = figure(3);
        set(fig3,'Position',[0 0 1200 800])

        subplot(2,1,1)
        hold all
        
        userdata.ADC.plot_F0 = plot(0,0);
        userdata.ADC.plot_F1 = plot(0,0);
        userdata.ADC.plot_F4 = plot(0,0);
        userdata.ADC.plot_F5 = plot(0,0);
        userdata.ADC.plot_F6 = plot(0,0);
        userdata.ADC.plot_F7 = plot(0,0);
        userdata.ADC.plot_D4 = plot(0,0);
        userdata.ADC.plot_D6 = plot(0,0);
        
        title('Sensors')
        xlabel('Time (sec)')
        ylabel('ADC (0-1023)')
        legend([userdata.ADC.plot_F0 userdata.ADC.plot_F1 userdata.ADC.plot_F4 userdata.ADC.plot_F5 userdata.ADC.plot_F6 userdata.ADC.plot_F7 userdata.ADC.plot_D4 userdata.ADC.plot_D6],...
            'F0','F1','F4','F5','F6','F7','D4','D6','Location','NorthEastOutside')
        ylim([-100 1124])
        grid on
        hold off
        
        subplot(2,2,4)
        hold on
        plot_arc(60*pi/180,-240*pi/180,0,0,1)
        plot_arc(-25*pi/180,-155*pi/180,0,1.1,0.55)
        plot(-1,0,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(-1,0,'   F0','FontSize',12)
        plot(0,-1,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(0,-1,'   F1','FontSize',12)
        plot(1,0,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(1,0,'   F4','FontSize',12)
        plot(sqrt(2)/2,sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(sqrt(2)/2,sqrt(2)/2,'   F5','FontSize',12)
        plot(sqrt(2)/2,-sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(sqrt(2)/2,-sqrt(2)/2,'   F6','FontSize',12)
        plot(-sqrt(2)/2,sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(-sqrt(2)/2,sqrt(2)/2,'   F7','FontSize',12)
        plot(-sqrt(2)/2,-sqrt(2)/2,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(-sqrt(2)/2,-sqrt(2)/2,'   D4','FontSize',12)
        plot(0,0.55,'bs','MarkerSize',10,'MarkerFaceColor','b')
        text(0,0.55,'   D6','FontSize',12)
        hold off
        ylim([-1.5 1.5])
        ylim([-1.5 1.5])
        axis equal
        grid off
        
        
        
        text_x = 100;
        text_y = 40;
        h.F0 = uicontrol('Style', 'text', 'String', 'F0','Fontsize',18,...
            'Position', [text_x 8*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.F1 = uicontrol('Style', 'text', 'String', 'F1','Fontsize',18,...
            'Position', [text_x 7*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.F4 = uicontrol('Style', 'text', 'String', 'F4','Fontsize',18,...
            'Position', [text_x 6*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.F5 = uicontrol('Style', 'text', 'String', 'F5','Fontsize',18,...
            'Position', [text_x 5*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.F6 = uicontrol('Style', 'text', 'String', 'F6','Fontsize',18,...
            'Position', [text_x 4*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.F7 = uicontrol('Style', 'text', 'String', 'F7','Fontsize',18,...
            'Position', [text_x 3*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.D4 = uicontrol('Style', 'text', 'String', 'D4','Fontsize',18,...
            'Position', [text_x 2*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        h.D6 = uicontrol('Style', 'text', 'String', 'D6','Fontsize',18,...
            'Position', [text_x 1*text_y 80 20],'BackgroundColor',userdata.defaultBackground);
        
        text_x_value = 60;
        h.F0_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 8*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        h.F1_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 7*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        h.F4_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 6*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        h.F5_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 5*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        h.F6_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 4*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        h.F7_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 3*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        h.D4_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 2*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
        h.D6_value = uicontrol('Style', 'text', 'String', 'NaN', 'Fontsize',18,...
           'Position', [text_x+text_x_value 1*text_y 80 20], 'BackgroundColor',userdata.defaultBackground);
       
        
        % Infinite loop to plot the data
        i=1;
        userdata.flag_plot_sensors = 1;
        
        tic
        while (userdata.flag_plot_sensors)
            nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
            if(nBytes~=0)
                userdata.time(i) = toc; % Grab the time passed
                
                % Grab data from M2 until line terminator
                % Plot the data
                % Same for all below
                data = fgetl(userdata.handle);
                userdata.ADC.F0_data(i,1) = str2double(data);
                set(userdata.ADC.plot_F0,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.F0_data(1:i,1))
                
                data = fgetl(userdata.handle);
                userdata.ADC.F1_data(i,1) = str2double(data);
                set(userdata.ADC.plot_F1,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.F1_data(1:i,1))
                
               data = fgetl(userdata.handle);
                userdata.ADC.F4_data(i,1) = str2double(data);
                set(userdata.ADC.plot_F4,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.F4_data(1:i,1))
                
                data = fgetl(userdata.handle);
                userdata.ADC.F5_data(i,1) = str2double(data);
                set(userdata.ADC.plot_F5,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.F5_data(1:i,1))
                
                data = fgetl(userdata.handle);
                userdata.ADC.F6_data(i,1) = str2double(data);
                set(userdata.ADC.plot_F6,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.F6_data(1:i,1))
                
                data = fgetl(userdata.handle);
                userdata.ADC.F7_data(i,1) = str2double(data);
                set(userdata.ADC.plot_F7,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.F7_data(1:i,1))
                
                data = fgetl(userdata.handle);
                userdata.ADC.D4_data(i,1) = str2double(data);
                set(userdata.ADC.plot_F0,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.D4_data(1:i,1))
                
                data = fgetl(userdata.handle);
                userdata.ADC.D6_data(i,1) = str2double(data);
                set(userdata.ADC.plot_D6,... 
                    'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.ADC.D6_data(1:i,1))
                
                set(h.F0_value, 'String',userdata.ADC.F0_data(i,1))
                set(h.F1_value, 'String',userdata.ADC.F1_data(i,1))
                set(h.F4_value, 'String',userdata.ADC.F4_data(i,1))
                set(h.F5_value, 'String',userdata.ADC.F5_data(i,1))
                set(h.F6_value, 'String',userdata.ADC.F6_data(i,1))
                set(h.F7_value, 'String',userdata.ADC.F7_data(i,1))
                set(h.D4_value, 'String',userdata.ADC.D4_data(i,1))
                set(h.D6_value, 'String',userdata.ADC.D6_data(i,1))
                
                drawnow
                i=i+1;
            else
                % Send M2 the instruction to send sensors data
                fwrite(userdata.handle, 7);
            end
        end
    end

