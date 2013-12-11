function get_robot1_variables(i)

global userdata

data = fgetl(userdata.handle);
userdata.team.robot1.x(i) = str2double(data);
data = fgetl(userdata.handle);
userdata.team.robot1.y(i) = str2double(data);
set(userdata.team.robot1.plot,'xdata',userdata.team.robot1.x(i),'ydata',userdata.team.robot1.y(i))

data = fgetl(userdata.handle);
userdata.team.robot1.yaw(i) = str2double(data);
set(userdata.h.robot1.var6_value, 'String',userdata.team.robot1.yaw(i))
scale = 20;
set(userdata.team.robot1.plot_dir,...
    'xdata',[userdata.team.robot1.x(i) -sind(userdata.team.robot1.yaw(i))*scale+userdata.team.robot1.x(i)],...
    'ydata',[userdata.team.robot1.y(i)  cosd(userdata.team.robot1.yaw(i))*scale+userdata.team.robot1.y(i)]);

data = fgetl(userdata.handle);
userdata.team.robot1.status_gtg = str2double(data);
set(userdata.h.robot1.var1_value, 'String',userdata.team.robot1.status_gtg)


data = fgetl(userdata.handle);
userdata.team.robot1.angle_to_goal = str2double(data);
set(userdata.h.robot1.var4_value, 'String',userdata.team.robot1.angle_to_goal)

data = fgetl(userdata.handle);
userdata.team.robot1.dist_to_goal = str2double(data);
set(userdata.h.robot1.var5_value, 'String',userdata.team.robot1.dist_to_goal)

data = fgetl(userdata.handle);
userdata.team.robot1.camera_x(i) = str2double(data);

data = fgetl(userdata.handle);
userdata.team.robot1.camera_y(i) = str2double(data);

data = fgetl(userdata.handle);
userdata.team.robot1.commands_var(i) = str2double(data);
set(userdata.h.robot1.var7_value, 'String',userdata.team.robot1.commands_var(i))

data = fgetl(userdata.handle);
userdata.team.robot1.diff_theta(i) = str2double(data);
set(userdata.h.robot1.var8_value, 'String',userdata.team.robot1.diff_theta(i))


% mean_camera_y = mean(userdata.team.robot1.camera_x)
% mean_camera_x = mean(userdata.team.robot1.camera_y)
% temp_x = userdata.team.robot1.x(i)
% temp_y = userdata.team.robot1.y(i)


end

