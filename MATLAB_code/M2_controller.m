function M2_controller

global userdata


% Create a figure and an axes to contain the main window.

scrsz = get(0,'ScreenSize');
h.figures.main_fig = figure('Position',[scrsz(3)*0.08 scrsz(4)*0.5 scrsz(3)*0.5 scrsz(4)*0.5],...
    'Name','M2 Controller', 'NumberTitle','off');
userdata.defaultBackground = get(h.figures.main_fig,'Color');
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
    'Fontsize',14,'Position', [400 20 90 50], 'callback', @plot_localize);
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









%% CONNECT to M2 function
    function connect(src,evnt)
        % Find serial ports and connect to M2
        delete(instrfind);
        port = '/dev/tty.usbmodem411';
        userdata.handle = serial(port,'Baudrate', 9600);
        fopen(userdata.handle);
        
        % Enable text to know that we are Connected!
        h.text6 = uicontrol('Style', 'text', 'String', 'Connected!','Fontsize',18,...
            'Position', [20 130 130 20], 'BackgroundColor',userdata.defaultBackground);
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
        fclose(userdata.handle);
        close all
        clear all
    end




%% STOP PLOTTING function
    function stop_plotting(src,evnt)
        %Disable plotting the IMU and the angle data
        userdata.flag_plot_sensors = 0;
    end



end

