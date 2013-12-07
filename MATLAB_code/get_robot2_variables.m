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
userdata.team.robot2.status_gtg = str2double(data);
set(userdata.h.robot2.var1_value, 'String',userdata.team.robot2.status_gtg)


data = fgetl(userdata.handle);
userdata.team.robot2.angle_to_goal = str2double(data);
set(userdata.h.robot2.var4_value, 'String',userdata.team.robot2.angle_to_goal)

data = fgetl(userdata.handle);
userdata.team.robot2.dist_to_goal = str2double(data);
set(userdata.h.robot2.var5_value, 'String',userdata.team.robot2.dist_to_goal)

data = fgetl(userdata.handle);
userdata.team.robot2.camera_x(i) = str2double(data);

data = fgetl(userdata.handle);
userdata.team.robot2.camera_y(i) = str2double(data);

data = fgetl(userdata.handle);
userdata.team.robot2.commands_var(i) = str2double(data);
set(userdata.h.robot2.var7_value, 'String',userdata.team.robot2.commands_var(i))

data = fgetl(userdata.handle);
userdata.team.robot2.diff_theta(i) = str2double(data);
set(userdata.h.robot2.var8_value, 'String',userdata.team.robot2.diff_theta(i))

% 
% mean_camera_y = mean(userdata.team.robot2.camera_x)
% mean_camera_x = mean(userdata.team.robot2.camera_y)
% temp_x = userdata.team.robot2.x(i)
% temp_y = userdata.team.robot2.y(i)


end

