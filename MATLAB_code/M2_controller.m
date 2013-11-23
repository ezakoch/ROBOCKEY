function M2_controller

global userdata


% Create a figure and an axes to contain the main window.

scrsz = get(0,'ScreenSize');
h.figures.main_fig = figure('Position',[scrsz(3)*0.08 scrsz(4)*0.5 scrsz(3)*0.5 scrsz(4)*0.5],...
    'Name','M2 Controller', 'NumberTitle','off');
defaultBackground = get(h.figures.main_fig,'Color');
h.handle = [];

% Create a uicontrol object to let user connect to the M2
h.button_connect = uicontrol('Style', 'pushbutton', 'String', 'Connect',...
    'Fontsize',14,'Position', [20 70 90 50], 'callback', @connect);
% Create a uicontrol object to let user disconnect from the M2
h.button_disconnect = uicontrol('Style', 'pushbutton', 'String', 'Disconnect',...
    'Fontsize',14,'Position', [120 70 90 50], 'callback', @disconnect);
h.button_exit = uicontrol('Style','pushbutton','String','Exit','Fontsize',14,...
    'Fontsize',14,'Position', [20 20 90 50],'Callback',@close_callback);
h.button_plot_sensors = uicontrol('Style', 'pushbutton', 'String', 'Plot Sensors',...
    'Fontsize',14,'Position', [500 70 90 50], 'callback', @plot_sensors);
h.button_stop_plotting = uicontrol('Style', 'pushbutton', 'String', 'Stop Plotting',...
    'Fontsize',14,'Position', [500 20 90 50], 'callback', @stop_plotting);
h.button_localize = uicontrol('Style', 'pushbutton', 'String', 'Localize',...
    'Fontsize',14,'Position', [400 20 90 50], 'callback', @localize);
h.button_test = uicontrol('Style', 'pushbutton', 'String', 'ENEMIES',...
    'Fontsize',14,'Position', [400 120 90 50], 'callback', @plot_enemies);


h.button_test = uicontrol('Style', 'pushbutton', 'String', 'TEST',...
    'Fontsize',14,'Position', [400 70 90 50], 'callback', @plot_test);



%h.kp_cur = uicontrol('Style', 'text', 'String', '', 'Fontsize',18,...
%    'Position', [220 290 80 20], 'BackgroundColor',defaultBackground);
%h.ki_cur = uicontrol('Style', 'text', 'String', '','Fontsize',18,...
%    'Position', [300 290 80 20], 'BackgroundColor',defaultBackground);
%h.kd_cur = uicontrol('Style', 'text', 'String', '','Fontsize',18,...
%    'Position', [380 290 80 20], 'BackgroundColor',defaultBackground);


%h.text1 = uicontrol('Style', 'text', 'String', 'kp','Fontsize',18,...
%    'Position', [220 320 80 20], 'BackgroundColor',defaultBackground);
%h.text2 = uicontrol('Style', 'text', 'String', 'ki','Fontsize',18,...
%    'Position', [300 320 80 20], 'BackgroundColor',defaultBackground);
%h.text3 = uicontrol('Style', 'text', 'String', 'kd','Fontsize',18,...
%    'Position', [380 320 80 20], 'BackgroundColor',defaultBackground);
%h.text4 = uicontrol('Style', 'text', 'String', 'Current Values','Fontsize',18,...
%    'Position', [20 290 200 20], 'BackgroundColor',defaultBackground);
%h.text5 = uicontrol('Style', 'text', 'String', 'Values to Set','Fontsize',18,...
%    'Position', [20 250 200 20], 'BackgroundColor',defaultBackground);



%h.button4 = uicontrol('Style', 'pushbutton', 'String', 'Set',...
%    'Fontsize',14,'Position', [500 240 90 50], 'callback', @setter);



%h.button7 = uicontrol('Style', 'pushbutton', 'String', 'Calibrate IMU',...
%    'Fontsize',10,'Position', [400 70 90 40], 'callback', @calibrate_IMU);
%h.button8 = uicontrol('Style', 'pushbutton', 'String', 'Stop Calibrating',...
%    'Fontsize',10,'Position', [400 20 90 40], 'callback', @stop_calibrating);
%h.button8 = uicontrol('Style', 'pushbutton', 'String', 'Get Angle',...
%    'Fontsize',14,'Position', [500 120 90 40], 'callback', @get_angle);


