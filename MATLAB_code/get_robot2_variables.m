function get_robot2_variables(i)

global userdata

data = fgetl(userdata.handle);
userdata.team.robot2.x(i) = str2double(data);
data = fgetl(userdata.handle);
userdata.team.robot2.y(i) = str2double(data);
set(userdata.team.robot2.plot,'xdata',userdata.team.robot2.x(i),'ydata',userdata.team.robot2.y(i))

data = fgetl(userdata.handle);
userdata.team.robot2.yaw(i) = str2double(data);
set(userdata.h.robot2.var6_value, 'String',userdata.team.robot2.yaw(i))
scale = 20;
set(userdata.team.robot2.plot_dir,...
    'xdata',[userdata.team.robot2.x(i) -sind(userdata.team.robot2.yaw(i))*scale+userdata.team.robot2.x(i)],...
    'ydata',[userdata.team.robot2.y(i)  cosd(userdata.team.robot2.yaw(i))*scale+userdata.team.robot2.y(i)]);

data = fgetl(userdata.handle);
userdata.debugging.status_gtg = str2double(data);
set(userdata.h.robot2.var1_value, 'String',userdata.debugging.status_gtg)

data = fgetl(userdata.handle);
userdata.debugging.dirx = str2double(data);
set(userdata.h.robot2.var2_value, 'String',userdata.debugging.dirx)

data = fgetl(userdata.handle);
userdata.debugging.diry = str2double(data);
set(userdata.h.robot2.var3_value, 'String',userdata.debugging.diry)

data = fgetl(userdata.handle);
userdata.debugging.angle_to_goal = str2double(data);
set(userdata.h.robot2.var4_value, 'String',userdata.debugging.angle_to_goal)

data = fgetl(userdata.handle);
userdata.debugging.dist_to_goal = str2double(data);
set(userdata.h.robot2.var5_value, 'String',userdata.debugging.dist_to_goal)

data = fgetl(userdata.handle);
userdata.debugging.camera_x(i) = str2double(data);

data = fgetl(userdata.handle);
userdata.debugging.camera_y(i) = str2double(data);

data = fgetl(userdata.handle);
userdata.debugging.commands_var(i) = str2double(data);
set(userdata.h.robot2.var7_value, 'String',userdata.debugging.commands_var(i))

data = fgetl(userdata.handle);
userdata.debugging.diff_theta(i) = str2double(data);
set(userdata.h.robot2.var8_value, 'String',userdata.debugging.diff_theta(i))

data = fgetl(userdata.handle);
userdata.debugging.bank(i) = str2double(data);
set(userdata.h.robot2.var9_value, 'String',userdata.debugging.bank(i))

data = fgetl(userdata.handle);
userdata.debugging.OCR1B(i) = str2double(data);
set(userdata.h.robot2.var10_value, 'String',userdata.debugging.OCR1B(i))

data = fgetl(userdata.handle);
userdata.debugging.OCR1C(i) = str2double(data);
set(userdata.h.robot2.var11_value, 'String',userdata.debugging.OCR1C(i))


% mean_camera_y = mean(userdata.debugging.camera_x)
% mean_camera_x = mean(userdata.debugging.camera_y)
% temp_x = userdata.team.robot2.x(i)
% temp_y = userdata.team.robot2.y(i)


end

