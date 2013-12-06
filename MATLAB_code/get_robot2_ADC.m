function get_robot2_ADC(i)

global userdata

% Grab data from M2 until line terminator
% Plot the data
% Same for all below
data = fgetl(userdata.handle);
userdata.ADC.robot2.PT_LO(i,1) = str2double(data);
set(userdata.ADC.robot2.plot_PT_LO,...
    'xdata',userdata.time(1,1:i),...
    'ydata',userdata.ADC.robot2.PT_LO(1:i,1))

data = fgetl(userdata.handle);
userdata.ADC.robot2.PT_LI(i,1) = str2double(data);
set(userdata.ADC.robot2.plot_PT_LI,...
    'xdata',userdata.time(1,1:i),...
    'ydata',userdata.ADC.robot2.PT_LI(1:i,1))

data = fgetl(userdata.handle);
userdata.ADC.robot2.PT_RI(i,1) = str2double(data);
set(userdata.ADC.robot2.plot_PT_RI,...
    'xdata',userdata.time(1,1:i),...
    'ydata',userdata.ADC.robot2.PT_RI(1:i,1))

data = fgetl(userdata.handle);
userdata.ADC.robot2.PT_RO(i,1) = str2double(data);
set(userdata.ADC.robot2.plot_PT_RO,...
    'xdata',userdata.time(1,1:i),...
    'ydata',userdata.ADC.robot2.PT_RO(1:i,1))

data = fgetl(userdata.handle);
userdata.ADC.robot2.PT_BL(i,1) = str2double(data);
set(userdata.ADC.robot2.plot_PT_BL,...
    'xdata',userdata.time(1,1:i),...
    'ydata',userdata.ADC.robot2.PT_BL(1:i,1))

data = fgetl(userdata.handle);
userdata.ADC.robot2.PT_BR(i,1) = str2double(data);
set(userdata.ADC.robot2.plot_PT_BR,...
    'xdata',userdata.time(1,1:i),...
    'ydata',userdata.ADC.robot2.PT_BR(1:i,1))

data = fgetl(userdata.handle);
userdata.ADC.robot2.PT_PUCK(i,1) = str2double(data);
set(userdata.ADC.robot2.plot_PT_PUCK,...
    'xdata',userdata.time(1,1:i),...
    'ydata',userdata.ADC.robot2.PT_PUCK(1:i,1))


set(userdata.ADC.robot2.PT_LO_value, 'String',userdata.ADC.robot2.PT_LO(i,1))
set(userdata.ADC.robot2.PT_LI_value, 'String',userdata.ADC.robot2.PT_LI(i,1))
set(userdata.ADC.robot2.PT_RI_value, 'String',userdata.ADC.robot2.PT_RI(i,1))
set(userdata.ADC.robot2.PT_RO_value, 'String',userdata.ADC.robot2.PT_RO(i,1))
set(userdata.ADC.robot2.PT_BL_value, 'String',userdata.ADC.robot2.PT_BL(i,1))
set(userdata.ADC.robot2.PT_BR_value, 'String',userdata.ADC.robot2.PT_BR(i,1))
set(userdata.ADC.robot2.PT_PUCK_value, 'String',userdata.ADC.robot2.PT_PUCK(i,1))


[~, id_max] = max([userdata.ADC.robot2.PT_LO(i,1)...
                   userdata.ADC.robot2.PT_LI(i,1)...
                   userdata.ADC.robot2.PT_RI(i,1)...
                   userdata.ADC.robot2.PT_RO(i,1)...
                   userdata.ADC.robot2.PT_BL(i,1)...
                   userdata.ADC.robot2.PT_BR(i,1)]);
if (userdata.ADC.robot2.PT_PUCK(i,1) < 200)
    set(userdata.ADC.robot2.sensors(7), 'MarkerFaceColor','r' );
end

set(userdata.ADC.robot2.sensors(userdata.ADC.robot2.ids==id_max), 'MarkerFaceColor','r' );
set(userdata.ADC.robot2.sensors(userdata.ADC.robot2.ids~=id_max), 'MarkerFaceColor','b' );


end