%h.kp_set = uicontrol('Style', 'edit', 'String', '','Fontsize',18, 'BackgroundColor',defaultBackground,...
%    'Position', [220 240 80 40], 'callback', @kp_callback);
%h.ki_set = uicontrol('Style', 'edit', 'String', '','Fontsize',18, 'BackgroundColor',defaultBackground,...
%    'Position', [300 240 80 40], 'callback', @ki_callback);
%h.kd_set = uicontrol('Style', 'edit', 'String', '','Fontsize',18, 'BackgroundColor',defaultBackground,...
%    'Position', [380 240 80 40], 'callback', @kd_callback);
%


%% PLOT the TEST data
    function plot_test(src,evnt)
        % Initialiaze figures
        figure(2);
        hold on
        userdata.figures.plot_test = plot(0,0,'b-');
        userdata.figures.plot_test2 = plot(0,0,'r-');
        hold off
        title('TEST DATA')
        xlabel('Time (sec)')
        ylabel('Received value')
        ylim([-200 200])
        grid on
        
        
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
                userdata.test_data(i,1) = str2double(data);
                set(userdata.figures.plot_test, 'xdata',userdata.time(1,1:i),...
                    'ydata',userdata.test_data(1:i,1))
                
                data = fgetl(userdata.handle);
                userdata.test_data2(i,1) = str2double(data);
%                 set(userdata.figures.plot_test2, 'xdata',userdata.time(1,1:i),...
%                     'ydata',userdata.test_data2(1:i,1))
%                 
               
                drawnow
                i=i+1;
            else
                % Send M2 the instruction to send sensors data
                fwrite(userdata.handle, 5);
            end
        end
    end







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



%% CONNECT to M2 function
    function connect(src,evnt)
        % Find serial ports and connect to M2
        delete(instrfind);
        port = '/dev/tty.usbmodem411';
        userdata.handle = serial(port,'Baudrate', 9600);
        fopen(userdata.handle);
        
        % Enable text to know that we are Connected!
        h.text6 = uicontrol('Style', 'text', 'String', 'Connected!','Fontsize',18,...
            'Position', [20 130 130 20], 'BackgroundColor',defaultBackground);
    end

%% DISCONNECT from M2 function
    function disconnect(src,evnt)
        fclose(userdata.handle);                % Close serial connection
        set(h.text6,'String','Disconnected!');  % Output text
    end


%% EXIT function
    function close_callback(src,evnt)
        % Disable plotting, close all figures and clear the workspace
        userdata.flag_plot_imu = 0;
        userdata.flag_plot_angle = 0;
        close all
        clear all
    end

%% PLOT the SENSORS data
    function plot_sensors(src,evnt)
        % Initialiaze figures
        figure(3);
        clf
        hold all
        userdata.ADC.plot_F0 = plot(0,0);
        userdata.ADC.plot_F1 = plot(0,0);
        userdata.ADC.plot_F4 = plot(0,0);
        userdata.ADC.plot_F5 = plot(0,0);
        userdata.ADC.plot_F6 = plot(0,0);
        userdata.ADC.plot_F7 = plot(0,0);
        userdata.ADC.plot_D4 = plot(0,0);
        userdata.ADC.plot_D6 = plot(0,0);
        hold off
        title('Sensors')
        xlabel('Time (sec)')
        ylabel('ADC (0-1023)')
        legend([userdata.ADC.plot_F0 userdata.ADC.plot_F1 userdata.ADC.plot_F4 userdata.ADC.plot_F5 userdata.ADC.plot_F6 userdata.ADC.plot_F7 userdata.ADC.plot_D4 userdata.ADC.plot_D6],...
            'F0','F1','F4','F5','F6','F7','D4','D6')
        ylim([0 1100])
        grid on
        
        
        
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
                
                drawnow
                i=i+1;
            else
                % Send M2 the instruction to send sensors data
                fwrite(userdata.handle, 7);
            end
        end
    end



%% STOP PLOTTING function
    function stop_plotting(src,evnt)
        %Disable plotting the IMU and the angle data
        userdata.flag_plot_sensors = 0;
    end



end

