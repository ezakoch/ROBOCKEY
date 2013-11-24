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