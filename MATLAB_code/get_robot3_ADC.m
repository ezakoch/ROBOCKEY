function get_robot3_ADC(i)

global userdata

num = 50;
if i<=num
    to_plot = 1:i;
else
    to_plot = i-num:i;
end
% Grab data from M2 until line terminator
% Plot the data
% Same for all below
data = fgetl(userdata.handle);
userdata.ADC.robot3.PT_LO(i,1) = str2double(data);
set(userdata.ADC.robot3.plot_PT_LO,...
    'xdata',userdata.time(1,to_plot),...
    'ydata',userdata.ADC.robot3.PT_LO(to_plot,1))

data = fgetl(userdata.handle);
userdata.ADC.robot3.PT_LI(i,1) = str2double(data);
set(userdata.ADC.robot3.plot_PT_LI,...
    'xdata',userdata.time(1,to_plot),...
    'ydata',userdata.ADC.robot3.PT_LI(to_plot,1))

data = fgetl(userdata.handle);
userdata.ADC.robot3.PT_RI(i,1) = str2double(data);
set(userdata.ADC.robot3.plot_PT_RI,...
    'xdata',userdata.time(1,to_plot),...
    'ydata',userdata.ADC.robot3.PT_RI(to_plot,1))

data = fgetl(userdata.handle);
userdata.ADC.robot3.PT_RO(i,1) = str2double(data);
set(userdata.ADC.robot3.plot_PT_RO,...
    'xdata',userdata.time(1,to_plot),...
    'ydata',userdata.ADC.robot3.PT_RO(to_plot,1))

data = fgetl(userdata.handle);
userdata.ADC.robot3.PT_BL(i,1) = str2double(data);
set(userdata.ADC.robot3.plot_PT_BL,...
    'xdata',userdata.time(1,to_plot),...
    'ydata',userdata.ADC.robot3.PT_BL(to_plot,1))

data = fgetl(userdata.handle);
userdata.ADC.robot3.PT_BR(i,1) = str2double(data);
set(userdata.ADC.robot3.plot_PT_BR,...
    'xdata',userdata.time(1,to_plot),...
    'ydata',userdata.ADC.robot3.PT_BR(to_plot,1))

data = fgetl(userdata.handle);
userdata.ADC.robot3.PT_PUCK(i,1) = str2double(data);
set(userdata.ADC.robot3.plot_PT_PUCK,...
    'xdata',userdata.time(1,to_plot),...
    'ydata',userdata.ADC.robot3.PT_PUCK(to_plot,1))


set(userdata.ADC.robot3.PT_LO_value, 'String',userdata.ADC.robot3.PT_LO(i,1))
set(userdata.ADC.robot3.PT_LI_value, 'String',userdata.ADC.robot3.PT_LI(i,1))
set(userdata.ADC.robot3.PT_RI_value, 'String',userdata.ADC.robot3.PT_RI(i,1))
set(userdata.ADC.robot3.PT_RO_value, 'String',userdata.ADC.robot3.PT_RO(i,1))
set(userdata.ADC.robot3.PT_BL_value, 'String',userdata.ADC.robot3.PT_BL(i,1))
set(userdata.ADC.robot3.PT_BR_value, 'String',userdata.ADC.robot3.PT_BR(i,1))
set(userdata.ADC.robot3.PT_PUCK_value, 'String',userdata.ADC.robot3.PT_PUCK(i,1))


[~, id_max] = max([userdata.ADC.robot3.PT_LO(i,1)...
                   userdata.ADC.robot3.PT_LI(i,1)...
                   userdata.ADC.robot3.PT_RI(i,1)...
                   userdata.ADC.robot3.PT_RO(i,1)...
                   userdata.ADC.robot3.PT_BL(i,1)...
                   userdata.ADC.robot3.PT_BR(i,1)]);
if (userdata.ADC.robot3.PT_PUCK(i,1) < 200)
    set(userdata.ADC.robot3.sensors(7), 'MarkerFaceColor','r' );
end

set(userdata.ADC.robot3.sensors(userdata.ADC.robot3.ids==id_max), 'MarkerFaceColor','r' );
set(userdata.ADC.robot3.sensors(userdata.ADC.robot3.ids~=id_max), 'MarkerFaceColor','b' );


end

