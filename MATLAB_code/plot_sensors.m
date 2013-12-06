%% PLOT the SENSORS data
function plot_sensors(src,evnt)

global userdata

init_plot_sensors_robot1();
init_plot_sensors_robot2();

% Infinite loop to plot the data
i=1;
userdata.flag_plot_sensors = 1;

tic
while (userdata.flag_plot_sensors)
    userdata.time(i) = toc; % Grab the time passed
    
    % ASK ROBOT 1 FOR EACH VARIABLES
    fwrite(userdata.handle, 4);
    nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
    while(nBytes==0)
        nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
    end
    get_robot1_ADC(i);
    
    %             % ASK ROBOT 2 FOR EACH VARIABLES
    %             fwrite(userdata.handle, 2);
    %             nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
    %             while(nBytes==0)
    %                 nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
    %             end
    %             get_robot2_variables(i);
    %
    %             % ASK ROBOT 3 FOR EACH VARIABLES
    %             fwrite(userdata.handle, 3);
    %             nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
    %             while(nBytes==0)
    %                 nBytes = userdata.handle.BytesAvailable;    % Check if we have available bytes from M2
    %             end
    %             get_robot3_variables(i);
    
    

    drawnow
    i=i+1;
end


end

